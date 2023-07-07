#ifndef SVM_H
#define SVM_H

#include <Eigen/Core>
#include <vector>

class SVM {
private:
    double learning_rate;
    double lambda_param;
    int n_iters;
    Eigen::VectorXd w;
    double b;
    std::vector<int> cls_map;

    void _init_weights_bias(const Eigen::MatrixXd& X);
    std::vector<int> _get_cls_map(const std::vector<int>& y);
    bool _satisfy_constraint(const Eigen::VectorXd& x, int idx);
    void _get_gradients(bool constrain, const Eigen::VectorXd& x, int idx, Eigen::VectorXd& dw, double& db);
    void _update_weights_bias(const Eigen::VectorXd& dw, double db);

public:
    SVM(double learning_rate = 1e-3, double lambda_param = 1e-2, int n_iters = 1000);
    void fit(const Eigen::MatrixXd& X, const std::vector<int>& y);
    std::vector<int> predict(const Eigen::MatrixXd& X);
};

#endif
