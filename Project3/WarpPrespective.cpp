#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "Header.h"
#include <opencv2/opencv.hpp>
//#include "Vec3b.h"
#include "WarpPrespective.h"
#include <vector>
using namespace std;
struct Pixel { unsigned char r; unsigned char g; unsigned char b; };

// Calculate the determinant of a 3x3 matrix
double determinant(const cv::Mat& mat) {
    if (mat.rows != 3 || mat.cols != 3) {
        throw std::invalid_argument("Matrix must be 3x3.");
    }
    return mat.at<double>(0, 0) * (mat.at<double>(1, 1) * mat.at<double>(2, 2) - mat.at<double>(1, 2) * mat.at<double>(2, 1)) -
        mat.at<double>(0, 1) * (mat.at<double>(1, 0) * mat.at<double>(2, 2) - mat.at<double>(1, 2) * mat.at<double>(2, 0)) +
        mat.at<double>(0, 2) * (mat.at<double>(1, 0) * mat.at<double>(2, 1) - mat.at<double>(1, 1) * mat.at<double>(2, 0));
}

// Calculate the adjoint (also called adjugate) of a 3x3 matrix
cv::Mat adjoint(const cv::Mat& mat) {
    if (mat.rows != 3 || mat.cols != 3) {
        throw std::invalid_argument("Matrix must be 3x3.");
    }

    cv::Mat adj(3, 3, CV_64F);
    adj.at<double>(0, 0) = mat.at<double>(1, 1) * mat.at<double>(2, 2) - mat.at<double>(1, 2) * mat.at<double>(2, 1);
    adj.at<double>(0, 1) = mat.at<double>(0, 2) * mat.at<double>(2, 1) - mat.at<double>(0, 1) * mat.at<double>(2, 2);
    adj.at<double>(0, 2) = mat.at<double>(0, 1) * mat.at<double>(1, 2) - mat.at<double>(0, 2) * mat.at<double>(1, 1);

    adj.at<double>(1, 0) = mat.at<double>(1, 2) * mat.at<double>(2, 0) - mat.at<double>(1, 0) * mat.at<double>(2, 2);
    adj.at<double>(1, 1) = mat.at<double>(0, 0) * mat.at<double>(2, 2) - mat.at<double>(0, 2) * mat.at<double>(2, 0);
    adj.at<double>(1, 2) = mat.at<double>(0, 2) * mat.at<double>(1, 0) - mat.at<double>(0, 0) * mat.at<double>(1, 2);

    adj.at<double>(2, 0) = mat.at<double>(1, 0) * mat.at<double>(2, 1) - mat.at<double>(1, 1) * mat.at<double>(2, 0);
    adj.at<double>(2, 1) = mat.at<double>(0, 1) * mat.at<double>(2, 0) - mat.at<double>(0, 0) * mat.at<double>(2, 1);
    adj.at<double>(2, 2) = mat.at<double>(0, 0) * mat.at<double>(1, 1) - mat.at<double>(0, 1) * mat.at<double>(1, 0);

    return adj;
}

// Calculate the inverse of a 3x3 matrix using the adjoint method
cv::Mat inverse(const cv::Mat& mat) {
    if (mat.rows != 3 || mat.cols != 3) {
        throw std::invalid_argument("Matrix must be 3x3.");
    }

    double det = determinant(mat);
    if (det == 0) {
        throw std::runtime_error("Matrix is singular and cannot be inverted.");
    }

    cv::Mat adj = adjoint(mat);
    cv::Mat inv(3, 3, CV_64F);

    // Inverse is adjoint / determinant
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            inv.at<double>(i, j) = adj.at<double>(i, j) / det;
        }
    }

    return inv;
}

// Implementing MyWarpPerspective
void WarpPerspective::MyWarpPerspective() {
    if (H.rows != 3 || H.cols != 3) {
        throw std::invalid_argument("Homography matrix H must be 3x3.");
    }

    dst.create(dsize, src.type());

    cv::Mat H_inv = inverse(H);

    for (int y_dst = 0; y_dst < dsize.height; ++y_dst) {
        for (int x_dst = 0; x_dst < dsize.width; ++x_dst) {
            cv::Mat src_pt(3, 1, CV_64F);
            src_pt.at<double>(0, 0) = x_dst;
            src_pt.at<double>(1, 0) = y_dst;
            src_pt.at<double>(2, 0) = 1;

            cv::Mat dst_pt = H_inv * src_pt;

            double x_src = dst_pt.at<double>(0, 0) / dst_pt.at<double>(2, 0);
            double y_src = dst_pt.at<double>(1, 0) / dst_pt.at<double>(2, 0);

            int x_src_int = static_cast<int>(std::floor(x_src));
            int y_src_int = static_cast<int>(std::floor(y_src));

            if (x_src_int >= 0 && x_src_int < src.cols - 1 && y_src_int >= 0 && y_src_int < src.rows - 1) {
                double x_diff = x_src - x_src_int;
                double y_diff = y_src - y_src_int;

                // Bilinear interpolation
                

                //vector<Pixel> val11 = src.at<Pixel>(y_src_int, x_src_int);
                cv::Vec3b val11 = src.at<cv::Vec3b>(y_src_int, x_src_int + 1);
                cv::Vec3b val12 = src.at<cv::Vec3b>(y_src_int, x_src_int + 1);
                cv::Vec3b val21 = src.at<cv::Vec3b>(y_src_int + 1, x_src_int);
                cv::Vec3b val22 = src.at<cv::Vec3b>(y_src_int + 1, x_src_int + 1);
                //Calculating the pixel value by sending it to the interpolate function
               /* cv::Vec3b v = interpolate(val11,val12,val21,val22,x_diff, y_diff);
                dst.at<cv::Vec3b>(y_dst, x_dst) = v;*/

                cv::Vec3b interpolated;
                for (int k = 0; k < 3; ++k) {
                    interpolated[k] = static_cast<uchar>((1 - x_diff) * (1 - y_diff) * val11[k] +
                        x_diff * (1 - y_diff) * val12[k] +
                        (1 - x_diff) * y_diff * val21[k] +
                        x_diff * y_diff * val22[k]);
                }
                dst.at<cv::Vec3b>(y_dst, x_dst) = interpolated;
            }
            else {
                //cast from Scalar-borderValue to cv::Vec3b:
                dst.at<cv::Vec3b>(y_dst, x_dst) = cv::Vec3b
                    (static_cast<uchar>(borderValue[0]),
                    static_cast<uchar>(borderValue[1]),
                    static_cast<uchar>(borderValue[2]));
            }
        }
    }
}