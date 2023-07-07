#define EXPORT_API __declspec(dllexport)
#include <Eigen/Dense>
using namespace Eigen;

extern "C" {
EXPORT_API float getTrace() {
    MatrixXf m = MatrixXf::Random(4,4);
    return m.trace();
}
}