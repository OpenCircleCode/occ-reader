#ifndef READER_HPP
# define READER_HPP

# include <iostream>
# include <vector>
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/imgproc/imgproc.hpp"

# define PI 3.14159265
# define STEP_ANGLE 10

class Reader {
    public:
        Reader(std::string file);
		~Reader();

		cv::Point				getCirclesCenter() const;
		std::vector<double>		getCirclesRadius() const;
		std::vector<cv::Point>	getPoints() const;

    private:
		Reader();
		void			detectHooks();
		void			findCircles();
		void			findPoints();
		
		cv::Mat						img;
		std::vector<cv::Vec3f>	hooks;
		cv::Point				circlesCenter;
		std::vector<double>		circlesRadius;

		std::vector<cv::Point>	points;
};

#endif