#include "Reader.hpp"

Reader::Reader(std::string file) {
	img = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	width = img.cols;
	height = img.rows;

	if (!img.data) {
		std::cerr << "can not load: " << file << std::endl;
		return;
	}

    // GaussianBlur( img, img, cv::Size(9, 9), 2, 2 ); /// USEFULL?
    detectAnchors();
	findCircles();
	findPointsNbr();
	
	findPoints();
	extractBinary();
	binaryToHash();
}

Reader::Reader() {}

Reader::~Reader() {}

void		Reader::readFromImg(cv::Mat image) {
	img = image;
	// GaussianBlur( img, img, cv::Size(9, 9), 2, 2 );
	detectAnchors();
	findCircles();
	
	findPoints();
	extractBinary();
	binaryToHash();
}

void    Reader::detectAnchors() {
	cv::HoughCircles( img, anchors, CV_HOUGH_GRADIENT, 1, height / 2, 200, 40, height / 80, height / 20 );
	if (anchors.size() <= 1) {
		std::cerr << "Anchor detection failed" << std::endl;
		return;
	}
}

void	Reader::findCircles() {
	cv::Vec3f estAnchor, northAnchor, westAnchor, southAnchor;

	for (unsigned int k = 0; k < 2; k++) {
		double lowTarget = anchors[0][k];
		double highTarget = anchors[0][k];
		unsigned int lowInd = 0;
		unsigned int highInd = 0;
		for(unsigned int i = 0; i < anchors.size(); i++) {
			if (anchors[i][k] < lowTarget) {
				lowTarget = anchors[i][k];
				lowInd = i;
			}
			if (anchors[i][k] > highTarget) {
				highTarget = anchors[i][k];
				highInd = i;
			}
		}
		if (k == 0) {
			westAnchor = anchors[lowInd];
			estAnchor = anchors[highInd];
		} else {
			northAnchor = anchors[lowInd];
			southAnchor = anchors[highInd];
		}
	}

	circlesCenter.x = (estAnchor[0] - westAnchor[0]) / 2 + westAnchor[0];
	circlesCenter.y = (southAnchor[1] - northAnchor[1]) / 2 + northAnchor[1];

	double thirdRadius = estAnchor[0] - circlesCenter.x;

	circlesRadius.push_back(thirdRadius + 2 * thirdRadius / 9);
	circlesRadius.push_back(thirdRadius + 1 * thirdRadius / 9);
	circlesRadius.push_back(thirdRadius);
	circlesRadius.push_back(thirdRadius - 1 * thirdRadius / 9);
}

void	Reader::findPointsNbr() {
	std::string binaryNbr;

	for (unsigned int i = 0; i < anchors.size(); i++) {
		cv::Point p(anchors[i][1], anchors[i][0]);
		cv::Scalar color = img.at<unsigned char>(p);
		bool b = (color.val[0] <= 200) ? true : false;
		b ? binaryNbr.append("1") : binaryNbr.append("0");
	}

	if (binaryNbr.compare("1111") == 0)
		binaryNbr = 36;
	else if (binaryNbr.compare("1110") == 0)
		binaryNbr = 40;
	else if (binaryNbr.compare("1101") == 0)
		binaryNbr = 45;
	else if (binaryNbr.compare("1100") == 0)
		binaryNbr = 60;
	else if (binaryNbr.compare("1011") == 0)
		binaryNbr = 72;
	else if (binaryNbr.compare("1010") == 0)
		binaryNbr = 90;
	else if (binaryNbr.compare("1001") == 0)
		binaryNbr = 120;
	else 
		binaryNbr = 36;
}

void	Reader::findPoints() {
	for (unsigned int i = 0; i < circlesRadius.size(); i++) {
		for (int j = 450; j > 90; j -= STEP_ANGLE) {
			cv::Point p;
			p.x = circlesRadius[i] * sin(j * PI / 180) + circlesCenter.x;
			p.y = circlesRadius[i] * cos(j * PI / 180) + circlesCenter.y;
			points.push_back(p);
		}
	}
	points.erase(points.begin() + 99);
	points.erase(points.begin() + 90);
	points.erase(points.begin() + 81);
	points.erase(points.begin() + 72);
}

void	Reader::extractBinary() {
	for (unsigned int i = 0; i < points.size(); i++) {
		cv::Scalar color = img.at<unsigned char>(points[i].y, points[i].x);
		bool b = (color.val[0] <= 200) ? true : false; /////NAIVE CONDITION
		values.push_back(b);
	}
}

void	Reader::binaryToHash() {
	std::string str = getBinary();

    std::stringstream sstream(str);
    // std::string output;
    while(sstream.good())
    {
        std::bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        hash += c;
    }

	// std::cout << output << std::endl;
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

std::string		Reader::getBinary() const {
	std::string str;
	for (unsigned int i = 0; i < values.size(); i++) {
		str.append(to_string(values[i]));
	}
	return str;
}

std::string		Reader::getHash() const {
	return hash;
}

std::vector<cv::Vec3f>	Reader::getAnchors() const {
	return this->anchors;
}

cv::Mat					Reader::getWorkingImg() const {
	return this->img;
}