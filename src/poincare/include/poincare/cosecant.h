#ifndef POINCARE_COSCANT_H
#define POINCARE_COSCANT_H

#include <poincare/approximation_helper.h>
#include <poincare/expression.h>

namespace Poincare {

class CosecantNode final : public ExpressionNode {
public:
  constexpr static AliasesList k_functionName = "csc";

  // TreeNode
  size_t size() const override { return sizeof(CosecantNode); }
  int numberOfChildren() const override;
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "Cosecant";
  }
#endif

  // Properties
  Type type() const override { return Type::Cosecant; }

  template<typename T> static Complex<T> computeOnComplex(const std::complex<T> c, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit = Preferences::AngleUnit::Radian);

private:
  // Layout
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  // Simplication
  Expression shallowReduce(const ReductionContext& reductionContext) override;
  LayoutShape leftLayoutShape() const override { return LayoutShape::MoreLetters; };
  LayoutShape rightLayoutShape() const override { return LayoutShape::BoundaryPunctuation; }
  // Evaluation
  Evaluation<float> approximate(SinglePrecision p, const ApproximationContext& approximationContext) const override {
    return ApproximationHelper::MapOneChild<float>(this, approximationContext, computeOnComplex<float>);
  }
  Evaluation<double> approximate(DoublePrecision p, const ApproximationContext& approximationContext) const override {
    return ApproximationHelper::MapOneChild<double>(this, approximationContext, computeOnComplex<double>);
  }
};

class Cosecant final : public ExpressionOneChild<Cosecant, CosecantNode> {
public:
  using ExpressionBuilder::ExpressionBuilder;
};

}

#endif
