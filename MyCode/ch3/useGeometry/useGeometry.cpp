#include <iostream>
#include <cmath>

using namespace std;
#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace Eigen;

int main(int argc, char *argv[])
{
    Matrix3d rotation_matrix = Matrix3d::Identity();

    AngleAxis rotation_vector(M_PI / 4, Vector3d(0, 0, 1));

    cout.precision(3);
    cout << "旋转矩阵： \n"
         << rotation_vector.matrix() << endl;

    rotation_matrix = rotation_vector.toRotationMatrix();

    Vector3d v(1, 0, 0);
    Vector3d v_rotated = rotation_vector * v;
    cout << "(1,0,0)经过旋转向量旋转后 = " << v_rotated.transpose() << endl;

    v_rotated = rotation_matrix * v;

    cout << "(1,0,0)经过旋转矩阵旋转后 = " << v_rotated.transpose() << endl;

    // 欧拉角，将旋转矩阵转换为欧拉角
    Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0); // ZYX yaw pitch row
}