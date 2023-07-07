#ifndef PROJET_ANNUEL_MULTLAYERPERCEPTRON_H
#define PROJET_ANNUEL_MULTLAYERPERCEPTRON_H

#include <map>
#include <functional>
#include <vector>
#include <Eigen/Core>

using namespace std;

class Layer {
public:
    Layer(int num_inputs, int num_outputs,
          function<Eigen::VectorXd(Eigen::VectorXd)> activation,
          function<Eigen::VectorXd(Eigen::VectorXd)> activation_derivative);

    Eigen::MatrixXd getWeights();
    Eigen::VectorXd getBias();
    void setWeights(Eigen::MatrixXd newWeights);
    void setBias(Eigen::VectorXd newBias);

    Eigen::VectorXd applyActivation(Eigen::VectorXd x);
    Eigen::VectorXd applyActivationDerivative(Eigen::VectorXd x);

private:
    Eigen::MatrixXd weights;
    Eigen::VectorXd bias;
    function<Eigen::VectorXd(Eigen::VectorXd)> activation;
    function<Eigen::VectorXd(Eigen::VectorXd)> activation_derivative;
};

class MultLayerPerceptron {
public:
    MultLayerPerceptron(int num_inputs, int num_outputs, int num_hidden_layers,
                        int num_neurons_per_hidden_layer, double learning_rate, string activation_func_name);

    Eigen::VectorXd predict(Eigen::VectorXd input);
    void train(Eigen::VectorXd input, Eigen::VectorXd target);

private:
    vector<Layer> layers;
    int num_inputs;
    int num_outputs;
    int num_hidden_layers;
    int num_neurons_per_hidden_layer;
    double learning_rate;
    map<string, function<Eigen::VectorXd(Eigen::VectorXd)>> activation_functions;
    map<string, function<Eigen::VectorXd(Eigen::VectorXd)>> activation_derivatives;
};

#endif //PROJET_ANNUEL_MULTLAYERPERCEPTRON_H
