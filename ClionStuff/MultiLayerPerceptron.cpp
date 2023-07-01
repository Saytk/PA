//
// Created by erwan on 02/06/2023.
//

#include "MultiLayerPerceptron.h"
#include <Eigen/Dense>
#include <exception>
#include <map>
#include <functional>
#include "iostream"
using namespace std;



#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif
int DLL_EXPORT hello()
{
    return 3;
}

