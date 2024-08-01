#include <opencv2/opencv.hpp>
#include "Vec3b.h" 

class WarpPerspective {
    
private:
    cv::Mat src;
    cv::Mat dst;
    cv::Mat H;
    cv::Size dsize; 
    int flags;
    int borderMode;
    cv::Vec3b borderValue;
public:
    // Constructor with parameters
    WarpPerspective(const cv::Mat& src, cv::Mat& dst, const cv::Mat& H,
        const cv::Size& dsize, int flags, int borderMode, const cv::Vec3b& borderValue)
        : src(src), dst(dst), H(H), dsize(dsize), flags(flags), borderMode(borderMode), borderValue(borderValue) {
    }

    // Default constructor (if needed)
    WarpPerspective()
        : src(), dst(), H(), dsize(cv::Size()), flags(0), borderMode(cv::BORDER_CONSTANT), borderValue(cv::Vec3b(0, 0, 0)) {
    }

    // Function declarations
    void MyWarpPerspective();

};
double determinant(const cv::Mat& mat);
cv::Mat adjoint(const cv::Mat& mat);
cv::Mat inverse(const cv::Mat& mat);


