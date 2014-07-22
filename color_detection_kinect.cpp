#include <iostream>
#include <opencv2/opencv.hpp>

#include "openkinect_OpenCvExample.cpp"

using namespace std;

int main(int argc, char **argv)
{
	// Init Kinect
	Freenect::Freenect freenect;
	MyFreenectDevice & device =
	    freenect.createDevice < MyFreenectDevice > (0);

	device.startVideo();
	device.startDepth();

	// Create a window called "Control"
	cv::namedWindow("Control", cv::WINDOW_AUTOSIZE);

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	// Create trackbars in "Control" window
	// Hue (0 - 179)
	cv::createTrackbar("LowH", "Control", &iLowH, 179);
	cv::createTrackbar("HighH", "Control", &iHighH, 179);

	// Saturation (0 - 255)
	cv::createTrackbar("LowS", "Control", &iLowS, 255);
	cv::createTrackbar("HighS", "Control", &iHighS, 255);

	// Value (0 - 255)
	cv::createTrackbar("LowV", "Control", &iLowV, 255);
	cv::createTrackbar("HighV", "Control", &iHighV, 255);

	while (true) {

		cv::Mat imgOriginal;
		cv::Mat imgHSV;
		cv::Mat imgThresholded;
		cv::Mat imgDepth;

		// Read a new frame from video
		if (!device.getVideo(imgOriginal)) {
			cout << "Cannot read a frame from video stream. RGB" <<
			    endl;
			return 1;
		}

		// Read also Depth image
		if (device.getDepth(imgDepth)) {
			// Show depth image
			imgDepth.convertTo(imgDepth, CV_8UC1, 255.0 / 2048.0);
			cv::imshow("Depth", imgDepth);
		}

		// Convert the captured frame from BGR to HSV
		cv::cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV);

		// Threshold the image
		cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV),
			    cv::Scalar(iHighH, iHighS, iHighV), imgThresholded);

		// Morphological opening (remove small objects from the foreground)
		cv::erode(imgThresholded, imgThresholded,
			  getStructuringElement(cv::MORPH_ELLIPSE,
						cv::Size(5, 5)));
		cv::dilate(imgThresholded, imgThresholded,
			   getStructuringElement(cv::MORPH_ELLIPSE,
						 cv::Size(5, 5)));

		// Morphological closing (fill small holes in the foreground)
		cv::dilate(imgThresholded, imgThresholded,
			   getStructuringElement(cv::MORPH_ELLIPSE,
						 cv::Size(5, 5)));
		cv::erode(imgThresholded, imgThresholded,
			  getStructuringElement(cv::MORPH_ELLIPSE,
						cv::Size(5, 5)));

		// Show the thresholded image
		cv::imshow("Thresholded Image", imgThresholded);

		// Show the original image
		cv::imshow("Original", imgOriginal);

		// Wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (cv::waitKey(30) == 27) {

			cout << "esc key is pressed by user" << endl;

			cvDestroyWindow("Control");
			cvDestroyWindow("Depth");
			cvDestroyWindow("Original");
			cvDestroyWindow("Thresholded Image");

			device.stopVideo();
			device.stopDepth();

			return 0;
		}

	}

}
