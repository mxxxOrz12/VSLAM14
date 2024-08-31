#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;

string image_file = "/home/maxiao/VSLAM_Learn/MyCode/ch5/imageBasic/distorted.png";

int main(int argc, char *argv[])
{
    double k1 = -0.28340811, k2 = 0.07395907, p1 = 0.00019359, p2 = 1.76187114e-05;

    double fx = 458.654, fy = 457.296, cx = 367.215, cy = 248.375;

    cv::Mat img = cv::imread(image_file, 0);
    int rows = img.rows, cols = img.cols;
    cv::Mat img_undistort = cv::Mat(rows, cols, CV_8UC1);

    for (int v = 0; v < rows; v++)
    {
        for (int u = 0; u < cols; u++)
        {
            double x = (u - cx) / fx, y = (v - cy) / fy;
            double r = sqrt(pow(x, 2) + pow(y, 2));
            double x_distorted = x * (1 + k1 * pow(r, 2) + k2 * pow(r, 4)) + 2 * p1 * x * y + p2 * (pow(r, 2) + 2 * pow(x, 2));

            double y_distorted = y * (1 + k1 * pow(r, 2) + k2 * pow(r, 4)) + p1 * (pow(r, 2) + 2 * pow(y, 2)) + 2 * p2 * x * y;

            double u_distorted = fx * x_distorted + cx;
            double v_distorted = fy * y_distorted + cy;

            if (u_distorted >= 0 && v_distorted >= 0 && u_distorted < cols && v_distorted < rows)
            {
                img_undistort.at<uchar>(v, u) = img.at<uchar>((int)v_distorted, (int)u_distorted);
            }
            else
            {
                img_undistort.at<uchar>(v, u) = 0;
            }
        }
    }

    // cv::imshow("distorted", img);
    // cv::imshow("undistorted", img_undistort);
    // cv::waitKey(0);

    uchar pixel_value = img.at<uchar>(10, 40);
    cout << "像素值为： " << pixel_value << endl;

    return 0;
}