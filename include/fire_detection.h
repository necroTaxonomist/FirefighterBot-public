#include "opencv2/opencv.hpp"
#include "include/units.h"

typedef float AngleDeg;
typedef double DurationSec;
#include <iostream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <utility>

#define MAX_FLAME_COUNT 20
#ifdef DEBUG
#define OUT(X) std::cout << X << '\n'
#else
#define OUT
#endif

constexpr AngleDeg map(float val)
{
	double min_in = 0;
	double max_in = 640;
	double camera_FOV = 62.2;
	double mid = camera_FOV / 2;
	double min_out = mid - camera_FOV;
	double max_out = camera_FOV - mid;
	return (val - min_in) * (max_out-min_out) / (max_in-min_in) + min_out;
}


std::pair<bool,AngleDeg> processCapture(DurationSec duration=120)
{	
	DurationSec const normalizeTime = 4.0;
	duration += normalizeTime;
	cv::VideoCapture cap(0);
	int const width = cap.get(CV_CAP_PROP_FRAME_WIDTH);

	// Create a window
	//cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
	
	if (!cap.isOpened()) 
	{
		std::cerr << "Unable to open camera" << '\n';
		exit(EXIT_FAILURE);
	}
	
	// Start timer
	auto start = std::chrono::system_clock::now();
	
	// Flame Detection
	int flame_count = 0;
	
	//MOG2 Background subtractor
	cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorMOG2(); 
	
	AngleDeg angle = 0;

	// All frames
	cv::Mat frame;
    cv::Mat fgMaskMOG2;

	cv::Mat erosion,dilation;
	
	// Get erosion/dilation kernel
	int const edsz = 3;
	cv::Mat kernel = cv::getStructuringElement(
						cv::MORPH_RECT,
						cv::Size(2*edsz+1,2*edsz+1),
						cv::Point(edsz,edsz) );
	std::vector<std::vector<cv::Point> > contours;				
	std::vector<cv::Vec4i> hierarchy;
	
	
	
	do 
	{
		//read the current frame
        if(!cap.read(frame)) {
            std::cerr << "Unable to read next frame." << '\n';
            std::cerr << "Exiting..." << '\n';
            exit(EXIT_FAILURE);
        }
        
        // Correct image orientation
        cv::flip(frame,frame,0);
        OUT(1);
        
        // Blur image to delete noise
        cv::medianBlur(frame, frame, 3);
		OUT(2);

		// Extract red channel
	    cv::Mat channels[3];
        cv::split(frame, channels);
        auto & redChannel = channels[2];
		OUT(3);
		
		// Only keep red values greater than 150
        redChannel = cv::max(redChannel, 150);
		cv::Mat frame_red(redChannel);        
		OUT(4);

		// Apply background subtractor to detect only moving objects
		pMOG2->apply(frame_red,fgMaskMOG2);
		OUT(5);

		// Erode to get rid of noise
		cv::erode(fgMaskMOG2,erosion,kernel);
		OUT(6);
		// Dilate to enhance non-noisy parts
		cv::dilate(erosion,dilation,kernel);
		OUT(7);

		// Identify all contours/shapes in image
		cv::findContours(dilation, contours, hierarchy,
						cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE,
						cv::Point(0,0));
		OUT(8);
						
		// Identify center of mass of all contours
		float cX = 0;
		for(auto & cnt : contours) 
		{
			cv::Moments mu = cv::moments(cnt);
			cX += mu.m10/mu.m00;
		}
		cX /= contours.size();
		OUT(9); 
		float center = cX;
		
		// Print
		auto t_now = std::chrono::system_clock::now();
		auto time_diff = std::chrono::duration<float>(t_now-start);
		if (time_diff.count() > normalizeTime)
		{
			if (!std::isnan(center)) {
				++flame_count;
				angle = map(center);
				std::cout << "Angle: " << angle << '\n';
				
				if(flame_count >= MAX_FLAME_COUNT) 
				{
					cv::destroyAllWindows();
					cap.release();
					pMOG2.release();
					return std::pair<bool,AngleDeg>(true,angle);
				}
				
			 } else {
				std::cout << "no flame" << '\n';
				flame_count = 0;
			}
		}
		cv::imshow("Result",dilation);

		

		// When the "esc" key is pressed, the break
		if (cv::waitKey(1) == 27) {
			break;
		}
		t_now = std::chrono::system_clock::now();
		time_diff = std::chrono::duration<float>(t_now-start);
		if (time_diff.count() > duration)
		{
			cv::destroyAllWindows();
			cap.release();
			pMOG2.release();
			return std::pair<bool,AngleDeg>(false,0);
		}
		//std::cout << "Time Diff: " << time_diff.count() << '\n';
	} while(cap.isOpened());

	// Delete all windows
	cv::destroyAllWindows();
	// Mat type variable release
	// resultFrame.release();
	pMOG2.release();

	cap.release();


}
