#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include "src/Reader.hpp"

// int main(int argc, char** argv) {

//   cv::Mat src = cv::imread( argv[1], 1 );
//   Reader reader(argv[1]);

//   cv::Point center = reader.getCirclesCenter();
//   std::vector<double> radius = reader.getCirclesRadius();
//   std::vector<cv::Point> points = reader.getPoints();

//   // for (unsigned int i = 0; i < radius.size(); i++) {
//   //   circle( src, center, radius[i], cv::Scalar(0, 0, 255), 3, 8, 0 );
//   // }

//   for (unsigned int i = 0; i < points.size(); i++) {
//     circle( src, points[i], 3, cv::Scalar(0, 255, 0), 3, 8, 0 );
//   }

//   cv::namedWindow( "Open Circle Code", CV_WINDOW_AUTOSIZE );
//   cv::imshow( "Open Circle Code", src );

//   cv::waitKey(0);
//   return 0;
// }

int main() {
	cv::VideoCapture stream(0);

	if (!stream.isOpened()) {
		std::cout << "stream error" << std::endl;
		return 1;
	}

	double width = stream.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = stream.get(CV_CAP_PROP_FRAME_HEIGHT);
	cv::Mat	cameraFrame;
	cv::Mat	workFrame;
	cv::Point p(width / 2, height / 2);

	while(true) {
		stream >> cameraFrame;
		cvtColor( cameraFrame, workFrame, CV_BGR2GRAY );

		Reader reader;
		reader.readFromImg(workFrame);


		std::vector<cv::Vec3f>	hooks = reader.getHooks();
		std::vector<cv::Point>	points = reader.getPoints();
		std::cout << hooks.size() << std::endl;
		if (hooks.size() > 1) {
			for (unsigned int i = 0; i < hooks.size(); i++) {
				cv::Point p2(hooks[i][0], hooks[i][1]);
				circle( cameraFrame, p2, 20, cv::Scalar(0, 0, 255), 3, 8, 0 );
			}
			for (unsigned int i = 0; i < points.size(); i++) {
				circle( cameraFrame, points[i], 3, cv::Scalar(0, 0, 255), 3, 8, 0 );
			}
		}

		std::string binary = reader.getString();
		if (binary != "")
			std::cout << binary << std::endl;

		circle( cameraFrame, p, height / 2 - 50, cv::Scalar(0, 255, 0), 3, 8, 0 );

		cv::imshow("cam", cameraFrame);
		if(cv::waitKey(1) >= 27)
			break;
	}
	return 0;
}