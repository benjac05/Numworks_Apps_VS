#ifndef POINCARE_LIST_SUM_H
#define POINCARE_LIST_SUM_H

#include <poincare/list.h>

namespace Poincare {

class ListSumNode : public ExpressionNode {
public:
  constexpr static AliasesList k_functionName = "sum";
  size_t size() const override { return sizeof(ListSumNode); }
  int numberOfChildren() const override;
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "ListSum";
  }
#endif
  Type type() const override { return Type::ListSum; }
  LayoutShape leftLayoutShape() const override { return LayoutShape::MoreLetters; };
  LayoutShape rightLayoutShape() const override { return LayoutShape::BoundaryPunctuation; }

private:
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const override;

  Expression shallowReduce(const ReductionContext& reductionContext) override;

  // Evaluation
  Evaluation<float> approximate(SinglePrecision p, const ApproximationContext& approximationContext) const override { return templatedApproximate<float>(approximationContext); }
  Evaluation<double> approximate(DoublePrecision p, const ApproximationContext& approximationContext) const override { return templatedApproximate<double>(approximationContext); }
  template<typename T> Evaluation<T> templatedApproximate(const ApproximationContext& approximationContext) const;
};

class ListSum : public ExpressionOneChild<ListSum, ListSumNode> {
public:
  using ExpressionBuilder::ExpressionBuilder;
  Expression shallowReduce(ReductionContext reductionContext);
};

}

#endif
