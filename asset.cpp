#include "asset.hpp"
#include <cmath>
#include <numeric>
#include <stdexcept>

Asset::Asset(const std::string &name) : name(name), averageReturn(0.0), standardDeviation(0.0) {}

void Asset::addPrice(double price)
{
    prices.push_back(price);
}

// return calculation for step 1
void Asset::calculateReturns()
{
    if (prices.size() <= 1)
    {
        throw std::runtime_error("Insufficient data to calculate returns.");
    }

    for (size_t i = 1; i < prices.size(); ++i)
    {
        double returnRate = (prices[i] - prices[i - 1]) / prices[i - 1];
        returns.push_back(returnRate);
    }

    averageReturn = calculateSum(returns) / returns.size();
    double sumSquared = std::inner_product(returns.begin(), returns.end(), returns.begin(), 0.0);
    standardDeviation = std::sqrt((sumSquared - std::pow(averageReturn, 2) * returns.size()) / (returns.size() - 1));
    ;
}

// avg return calculation for step 2
double Asset::getAverageReturn() const
{
    return averageReturn;
}

// sample stdv calculation for step 3
double Asset::getStandardDeviation() const
{
    return standardDeviation;
}

// population covariance calculation for step 4
double Asset::calculateCovariance(const Asset &asset1, const Asset &asset2)
{
    if (asset1.returns.size() != asset2.returns.size())
    {
        throw std::runtime_error("Asset return series sizes do not match.");
    }

    double sumProduct = 0.0;
    for (size_t i = 0; i < asset1.returns.size(); ++i)
    {
        sumProduct += (asset1.returns[i] - asset1.averageReturn) * (asset2.returns[i] - asset2.averageReturn);
    }

    return sumProduct / asset1.returns.size();
}

// correlation  calculation for step 5
double Asset::calculateCorrelation(const Asset &asset1, const Asset &asset2)
{
    double covariance = calculateCovariance(asset1, asset2);
    return covariance / (asset1.standardDeviation * asset2.standardDeviation);
}

// sum calculation for step 6
double Asset::calculateSum(const std::vector<double> &data) const
{
    return std::accumulate(data.begin(), data.end(), 0.0);
}
