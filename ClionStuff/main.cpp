#include "MultLayerPerceptron.h"
#include <Eigen/Dense>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    // create a small toy dataset for training and testing (XOR problem)
    vector<Eigen::VectorXd> inputs = {Eigen::VectorXd(2), Eigen::VectorXd(2), Eigen::VectorXd(2), Eigen::VectorXd(2)};
    vector<Eigen::VectorXd> targets = {Eigen::VectorXd(1), Eigen::VectorXd(1), Eigen::VectorXd(1), Eigen::VectorXd(1)};

    inputs[0] << 0, 0;
    targets[0] << 0;

    inputs[1] << 0, 1;
    targets[1] << 1;

    inputs[2] << 1, 0;
    targets[2] << 1;

    inputs[3] << 1, 1;
    targets[3] << 0;

    // create a multilayer perceptron with 2 inputs, 1 output, 2 hidden layers, and 5 neurons per hidden layer
    MultLayerPerceptron mlp(2, 1, 2, 5, 0.01, "tanh");

    // train the MLP with the dataset
    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < inputs.size(); j++) {
            mlp.train(inputs[j], targets[j]);
        }
    }

    // use the trained MLP to predict the output for the same dataset
    for (int j = 0; j < inputs.size(); j++) {
        Eigen::VectorXd output = mlp.predict(inputs[j]);
        cout << "Input: " << inputs[j].transpose()
             << ", Predicted output: " << output.transpose()
             << ", Target output: " << targets[j].transpose() << endl;
    }

    return 0;
}
