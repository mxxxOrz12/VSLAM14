#include <iostream>
#include <Eigen/Dense>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello SLAM!" << endl;
    Eigen::MatrixXd mat1(3, 3);
    mat1 << 1, 2, 3,
        4, 5, 6,
        7, 8, 9;
    cout << mat1 << endl;

    Eigen::Vector3d v(1, 2, 3);
    Eigen::MatrixXd result = mat1 * v;

    cout << result << endl;
    return 0;
}