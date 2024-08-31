// #include <pangolin/pangolin.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;
using namespace Eigen;

string trajectory_file = "/home/maxiao/VSLAM_Learn/MyCode/ch3/plotTrajectory/trajectory.txt";

void DrawTrajectory(vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>>);

int main(int argc, char *argv[])
{
    vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>> poses;
    ifstream fin(trajectory_file);

    if (!fin)
    {
        return 1;
    }

    while (!fin.eof())
    {
        double time, tx, ty, tz, qx, qy, qz, qw;
        fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        Isometry3d Twr(Quaterniond(qw, qx, qy, qz));
        Twr.pretranslate(Vector3d(tx, ty, tz));
        poses.push_back(Twr);
    }
    cout << "read total" << poses.size() << endl;

    return 0;
}