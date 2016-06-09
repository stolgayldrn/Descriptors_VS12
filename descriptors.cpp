#include "descriptors.h"
/** @brief The image class for holding OpenCV and EZSIFT libraries based features, 
address and statistical information about and image or dsc file.

@param file_path The disk address of image file
@param dsc_path The disk address of dsc file
@param feature Type of extracted descriptor: AKAZE_FEATS, EZ_SIFT, EZ_ROOT_SIFT, OPENCV_SIFT, HESSIAN_SIFT, VL_SIFT.
*/
Descriptors::Descriptors(const char* file_path, const char* dsc_path, FeatureType feature): Xs(nullptr), 
Ys(nullptr), Sizes(nullptr), Angles(nullptr), EZ_keypoints(nullptr), 
flags(0), height(0), width(0), isExist_OpencvMatImage(false), resize(false)
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
/** @brief The image class for holding OpenCV and EZSIFT libraries based features, 
address and statistical information about and image or dsc file.

@param file_path The disk address of image file.
@param ImageMat OpenCV based image matrix.
@param dsc_path The disk address of dsc file.
@param feature Type of extracted descriptor: AKAZE_FEATS, EZ_SIFT, EZ_ROOT_SIFT, OPENCV_SIFT, HESSIAN_SIFT, VL_SIFT.
*/
Descriptors::Descriptors(const char* file_path, const cv::Mat ImageMat, const char* dsc_path, FeatureType feature) : Xs(nullptr), Ys(nullptr), Sizes(nullptr),
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
/** @brief The image class for holding OpenCV and EZSIFT libraries based features, 
address and statistical information about and image or dsc file.

@param dsc_path The disk address of dsc file.
@param feature Type of extracted descriptor: AKAZE_FEATS, EZ_SIFT, EZ_ROOT_SIFT, OPENCV_SIFT, HESSIAN_SIFT, VL_SIFT.
*/
Descriptors::Descriptors(const char* dsc_path, FeatureType feature): Xs(nullptr), 
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
/** @brief The destructor*/
Descriptors::~Descriptors(void)
{
	try
	{
		/*releaseBasePointers();
		releaseOpencvFeats();
		releseEZSIFT();*/
	}
	catch (cv::Exception e)
	{
		printf("\nDescriptors: destructor error.");
	}
}

//int Descriptors::releaseBasePointers()
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
//int Descriptors::releseEZSIFT()
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
//int Descriptors::releaseOpencvFeats()
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
/** @brief Copying OpenCV type Keypoints

@param CV_Keypoints OUTPUT

@note It will be copied if the descriptors is an OpenCV type descriptor.
*/
 int Descriptors::getCopyOfOpencvKeypoints(std::vector<cv::KeyPoint> &CV_Keypoints) const
 {
	if (isExist_CV||isRead)
	{
		CV_Keypoints = CV_keypoints;
		return 1;
	}
	 printf("\nIt is not a CV type descriptor.");
	 return 0;
 }
 /** @brief Copying OpenCV type descriptors.

 @param CV_Descriptors OUTPUT

 @note It will be copied if the descriptors is an OpenCV type descriptor.
 */
 int  Descriptors::getCopyOfOpencvDescriptors(cv::Mat &CV_Descriptors) const
 {
	if (isExist_CV||isRead)
	{
		CV_Descriptors = CV_descriptors;
		return 1;
	}
	printf("\nIt is not a CV type descriptor.");
	return 0;
}
 /** @brief Returns total number of descriptor.*/
unsigned int Descriptors::getNumOfDescriptors() const
{
	return numDesc;
}
/** @brief Returns X coordinates vector of keypoints.*/
std::vector<float> Descriptors::getCoordsX() const
{
	std::vector<float> coords;
	for (unsigned int i = 0; i < numDesc; i++)
		coords.push_back(Xs[i]);
	return coords;
}
/** @brief Returns Y coordinates vector of keypoints.*/
std::vector<float> Descriptors::getCoordsY() const
{
	std::vector<float> coords;
	for (unsigned int i = 0; i < numDesc; i++)
		coords.push_back(Ys[i]);
	return coords;
}
/** @brief Returns (X,Y) coordinates vector of keypoints.*/
std::vector<cv::Point2f> Descriptors::getCoords() const
{
	std::vector<cv::Point2f> coords;
	for (unsigned int i = 0; i < numDesc; i++)
		coords.push_back(cv::Point2f(Xs[i], Ys[i]));
	return coords;
}
/** @brief Returns size of the feature type.

@note 61 for AKAZE_FEATS, 128 for all SIFT features (EZ_SIFT, HESSIAN_SIFT, vb.)
*/
int Descriptors::getFeatureSize() const
{
	return featSize;
}
/** @brief Returns height of image matrix.

@note If image is resized, returned height will be height of resized version, not height of original image. 

*/
int Descriptors::getImageHeight() const
{
	return height;
}
/** @brief Returns width of image matrix.

@note If image is resized, returned width will be width of resized version, not width of original image.

*/
int Descriptors::getImageWidth() const
{
	return width;
}
/** @brief Copying the image matrix.

@param writeImg OUTPUT

@note If image is resized, returned image will be the resized version of image.

*/
int Descriptors::getCopyOfImageMatrix(cv::Mat &writeImg) const
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
/** @brief Returns the image matrix.*/
cv::Mat Descriptors::getImageMat() const
{
	if (isExist_OpencvMatImage)
		return image;
	else
		printf("No Opencv Image");
	return cv::Mat::ones(2, 2, CV_8U)*-1;
}
/** @brief Returns the OpenCV type descriptor.

@note It will be returned if the descriptors is an OpenCV type descriptor.

*/
cv::Mat Descriptors::getOpencvDescriptors() const
{
	if (isExist_CV)
		return CV_descriptors;
	else
		printf("No Opencv Descriptors");
	return cv::Mat::ones(2, 2, CV_8U)*-1;
}
/** @brief Returns feature type of the descriptor object: AKAZE_FEATS, 
EZ_SIFT, EZ_ROOT_SIFT, OPENCV_SIFT, HESSIAN_SIFT, VL_SIFT.*/
FeatureType Descriptors::getFeatureType() const
{
	return featType;
}
/** @brief Returns OpenCV type keypoints

@note It will be returned if the descriptors is an OpenCV type descriptor.

*/
std::vector<cv::KeyPoint> Descriptors::getOpencvKeypoints() const
{
	if (isExist_CV)
		return CV_keypoints;
	else
		printf("No Opencv Descriptors");
	std::vector<cv::KeyPoint> returnVec;
	returnVec.push_back(cv::KeyPoint());
	return returnVec;
}
/** @brief Converts the EZ_SIFT type descriptors to OpenCV type descriptor.

1. Keypoints are converted.

2. EZ_Descriptors are converted.

*/
void Descriptors::convertEzsiftToOpencv() 
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
		printf("\nError during convertEzsiftToOpencv");
}
/************************************************************************/
/* other functions														*/
/************************************************************************/

