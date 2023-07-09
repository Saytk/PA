// rbf.h
#ifndef RBF_H
#define RBF_H

#include <vector>

class RBF {
public:
    RBF(int k = 2, double lr = 0.01, int epochs = 100, bool inferStds = true);

    double rbf(double x, double c, double s);
    std::pair<std::vector<double>, std::vector<double>> kmeans(const std::vector<double>& X, int k);
    void fit(const std::vector<double>& X, const std::vector<double>& y);
    std::vector<double> predict(const std::vector<double>& X);

private:
    int k;
    double lr;
    int epochs;
    bool inferStds;
    std::vector<double> w;
    std::vector<double> b;
    std::vector<double> centers;
    std::vector<double> stds;

    void initializeWeights();
    double variance(const std::vector<double>& data);
};

#endif // RBF_H
