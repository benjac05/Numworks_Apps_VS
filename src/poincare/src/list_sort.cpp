#include <poincare/list_sort.h>
#include <poincare/layout_helper.h>
#include <poincare/list_complex.h>
#include <poincare/helpers.h>
#include <poincare/serialization_helper.h>

namespace Poincare {

int ListSortNode::numberOfChildren() const {
  return ListSort::s_functionHelper.numberOfChildren();
}

int ListSortNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, ListSort::s_functionHelper.aliasesList().mainAlias());
}

Layout ListSortNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const {
  return LayoutHelper::Prefix(ListSort(this), floatDisplayMode, numberOfSignificantDigits, ListSort::s_functionHelper.aliasesList().mainAlias(), context);
}

Expression ListSortNode::shallowReduce(const ReductionContext& reductionContext) {
  return ListSort(this).shallowReduce(reductionContext);
}

template<typename T> Evaluation<T> ListSortNode::templatedApproximate(const ApproximationContext& approximationContext) const {
  Evaluation<T> child = childAtIndex(0)->approximate(T(), approximationContext);
  if (child.type() != EvaluationNode<T>::Type::ListComplex) {
    return Complex<T>::Undefined();
  }
  ListComplex<T> listChild = static_cast<ListComplex<T>&>(child);
  listChild = listChild.sort();
  return std::move(listChild);
}

Expression ListSort::shallowReduce(ReductionContext reductionContext) {
  Expression child = childAtIndex(0);
  if (child.type() != ExpressionNode::Type::List) {
    if (!child.deepIsList(reductionContext.context())) {
      return replaceWithUndefinedInPlace();
    }
    return *this;
  }
  List list = static_cast<List &>(child);
  ApproximationContext approximationContext(reductionContext, true);
  void * pack[] = {list.node(), &approximationContext, const_cast<bool *>(&k_nanIsGreatest), &list};
  Helpers::Sort(
      // Swap
      [](int i, int j, void * ctx, int n) {
        void ** p = reinterpret_cast<void **>(ctx);
        Expression * e = reinterpret_cast<Expression *>(p[3]);
        assert(e->numberOfChildren() == n && 0 <= i && 0 <= j && i < n && j < n);
        e->swapChildrenInPlace(i, j);
      },
      // Compare
      Helpers::ListEvaluationComparisonAtIndex,
      pack,
      child.numberOfChildren());
  replaceWithInPlace(child);
  return child;
}

template Evaluation<float> ListSortNode::templatedApproximate<float>(const ApproximationContext& approximationContext) const;
template Evaluation<double> ListSortNode::templatedApproximate<double>(const ApproximationContext& approximationContext) const;

}
