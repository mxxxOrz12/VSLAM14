#include <iostream>
#include <chrono>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    cv::Mat image;
    image = cv::imread("/home/maxiao/VSLAM_Learn/MyCode/ch5/imageBasic/ubuntu.png", 1);

    // if (image.data == nullptr)
    // {
    //     cerr << "文件" << argv[1] << "不存在 " << endl;
    //     return 0;
    // }

    cout << "图像宽为： " << image.cols << "高为： " << image.rows << ",通道数： " << image.channels() << endl;
    cv::imshow("image", image);
    cv::waitKey(0);

    if (image.type() != CV_8UC1 && image.type() != CV_8UC3)
    {
        cout << "请输入一张彩色图或灰度图" << endl;
        return 0;
    }

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    for (size_t y = 0; y < image.rows; y++)
    {
        unsigned char *row_ptr = image.ptr<unsigned char>(y);
        for (size_t x = 0; x < image.cols; x++)
        {
            unsigned char *data_ptr = &row_ptr[x * image.channels()];

            for (int c = 0; c != image.channels(); c++)
            {
                unsigned int data = data_ptr[c];
                // cout << "当前像素值为： " << data;
            }
            // cout << endl;
        }
    }

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);

    cout << "遍历图像用时： " << time_used.count() << "秒" << endl;

    // cv::Mat image_another = image;
    // image_another(cv::Rect(0, 0, 100, 100)).setTo(0);
    // cv::imshow("image", image);

    // cv::waitKey(0);

    cv::Mat image_clone = image.clone();
    image_clone(cv::Rect(0, 0, 100, 100)).setTo(255);

    cv::imshow("image_clone", image_clone);
    cv::imshow("image", image);

    cv::waitKey(0);

    return 0;
}