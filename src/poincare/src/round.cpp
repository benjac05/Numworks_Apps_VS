#include <poincare/round.h>
#include <poincare/layout_helper.h>
#include <poincare/power.h>
#include <poincare/rational.h>
#include <poincare/serialization_helper.h>
#include <poincare/simplification_helper.h>
#include <poincare/undefined.h>
#include <assert.h>
#include <cmath>
#include <utility>

namespace Poincare {

Layout RoundNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const {
  return LayoutHelper::Prefix(Round(this), floatDisplayMode, numberOfSignificantDigits, Round::s_functionHelper.aliasesList().mainAlias(), context);
}

int RoundNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, Round::s_functionHelper.aliasesList().mainAlias());
}

Expression RoundNode::shallowReduce(const ReductionContext& reductionContext) {
  return Round(this).shallowReduce(reductionContext);
}

template<typename T>
Evaluation<T> RoundNode::templatedApproximate(const ApproximationContext& approximationContext) const {
  return ApproximationHelper::Map<T>(
      this,
      approximationContext,
      [] (const std::complex<T> * c, int numberOfComplexes, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit, void * ctx) {
        assert(numberOfComplexes == 2 || numberOfComplexes == 1);
        T f1 = ComplexNode<T>::ToScalar(c[0]);
        T f2;
        if (numberOfComplexes == 1) {
          f2 = 0.0;
        } else {
          f2 = ComplexNode<T>::ToScalar(c[1]);
        }
        if (std::isnan(f2) || f2 != std::round(f2)) {
          return Complex<T>::RealUndefined();
        }
        T err = std::pow(10, std::floor(f2));
        return Complex<T>::Builder(std::round(f1*err)/err);
      });
}

Expression Round::shallowReduce(ReductionContext reductionContext) {
  {
    if (numberOfChildren() == 2 && childAtIndex(1).hasUnit()) {
      // Number of digits cannot have units
      return replaceWithUndefinedInPlace();
    }
    Expression e = SimplificationHelper::defaultShallowReduce(
        *this,
        &reductionContext,
        SimplificationHelper::BooleanReduction::UndefinedOnBooleans,
        SimplificationHelper::UnitReduction::ExtractUnitsOfFirstChild,
        SimplificationHelper::MatrixReduction::UndefinedOnMatrix,
        SimplificationHelper::ListReduction::DistributeOverLists
    );
    if (!e.isUninitialized()) {
      return e;
    }
  }
  Expression secondChild;
  if (numberOfChildren() == 1) {
    secondChild = Rational::Builder(0);
  } else {
    secondChild = childAtIndex(1);
  }
  /* We reduce only round(Rational, Rational). We do not reduce
   * round(Float, Float) which is equivalent to what is done in approximate. */
  if (childAtIndex(0).type() == ExpressionNode::Type::Rational && secondChild.type() == ExpressionNode::Type::Rational) {
    Rational r1 = childAtIndex(0).convert<Rational>();
    Rational r2 = secondChild.convert<Rational>();
    if (!r2.isInteger()) {
      return replaceWithUndefinedInPlace();
    }
    const Rational ten = Rational::Builder(10);
    if (Power::RationalExponentShouldNotBeReduced(ten, r2)) {
      return *this;
    }
    Rational err = Rational::IntegerPower(ten, r2.signedIntegerNumerator());
    Rational mult = Rational::Multiplication(r1, err);
    IntegerDivision d = Integer::Division(mult.signedIntegerNumerator(), mult.integerDenominator());
    Integer rounding = d.quotient;
    Integer multDenominator = mult.integerDenominator();
    if (Rational::NaturalOrder(Rational::Builder(d.remainder, multDenominator), Rational::Builder(1,2)) >= 0) {
      rounding = Integer::Addition(rounding, Integer(1));
    }
    Rational result = Rational::Multiplication(Rational::Builder(rounding), Rational::IntegerPower(Rational::Builder(1,10), r2.signedIntegerNumerator()));
    if (result.numeratorOrDenominatorIsInfinity()) {
      return *this;
    }
    replaceWithInPlace(result);
    return std::move(result);
  }
  return *this;
}

}
