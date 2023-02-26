#ifndef POINCARE_HYPERBOLIC_ARC_COSINE_H
#define POINCARE_HYPERBOLIC_ARC_COSINE_H

#include <poincare/approximation_helper.h>
#include <poincare/hyperbolic_trigonometric_function.h>

namespace Poincare {

class HyperbolicArcCosineNode final : public HyperbolicTrigonometricFunctionNode {
public:
  constexpr static AliasesList k_functionName = "arcosh";

  // TreeNode
  size_t size() const override { return sizeof(HyperbolicArcCosineNode); }
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "HyperbolicArcCosine";
  }
#endif

  // Properties
  Type type() const override { return Type::HyperbolicArcCosine; }
private:
  // Simplification
  bool isNotableValue(Expression e, Context * context) const override { return e.isRationalOne(); }
  // Layout
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  //Evaluation
  template<typename T> static Complex<T> computeOnComplex(const std::complex<T> c, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit);
  Evaluation<float> approximate(SinglePrecision p, const ApproximationContext& approximationContext) const override {
    return ApproximationHelper::MapOneChild<float>(this, approximationContext, computeOnComplex<float>);
  }
  Evaluation<double> approximate(DoublePrecision p, const ApproximationContext& approximationContext) const override {
    return ApproximationHelper::MapOneChild<double>(this, approximationContext, computeOnComplex<double>);
  }
};


class HyperbolicArcCosine final : public ExpressionOneChild<HyperbolicArcCosine, HyperbolicArcCosineNode, HyperbolicTrigonometricFunction> {
public:
  using ExpressionBuilder::ExpressionBuilder;
};

}

#endif
