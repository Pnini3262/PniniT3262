//#include <iostream>
//
//// Structure to represent a color with three channels: Blue, Green, and Red
//struct Vec3b {
//    unsigned char b; // Blue channel
//    unsigned char g; // Green channel
//    unsigned char r; // Red channel
//    static int depth();
//    static int channels();
//   
//    // Constructor
//    Vec3b(unsigned char blue = 0, unsigned char green = 0, unsigned char red = 0);
//
//    // Equality operator
//    bool operator==(const Vec3b& other) const;
//    // Inequality operator
//    bool operator!=(const Vec3b& other) const;
//
//    const unsigned char& operator[](int index) const;
//
//    unsigned char& operator[](int index);
//
//};
//
//Vec3b interpolate(const Vec3b& val11, const Vec3b& val12, const Vec3b& val21, const Vec3b& val22,
//    double x_diff, double y_diff);
//
//
//// Overload the output stream operator to print Vec3b
////std::ostream& operator<<(std::ostream& os, const Vec3b& color);