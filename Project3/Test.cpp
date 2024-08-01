#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "opencv2/opencv.hpp"
#include "Header.h"
#include "WarpPrespective.h"
using namespace cv;
using namespace std;


int add(int a, int b) {
    return a + b;
}

bool areMatsEqual(const cv::Mat& mat1, const cv::Mat& mat2) {
    // בדוק אם המידות והסוגים תואמים
    if (mat1.size() != mat2.size() || mat1.type() != mat2.type()) {
        return false;
    }

    // השווה את הערכים של כל תא בתמונות
    for (int i = 0; i < mat1.rows; ++i) {
        for (int j = 0; j < mat1.cols; ++j) {
            if (mat1.at<double>(i, j) != mat2.at<double>(i, j)) {
                return false;
            }
        }
    }

    return true;
}

    // Test case
TEST_CASE("Testing the add function") {
    // Act
    // Assert
    // REQUIRE();//Stop everything! and throw exception
    //CHECK_EQ();
    CHECK(add(2, 3) == 5);
    CHECK(add(-1, 1) == 0);
    CHECK(add(0, 0) == 0);
    CHECK(add(-2, -3) == -5);
}
 TEST_CASE("Testing the warpPrespective function") {

        const int MAX_FEATURES = 500;
        const float GOOD_MATCH_PERCENT = 0.15f;

        // Read reference image
        Mat imReference = imread("C:\\pnini\\Practicum\\alignMat.jpg");
        //Mat imReference = imread("C:\\Users\\USER\\Pictures\\אוסף תמונות\\ילדים\\111268.JPG");

        // Read image to be aligned
        Mat im = imread("C:\\pnini\\Practicum\\change.jpg");
        //Mat im = imread("C:\\pnini\\Practicum\\111268.JPG");

        // Check if images were read successfully
        if (im.empty() || imReference.empty()) {
            cout << "Could not open or find the images!" << endl;
        }

        // Convert images to grayscale
        Mat im1Gray, im2Gray;
        cvtColor(im, im1Gray, cv::COLOR_BGR2GRAY);
        cvtColor(imReference, im2Gray, cv::COLOR_BGR2GRAY);

        // Variables to store keypoints and descriptors
        std::vector<KeyPoint> keypoints1, keypoints2;
        Mat descriptors1, descriptors2;

        // Detect ORB features and compute descriptors.
        Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
        orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
        orb->detectAndCompute(im2Gray, Mat(), keypoints2, descriptors2);

        // Match features.
        std::vector<DMatch> matches;
        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
        matcher->match(descriptors1, descriptors2, matches, Mat());

        // Sort matches by score
        std::sort(matches.begin(), matches.end());

        // Remove not so good matches
        const int numGoodMatches = (int) matches.size() *(int) GOOD_MATCH_PERCENT;
        matches.erase(matches.begin() + numGoodMatches, matches.end());

        // Draw top matches
        Mat imMatches;
        drawMatches(im, keypoints1, imReference, keypoints2, matches, imMatches);
        imwrite("matches.jpg", imMatches);

        // Extract location of good matches
        std::vector<Point2f> points1, points2;
        for (size_t i = 0; i < matches.size(); i++)
        {
            points1.push_back(keypoints1[matches[i].queryIdx].pt);
            points2.push_back(keypoints2[matches[i].trainIdx].pt);
        }

        // Find homography
        Mat h = findHomography(points1, points2, RANSAC);

        // Use homography to warp image
        Mat im1Reg;
        Mat im2Reg;
        int flags = INTER_NEAREST;
        int borderMode = 0; // or use BORDER_REPLICATE
        const Scalar borderValue = Scalar(0, 0, 255);
        //sent to OpenCV::warpPrespective
        cv::warpPerspective(im, im1Reg, h, imReference.size(), flags, borderMode, borderValue);
        //MyWarp:
        WarpPerspective wp(im, im2Reg, h, imReference.size(), flags, borderMode, (0, 0, 0));
        wp.MyWarpPerspective();

        //imshow("image1", im1Reg);
        //imshow("image2", im2Reg);
        //waitKey(100000);
        ////////////////////////////////////////////////////////////////////

        CHECK(areMatsEqual(im1Reg, im2Reg) == true);

        //CHECK(im1Reg = im2Reg);
    }