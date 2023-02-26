#include <poincare/logical_operator.h>
#include <poincare/approximation_helper.h>
#include <poincare/boolean.h>
#include <poincare/complex.h>
#include <poincare/float.h>
#include <poincare/layout_helper.h>
#include <poincare/rational.h>
#include <poincare/serialization_helper.h>
#include <poincare/simplification_helper.h>
#include <ion/unicode/utf8_helper.h>
#include <cmath>

namespace Poincare {

void fillBufferWithStartingAndEndingSpace(char * nameBuffer, int sizeOfBuffer, const char * operatorName, bool startingSpace) {
  int currentIndex = 0;
  if (startingSpace) {
    nameBuffer[0] = ' ';
    currentIndex++;
  }
  currentIndex += strlcpy(nameBuffer + currentIndex, operatorName, sizeOfBuffer - currentIndex);
  nameBuffer[currentIndex] = ' ';
  nameBuffer[currentIndex + 1] = 0;
}

int LogicalOperatorNode::LogicalOperatorTypePrecedence(const ExpressionNode * operatorExpression) {
  if (operatorExpression->type() == Type::LogicalOperatorNot) {
    return 2;
  }
  if (operatorExpression->type() == Type::BinaryLogicalOperator) {
    const BinaryLogicalOperatorNode * binaryOperatorExpression = static_cast<const BinaryLogicalOperatorNode *>(operatorExpression);
    BinaryLogicalOperatorNode::OperatorType operatorType = binaryOperatorExpression->operatorType();
    return operatorType == BinaryLogicalOperatorNode::OperatorType::And || operatorType == BinaryLogicalOperatorNode::OperatorType::Nand ? 1 : 0;
  }
  return -1;
}

bool LogicalOperatorNode::childAtIndexNeedsUserParentheses(const Expression & child, int childIndex) const {
  int childPrecedence = LogicalOperatorTypePrecedence(child.node());
  return childPrecedence != -1 && childPrecedence != LogicalOperatorTypePrecedence(this);
}

// Not Operator

Layout LogicalOperatorNotNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const {
  char nameBuffer[k_sizeOfNameBuffer];
  fillBufferWithStartingAndEndingSpace(nameBuffer, k_sizeOfNameBuffer, operatorName(), false);
  return LayoutHelper::Prefix(LogicalOperatorNot(this), floatDisplayMode, numberOfSignificantDigits, nameBuffer, context, false);
}

int LogicalOperatorNotNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  char nameBuffer[k_sizeOfNameBuffer];
  fillBufferWithStartingAndEndingSpace(nameBuffer, k_sizeOfNameBuffer, operatorName(), false);
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, nameBuffer, SerializationHelper::ParenthesisType::None);
}

template<typename T>
Evaluation<T> LogicalOperatorNotNode::templatedApproximate(const ApproximationContext& approximationContext) const {
  return
    ApproximationHelper::MapOneChild<T>(
      this,
      approximationContext,
      [](const std::complex<T> c, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) { return Complex<T>::Undefined(); },
      [](const bool b) {
        Evaluation<T> result = BooleanEvaluation<T>::Builder(!b);
        return result;
      }
    );
}

Expression LogicalOperatorNotNode::shallowReduce(const ReductionContext& reductionContext) {
  return LogicalOperatorNot(this).shallowReduce(reductionContext);
}

Expression LogicalOperatorNot::shallowReduce(ReductionContext reductionContext) {
  {
    Expression e = SimplificationHelper::defaultShallowReduce(
        *this,
        &reductionContext,
        SimplificationHelper::BooleanReduction::DefinedOnBooleans,
        SimplificationHelper::UnitReduction::BanUnits,
        SimplificationHelper::MatrixReduction::UndefinedOnMatrix,
        SimplificationHelper::ListReduction::DistributeOverLists
    );
    if (!e.isUninitialized()) {
      return e;
    }
  }
  Expression child = childAtIndex(0);
  if (!child.hasBooleanValue()) {
    return replaceWithUndefinedInPlace();
  }
  if (child.type() != ExpressionNode::Type::Boolean) {
    // Let approximation handle this
    return *this;
  }
  Expression result = Boolean::Builder(!static_cast<Boolean &>(child).value());
  replaceWithInPlace(result);
  return result;
}


// Binary Logical Operator

bool BinaryLogicalOperatorNode::IsBinaryLogicalOperator(const char * name, int nameLength, OperatorType * type) {
  for (int i = 0; i < k_numberOfOperators; i++) {
    if (UTF8Helper::CompareNonNullTerminatedStringWithNullTerminated(name, nameLength, k_operatorNames[i].name) == 0) {
      if (type) {
        *type = k_operatorNames[i].type;
      }
      return true;
    }
  }
  return false;
}

