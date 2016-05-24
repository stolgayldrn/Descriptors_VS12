/*
Copyright (C) 2015-20 S.Tolga Yildiran.
All rights reserved.

This file is part of Tolga Yildiran Video Search library and is made available under
the terms of the BSD license (see the COPYING file).
*/
/************************************************************************/
/* Tolga Yildiran														*/
/* 09/10/2015															*/
/************************************************************************/
#include "descriptors.h"

descriptors::descriptors(const char* file_path, const char* dsc_path, 
	FeatureType feature): Xs(nullptr), Ys(nullptr), Sizes(nullptr), 
	Angles(nullptr), EZ_keypoints(nullptr), flags(0), height(0), width(0), isExist_OpencvMatImage(false), resize(false)
{
	filePath = file_path;
	dscFilePath = dsc_path;
	featType = feature;
	numDesc = 0;
	isExist_CV = false;
	isExist_EZSIFT = false;
	isRead = false;
	switch (featType)
	{
	case AKAZE_FEATS:
		featSize = 61;
		header = "AKAZE";
		break;
	case EZ_SIFT:
		featSize = 128;
		header = "EZ_SIFT_";
		break;
	case EZ_ROOT_SIFT:
		featSize = 128;
		header = "EZ_ROOT_SIFT_";
		break;
	case OPENCV_SIFT:
		featSize = 128;
		header = "OPENCV_SIFT_";
		break;
	case HESSIAN_SIFT:
		featSize = 128;
		header = "HESSIAN_SIFT_";
		break;
	case VL_SIFT:
		featSize = 128;
		header = "VL_SIFT_";
		break;
	default:
		featSize = 128;
		break;
	}
}

descriptors::descriptors(const char* file_path, const cv::Mat ImageMat, const char* dsc_path, FeatureType feature) : Xs(nullptr), Ys(nullptr), Sizes(nullptr),
Angles(nullptr), EZ_keypoints(nullptr), flags(0), height(0), width(0), isExist_OpencvMatImage(false), resize(false)
{
	filePath = file_path;
	dscFilePath = dsc_path;
	featType = feature;
	numDesc = 0;
	isExist_CV = false;
	isExist_EZSIFT = false;
	isRead = false;
	image = ImageMat;
	switch (featType)
	{
	case AKAZE_FEATS:
		featSize = 61;
		header = "AKAZE";
		break;
	case EZ_SIFT:
		featSize = 128;
		header = "EZ_SIFT_";
		break;
	case EZ_ROOT_SIFT:
		featSize = 128;
		header = "EZ_ROOT_SIFT_";
		break;
	case OPENCV_SIFT:
		featSize = 128;
		header = "OPENCV_SIFT_";
		break;
	case HESSIAN_SIFT:
		featSize = 128;
		header = "HESSIAN_SIFT_";
		break;
	case VL_SIFT:
		featSize = 128;
		header = "VL_SIFT_";
		break;
	default:
		featSize = 128;
		break;
	}
}

descriptors::descriptors(const char* dsc_path, FeatureType feature): Xs(nullptr), 
Ys(nullptr), Sizes(nullptr), Angles(nullptr), EZ_keypoints(nullptr), flags(0), height(0), width(0), isExist_OpencvMatImage(false), resize(false)
{
	dscFilePath = dsc_path;
	filePath = "";
	featType = feature;
	numDesc = 0;
	isExist_CV = false;
	isExist_EZSIFT = false;
	isRead = false;
	switch (featType)
	{
	case AKAZE_FEATS:
		featSize = 61;
		header = "AKAZE";
		break;
	case EZ_SIFT:
		featSize = 128;
		header = "EZ_SIFT_";
		break;
	case EZ_ROOT_SIFT:
		featSize = 128;
		header = "EZ_ROOT_SIFT_";
		break;
	case OPENCV_SIFT:
		featSize = 128;
		header = "OPENCV_SIFT_";
		break;
	case HESSIAN_SIFT:
		featSize = 128;
		header = "HESSIAN_SIFT_";
		break;
	case VL_SIFT:
		featSize = 128;
		header = "VL_SIFT_";
		break;
	default:
		featSize = 128;
		break;
	}
}

descriptors::~descriptors(void)
{
	try
	{
		/*ReleaseBasePointers();
		ReleaseCV_Feats();
		ReleseEZSIFT();*/
	}
	catch (cv::Exception e)
	{
		printf("\nDescriptors: destructor error.");
	}
}

