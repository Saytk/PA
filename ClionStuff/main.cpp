#include "MultLayerPerceptron.h"
#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include "SVM.h"
#include <Eigen/Core>
using namespace std;

int main() {
    if(false) {

        //region MLP_test
        // create a small toy dataset for training and testing (XOR problem)
        vector<Eigen::VectorXd> inputs = {Eigen::VectorXd(2), Eigen::VectorXd(2), Eigen::VectorXd(2),
                                          Eigen::VectorXd(2)};
        vector<Eigen::VectorXd> targets = {Eigen::VectorXd(1), Eigen::VectorXd(1), Eigen::VectorXd(1),
                                           Eigen::VectorXd(1)};

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
        //endregion

    }
    if(true){

        //region SVM_test
        //!!!!---------------SVM------------!!!!
        // Données d'entraînement
        Eigen::MatrixXd X(10, 2);
        X << 1.0, 2.0,
                2.0, 3.0,
                3.5, 3.8,
                0.0, 0.0,
                2.0, 2.0,
                3.0, 3.15,
                5.5, 6.3,
                7.8, 9.3,
                7.3, 6.9,
                6.2, 8.6;

        std::vector<int> y = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1};

        // Création et entraînement du modèle SVM
        SVM model;
        model.fit(X, y);

        // Données de test
        Eigen::MatrixXd X_test(10, 2);
        X_test << 1.0, 1.0,
                2.0, 2.0,
                3.0, 3.0,
                4.0, 4.0,
                5.0, 5.0,
                6.0, 6.0,
                7.0, 7.0,
                8.0, 8.0,
                9.0, 9.0,
                10.0, 10.0;
        std::vector<int> y_test = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1};

        // Prédiction des classes pour les données de test
        std::vector<int> predictions = model.predict(X_test);

        // Comparaison avec les étiquettes réelles
        for (int i = 0; i < predictions.size(); i++) {
            std::cout << "Point (" << X_test(i, 0) << ", " << X_test(i, 1) << ") : Classe prédite = " << predictions[i] << ", Classe réelle = " << y_test[i] << std::endl;
        }
	   //affichage de la fonction linéaire résultante au modèle
        std::cout << "y = (" << (model.w[0] / model.w[1])*-1 << ")x + (" <<(model.b / model.w[1])*-1 << ")" << std::endl;

        //endregion

    }
    return 0;
}
