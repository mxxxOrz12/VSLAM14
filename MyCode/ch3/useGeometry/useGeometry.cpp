#include <iostream>
#include <cmath>

using namespace std;
#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace Eigen;

int main(int argc, char *argv[])
{
     Matrix3d rotation_matrix = Matrix3d::Identity();

     AngleAxisd rotation_vector(M_PI / 4, Vector3d(0, 0, 1));

     cout.precision(3);
     cout << "旋转矩阵： \n"
          << rotation_vector.matrix() << endl;

     rotation_matrix = rotation_vector.toRotationMatrix();

     cout << "旋转轴： \n"
          << rotation_vector.axis().transpose() << endl;
     cout << "旋转角： " << rotation_vector.angle() << endl;

     Vector3d v(1, 0, 0);
     Vector3d v_rotated = rotation_vector * v;
     cout << "(1,0,0)经过旋转向量旋转后 = " << v_rotated.transpose() << endl;

     v_rotated = rotation_matrix * v;

     cout << "(1,0,0)经过旋转矩阵旋转后 = " << v_rotated.transpose() << endl;

     // 欧拉角，将旋转矩阵转换为欧拉角
     Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0); // ZYX yaw pitch row
     cout << "yaw pitch roll = " << euler_angles.transpose() << endl;

     // 欧式变换矩阵 Eigen::Isometry

     Isometry3d T = Isometry3d::Identity(); // 实际为4*4 的矩阵 初始化为单位矩阵
     T.rotate(rotation_matrix);             // 旋转矩阵赋值

     T.pretranslate(Vector3d(1, 3, 4));

     cout << "变换矩阵： \n"
          << T.matrix() << endl;

     // 用变换矩阵进行坐标变换
     Vector3d v_transformed = T * v;
     cout << "v transformed = \n"
          << v_transformed.transpose() << endl;

     // 四元数
     // 可以直接把轴角赋值给四元数
     Quaterniond q = Quaterniond(rotation_vector);
     cout << "从轴角变换得到的四元数 = " << q.coeffs().transpose() << endl;

     q = Quaterniond(rotation_matrix);
     cout << "从旋转矩阵变换得到的四元数 = " << q.coeffs().transpose() << endl;

     v_rotated = q * v; // 数学上 qvq^{-1}

     cout << "(1,0,0) 经过四元数旋转后 =  " << v_rotated.transpose() << endl;

     cout << "等于 " << (q * Quaterniond(0, 1, 0, 0) * q.inverse()).coeffs().transpose() << endl;

     return 0;
}