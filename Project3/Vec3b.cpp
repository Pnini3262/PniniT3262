//#include <iostream>
//#include "opencv2/opencv.hpp"
//// Structure to represent a color with three channels: Blue, Green, and Red
//struct Vec3b {
//    unsigned char b; // Blue channel
//    unsigned char g; // Green channel
//    unsigned char r; // Red channel
//    static int depth() { return CV_8UC3; }
//    static int channels() { return 3; }
//    static int type() { return CV_8UC3; }
//
//
//    // Constructor
//    Vec3b(unsigned char blue = 0, unsigned char green = 0, unsigned char red = 0)
//        : b(blue), g(green), r(red) {}
//
//    // Equality operator
//    bool operator==(const Vec3b& other) const {
//        return b == other.b && g == other.g && r == other.r;
//    }
//
//    // Inequality operator
//    bool operator!=(const Vec3b& other) const {
//        return !(*this == other);
//    }
//    // Access operator
//    unsigned char& operator[](int index) {
//        switch (index) {
//        case 0: return b;
//        case 1: return g;
//        case 2: return r;
//        default: throw std::out_of_range("Index out of range");
//        }
//    }
//
//    const unsigned char& operator[](int index) const {
//        switch (index) {
//        case 0: return b;
//        case 1: return g;
//        case 2: return r;
//        default: throw std::out_of_range("Index out of range");
//        }
//    }
//    
//};
//
//// Function to compute the interpolated color
//
//Vec3b interpolate(const Vec3b& val11, const Vec3b& val12, const Vec3b& val21, const Vec3b& val22,
//    double x_diff, double y_diff) {
//    // Compute interpolated values for each channel
//    unsigned char b_interpolated = static_cast<unsigned char>((1 - x_diff) * (1 - y_diff) * val11.b +
//        x_diff * (1 - y_diff) * val12.b +
//        (1 - x_diff) * y_diff * val21.b +
//        x_diff * y_diff * val22.b);
//
//    unsigned char g_interpolated = static_cast<unsigned char>((1 - x_diff) * (1 - y_diff) * val11.g +
//        x_diff * (1 - y_diff) * val12.g +
//        (1 - x_diff) * y_diff * val21.g +
//        x_diff * y_diff * val22.g);
//
//    unsigned char r_interpolated = static_cast<unsigned char>((1 - x_diff) * (1 - y_diff) * val11.r +
//        x_diff * (1 - y_diff) * val12.r +
//        (1 - x_diff) * y_diff * val21.r +
//        x_diff * y_diff * val22.r);
//
//    // Return the interpolated Vec3b
//    return Vec3b(b_interpolated, g_interpolated, r_interpolated);
//}
//// Overload the output stream operator to print Vec3b
//std::ostream& operator<<(std::ostream& os, const Vec3b& color) {
//    os << "B: " << static_cast<int>(color.b) << " G: " << static_cast<int>(color.g) << " R: " << static_cast<int>(color.r);
//    return os;
//
//}
//
