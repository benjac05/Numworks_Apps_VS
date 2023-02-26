#ifndef POINCARE_CHI2_DISTRIBUTION_H
#define POINCARE_CHI2_DISTRIBUTION_H

#include <poincare/continuous_distribution.h>
#include <poincare/expression.h>
#include <float.h>

namespace Poincare {

class Chi2Distribution final : public ContinuousDistribution {
public:
  Type type() const override { return Type::ChiSquared; }
  bool isSymmetrical() const override { return false; }

  template <typename T> static T EvaluateAtAbscissa(T x, T k);
  float evaluateAtAbscissa(float x, const float * parameters) const override { return EvaluateAtAbscissa<float>(x, parameters[0]); }
  double evaluateAtAbscissa(double x, const double * parameters) const override { return EvaluateAtAbscissa<double>(x, parameters[0]); }

  template <typename T> static T CumulativeDistributiveFunctionAtAbscissa(T x, T k);
  float cumulativeDistributiveFunctionAtAbscissa(float x, const float * parameters) const override { return CumulativeDistributiveFunctionAtAbscissa<float>(x, parameters[0]); }
  double cumulativeDistributiveFunctionAtAbscissa(double x, const double * parameters) const override { return CumulativeDistributiveFunctionAtAbscissa<double>(x, parameters[0]); }

  template <typename T> static T CumulativeDistributiveInverseForProbability(T probability, T k);
  float cumulativeDistributiveInverseForProbability(float x, const float * parameters) const override { return CumulativeDistributiveInverseForProbability<float>(x, parameters[0]); }
  double cumulativeDistributiveInverseForProbability(double x, const double * parameters) const override { return CumulativeDistributiveInverseForProbability<double>(x, parameters[0]); }

  bool parametersAreOK(const float * parameters) const override { return KIsOK(parameters[0]); }
  bool parametersAreOK(const double * parameters) const override { return KIsOK(parameters[0]); }

  static bool ExpressionKIsOK(bool * result, const Expression &k, Context * context);
  bool expressionParametersAreOK(bool * result, const Expression * parameters, Context * context) const override { return ExpressionKIsOK(result, parameters[0], context); }
private:
  constexpr static int k_maxRegularizedGammaIterations = 1000;
  constexpr static double k_regularizedGammaPrecision = DBL_EPSILON;
  template<typename T> static bool KIsOK(T k);
};

}

#endif
