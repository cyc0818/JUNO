#ifndef BDTE_UTILS_H
#define BDTE_UTILS_H

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

namespace BDTE {

/**
 * @brief Compute the linear interpolation at t between v0 and v1
 *
 * @tparam T A numeric type
 * @param[in] v0
 * @param[in] v1
 * @param[in] t Distance in [0;1] between v0 and v1
 * @return The computed linear interpolation
 */
template <typename T>
static inline double Lerp(T v0, T v1, T t) {
  return (1 - t) * v0 + t * v1;
}

/**
 * @brief Compute the quantiles given in probs of inData
 *
 * @tparam T A numeric type
 * @param[in] inData The distribution to compute the quantiles from
 * @param[in] probs The quantiles to compute
 * @return A vector containing the quantiles as defined in probs;
 */
template <typename T>
static inline std::vector<T> Quantile(const std::vector<T>& inData, const std::vector<double>& probs) {
  if (inData.empty()) {
    return std::vector<T>();
  }

  if (1 == inData.size()) {
    return std::vector<T>(1, inData[0]);
  }

  std::vector<T> data = inData;
  std::sort(data.begin(), data.end());
  std::vector<T> quantiles;
  quantiles.reserve(probs.size());

  for (const double& prob : probs) {
    T poi = Lerp<T>(0, data.size() - 1, prob);

    size_t left = std::max(size_t(std::floor(poi)), size_t(0));
    size_t right = std::min(size_t(std::ceil(poi)), size_t(data.size() - 1));

    T datLeft = data.at(left);
    T datRight = data.at(right);

    T quantile = Lerp<T>(datLeft, datRight, poi - left);

    quantiles.push_back(quantile);
  }

  return quantiles;
}

/**
 * @brief Compute the mean of a distribution
 *
 * @tparam T A numeric type
 * @param[in] inData The data to extract the mean from
 * @return The mean of the distribution
 */
template <typename T>
static inline T mean(const std::vector<T>& inData) {
  if (inData.empty()) return 0;

  if (inData.size() == 1) return inData.at(0);

  T sum = std::accumulate(std::begin(inData), std::end(inData), (T)0.);
  return sum / (T)inData.size();
}

/**
 * @brief Compute the central moment of order `order` of a distribution
 *
 * The moment is computed as
 *
 * \f$ m_i = \frac{1}{N} \sum_{n}^N(x[n]-\bar{x})^i \f$
 *
 * @tparam T A numeric type
 * @param[in] inData The distribution to compute the moment from
 * @param[in] mean The mean of the distribution
 * @param[in] order The order of the moment to compute
 * @return The moment of order `order`
 */
template <typename T>
static inline T central_moment(const std::vector<T>& inData, const T& mean, const int& order) {
  if (inData.empty()) return 0;

  return std::accumulate(std::begin(inData), std::end(inData), (T)0.,
                         [mean, order](const T& acc, const T& data) { return acc + pow(data - mean, order); }) /
         (T)inData.size();
}

/**
 * @brief Compute the standard deviation of a distribution
 *
 * @tparam T A numeric type
 * @param[in] inData The distribution to compute the standard deviation from
 * @param[in] mean The mean of the distribution
 * @return The standard deviation of the distribution
 */
template <typename T>
static inline T std_dev(const std::vector<T>& inData, const T& mean) {
  return std::sqrt(central_moment(inData, mean, 2));
}

/**
 * @brief Compute the skewness of the given distribution
 *
 * It mimic the [scipy definition](https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.skew.html)
 *
 * @tparam T A numeric type
 * @param[in] inData The distribution to compute the skewness from
 * @param[in] mean The mean of the distribution
 * @return The skewness of the distribution
 */
template <typename T>
static inline T skewness(const std::vector<T>& inData, const T& mean) {
  T m3 = central_moment(inData, mean, 3);
  T m2 = central_moment(inData, mean, 2);

  return m3 / pow(m2, 3. / 2.);
}

/**
 * @brief Compute the kurtosis of a distribution
 *
 * It mimic the [scipy definition](https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.kurtosis.html)
 *
 * @tparam T A numeric type
 * @param[in] inData The distribution to compute the kurtosis from
 * @param[in] mean The mean of the distribution
 * @return The kurtosis of the distribution
 */
template <typename T>
static inline T kurtosis(const std::vector<T>& inData, const T& mean) {
  T m2 = central_moment(inData, mean, 2);
  T m4 = central_moment(inData, mean, 2);

  return m4 / pow(m2, 2);
}

}  // namespace BDTE

#endif
