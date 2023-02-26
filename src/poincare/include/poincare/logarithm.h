#ifndef POINCARE_LOGARITHM_H
#define POINCARE_LOGARITHM_H

#include <poincare/expression.h>
#include <poincare/expression_node_with_up_to_two_children.h>
#include <poincare/integer.h>
#include <poincare/addition.h>
#include <poincare/complex.h>

namespace Poincare {

class LogarithmNode final : public ExpressionNodeWithOneOrTwoChildren {
public:
  constexpr static AliasesList k_functionName = "log";

  // TreeNode
  size_t size() const override { return sizeof(LogarithmNode); }
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "Logarithm";
  }
#endif

  // Properties
  Type type() const override { return Type::Logarithm; }

  // Layout
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  // Simplification
  Expression shallowBeautify(const ReductionContext& reductionContext) override;
  Expression shallowReduce(const ReductionContext& reductionContext) override;
  LayoutShape leftLayoutShape() const override { return LayoutShape::MoreLetters; };
  LayoutShape rightLayoutShape() const override { return LayoutShape::BoundaryPunctuation; }
  // Derivation
  bool derivate(const ReductionContext& reductionContext, Symbol symbol, Expression symbolValue) override;
  Expression unaryFunctionDifferential(const ReductionContext& reductionContext) override;
  // Evaluation
  template<typename U> static Complex<U> computeOnComplex(const std::complex<U> c, Preferences::ComplexFormat, Preferences::AngleUnit angleUnit) {
    /* log has a branch cut on ]-inf, 0]: it is then multivalued on this cut. We
     * followed the convention chosen by the lib c++ of llvm on ]-inf+0i, 0+0i]
     * (warning: log takes the other side of the cut values on ]-inf-0i, 0-0i]).
     * We manually handle the case where the argument is null, as the lib c++
     * gives log(0) = -inf, which is only a generous shorthand for the limit. */
    return Complex<U>::Builder(c == std::complex<U>(0) ? std::complex<U>(NAN, NAN) : std::log10(c));
  }
  Evaluation<float> approximate(SinglePrecision p, const ApproximationContext& approximationContext) const override { return templatedApproximate<float>(approximationContext); }
  Evaluation<double> approximate(DoublePrecision p, const ApproximationContext& approximationContext) const override { return templatedApproximate<double>(approximationContext); }
  template<typename U> Evaluation<U> templatedApproximate(const ApproximationContext& approximationContext) const;
};

class Logarithm final : public ExpressionUpToTwoChildren<Logarithm, LogarithmNode> {
  friend class ExpressionNode;
  friend class LogarithmNode;
public:
  using ExpressionBuilder::ExpressionBuilder;
  Expression shallowReduce(ReductionContext reductionContext);
  Expression shallowBeautify();
  bool derivate(const ReductionContext& reductionContext, Symbol symbol, Expression symbolValue);
  Expression unaryFunctionDifferential(const ReductionContext& reductionContext);

private:
  void deepReduceChildren(const ReductionContext& reductionContext);
  Expression simpleShallowReduce(const ReductionContext& reductionContext);
  Integer simplifyLogarithmIntegerBaseInteger(Integer i, Integer & base, Addition & a, bool isDenominator);
  Expression splitLogarithmInteger(Integer i, bool isDenominator, const ReductionContext& reductionContext);
  bool parentIsAPowerOfSameBase() const;
};

}

#endif
