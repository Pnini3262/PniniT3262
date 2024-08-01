//#include "MyWarpPrespective.h"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <array>
using namespace cv;
using Pixel = std::array<unsigned char, 3>; // הגדרת סוג חדש לפיקסל
void MyWarpPerspective(const Mat& src, Mat& dst, const Mat& H, const Size& dsize, int flags, int borderMode = cv::BORDER_CONSTANT, const cv::Scalar& borderValue = cv::Scalar()) {
    if (H.rows != 3 || H.cols != 3) {
        throw std::invalid_argument("Homography matrix H must be 3x3.");
    }
    dst.create(dsize, src.type());
    Mat H_inv;
    if (!invert(H, H_inv, DECOMP_SVD)) {
        throw std::runtime_error("Homography matrix inversion failed.");
    }
    for (int y_dst = 0; y_dst < dsize.height; ++y_dst) {
        for (int x_dst = 0; x_dst < dsize.width; ++x_dst) {
            Mat src_pt(3, 1, CV_64F);
            src_pt.at<double>(0, 0) = x_dst;
            src_pt.at<double>(1, 0) = y_dst;
            src_pt.at<double>(2, 0) = 1;
            Mat dst_pt = H_inv * src_pt;
            double x_src = dst_pt.at<double>(0, 0) / dst_pt.at<double>(2, 0);
            double y_src = dst_pt.at<double>(1, 0) / dst_pt.at<double>(2, 0);
            if (x_src >= 0 && x_src < src.cols - 1 && y_src >= 0 && y_src < src.rows - 1) {
                Rect roi(0, 0, src.cols, src.rows);
                Mat src_roi = src(roi);
                double x_floor = std::floor(x_src);
                double y_floor = std::floor(y_src);
                double x_ceil = std::ceil(x_src);
                double y_ceil = std::ceil(y_src);
                Pixel top_left = src_roi.at<Pixel>(Point(x_floor, y_floor));
                Pixel top_right = src_roi.at<Pixel>(Point(x_ceil, y_floor));
                Pixel bottom_left = src_roi.at<Pixel>(Point(x_floor, y_ceil));
                Pixel bottom_right = src_roi.at<Pixel>(Point(x_ceil, y_ceil));
                double dx = x_src - x_floor;
                double dy = y_src - y_floor;
                Pixel top, bottom, interpolated;
                for (int i = 0; i < 3; ++i) {
                    top[i] = top_left[i] * (1 - dx) + top_right[i] * dx;
                    bottom[i] = bottom_left[i] * (1 - dx) + bottom_right[i] * dx;
                    interpolated[i] = top[i] * (1 - dy) + bottom[i] * dy;
                }
                dst.at<Pixel>(Point(x_dst, y_dst)) = interpolated;
            }
            else {
                Pixel border_pixel = { static_cast<unsigned char>(borderValue[0]), static_cast<unsigned char>(borderValue[1]), static_cast<unsigned char>(borderValue[2]) };
                dst.at<Pixel>(Point(x_dst, y_dst)) = border_pixel;
            }
        }
    }
}