#include <poincare/division_remainder.h>
#include <poincare/approximation_helper.h>
#include <poincare/division_quotient.h>
#include <poincare/infinity.h>
#include <poincare/layout_helper.h>
#include <poincare/rational.h>
#include <poincare/serialization_helper.h>
#include <poincare/simplification_helper.h>
#include <poincare/undefined.h>
#include <cmath>

namespace Poincare {

int DivisionRemainderNode::numberOfChildren() const { return DivisionRemainder::s_functionHelper.numberOfChildren(); }

Layout DivisionRemainderNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const {
  return LayoutHelper::Prefix(DivisionRemainder(this), floatDisplayMode, numberOfSignificantDigits, DivisionRemainder::s_functionHelper.aliasesList().mainAlias(), context);
}

int DivisionRemainderNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, DivisionRemainder::s_functionHelper.aliasesList().mainAlias());
}

Expression DivisionRemainderNode::shallowReduce(const ReductionContext& reductionContext) {
  return DivisionRemainder(this).shallowReduce(reductionContext);
}

template<typename T>
Evaluation<T> DivisionRemainderNode::templatedApproximate(const ApproximationContext& approximationContext) const {
   return ApproximationHelper::Map<T>(this,
      approximationContext,
      [] (const std::complex<T> * c, int numberOfComplexes, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit, void * ctx) {
        assert(numberOfComplexes == 2);
        T f1 = ComplexNode<T>::ToScalar(c[0]);
        T f2 = ComplexNode<T>::ToScalar(c[1]);
        if (std::isnan(f1) || std::isnan(f2) || f1 != (int)f1 || f2 != (int)f2) {
          return Complex<T>::RealUndefined();
          }
        return Complex<T>::Builder(std::round(f1 - f2 * DivisionQuotient::TemplatedQuotient(f1, f2)));
      });
}

Expression DivisionRemainder::shallowReduce(ReductionContext reductionContext) {
  {
    Expression e = SimplificationHelper::defaultShallowReduce(
        *this,
        &reductionContext,
        SimplificationHelper::BooleanReduction::UndefinedOnBooleans,
        SimplificationHelper::UnitReduction::BanUnits,
        SimplificationHelper::MatrixReduction::UndefinedOnMatrix,
        SimplificationHelper::ListReduction::DistributeOverLists
    );
    if (!e.isUninitialized()) {
      return e;
    }
  }
  Expression c0 = childAtIndex(0);
  Expression c1 = childAtIndex(1);
  if (c0.type() == ExpressionNode::Type::Rational) {
    Rational r0 = static_cast<Rational &>(c0);
    if (!r0.isInteger()) {
      return replaceWithUndefinedInPlace();
    }
  }
  if (c1.type() == ExpressionNode::Type::Rational) {
    Rational r1 = static_cast<Rational &>(c1);
    if (!r1.isInteger()) {
      return replaceWithUndefinedInPlace();
    }
  }
  if (c0.type() != ExpressionNode::Type::Rational || c1.type() != ExpressionNode::Type::Rational) {
    return *this;
  }
  Rational r0 = static_cast<Rational &>(c0);
  Rational r1 = static_cast<Rational &>(c1);

  Integer a = r0.signedIntegerNumerator();
  Integer b = r1.signedIntegerNumerator();
  Expression result = Reduce(a, b);
  replaceWithInPlace(result);
  return result;
}

Expression DivisionRemainder::Reduce(const Integer & a, const Integer & b) {
  if (b.isZero()) {
    return Undefined::Builder();
  }
  Integer result = Integer::Division(a, b).remainder;
  assert(!result.isOverflow());
  return Rational::Builder(result);
}

}
