#ifndef POINCARE_GEOMETRIC_DISTRIBUTION_H
#define POINCARE_GEOMETRIC_DISTRIBUTION_H

#include <poincare/expression.h>
#include <poincare/discrete_distribution.h>
#include <poincare/preferences.h>

namespace Poincare {

/* We chose the definition:
 * 0 < p <= 1 for distribution of success
 * k number of trials needed to get one success, where k ∈ {1, 2, 3, ...}. */

class GeometricDistribution final : public DiscreteDistribution {
public:
  Type type() const override { return Type::Geometric; }
  bool isSymmetrical() const override { return false; }

  template<typename T> static T EvaluateAtAbscissa(T x, const T p);
  float evaluateAtAbscissa(float x, const float * parameters) const override { return EvaluateAtAbscissa<float>(x, parameters[0]); }
  double evaluateAtAbscissa(double x, const double * parameters) const override { return EvaluateAtAbscissa<double>(x, parameters[0]); }

  template<typename T> static T CumulativeDistributiveInverseForProbability(T probability, T p);
  float cumulativeDistributiveInverseForProbability(float x, const float * parameters) const override { return CumulativeDistributiveInverseForProbability<float>(x, parameters[0]); }
  double cumulativeDistributiveInverseForProbability(double x, const double * parameters) const override { return CumulativeDistributiveInverseForProbability<double>(x, parameters[0]); }

  bool parametersAreOK(const float * parameters) const override { return PIsOK(parameters[0]); }
  bool parametersAreOK(const double * parameters) const override { return PIsOK(parameters[0]); }

  static bool ExpressionPIsOK(bool * result, const Expression &p, Context * context);
  bool expressionParametersAreOK(bool * result, const Expression * parameters, Context * context) const override { return ExpressionPIsOK(result, parameters[0], context); }

private:
  template<typename T> static T parameterP(T* parameters) { return parameters[0]; }
  template<typename T> static bool PIsOK(T p);
};

}

#endif
