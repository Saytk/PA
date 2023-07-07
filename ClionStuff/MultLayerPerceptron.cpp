#include "MultLayerPerceptron.h"
#include <Eigen/Dense>
Layer::Layer(int num_inputs, int num_outputs,
             function<Eigen::VectorXd(Eigen::VectorXd)> activation,
             function<Eigen::VectorXd(Eigen::VectorXd)> activation_derivative)
{
    this->weights = Eigen::MatrixXd::Random(num_outputs, num_inputs) / sqrt(num_inputs);
    this->bias = Eigen::VectorXd::Random(num_outputs) / sqrt(num_inputs);
    this->activation = activation;
    this->activation_derivative = activation_derivative;
}

Eigen::MatrixXd Layer::getWeights() {
    return weights;
}
Eigen::VectorXd Layer::getBias() {
    return bias;
}
void Layer::setWeights(Eigen::MatrixXd newWeights) {
    weights = newWeights;
}
void Layer::setBias(Eigen::VectorXd newBias) {
    bias = newBias;
}
Eigen::VectorXd Layer::applyActivation(Eigen::VectorXd x) {
    return activation(x);
}
Eigen::VectorXd Layer::applyActivationDerivative(Eigen::VectorXd x) {
    return activation_derivative(x);
}

MultLayerPerceptron::MultLayerPerceptron(int num_inputs, int num_outputs, int num_hidden_layers,
                                         int num_neurons_per_hidden_layer, double learning_rate, string activation_func_name)
{
    this->num_inputs = num_inputs;
    this->num_outputs = num_outputs;
    this->num_hidden_layers = num_hidden_layers;
    this->num_neurons_per_hidden_layer = num_neurons_per_hidden_layer;
    this->learning_rate = learning_rate;

    activation_functions["sigmoid"] = [](Eigen::VectorXd x) { return 1 / (1 + (-x.array()).exp()); };
    activation_functions["tanh"] = [](Eigen::VectorXd x) { return x.array().tanh(); };
    activation_functions["relu"] = [](Eigen::VectorXd x) { return x.cwiseMax(0); };

    activation_derivatives["sigmoid"] = [](Eigen::VectorXd x) { return x.array() * (1 - x.array()); };
    activation_derivatives["tanh"] = [](Eigen::VectorXd x) { return 1 - x.array().pow(2); };
    activation_derivatives["relu"] = [](Eigen::VectorXd x) { return (x.array() > 0).cast<double>(); };

    auto activation = activation_functions[activation_func_name];
    auto activation_derivative = activation_derivatives[activation_func_name];

    for (int i = 0; i < num_hidden_layers; i++)
    {
        if (i == 0)
        {
            layers.push_back(Layer(num_inputs, num_neurons_per_hidden_layer, activation, activation_derivative));
        }
        else
        {
            layers.push_back(Layer(num_neurons_per_hidden_layer, num_neurons_per_hidden_layer, activation, activation_derivative));
        }
    }

    layers.push_back(Layer(num_neurons_per_hidden_layer, num_outputs, activation, activation_derivative));
}

Eigen::VectorXd MultLayerPerceptron::predict(Eigen::VectorXd input) {
    Eigen::VectorXd output = input;
    for (int i = 0; i < layers.size(); i++)
    {
        output = layers[i].getWeights() * output + layers[i].getBias();
        output = layers[i].applyActivation(output);
    }
    return output;
}

void MultLayerPerceptron::train(Eigen::VectorXd input, Eigen::VectorXd target) {
    Eigen::VectorXd output = input;
    vector<Eigen::VectorXd> outputs;
    outputs.push_back(output);
    for (int i = 0; i < layers.size(); i++)
    {
        output = layers[i].getWeights() * output + layers[i].getBias();
        output = layers[i].applyActivation(output);
        outputs.push_back(output);
    }

    Eigen::VectorXd error = target - output;
    for (int i = layers.size() - 1; i >= 0; i--)
    {
        error = layers[i].applyActivationDerivative(outputs[i + 1]).cwiseProduct(error);
        Eigen::MatrixXd delta = learning_rate * error * outputs[i].transpose();
        layers[i].setWeights(layers[i].getWeights() + delta);
        layers[i].setBias(layers[i].getBias() + learning_rate * error);

        if (i != 0)
        {
            error = layers[i].getWeights().transpose() * error;
        }
    }
}
