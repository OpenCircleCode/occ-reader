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
		Reader();
        Reader(std::string file);
		~Reader();

		void					readFromImg(cv::Mat);

		cv::Point				getCirclesCenter() const;
		std::vector<double>		getCirclesRadius() const;
		std::vector<cv::Point>	getPoints() const;
		std::string				getBinary() const;
		std::string				getHash() const;
		std::vector<cv::Vec3f>	getAnchors() const;
		cv::Mat					getWorkingImg() const;

    private:
		void			detectAnchors();
		void			findCircles();
		void			findPoints();
		void			extractBinary();
		void			binaryToHash();
		
		cv::Mat					img;
		double					width;
		double					height;
		std::vector<cv::Vec3f>	anchors;
		cv::Point				circlesCenter;
		std::vector<double>		circlesRadius;

		std::vector<cv::Point>	points;
		std::vector<bool>		values;
		std::string				binary;
		std::string				hash;
		std::string				string;
};

#endif