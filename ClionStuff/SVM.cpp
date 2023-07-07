#include "SVM.h"

#include <Eigen/Core>

//initialise les poids et le biais
void SVM::_init_weights_bias(const Eigen::MatrixXd& X) {
    int n_features = X.cols();
    w = Eigen::VectorXd::Zero(n_features);
    b = 0.0;
}

//change les labels de -1 à 0
std::vector<int> SVM::_get_cls_map(const std::vector<int>& y) {
    std::vector<int> cls_map;
    for (int i = 0; i < y.size(); i++) {
        cls_map.push_back((y[i] <= 0) ? -1 : 1);
    }
    return cls_map;
}

//vérifie si la contraintes est satisfaites
bool SVM::_satisfy_constraint(const Eigen::VectorXd& x, int idx) {
    double linear_model = x.dot(w) + b;
    return cls_map[idx] * linear_model >= 1;
}

//met à jour les poids avec le gradient
void SVM::_get_gradients(bool constrain, const Eigen::VectorXd& x, int idx, Eigen::VectorXd& dw, double& db) {
// si les point sont sur le bon coté
    if (constrain) {
        dw = lambda_param * w;
        db = 0.0;
        return;
    }
//si les points sont sur le mauvais coté
    dw = lambda_param * w - cls_map[idx] * x;
    db = -cls_map[idx];
}

//met à jour les poids et le biais
void SVM::_update_weights_bias(const Eigen::VectorXd& dw, double db) {
    w -= learning_rate * dw;
    b -= learning_rate * db;
}

SVM::SVM(double learning_rate, double lambda_param, int n_iters)
        : learning_rate(learning_rate), lambda_param(lambda_param), n_iters(n_iters) {}


 //utilise les méthodes d'initialisation et entraine le model
void SVM::fit(const Eigen::MatrixXd& X, const std::vector<int>& y) {
    _init_weights_bias(X);
    cls_map = _get_cls_map(y);

    for (int iter = 0; iter < n_iters; iter++) {
        for (int idx = 0; idx < X.rows(); idx++) {
            //verifie si les point valident la contrainte
            bool constrain = _satisfy_constraint(X.row(idx).transpose(), idx);
            Eigen::VectorXd dw(X.cols());
            double db;
            _get_gradients(constrain, X.row(idx).transpose(), idx, dw, db);
            _update_weights_bias(dw, db);
        }
    }
}
//predictions du model
std::vector<int> SVM::predict(const Eigen::MatrixXd& X) {
    std::vector<int> predictions;
    for (int i = 0; i < X.rows(); i++) {
        double estimate = X.row(i).dot(w) + b;
        int prediction = (estimate < 0) ? 0 : 1;
        predictions.push_back(prediction);
    }
    return predictions;
}