//int descriptors::ReleaseBasePointers()
//{
//	if(numDesc)
//	{
//		delete[] Xs;
//		delete[] Ys;
//		delete[] Sizes;
//		delete[] Angles;
//		numDesc = 0;
//		return 1;
//	}
//	else
//		return 0;
//	
//}
//
//int descriptors::ReleseEZSIFT()
//{
//	if (isExist_EZSIFT)
//	{
//		free(EZ_keypoints);
//		isExist_EZSIFT = false;
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//
//int descriptors::ReleaseCV_Feats()
//{
//	if (isExist_CV)
//	{
//		CV_keypoints.clear();
//		CV_keypoints.shrink_to_fit();
//		CV_descriptors->release();
//		delete CV_descriptors;
//		isExist_CV = false;
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//
//}

 int descriptors::CopyOpencvKeypoints(std::vector<cv::KeyPoint> &CV_Keypoints) const
 {
	if (isExist_CV||isRead)
	{
		CV_Keypoints = CV_keypoints;
		return 1;
	}
	 printf("\nIt is not a CV type descriptor.");
	 return 0;
 }

 int  descriptors::CopyOpencvDescriptors(cv::Mat &CV_Descriptors) const
 {
	if (isExist_CV)
	{
		CV_Descriptors = CV_descriptors;
		return 1;
	}
	printf("\nIt is not a CV type descriptor.");
	return 0;
}

unsigned int descriptors::GetNumOfDescriptors() const
{
	return numDesc;
}

std::vector<float> descriptors::getCoordsX() const
{
	std::vector<float> coords;
	for (unsigned int i = 0; i < numDesc; i++)
		coords.push_back(Xs[i]);
	return coords;
}

std::vector<float> descriptors::getCoordsY() const
{
	std::vector<float> coords;
	for (unsigned int i = 0; i < numDesc; i++)
		coords.push_back(Ys[i]);
	return coords;
}


std::vector<cv::Point2f> descriptors::GetCoords() const
{
	std::vector<cv::Point2f> coords;
	for (unsigned int i = 0; i < numDesc; i++)
		coords.push_back(cv::Point2f(Xs[i], Ys[i]));
	return coords;
}

int descriptors::GetFeatureSize() const
{
	return featSize;
}

int descriptors::GetImageHeight() const
{
	return height;
}

int descriptors::GetImageWidth() const
{
	return width;
}

int descriptors::GetImage__Copy(cv::Mat &writeImg) const
{
	try
	{
		image.copyTo(writeImg);
		return 1;
	}
	catch (cv::Exception)
	{
		return 0;
	}
}

cv::Mat descriptors::GetImageMat() const
{
	if (isExist_OpencvMatImage)
		return image;
	else
		printf("No Opencv Image");
	return cv::Mat::ones(2, 2, CV_8U)*-1;
}

cv::Mat descriptors::GetOpencvDescriptors() const
{
	if (isExist_CV)
		return CV_descriptors;
	else
		printf("No Opencv Descriptors");
	return cv::Mat::ones(2, 2, CV_8U)*-1;
}

FeatureType descriptors::GetFeatureType() const
{
	return featType;
}

std::vector<cv::KeyPoint> descriptors::GetOpencvKeypoints() const
{
	if (isExist_CV)
		return CV_keypoints;
	else
		printf("No Opencv Descriptors");
	std::vector<cv::KeyPoint> returnVec;
	returnVec.push_back(cv::KeyPoint());
	return returnVec;
}

void descriptors::ConvertEzsiftToOpencv() 
{
	if (isExist_EZSIFT && !isExist_CV)
	{
		CV_descriptors = cv::Mat::zeros(numDesc, featSize, CV_8UC1);
		for (int k = 0; k < numDesc; k++)
		{
			cv::KeyPoint cv_key_point(cv::Point2f(EZ_keypoints[k].row, EZ_keypoints[k].col),
				EZ_keypoints[k].scale, EZ_keypoints[k].ori);
			CV_keypoints.push_back(cv_key_point);
		}
		for (unsigned int y = 0; y<numDesc; y++)
		{
			const float* srcRow = EZ_keypoints[y].descrip;
			unsigned char* descRowCV = (&CV_descriptors.data[CV_descriptors.step*y]);
			for (int j = 0; j<featSize; j++)
				descRowCV[j] = srcRow[j];
		}

		isExist_CV = true;
	}
	else
		printf("\nError during ConvertEzsiftToOpencv");
}
/************************************************************************/
/* other functions														*/
/************************************************************************/

