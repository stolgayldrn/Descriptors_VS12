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

using namespace cv;
using namespace std;

typedef enum FeatureType { AKAZE_FEATS=101, EZ_SIFT, EZ_ROOT_SIFT, OPENCV_SIFT, HESSIAN_SIFT, VL_SIFT };

class descriptors
{
public:
	descriptors(const char* file_path, const char* dsc_path, FeatureType feature);
	descriptors(const char* dsc_path, FeatureType feature);
	~descriptors(void);
	/*int ReleaseBasePointers();
	int ReleseEZSIFT();
	int ReleaseCV_Feats();*/
	int get_KeyPoint(vector<KeyPoint> CV_Keypoints) ;
	int get_descriptors(Mat CV_Descriptors) ;
	unsigned int get_num_descriptors() const;
protected:
	string header;
	unsigned int numDesc;
	string filePath;
	string dscFilePath;
	int featSize;
	FeatureType featType;
	float *Xs, *Ys, *Sizes, *Angles;
	vector<KeyPoint> CV_keypoints;
	Keypoint EZ_keypoints;
	Mat CV_descriptors;
	bool isExist_CV;
	bool isExist_EZSIFT;
	bool isRead;
	int flags;
};

class uchar_descriptors: public descriptors
{
public:
	uchar_descriptors(const char* file_path, const char* dsc_path, FeatureType feature) :descriptors(file_path, dsc_path, feature), descs(nullptr)
	{
		header += "_uChar";
		flags = 1;
	};
	
	uchar_descriptors(const char* dsc_path, FeatureType feature) :descriptors(dsc_path, feature), descs(nullptr)
	{
		flags = 1;
	};
	~uchar_descriptors(void);
	int write_dsc();		// arranged to v2
	int write_low_dsc();
	int read_dsc();			// arranged to v2
	int read_dsc_v1();
	void recursive_extract_akaze(Mat* Image, int rec, float threshold);
	int extract_AKAZE_feats();
	int extract_AKAZE_low_feats();
	int extract_EZ_SIFT();
	unsigned char* get_data() const;
	int ReleaseBasePointers();
	int ReleaseData();
	int ReleseEZSIFT();
	int ReleaseCV_Feats();
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
