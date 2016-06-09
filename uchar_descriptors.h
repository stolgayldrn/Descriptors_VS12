/*
Copyright (C) 2015-2020 S.Tolga Yildiran.
All rights reserved.

This file is part of Tolga Yildiran Video Search library and is made available under
the terms of the BSD license (see the COPYING file).
*/
/************************************************************************/
/* Tolga Yildiran														*/
/* 24/05/2016															*/
/* stolgayldrn@gmail.com												*/	
/************************************************************************/

#ifndef UCHAR_DESCRIPTORS_H
#define UCHAR_DESCRIPTORS_H

#include "descriptors.h"
class UcharDescriptors : public Descriptors
{
public:
	UcharDescriptors(const char* file_path, const char* dsc_path, FeatureType feature) :Descriptors(file_path, dsc_path, feature), descs(nullptr)
	{
		header += "_uChar";
		flags = 1;
		isExist_OpencvMatImage = false;
		resize = false;
	};

	UcharDescriptors(const char* file_path, const cv::Mat ImageMat, const char* dsc_path, FeatureType feature) :Descriptors(file_path, ImageMat, dsc_path, feature), descs(nullptr)
	{
		header += "_uChar";
		flags = 1;
		isExist_OpencvMatImage = true;
		resize = false;
	};

	UcharDescriptors(const char* dsc_path, FeatureType feature) :Descriptors(dsc_path, feature), descs(nullptr)
	{
		flags = 1;
		isExist_OpencvMatImage = false;
		resize = false;
	};
	~UcharDescriptors(void);
	int writeDSC();		// arranged to v2
	int readDSC();			// arranged to v2
	int readDSC__v1();
	void recursiveExtractAKAZE(cv::Mat* Image, int rec, double threshold);
	static void resizeImage(cv::Mat* Image, double maxSize);
	void setResizeImage(bool reSize);
	int extractAKAZE();
	int extractEZSIFT();
	unsigned char* getUcharDescriptors() const;
	int  getReadModeDescriptors(cv::Mat &CV_Descriptors) const;
	int releaseBasePointers();
	int releaseData();
	int releseEZSIFT();
	int releaseOpencvFeats();
	int releaseImgMat();
private:
	unsigned char* descs;
};

/*Other Functions*/
void findMatches(UcharDescriptors &descriptor_1, UcharDescriptors &descriptor_2, std::vector<cv::DMatch >& good_matches);
int findIntersectedFeatures(std::string imgPath, cv::Mat img1, UcharDescriptors& descriptor_1, std::vector<cv::DMatch >& inMatches);
int findFlannBasedGoodMatches(cv::Mat &descriptors_1, cv::Mat &descriptors_2, std::vector< cv::DMatch > &matches, std::vector< cv::DMatch > &good_matches, float gm_distance);

#endif
