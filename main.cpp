#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

/** @function main */
int main(int argc, char** argv)
{
  Mat src, src_gray;

  /// Read the image
  src = imread( argv[1], 1 );

  if( !src.data )
    { return -1; }

  /// Convert it to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 400, 200, 40, 10, 30 );

  /// Draw the circles detected
	for( size_t i = 0; i < circles.size(); i++ )
	{
		// std::cout << circles[i][0] << " : " << circles[i][1] << std::endl;
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
  }
  
  if (circles.size() > 1) {
    std::vector<float> crds;
    for (unsigned int i = 0; i < circles.size(); i++) {
      // for (unsigned int i = 0; i < crds.size(); i++) {

      // }

      if (find(crds.begin(), crds.end(), circles[i][0]) == crds.end())
        crds.push_back(circles[i][0]);
      if (find(crds.begin(), crds.end(), circles[i][1]) == crds.end())
        crds.push_back(circles[i][1]);
    }

    std::sort(crds.begin(), crds.end());

    // for (unsigned int i = 0; i < crds.size(); i++) {
    //   std::cout << crds[i] << std::endl;
    // }

    float x = (crds[2] - crds[0]) / 2 + crds[0];
    float y = (crds[2] - crds[0]) / 2 + crds[0];
    double rad = crds[1] - crds[0];

    Point centr(x, y);
    circle( src, centr, rad, Scalar(0,0,255), 3, 8, 0 );
    
    circle( src, centr, rad - 1 * rad / 12, Scalar(0,0,255), 3, 8, 0 );
    circle( src, centr, rad + 1 * rad / 12, Scalar(0,0,255), 3, 8, 0 );
    circle( src, centr, rad + 2 * rad / 12, Scalar(0,0,255), 3, 8, 0 );
  }



  /// Show your results
  namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  imshow( "Hough Circle Transform Demo", src );

  waitKey(0);
  return 0;
}