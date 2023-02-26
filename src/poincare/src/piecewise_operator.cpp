#include <poincare/piecewise_operator.h>
#include <poincare/boolean.h>
#include <poincare/piecewise_operator_layout.h>
#include <poincare/serialization_helper.h>
#include <poincare/simplification_helper.h>
#include <poincare/variable_context.h>

namespace Poincare {

Layout PiecewiseOperatorNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const {
  PiecewiseOperatorLayout l = PiecewiseOperatorLayout::Builder();
  int n = numberOfChildren();
  int i = 0;
  while (i + 1 < n) {
    Layout leftChildLayout = childAtIndex(i)->createLayout(floatDisplayMode, numberOfSignificantDigits, context);
    Layout rightChildLayout = childAtIndex(i + 1)->createLayout(floatDisplayMode, numberOfSignificantDigits, context);
    l.addRow(leftChildLayout, rightChildLayout);
    i += 2;
  }
  if (i < n) {
    // Last child has no condition
    assert(n % 2 == 1 && i == n - 1);
    Layout leftChildLayout = childAtIndex(i)->createLayout(floatDisplayMode, numberOfSignificantDigits, context);
    l.addRow(leftChildLayout);
  }
  return std::move(l);
}

int PiecewiseOperatorNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, PiecewiseOperator::s_functionHelper.aliasesList().mainAlias());
}

Expression PiecewiseOperatorNode::shallowReduce(const ReductionContext& reductionContext) {
  return PiecewiseOperator(this).shallowReduce(reductionContext);
}

template<typename T>
Evaluation<T> PiecewiseOperatorNode::templatedApproximate(const ApproximationContext& approximationContext) const {
  // TODO: Distribute on lists
  int i = indexOfFirstTrueCondition<T>(approximationContext);
  if (i < 0) {
    return Complex<T>::Undefined();
  }
  return childAtIndex(i)->approximate(T(), approximationContext);
}

template<typename T>
int PiecewiseOperatorNode::indexOfFirstTrueCondition(const ApproximationContext& approximationContext) const {
  int n = numberOfChildren();
  assert(n > 0);
  int i = 0;
  while (i + 1 < n) {
    Evaluation<T> condition = childAtIndex(i + 1)->approximate(T(), approximationContext);
    if (condition.type() == EvaluationNode<T>::Type::BooleanEvaluation && static_cast<BooleanEvaluation<T>&>(condition).value()) {
      return i;
    }
    i += 2;
  }
  if (i < n) {
    // Last child has no condition and every other condition is false
    assert(n % 2 == 1 && i == n - 1);
    return i;
  }
  // Every condition is false.
  return -1;
}

Expression PiecewiseOperator::UntypedBuilder(Expression children) {
  assert(children.type() == ExpressionNode::Type::List);
  int n = children.numberOfChildren();
  // Check that each condition is boolean
  for (int i = 0; i < n; i++) {
    if (i % 2 == 0) {
      continue;
    }
    Expression condition = children.childAtIndex(i);
    if (!condition.hasBooleanValue() && condition.type() != ExpressionNode::Type::EmptyExpression) {
      return Expression();
    }
  }
  return UntypedBuilderMultipleChildren<PiecewiseOperator>(children);
}

Expression PiecewiseOperator::shallowReduce(ReductionContext reductionContext) {
  {
    /* Do not use defaultShallowReduce since it calls shallowReduceUndfined
     * and a piecewiseOperator can be defined even with an undefined child. */
    Expression e = SimplificationHelper::distributeReductionOverLists(*this, reductionContext);
    if (!e.isUninitialized()) {
      return e;
    }
  }
  int n = numberOfChildren();
  assert(n > 0);
  int i = 0;
  while (i + 1 < n) {
    Expression condition = childAtIndex(i + 1);
    if (condition.hasBooleanValue()) {
      // Skip conditions that are not booleans. They are always false
      if (condition.type() != ExpressionNode::Type::Boolean) {
        // The condition is undeterminated, can't reduce
        return *this;
      }
      if (static_cast<Boolean&>(condition).value()) {
        // Condition is true
        Expression result = childAtIndex(i);
        replaceWithInPlace(result);
        return result;
      }
    }
    i += 2;
  }
  if (i < n) {
    // Last child has no condition and every other condition is false
    assert(n % 2 == 1 && i == n - 1);
    Expression result = childAtIndex(i);
    replaceWithInPlace(result);
    return result;
  }
  // Every condition is false
  return replaceWithUndefinedInPlace();
}

template<typename T>
int PiecewiseOperator::indexOfFirstTrueConditionWithValueForSymbol(const char * symbol, T x, Context * context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const {
  assert(numberOfChildren() > 0);
  VariableContext variableContext = VariableContext(symbol, context);
  variableContext.setApproximationForVariable<T>(x);
  ApproximationContext approximationContext = ApproximationContext(&variableContext, complexFormat, angleUnit);
  return static_cast<PiecewiseOperatorNode *>(node())->indexOfFirstTrueCondition<T>(approximationContext);
}

template Evaluation<float> PiecewiseOperatorNode::templatedApproximate<float>(const ApproximationContext& approximationContext) const;
template Evaluation<double> PiecewiseOperatorNode::templatedApproximate<double>(const ApproximationContext& approximationContext) const;

template int PiecewiseOperatorNode::indexOfFirstTrueCondition<float>(const ApproximationContext& approximationContext) const;
template int PiecewiseOperatorNode::indexOfFirstTrueCondition<double>(const ApproximationContext& approximationContext) const;

template int PiecewiseOperator::indexOfFirstTrueConditionWithValueForSymbol<float>(const char * symbol, float x, Context * context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const;
template int PiecewiseOperator::indexOfFirstTrueConditionWithValueForSymbol<double>(const char * symbol, double x, Context * context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const;

}
