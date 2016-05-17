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

class descriptors
{
public:
	descriptors(const char* file_path, const char* dsc_path, FeatureType feature);
	descriptors(const char* file_path, const cv::Mat ImageMat, const char* dsc_path, FeatureType feature);
	descriptors(const char* dsc_path, FeatureType feature);
	~descriptors(void);
	/*int ReleaseBasePointers();
	int ReleseEZSIFT();
	int ReleaseCV_Feats();*/
	int CopyOpencvKeypoints(std::vector<cv::KeyPoint> &CV_Keypoints) const;
	int CopyOpencvDescriptors(cv::Mat &CV_Descriptors) const;
	unsigned int GetNumOfDescriptors() const;
	std::vector<float> getCoordsX() const;
	std::vector<float> getCoordsY() const;
	std::vector<cv::Point2f> GetCoords() const;
	int GetFeatureSize() const;
	int GetImageHeight() const;
	int GetImageWidth() const;
	int GetImage__Copy(cv::Mat &writeImg) const;
	cv::Mat GetImageMat() const;
	cv::Mat GetOpencvDescriptors() const;
	FeatureType GetFeatureType() const;
	std::vector<cv::KeyPoint> GetOpencvKeypoints() const;
	void ConvertEzsiftToOpencv();
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

class uchar_descriptors: public descriptors
{
public:
	uchar_descriptors(const char* file_path, const char* dsc_path, FeatureType feature) :descriptors(file_path, dsc_path, feature), descs(nullptr)
	{
		header += "_uChar";
		flags = 1;
		isExist_OpencvMatImage = false;
		resize = false;
	};

	uchar_descriptors(const char* file_path, const cv::Mat ImageMat, const char* dsc_path, FeatureType feature) :descriptors(file_path, ImageMat, dsc_path, feature), descs(nullptr)
	{
		header += "_uChar";
		flags = 1;
		isExist_OpencvMatImage = true;
		resize = false;
	};
	
	uchar_descriptors(const char* dsc_path, FeatureType feature) :descriptors(dsc_path, feature), descs(nullptr)
	{
		flags = 1;
		isExist_OpencvMatImage = false;
		resize = false;
	};
	~uchar_descriptors(void);
	int WriteDSC();		// arranged to v2
	int ReadDSC();			// arranged to v2
	int ReadDSC__ver1();
	void recursiveExtractAKAZE(cv::Mat* Image, int rec, double threshold);
	static void resizeImage(cv::Mat* Image, double maxSize);
	void setResizeImage(bool reSize);
	int ExtractAKAZE();
	int ExtractEZSIFT();
	unsigned char* GetUCHAR_descriptors() const;
	int  GetReadModeDescriptors(cv::Mat &CV_Descriptors) const;
	int ReleaseBasePointers();
	int ReleaseData();
	int ReleseEZSIFT();
	int ReleaseCV_Feats();
	int ReleaseImgMat();
private:
	unsigned char* descs;
};

class float_descriptors: public descriptors
{
public:
	float_descriptors(const char* file_path, const char* dsc_path, FeatureType feature) :descriptors(file_path, dsc_path, feature), descs(nullptr)
	{
		header += "type2_";
		header += "v2_";
	};
	float_descriptors(const char* dsc_path, FeatureType feature) :descriptors(dsc_path, feature), descs(nullptr)
	{
	}
	~float_descriptors(void);
	int write_dsc() const;
	int read_dsc_v1();
	int extract_EZ_SIFT();
	int extract_EZ_ROOT_SIFT();
	int ReleaseBasePointers();
	int ReleaseData();
	int ReleseEZSIFT();
	int ReleaseCV_Feats();
	float* get_data() const;
private:
	float* descs;
};

#endif
