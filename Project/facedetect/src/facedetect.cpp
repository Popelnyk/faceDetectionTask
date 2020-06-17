#include "facedetect.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/dnn.hpp>
 
#include <iostream>
#include <string>
#include <vector>
#include <utility>
 
const std::string caffeConfigFile = "deploy.prototxt.txt";
const std::string caffeWeightFile = "res10_300x300_ssd_iter_140000.caffemodel";


//namespace facedetect {

    std::vector<std::tuple<int, int, int ,int> > detect(std::string pathToPhoto) {
        cv::Mat img = cv::imread(pathToPhoto);
        cv::Mat outputImg, resizedImg;
        std::vector<std::tuple<int, int, int ,int> > coordinates(0);
        
	if (img.empty()) {
	    std::cout << "no photo !\n";
	    return coordinates;
	}
	
	try {
	    auto net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
 
	    cv::resize(img, resizedImg, cv::Size(300, 300));
	    cv::Mat inputBlob = cv::dnn::blobFromImage(resizedImg, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false, 5);
 
	    net.setInput(inputBlob, "data");
 
	    cv::Mat detection = net.forward("detection_out");
	    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
 
	    int faceCounter = 0;
 
	    for (int i = 0; i < detectionMat.rows; i++)
	    {
                float confidence = detectionMat.at<float>(i, 2);
 
                if (confidence >= 0.15)
		{
		    int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * resizedImg.cols);
		    int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * resizedImg.rows);
		    int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * resizedImg.cols);
		    int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * resizedImg.rows);
 		
 		    int srcx1 = static_cast<int>(detectionMat.at<float>(i, 3) * img.cols);
		    int srcy1 = static_cast<int>(detectionMat.at<float>(i, 4) * img.rows);
		    int srcx2 = static_cast<int>(detectionMat.at<float>(i, 5) * img.cols);
		    int srcy2 = static_cast<int>(detectionMat.at<float>(i, 6) * img.rows);
		    
 		    if(x1 >= 0 && y1 >= 0 && x2 <= 300 && y2 <= 300) {
		        cv::Rect region(x1, y1, abs(x2 - x1), abs(y2 - y1));
		        int blurRate = std::max(abs(x2 - x1), abs(y2 - y1)) / 12;
		        cv::GaussianBlur(resizedImg(region), resizedImg(region), cv::Size(0, 0), blurRate);
		        
		        faceCounter++;
		        coordinates.push_back(std::make_tuple(srcx1, srcy1, srcx2, srcy2));
		    }
		}
	    }
            std::cout << "Done!\n" << pathToPhoto << "\nTotal faces detected: " << faceCounter << "\n\n";
        }
	catch (cv::Exception& e) {
            std::cerr << "Exception: " << e.what() << "\n";
	};
 
 	cv::resize(resizedImg, outputImg, cv::Size(img.cols/2, img.rows/2));
	cv::imwrite(pathToPhoto, outputImg);
	return coordinates;
    }
//}

