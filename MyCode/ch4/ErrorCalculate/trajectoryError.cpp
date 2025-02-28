#include <iostream>
#include <fstream>
#include <Eigen/Core>
#include <Eigen/Geometry>
// #include <pangolin/pangolin.h>
#include "sophus/se3.hpp"

using namespace Sophus;
using namespace std;

string groundtruth_file = "./groundtruth.txt";
string estimated_file = "./estimated.txt";

typedef vector<Sophus::SE3d, Eigen::aligned_allocator<Sophus::SE3d>> TrajectoryType;

void DrawTrajectory(const TrajectoryType *gt, const TrajectoryType &esti);

TrajectoryType ReadTrajectory(const string &path);

int main(int argc, char *argv[])
{
    TrajectoryType groundtruth = ReadTrajectory(groundtruth_file);
    TrajectoryType estimated = ReadTrajectory(estimated_file);

    assert(!groundtruth.empty() && !estimated.empty());
    assert(groundtruth.size() == estimated.size());

    // compute rmse
    double rmse = 0;
    for (size_t i = 0; i < estimated.size(); i++)
    {
        Sophus::SE3d p1 = estimated[i], p2 = groundtruth[i];
        double error = (p2.inverse() * p1).log().norm();
        rmse += error * error;
    }
    rmse = rmse / double(estimated.size());
    rmse = sqrt(rmse);

    cout << "RMSE = " << rmse << endl;

    return 0;
}

TrajectoryType ReadTrajectory(const string &path)
{
    ifstream fin(path);
    TrajectoryType trajectory;
    if (!fin)
    {
        return trajectory;
    }
    while (!fin.eof())
    {
        double time, tx, ty, tz, qx, qy, qz, qw;
        fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;

        Sophus::SE3d p1(Eigen::Quaterniond(qw, qx, qy, qz), Eigen::Vector3d(tx, ty, tz));
        trajectory.push_back(p1);
    }
    return trajectory;
}