#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include "src/Reader.hpp"

int main(int argc, char** argv) {

  cv::Mat src = cv::imread( argv[1], 1 );
  Reader reader(argv[1]);

  cv::Point center = reader.getCirclesCenter();
  std::vector<double> radius = reader.getCirclesRadius();
  std::vector<cv::Point> points = reader.getPoints();

  for (unsigned int i = 0; i < radius.size(); i++) {
    circle( src, center, radius[i], cv::Scalar(0, 0, 255), 3, 8, 0 );
  }

  for (unsigned int i = 0; i < points.size(); i++) {
    circle( src, points[i], 3, cv::Scalar(0, 255, 0), 3, 8, 0 );
  }

  cv::namedWindow( "Open Circle Code", CV_WINDOW_AUTOSIZE );
  cv::imshow( "Open Circle Code", src );

  cv::waitKey(0);
  return 0;
}