#include "Reader.hpp"

Reader::Reader(std::string file) {
	img = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	if (!img.data) {
		std::cerr << "can not load: " << file << std::endl;
		return;
	}

    GaussianBlur( img, img, cv::Size(9, 9), 2, 2 );
    detectHooks();
	findCircles();
	
	findPoints();
}

Reader::~Reader() {};

void    Reader::detectHooks() {
	cv::HoughCircles( img, hooks, CV_HOUGH_GRADIENT, 1, 400, 200, 40, 10, 30 );
	if (hooks.size() <= 1) {
		std::cerr << "hook detection failed" << std::endl;
		return;
	}
}

void	Reader::findCircles() {
	std::vector<float> crds;
	
	for (unsigned int i = 0; i < hooks.size(); i++) {
		if (find(crds.begin(), crds.end(), hooks[i][0]) == crds.end())
        	crds.push_back(hooks[i][0]);
      	if (find(crds.begin(), crds.end(), hooks[i][1]) == crds.end())
        	crds.push_back(hooks[i][1]);
		}
		
	std::sort(crds.begin(), crds.end());
	circlesCenter.x = (crds[2] - crds[0]) / 2 + crds[0];
	circlesCenter.y = (crds[2] - crds[0]) / 2 + crds[0];

	double radiusRef = crds[1] - crds[0];
	circlesRadius.push_back(radiusRef + 2 * radiusRef / 12);
	circlesRadius.push_back(radiusRef + 1 * radiusRef / 12);
	circlesRadius.push_back(radiusRef);
	circlesRadius.push_back(radiusRef - 1 * radiusRef / 12);
}

void	Reader::findPoints() {
	for (unsigned int i = 0; i < circlesRadius.size(); i++) {
		for (unsigned int j = 0; j < 360; j += STEP_ANGLE) {
			cv::Point p;
			p.x = circlesRadius[i] * sin(j * PI / 180) + circlesCenter.x;
			p.y = circlesRadius[i] * cos(j * PI / 180) + circlesCenter.y;
			points.push_back(p);
		}
	}
}

cv::Point			Reader::getCirclesCenter() const {
	return circlesCenter;
}

std::vector<double>		Reader::getCirclesRadius() const {
	return circlesRadius;
}

std::vector<cv::Point>	Reader::getPoints() const {
	return points;
}