const char * BinaryLogicalOperatorNode::operatorName() const {
  assert(m_typeOfOperator != OperatorType::LengthOfEnum);
  for (int i = 0; i < k_numberOfOperators; i++) {
    if (k_operatorNames[i].type == m_typeOfOperator) {
      return k_operatorNames[i].name;
    }
  }
  assert(false);
  return k_operatorNames[0].name; // silence compiler
}

bool BinaryLogicalOperatorNode::evaluate(bool a, bool b) const {
  switch (m_typeOfOperator) {
  case OperatorType::And:
    return a && b;
  case OperatorType::Or:
    return a || b;
  case OperatorType::Xor:
    return a != b;
  case OperatorType::Nand:
    return !(a && b);
  default:
    assert(m_typeOfOperator == OperatorType::Nor);
    return !(a || b);
  }
}

Layout BinaryLogicalOperatorNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const {
  char nameBuffer[k_sizeOfNameBuffer];
  fillBufferWithStartingAndEndingSpace(nameBuffer, k_sizeOfNameBuffer, operatorName(), true);
  return LayoutHelper::Infix(BinaryLogicalOperator(this), floatDisplayMode, numberOfSignificantDigits, nameBuffer, context);
}

int BinaryLogicalOperatorNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  char nameBuffer[k_sizeOfNameBuffer];
  fillBufferWithStartingAndEndingSpace(nameBuffer, k_sizeOfNameBuffer, operatorName(), true);
  return SerializationHelper::Infix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, nameBuffer);
}

template<typename T>
Evaluation<T> BinaryLogicalOperatorNode::templatedApproximate(const ApproximationContext& approximationContext) const {
  return
    ApproximationHelper::Map<T>(
      this,
      approximationContext,
      [](const std::complex<T> * c, int numberOfComplexes, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit, void * context) { return Complex<T>::Undefined(); },
      [](const bool * b, int numberOfBooleans, void * context) {
        assert(numberOfBooleans == 2);
        Evaluation<T> result = BooleanEvaluation<T>::Builder(static_cast<BinaryLogicalOperatorNode *>(context)->evaluate(b[0], b[1]));
        return result;
      },
      true,
      reinterpret_cast<void *>(const_cast<BinaryLogicalOperatorNode *>(this))
    );
}

Expression BinaryLogicalOperatorNode::shallowReduce(const ReductionContext& reductionContext) {
  return BinaryLogicalOperator(this).shallowReduce(reductionContext);
}

Expression BinaryLogicalOperator::shallowReduce(ReductionContext reductionContext) {
  {
    Expression e = SimplificationHelper::defaultShallowReduce(
        *this,
        &reductionContext,
        SimplificationHelper::BooleanReduction::DefinedOnBooleans,
        SimplificationHelper::UnitReduction::BanUnits,
        SimplificationHelper::MatrixReduction::UndefinedOnMatrix,
        SimplificationHelper::ListReduction::DistributeOverLists
    );
    if (!e.isUninitialized()) {
      return e;
    }
  }
  Expression leftChild = childAtIndex(0);
  Expression rightChild = childAtIndex(1);
  if (!leftChild.hasBooleanValue() || !rightChild.hasBooleanValue()) {
    return replaceWithUndefinedInPlace();
  }
  if (leftChild.type() != ExpressionNode::Type::Boolean || rightChild.type() != ExpressionNode::Type::Boolean) {
    // Let approximation handle this
    return *this;
  }
  bool leftValue = static_cast<Boolean &>(leftChild).value();
  bool rightValue = static_cast<Boolean &>(rightChild).value();
  Expression result = Boolean::Builder(static_cast<BinaryLogicalOperatorNode *>(node())->evaluate(leftValue, rightValue));
  replaceWithInPlace(result);
  return result;
}

BinaryLogicalOperator BinaryLogicalOperator::Builder(Expression firstChild, Expression secondChild, BinaryLogicalOperatorNode::OperatorType type) {
  TreeHandle h = TreeHandle::BuilderWithChildren(Initializer<BinaryLogicalOperatorNode>, sizeof(BinaryLogicalOperatorNode), {firstChild, secondChild});
  BinaryLogicalOperator expression = static_cast<BinaryLogicalOperator&>(h);
  static_cast<BinaryLogicalOperatorNode *>(expression.node())->setOperatorType(type);
  return expression;
}

template Evaluation<float> BinaryLogicalOperatorNode::templatedApproximate<float>(const ApproximationContext&) const;
template Evaluation<double> BinaryLogicalOperatorNode::templatedApproximate<double>(const ApproximationContext&) const;
template Evaluation<float> LogicalOperatorNotNode::templatedApproximate<float>(const ApproximationContext&) const;
template Evaluation<double> LogicalOperatorNotNode::templatedApproximate<double>(const ApproximationContext&) const;

}