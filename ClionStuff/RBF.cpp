// rbf.cpp
#include "rbf.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <tuple>

RBF::RBF(int k, double lr, int epochs, bool inferStds)
        : k(k), lr(lr), epochs(epochs), inferStds(inferStds) {
    w.resize(k);
    b.resize(1);
    initializeWeights();
}

void RBF::initializeWeights() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < k; i++) {
        w[i] = distribution(gen);
    }

    b[0] = distribution(gen);
}

double RBF::rbf(double x, double c, double s) {
    return exp(-1 / (2 * s * s) * pow(x - c, 2));
}

double RBF::variance(const std::vector<double>& data) {
    double mean = 0.0;
    double variance = 0.0;
    size_t n = data.size();

    for (double value : data) {
        mean += value;
    }

    mean /= n;

    for (double value : data) {
        variance += pow(value - mean, 2);
    }

    variance /= n;

    return variance;
}

std::pair<std::vector<double>, std::vector<double>> RBF::kmeans(const std::vector<double>& X, int k) {
    std::vector<double> clusters(k);
    std::vector<double> stds(k);

    // randomly select initial clusters from input data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, X.size() - 1);

    for (int i = 0; i < k; i++) {
        clusters[i] = X[distribution(gen)];
    }

    std::vector<double> prevClusters(k);
    bool converged = false;

    while (!converged) {
        std::vector<int> clusterCounts(k, 0);
        std::vector<double> newClusters(k, 0.0);

        for (const auto& point : X) {
            double minDistance = std::numeric_limits<double>::max();
            int closestCluster = 0;

            for (int i = 0; i < k; i++) {
                double distance = std::abs(point - clusters[i]);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestCluster = i;
                }
            }

            newClusters[closestCluster] += point;
            clusterCounts[closestCluster]++;
        }

        for (int i = 0; i < k; i++) {
            if (clusterCounts[i] > 0) {
                newClusters[i] /= clusterCounts[i];
            }
        }

        converged = true;
        for (int i = 0; i < k; i++) {
            if (std::abs(newClusters[i] - clusters[i]) > 1e-6) {
                converged = false;
                break;
            }
        }

        clusters = newClusters;
    }

    std::vector<int> closestCluster(X.size());

    for (size_t i = 0; i < X.size(); i++) {
        double minDistance = std::numeric_limits<double>::max();
        int closest = 0;

        for (int j = 0; j < k; j++) {
            double distance = std::abs(X[i] - clusters[j]);
            if (distance < minDistance) {
                minDistance = distance;
                closest = j;
            }
        }

        closestCluster[i] = closest;
    }

    std::vector<int> clustersWithNoPoints;
    for (int i = 0; i < k; i++) {
        std::vector<double> pointsForCluster;

        for (size_t j = 0; j < X.size(); j++) {
            if (closestCluster[j] == i) {
                pointsForCluster.push_back(X[j]);
            }
        }

        if (pointsForCluster.size() < 2) {
            clustersWithNoPoints.push_back(i);
            continue;
        }

        stds[i] = std::sqrt(variance(pointsForCluster));
    }

    if (!clustersWithNoPoints.empty()) {
        std::vector<double> pointsToAverage;
        for (int i = 0; i < k; i++) {
            if (std::find(clustersWithNoPoints.begin(), clustersWithNoPoints.end(), i) == clustersWithNoPoints.end()) {
                for (size_t j = 0; j < X.size(); j++) {
                    if (closestCluster[j] == i) {
                        pointsToAverage.push_back(X[j]);
                    }
                }
            }
        }

        double meanStd = std::sqrt(variance(pointsToAverage));
        for (int clusterIndex : clustersWithNoPoints) {
            stds[clusterIndex] = meanStd;
        }
    }

    return std::make_pair(clusters, stds);
}

void RBF::fit(const std::vector<double>& X, const std::vector<double>& y) {
    if (inferStds) {
        // compute stds from data
        std::tie(centers, stds) = kmeans(X, k);
    } else {
        // use a fixed std
        std::tie(centers, std::ignore) = kmeans(X, k);
        double dMax = 0.0;
        for (size_t i = 0; i < centers.size(); i++) {
            for (size_t j = i + 1; j < centers.size(); j++) {
                double d = std::abs(centers[i] - centers[j]);
                if (d > dMax) {
                    dMax = d;
                }
            }
        }
        double stdValue = dMax / std::sqrt(2 * k);
        stds = std::vector<double>(k, stdValue);
    }

    for (int epoch = 0; epoch < epochs; epoch++) {
        for (size_t i = 0; i < X.size(); i++) {
            // forward pass
            std::vector<double> a;
            for (size_t j = 0; j < centers.size(); j++) {
                a.push_back(rbf(X[i], centers[j], stds[j]));
            }
            double F = 0.0;
            for (size_t j = 0; j < centers.size(); j++) {
                F += a[j] * w[j];
            }
            F += b[0];
            double loss = pow(y[i] - F, 2);


            // backward pass
            double error = -(y[i] - F);

            // online update
            for (size_t j = 0; j < centers.size(); j++) {
                w[j] -= lr * a[j] * error;
            }
            b[0] -= lr * error;
        }
    }
}

std::vector<double> RBF::predict(const std::vector<double>& X) {
    std::vector<double> y_pred;
    for (size_t i = 0; i < X.size(); i++) {
        std::vector<double> a;
        for (size_t j = 0; j < centers.size(); j++) {
            a.push_back(rbf(X[i], centers[j], stds[j]));
        }
        double F = 0.0;
        for (size_t j = 0; j < centers.size(); j++) {
            F += a[j] * w[j];
        }
        F += b[0];
        y_pred.push_back(F);
    }
    return y_pred;
}
