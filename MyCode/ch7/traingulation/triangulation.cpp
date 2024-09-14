/*
 * @Author: mxxxOrz
 * @Date: 2024-09-07 13:44:28
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-09-08 18:43:48
 * @Description: 三角测量恢复深度
 *
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include <opencv2/imgproc.hpp>
#include "sophus/se3.hpp"
#include "sophus/so3.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace std;
using namespace cv;
using namespace Eigen;
using namespace Sophus;

void find_feature_matches(const Mat &img_1, const Mat &img_2,
                          vector<KeyPoint> &keypoints_1, vector<KeyPoint> &keypoints_2,
                          vector<DMatch> &matches);

void pose_estimation_2d2d(vector<KeyPoint> keypoints_1, vector<KeyPoint> keypoints_2,
                          vector<DMatch> matches, Mat &R, Mat &t);

Point2d pixel2cam(const Point2d &p, const Mat &K);

void triangulation(const vector<KeyPoint> &keypoint_1, const vector<KeyPoint> &keypoint_2,
                   const vector<DMatch> &matches, const Mat &R, const Mat &t,
                   vector<Point3d> &points);

bool depthFromTriangulation(
    const SE3d &T_search_ref,
    const Vector3d &f_ref,
    const Vector3d &f_cur,
    double &depth)
{
    Eigen::Matrix<double, 3, 1> A;
    A << T_search_ref.rotationMatrix() * f_ref, f_cur;
    const Matrix2d AtA = A.transpose() * A;
    if (AtA.determinant() < 0.000001)
        return false;
    const Vector2d depth2 = -AtA.inverse() * A.transpose() * T_search_ref.translation();
    depth = fabs(depth2[0]);
    return true;
}

inline ::cv::Scalar get_color(float depth)
{
    float up_th = 50, low_th = 10, th_range = up_th - low_th;
    if (depth > up_th)
        depth = up_th;
    if (depth < low_th)
        depth = low_th;
    return cv::Scalar(255 * depth / th_range, 0, 255 * (1 - depth / th_range));
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "请输入两张照片" << endl;
        return 1;
    }
    Mat img_1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    Mat img_2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);
    assert(img_1.data && img_2.data);

    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;

    find_feature_matches(img_1, img_2, keypoints_1, keypoints_2, matches);
    cout << "一共找到了 " << matches.size() << "组匹配点" << endl;

    Mat R, t;
    pose_estimation_2d2d(keypoints_1, keypoints_2, matches, R, t);

    // Mat t_x = (Mat_<double>(3, 3) << 0, -t.at<double>(2, 0), t.at<double>(1, 0),
    //            t.at<double>(2, 0), 0, -t.at<double>(0, 0),
    //            -t.at<double>(1, 0), t.at<double>(0, 0), 0);

    // cout << "t^R = " << endl
    //      << t_x * R << endl;

    // // 验证对极约束
    // Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);
    // for (DMatch D : matches)
    // {
    //     Point2d pt1 = pixel2cam(keypoints_1[D.queryIdx].pt, K);
    //     Mat y1 = (Mat_<double>(3, 1) << pt1.x, pt1.y, 1);
    //     Point2d pt2 = pixel2cam(keypoints_2[D.trainIdx].pt, K);
    //     Mat y2 = (Mat_<double>(3, 1) << pt2.x, pt2.y, 1);
    //     Mat d = y2.t() * t_x * R * y1;                 // x2T * t^R *x1 = 0;
    //     cout << "epipolar constraint = " << d << endl; // d = 0
    // }

    vector<Point3d> points;
    triangulation(keypoints_1, keypoints_2, matches, R, t, points);

    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);
    Mat img1_clone = img_1.clone();
    Mat img2_clone = img_2.clone();

    for (int i = 0; i < matches.size(); i++)
    {
        float depth1 = points[i].z;
        cout << "depth: " << depth1 << endl;
        Point2d pt1_cam = pixel2cam(keypoints_1[matches[i].queryIdx].pt, K);
        cv::circle(img1_clone, keypoints_1[matches[i].queryIdx].pt, 2, get_color(depth1), 2);

        Mat pt2_trans = R * (Mat_<double>(3, 1) << points[i].x, points[i].y, points[i].z) + t;
        float depth2 = pt2_trans.at<double>(2, 0);
        cv::circle(img2_clone, keypoints_2[matches[i].trainIdx].pt, 2, get_color(depth2), 2);
    }
    cv::imshow("img1", img1_clone);
    cv::imshow("img2", img2_clone);
    cv::waitKey(0);

    return 0;
}

void find_feature_matches(const Mat &img_1, const Mat &img_2,
                          vector<KeyPoint> &keypoints_1, vector<KeyPoint> &keypoints_2,
                          vector<DMatch> &matches)
{
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);

    vector<DMatch> match;
    matcher->match(descriptors_1, descriptors_2, match);

    // 找出所有匹配点之间的最小距离和最大距离
    double min_dist = 10000, max_dist = 0;
    for (int i = 0; i < descriptors_1.rows; i++)
    {
        double dist = match[i].distance;
        if (dist > max_dist)
            max_dist = dist;
        if (dist < min_dist)
            min_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    for (int i = 0; i < descriptors_1.rows; i++)
    {
        if (match[i].distance <= max(2 * min_dist, 30.0))
        {
            matches.push_back(match[i]);
        }
    }
}

/**
 * @description: 将像素坐标转为空间坐标,Z=1
 * @param {Point2d} &p  像素点坐标
 * @param {Mat} &K  相机内参
 * @return {*}
 */
