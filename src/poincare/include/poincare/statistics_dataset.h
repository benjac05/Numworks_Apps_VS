#ifndef POINCARE_STATISTICS_DATASET_H
#define POINCARE_STATISTICS_DATASET_H

#include "dataset_column.h"
#include "float_list.h"
#include "list_complex.h"
#include <algorithm>

/* This class is used to compute basic statistics functions on a dataset.
 *
 * === INSTANTIATION ===
 * You can build it either:
 * - with 2 columns (the values and the weights/population/frequencies.
 *   This second column needs to be positive).
 * - with 1 column (values only). In this case, all weights are equal to 1.
 *
 * === COMPLEXITY ===
 * There are two categories of methods:
 * - The ones which will always take the same time (like mean).
 * - The ones which memoize sorted indexes (like median).
 *
 * If you need to compute a mean, variance, standardDeviation, or any other
 * method that does not need sortedIndex, you can recreate a StatisticsDataset
 * object each time. You don't need to memoize the dataset.
 * (for example, that's what we do in Apps::Regression::Store)
 *
 * If you need to compute a median, a sortedElementAtCumulatedWeight, (or any
 * other method that needs sortedIndex) multiple times with the same datas,
 * you should memoize your dataset.
 * Indeed, the object memoizes m_sortedIndex and recomputes it only if you
 * ask it to.
 * (for example, that's what we do in Apps::Statistics::Store)
 *
 * === ENHANCEMENTS ===
 * More statistics method could be implemented here if factorization is needed.
 * */

namespace Poincare {

template<typename T>
class StatisticsDataset {
public:
  static StatisticsDataset<T> BuildFromChildren(const ExpressionNode * e, const ApproximationContext& approximationContext, ListComplex<T> evaluationArray[]);

  StatisticsDataset(const DatasetColumn<T> * values, const DatasetColumn<T> * weights) : m_values(values), m_weights(weights), m_sortedIndex(FloatList<float>::Builder()), m_recomputeSortedIndex(true), m_memoizedTotalWeight(NAN), m_lnOfValues(false) {}
  StatisticsDataset(const DatasetColumn<T> * values) : StatisticsDataset(values, nullptr) {}
  StatisticsDataset() : StatisticsDataset(nullptr, nullptr) {}

  bool isUndefined() { return m_values == nullptr; }

  void setHasBeenModified() { m_recomputeSortedIndex = true; m_memoizedTotalWeight = NAN; }
  int indexAtSortedIndex(int i) const;

  void setLnOfValues(bool b) { m_lnOfValues = b; }

  T totalWeight() const;
  T weightedSum() const;
  T offsettedSquaredSum(T offset) const;
  T squaredSum() const { return offsettedSquaredSum(0.0);}
  // sum(value(i) - (a + b * dataset.value(i))
  T squaredSumOffsettedByLinearTransformationOfDataset(StatisticsDataset<T> dataset, double a, double b) const;

  T mean() const { return weightedSum() / totalWeight(); }
  /* We use the Var(X) = E[(X-E[X])^2] definition instead of Var(X) = E[X^2] - E[X]^2
   * to ensure a positive result and to minimize rounding errors */
  T variance() const { return offsettedSquaredSum(mean()) / totalWeight(); }
  T standardDeviation() const { return std::sqrt(variance()); }
  T sampleStandardDeviation() const;

  // Need sortedIndex
  T sortedElementAtCumulatedFrequency(T freq, bool createMiddleElement) const;
  T sortedElementAtCumulatedWeight(T weight, bool createMiddleElement) const;
  T median() const { return sortedElementAtCumulatedFrequency(1.0/2.0, true); }
  int indexAtCumulatedFrequency(T freq, int * upperIndex = nullptr) const {
    assert(freq >= 0.0 && freq <= 1.0);
    return indexAtCumulatedWeight(freq * totalWeight(), upperIndex);
  }
  int indexAtCumulatedWeight(T weight, int * upperIndex = nullptr) const;
  int medianIndex(int * upperIndex = nullptr) const { return indexAtCumulatedFrequency(1.0/2.0, upperIndex); }

private:
  int datasetLength() const {
    assert(m_weights == nullptr || m_weights->length() == m_values->length());
    return m_values->length();
  }
  T valueAtIndex(int index) const;
  T weightAtIndex(int index) const;
  T privateTotalWeight() const;
  void buildSortedIndex() const;

  const DatasetColumn<T> * m_values;
  const DatasetColumn<T> * m_weights;
  /* This is just a list of int, but FloatList is the most optimized class for
   * containing numbers in the pool.*/
  mutable FloatList<float> m_sortedIndex;
  mutable bool m_recomputeSortedIndex;
  mutable double m_memoizedTotalWeight;
  bool m_lnOfValues;
};

}

#endif
