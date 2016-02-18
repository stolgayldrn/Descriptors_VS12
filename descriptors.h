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


typedef enum FeatureType { AKAZE_FEATS=101, EZ_SIFT, EZ_ROOT_SIFT, OPENCV_SIFT, HESSIAN_SIFT, VL_SIFT };

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
	int Get_CVKeyPoint(std::vector<cv::KeyPoint> &CV_Keypoints) const;
	int Get_CVDescriptors(cv::Mat &CV_Descriptors) const;
	unsigned int GetNumOfDescriptors() const;
	std::vector<float> getCoordsX() const;
	std::vector<float> getCoordsY() const;
	std::vector<cv::Point2f> GetCoords() const;
	int GetFeatureSize() const;
	int GetImageHeight() const;
	int GetImageWidth() const;
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
	bool isImMatExist;
	bool resize;
};

class uchar_descriptors: public descriptors
{
public:
	uchar_descriptors(const char* file_path, const char* dsc_path, FeatureType feature) :descriptors(file_path, dsc_path, feature), descs(nullptr)
	{
		header += "_uChar";
		flags = 1;
		isImMatExist = false;
		resize = false;
	};

	uchar_descriptors(const char* file_path, const cv::Mat ImageMat, const char* dsc_path, FeatureType feature) :descriptors(file_path, ImageMat, dsc_path, feature), descs(nullptr)
	{
		header += "_uChar";
		flags = 1;
		isImMatExist = true;
		resize = false;
	};
	
	uchar_descriptors(const char* dsc_path, FeatureType feature) :descriptors(dsc_path, feature), descs(nullptr)
	{
		flags = 1;
		isImMatExist = false;
		resize = false;
	};
	~uchar_descriptors(void);
	int WriteDSC();		// arranged to v2
	int WriteLowDSC();
	int ReadDSC();			// arranged to v2
	int ReadDSC__ver1();
	void recursiveExtractAKAZE(cv::Mat* Image, int rec, float threshold);
	static void resizeImage(cv::Mat* Image, double maxSize);
	void setResizeImage(bool reSize);
	int ExtractAKAZE();
	int ExtractAKAZE_low();
	int ExtractEZSIFT();
	unsigned char* GetUCHAR_descriptors() const;
	int  GetReadModeDescriptors(cv::Mat &CV_Descriptors);
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
