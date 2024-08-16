#include <iostream>
using namespace std;

#include <ctime>
#include <Eigen/Core>

#include <Eigen/Dense>
using namespace Eigen;

#define Matrix_SIZE 50

int main(int argc, char *argv[])
{
    // Matrix<float, 2, 3> matrix_23;

    // Vector3d v_3d;
    // // Matrix<double,3,1> 三维向量
    // Matrix<float, 3, 1> vd_3d;

    // // 初始化为0
    // Matrix3d matrix_33 = Matrix3d::Zero();
    // // Matrix<double,3,3>

    // // 动态大小的矩阵
    // Matrix<double, Dynamic, Dynamic> matrix_dynamic;

    // // 对Eigen定义的矩阵进行一些简单操作

    // matrix_23 << 1, 2, 3, 5, 7, 4;
    // cout << "矩阵matrix_23:\n"
    //      << matrix_23 << endl;

    // for (size_t i = 0; i < 2; i++)
    // {
    //     for (size_t j = 0; j < 3; j++)
    //     {
    //         cout << matrix_23(i, j) << "\t";
    //     }
    //     cout << endl;
    // }

    // // 矩阵乘法
    // v_3d << 3, 2, 1;
    // vd_3d << 4, 5, 6;

    // // 不能对两种不同类型的矩阵做乘法
    // Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
    // cout << "[1,2,3;5,7,4] * [3,2,1] = " << result << endl; // Matrix<float, 2, 3> matrix_23;

    // Vector3d v_3d;
    // // Matrix<double,3,1> 三维向量
    // Matrix<float, 3, 1> vd_3d;

    // // 初始化为0
    // Matrix3d matrix_33 = Matrix3d::Zero();
    // // Matrix<double,3,3>

    // // 动态大小的矩阵
    // Matrix<double, Dynamic, Dynamic> matrix_dynamic;

    // // 对Eigen定义的矩阵进行一些简单操作

    // matrix_23 << 1, 2, 3, 5, 7, 4;
    // cout << "矩阵matrix_23:\n"
    //      << matrix_23 << endl;

    // for (size_t i = 0; i < 2; i++)
    // {
    //     for (size_t j = 0; j < 3; j++)
    //     {
    //         cout << matrix_23(i, j) << "\t";
    //     }
    //     cout << endl;
    // }

    // // 矩阵乘法
    // v_3d << 3, 2, 1;
    // vd_3d << 4, 5, 6;

    // // 不能对两种不同类型的矩阵做乘法
    // Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
    // cout << "[1,2,3;5,7,4] * [3,2,1] = " << result << endl;

    // // Matrix<double, 2, 3> result_wrong_dimension = matrix_23.cast<double>() * v_3d;

    // matrix_33 = Matrix3d::Random();
    // cout << "随机数矩阵： \n"
    //      << matrix_33 << endl;

    // cout << "matrix33矩阵的和: " << matrix_33.sum() << endl;
    // cout << "matrix33矩阵的迹是: " << matrix_33.trace() << endl;
    // cout << "matrix33矩阵的逆是: \n"
    //      << matrix_33.inverse() << endl;
    // cout << "matrix33矩阵的行列式: " << matrix_33.determinant() << endl;

    // // 特征值

    // SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);
    // cout << "特征值： \n"
    //      << eigen_solver.eigenvalues() << endl;
    // cout << "特征向量： \n"
    //      << eigen_solver.eigenvectors() << endl;

    // // Matrix<double, 2, 3> result_wrong_dimension = matrix_23.cast<double>() * v_3d;

    // matrix_33 = Matrix3d::Random();
    // cout << "随机数矩阵： \n"
    //      << matrix_33 << endl;

    // cout << "matrix33矩阵的和: " << matrix_33.sum() << endl;
    // cout << "matrix33矩阵的迹是: " << matrix_33.trace() << endl;
    // cout << "matrix33矩阵的逆是: \n"
    //      << matrix_33.inverse() << endl;
    // cout << "matrix33矩阵的行列式: " << matrix_33.determinant() << endl;

    // // 特征值

    // SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);
    // cout << "特征值： \n"
    //      << eigen_solver.eigenvalues() << endl;
    // cout << "特征向量： \n"
    //      << eigen_solver.eigenvectors() << endl;

    Matrix<double, Matrix_SIZE, Matrix_SIZE> matrix_NN = MatrixXd::Random(Matrix_SIZE, Matrix_SIZE);
    matrix_NN = matrix_NN.transpose() * matrix_NN;
    Matrix<double, Matrix_SIZE, 1> v_Nd = MatrixXd::Random(Matrix_SIZE, 1);

    clock_t time_stt = clock();
    Matrix<double, Matrix_SIZE, 1> x = matrix_NN.inverse() * v_Nd;
    cout << "直接求逆矩阵求x花费时间：" << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;

    time_stt = clock();
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
    cout << "QR分解求逆矩阵求x花费时间：" << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;
}