Point2d pixel2cam(const Point2d &p, const Mat &K)
{
    return Point2d(
        (p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
        (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1));
}

void pose_estimation_2d2d(vector<KeyPoint> keypoints_1, vector<KeyPoint> keypoints_2,
                          vector<DMatch> matches, Mat &R, Mat &t)
{
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);

    // 将匹配点转换为Point2f
    vector<Point2f> points1;
    vector<Point2f> points2;

    for (size_t i = 0; i < (int)matches.size(); i++)
    {
        points1.push_back(keypoints_1[matches[i].queryIdx].pt);
        points2.push_back(keypoints_2[matches[i].trainIdx].pt);
    }

    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat(points1, points2, CV_FM_8POINT);
    cout << "fundamental_matrix is " << endl
         << fundamental_matrix << endl;

    Point2d principal_point(325.1, 249.7);
    double focal_length = 521;
    Mat essential_matrix;
    essential_matrix = findEssentialMat(points1, points2, focal_length, principal_point);
    cout << "essential_matrix is" << endl
         << essential_matrix << endl;

    Mat homography_matrix;
    homography_matrix = findHomography(points1, points2, RANSAC, 3);

    // 从本质矩阵中恢复旋转和平移信息，R t
    recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point);

    cout << "R is " << endl
         << R << endl;
    cout << "t is " << endl
         << t << endl;
}

void triangulation(const vector<KeyPoint> &keypoint_1, const vector<KeyPoint> &keypoint_2,
                   const vector<DMatch> &matches, const Mat &R, const Mat &t,
                   vector<Point3d> &points)
{
    Mat T1 = (Mat_<float>(3, 4) << 1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0);
    Mat T2 = (Mat_<float>(3, 4) << R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), t.at<double>(0, 0),
              R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2), t.at<double>(1, 0),
              R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2), t.at<double>(2, 0));
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);

    // 初始化左右两张图片的特征点（相机坐标）
    vector<Point2f> pts_1, pts_2;
    for (DMatch m : matches)
    {
        pts_1.push_back(pixel2cam(keypoint_1[m.queryIdx].pt, K));
        pts_2.push_back(pixel2cam(keypoint_2[m.trainIdx].pt, K));
    }
    Mat pts_4d;
    cv::triangulatePoints(T1, T2, pts_1, pts_2, pts_4d);

    // 转换成非齐次坐标
    for (int i = 0; i < pts_4d.cols; i++)
    {
        Mat x = pts_4d.col(i);
        x /= x.at<float>(3, 0);
        Point3d p(
            x.at<float>(0, 0),
            x.at<float>(1, 0),
            x.at<float>(2, 0));

        points.push_back(p); // 带有深度的点坐标
    }
}
