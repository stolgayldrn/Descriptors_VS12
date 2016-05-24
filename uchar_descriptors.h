#ifndef UCHAR_DESCRIPTORS_H
#define UCHAR_DESCRIPTORS_H

#include "descriptors.h"
class uchar_descriptors : public descriptors
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

/*Other Functions*/
void findMatches(uchar_descriptors &descriptor_1, uchar_descriptors &descriptor_2, std::vector<cv::DMatch >& good_matches);
int findIntersectedFeatures(std::string imgPath, cv::Mat img1, uchar_descriptors& descriptor_1, std::vector<cv::DMatch >& inMatches);
int findFlannBasedGoodMatches(cv::Mat &descriptors_1, cv::Mat &descriptors_2, std::vector< cv::DMatch > &matches, std::vector< cv::DMatch > &good_matches, float gm_distance);

#endif
