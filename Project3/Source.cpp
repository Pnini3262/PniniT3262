#include "Header.h"
#include <opencv2/opencv.hpp>
#include <stdexcept>
using namespace cv;

void MyWarpPerspective(const Mat& src, Mat& dst, const Mat& H, const Size& dsize, int flags, int borderMode = 0, const Scalar& borderValue = Scalar()) {
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

                Vec3b top_left = src_roi.at<Vec3b>(Point(x_floor, y_floor));
                Vec3b top_right = src_roi.at<Vec3b>(Point(x_ceil, y_floor));
                Vec3b bottom_left = src_roi.at<Vec3b>(Point(x_floor, y_ceil));
                Vec3b bottom_right = src_roi.at<Vec3b>(Point(x_ceil, y_ceil));

                double dx = x_src - x_floor;
                double dy = y_src - y_floor;

                Vec3b top = top_left * (1 - dx) + top_right * dx;
                Vec3b bottom = bottom_left * (1 - dx) + bottom_right * dx;
                Vec3b interpolated = top * (1 - dy) + bottom * dy;

                dst.at<Vec3b>(Point(x_dst, y_dst)) = interpolated;
            }
            else {
                dst.at<Vec3b>(Point(x_dst, y_dst)) = Vec3b(borderValue[0], borderValue[1], borderValue[2]);
            }
        }
    }
}

