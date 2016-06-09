/*
Copyright (C) 2015-20 S.Tolga Yildiran. 
All rights reserved.

This file is part of Tolga Yildiran Video Search library and is made available under
the terms of the BSD license (see the COPYING file).
*/
/************************************************************************/
/* Tolga Yildiran														*/
/* 09/10/2015															*/
/* stolgayldrn@gmail.com												*/
/************************************************************************/

#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include <string.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include "siftfast.h"
#include "dirent.h"
#include "vld.h"

#define dsc_magic 111222333444
#define MIN_FEATURE_SIZE 10
#define	MAX_FEATURE_SIZE 7000
#define MAX_IMAGE_SIZE 1200
#define MAX_IMAGE_SIZE_2 800
#define MAX_IMAGE_SIZE_3 600
#define IMAGE_RESIZE_ALG CV_INTER_LINEAR

enum FeatureType { AKAZE_FEATS=101, EZ_SIFT, EZ_ROOT_SIFT, OPENCV_SIFT, HESSIAN_SIFT, VL_SIFT };

class Descriptors
{
public:
	Descriptors(const char* file_path, const char* dsc_path, FeatureType feature);
	Descriptors(const char* file_path, const cv::Mat ImageMat, const char* dsc_path, FeatureType feature);
	Descriptors(const char* dsc_path, FeatureType feature);
	~Descriptors(void);
	/*int releaseBasePointers();
	int releseEZSIFT();
	int releaseOpencvFeats();*/
	int getCopyOfOpencvKeypoints(std::vector<cv::KeyPoint> &CV_Keypoints) const;
	int getCopyOfOpencvDescriptors(cv::Mat &CV_Descriptors) const;
	unsigned int getNumOfDescriptors() const;
	std::vector<float> getCoordsX() const;
	std::vector<float> getCoordsY() const;
	std::vector<cv::Point2f> getCoords() const;
	int getFeatureSize() const;
	int getImageHeight() const;
	int getImageWidth() const;
	int getCopyOfImageMatrix(cv::Mat &writeImg) const;
	cv::Mat getImageMat() const;
	cv::Mat getOpencvDescriptors() const;
	FeatureType getFeatureType() const;
	std::vector<cv::KeyPoint> getOpencvKeypoints() const;
	void convertEzsiftToOpencv();
	//TODO: Get Keypoint and Get Descriptors fix;
protected:
	std::string header;
	unsigned int numDesc;
	std::string filePath;
	std::string dscFilePath;
	int featSize;
	FeatureType featType;
	float *Xs, *Ys, *Sizes, *Angles;
	std::vector<cv::KeyPoint> CV_keypoints;
	Keypoint EZ_keypoints;
	cv::Mat CV_descriptors;
	cv::Mat image;
	bool isExist_CV;
	bool isExist_EZSIFT;
	bool isRead;
	int flags;
	int height;
	int width;
	bool isExist_OpencvMatImage;
	bool resize;
};




#endif
