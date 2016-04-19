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
	Angles(nullptr), EZ_keypoints(nullptr), flags(0), height(0), width(0), isImMatExist(false), resize(false)
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
Angles(nullptr), EZ_keypoints(nullptr), flags(0), height(0), width(0), isImMatExist(false), resize(false)
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
Ys(nullptr), Sizes(nullptr), Angles(nullptr), EZ_keypoints(nullptr), flags(0), height(0), width(0), isImMatExist(false), resize(false)
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

 int descriptors::Get_CVKeyPoint(std::vector<cv::KeyPoint> &CV_Keypoints) const
 {
	if (isExist_CV||isRead)
	{
		CV_Keypoints = CV_keypoints;
		return 1;
	}
	 printf("\nIt is not a CV type descriptor.");
	 return 0;
 }

 int  descriptors::Get_CVDescriptors(cv::Mat &CV_Descriptors) const
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

int descriptors::GetImageHeight() const
{
	return height;
}

int descriptors::GetImageWidth() const
{
	return width;
}

cv::Mat descriptors::GetImageMat() const
{
	return image;
}

int descriptors::GetFeatureSize() const
{
	return featSize;
}
/************************************************************************/
/* unsigned char type descriptor class									*/
/************************************************************************/
uchar_descriptors::~uchar_descriptors(void)
{
	try
	{
		flags *= ReleaseBasePointers();
		flags *= ReleaseData();
		flags *= ReleaseCV_Feats();
		flags *= ReleseEZSIFT();
		flags *= ReleaseImgMat();
	}
	catch (cv::Exception e)
	{
		printf("\ntype1_descriptors: destructor error.");
	}	
}

int uchar_descriptors::WriteDSC()
{
	if (dscFilePath =="")
	{
		printf("\nPath must not null.");
		return 0;
	}
	try
	{
		unsigned long long hash = dsc_magic;
		FILE* f= new FILE();
		fopen_s(&f, dscFilePath.c_str(), "wb");
		fwrite(&numDesc, sizeof(unsigned int), 1, f);
		fwrite(descs, sizeof(unsigned char), numDesc*featSize, f);
		fwrite(&hash, sizeof(unsigned long long), 1, f);
		fwrite(Xs, sizeof(float), numDesc, f);
		fwrite(Ys, sizeof(float), numDesc, f);
		fwrite(Sizes, sizeof(float), numDesc, f);
		fwrite(Angles, sizeof(float), numDesc, f);
		header += "_v3";
		size_t headerSize = header.size();
		char *cstr = new char[headerSize + 1];
		strcpy(cstr, header.c_str());
		fwrite(&headerSize, sizeof(unsigned int),1,f);
		fwrite(cstr, headerSize*sizeof(char*), 1, f);
		fclose(f);
		delete[] cstr;
		return 1;
	}
	catch (cv::Exception e)
	{
		printf("\nWrite dsc file error: %s", e.msg.c_str());
		return 0;
	}
}

int uchar_descriptors::ReadDSC()
{
	FILE *f;
	std::string  myFilePath = dscFilePath;
	fopen_s(&f, myFilePath.c_str(), "rb");
	if (f)
	{
		unsigned int myNumDesc;
		unsigned long long hash;
		
		fread(&myNumDesc, sizeof(unsigned int), 1, f);
		descs = new unsigned char[(myNumDesc + 4)*featSize];
		fread(descs, sizeof(unsigned char), (myNumDesc)*featSize, f);
		fread(&hash, sizeof(unsigned long long), 1, f);

		if (!feof(f) && (hash == dsc_magic))
		{
			Xs = new float[myNumDesc];
			Ys = new float[myNumDesc];
			Sizes = new float[myNumDesc];
			Angles = new float[myNumDesc];
			// read the geometry information
			fread(Xs, sizeof(float), myNumDesc, f);
			fread(Ys, sizeof(float), myNumDesc, f);
			fread(Sizes, sizeof(float), myNumDesc, f);
			fread(Angles, sizeof(float), myNumDesc, f);
		}
		unsigned int headerSize =  0;
		fread(&headerSize,sizeof(unsigned int), 1, f);
		/*char *cstr = new char[headerSize];
		fread(cstr, headerSize*sizeof(char*), 1, f);*/
		
		isRead = true;
		fclose(f);
		//header = cstr;
		numDesc = myNumDesc;
		//delete[] cstr;
	}
	else
	{
		printf("Cannot read file: %s\n", dscFilePath.c_str());
		return 0;
	}
	
	return 1;
}

int uchar_descriptors::ReadDSC__ver1()
{
	FILE *f;
	fopen_s(&f,dscFilePath.c_str(), "rb");
	if (f)
	{
		unsigned long long hash;
		char *cstr = new char[100];
		fread(cstr, sizeof(std::string), 1, f);
		fread(&numDesc, sizeof(unsigned int), 1, f);
		descs = new unsigned char[(numDesc+4)*featSize];
		fread(descs, sizeof(unsigned char), (numDesc)*featSize, f);
		fread(&hash, sizeof(unsigned long long), 1, f);

		if (!feof(f) && (hash == dsc_magic))
		{
			Xs = new float[numDesc];
			Ys = new float[numDesc];
			Sizes = new float[numDesc];
			Angles = new float[numDesc];
			// read the geometry information
			fread(Xs, sizeof(float), numDesc, f);
			fread(Ys, sizeof(float), numDesc, f);
			fread(Sizes, sizeof(float), numDesc, f);
			fread(Angles, sizeof(float), numDesc, f);
		}
		fclose(f);
		delete[] cstr;
	}
	else
	{
		printf("Cannot read file: %s\n", dscFilePath.c_str());
		return 0;
	}
	isRead = true;
	return 0;
}

void uchar_descriptors::recursiveExtractAKAZE(cv::Mat* Image, int rec, double threshold)
{
	CV_keypoints.clear();
	CV_descriptors.release();
	threshold = (numDesc<MIN_FEATURE_SIZE) ? threshold/4: threshold*2;
	cv::Ptr<cv::AKAZE> akazeRec = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 0, 3, threshold, 4, 4);
	akazeRec->detectAndCompute(*Image, cv::noArray(), CV_keypoints, CV_descriptors);
	numDesc = CV_descriptors.rows;
	akazeRec->clear();
	akazeRec.release();
	rec++;
	if (((numDesc < MIN_FEATURE_SIZE) || (numDesc > MAX_FEATURE_SIZE)) && (rec < 10))
		recursiveExtractAKAZE(Image, rec, threshold);
}

void uchar_descriptors::setResizeImage(bool reSize)
{
	resize = reSize;
}

void uchar_descriptors::resizeImage(cv::Mat* Image, double maxSize)
{
	if (Image->rows > maxSize || Image->cols > maxSize)
	{
		int h = Image->rows;
		int w = Image->cols; 
		double hs = h / maxSize;
		double ws = w / maxSize;
		std::cout << "__RESIZED__";
		if (h>w)
			cv::resize(*Image, *Image, cv::Size(w / hs, h / hs), 0, 0, IMAGE_RESIZE_ALG);
		else
			cv::resize(*Image, *Image, cv::Size(w / ws, h / ws), 0, 0, IMAGE_RESIZE_ALG);
	}
}

int uchar_descriptors::ExtractAKAZE()
{
	featSize = 61;
	cv::Mat *Image = new cv::Mat();

	std::string myFilePath = filePath;
	try
	{
		if (isImMatExist)
			*Image = image;
		else
			*Image = cv::imread(myFilePath, cv::IMREAD_GRAYSCALE);
		if (resize)
			resizeImage(Image, MAX_IMAGE_SIZE);
	}
	catch (std::exception e)
	{
		printf("\nExtract AKAZE:::imread error:%s", e.what());
	}

	if ( Image->cols>=featSize && Image->rows>=featSize)
	{
		height = Image->rows;
		width = Image->cols;
		cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();
		try
		{
			akaze->detectAndCompute(*Image, cv::noArray(), CV_keypoints, CV_descriptors);
		}
		catch (std::exception e)
		{
			printf("\nExtract AKAZE:::detect and compute error:%s", e.what());
		}

		numDesc = CV_descriptors.rows;
		int rec = 1;
		double threshold = 0.001;
		if (numDesc < MIN_FEATURE_SIZE || numDesc > MAX_FEATURE_SIZE)
			recursiveExtractAKAZE(Image, rec, threshold);
		if (numDesc)
		{
			descs = new unsigned char[(numDesc + 4)*featSize];
			Xs = new float[(numDesc+4)];
			Ys = new float[(numDesc+4)];
			Sizes = new float[(numDesc+4)];
			Angles = new float[(numDesc+4)];
			unsigned char *d = descs;
			for (unsigned int k=0; k<numDesc; k++)
			{
				cv::KeyPoint key = CV_keypoints[k];
				Xs[k] = key.pt.x;
				Ys[k] = key.pt.y;
				Sizes[k] = key.size;
				Angles[k] = key.angle;
				for(int j = 0; j < featSize; ++j)
				{
					int intdesc = int(CV_descriptors.data[j+(k*featSize)]);
					intdesc = (intdesc > 255) ? 255 : intdesc;
					*d=unsigned char(intdesc);
					d++;
				}
			}
			//printf("Feature extracted:  File: - %s... Num Features: %d\n", filePath.c_str(), numDesc);
		}
		else
		{
			printf("\nError at ExtractAKAZE:::: Unable to extract descriptors in frame %s.", filePath.c_str());
			delete Image;
			return 2;
		}
		akaze->clear();
		akaze.release();
	}
	else
	{
		printf("\nError at ExtractAKAZE:::: Image is too small or corrupted: %s. ", filePath.c_str());
		delete Image;
		return 3;
	}
	Image->empty();
	Image->release();
	Image->deallocate();
	isExist_CV = true;
	delete Image;
	return 1;
}



int uchar_descriptors::ExtractEZSIFT()
{
	cv::Mat *Image = new cv::Mat();
	std::string myFilePath = filePath;
	*Image = cv::imread(myFilePath, cv::IMREAD_GRAYSCALE);
	auto *numKpts = new int();

	EZ_keypoints  = extract_ezsift(Image->data,Image->cols, Image->rows,Image->step,numKpts);
	numDesc = *numKpts;
	if (numDesc)
	{
		descs = new unsigned char[(numDesc+4)*featSize];
		Xs = new float[(numDesc+4)];
		Ys = new float[(numDesc+4)];
		Sizes = new float[(numDesc+4)];
		Angles = new float[(numDesc+4)];
		float* Xp = Xs;
		float* Yp = Ys;
		float* Sp = Sizes;
		float* Ap = Angles;
		unsigned char* d = descs;

		for (int k = 0; k < *numKpts; k++)
		{
			struct KeypointSt key = EZ_keypoints[k];
			*Xp = key.row;
			*Yp = key.col;
			*Sp = key.scale;
			*Ap = key.ori;

			for (int l = 0; l < featSize; l++, d++)
				*d = key.descrip[l];
			Xp++;
			Yp++;
			Sp++;
			Ap++;
		}
	}
	//printf("Feature extracted:  File: - %s... Num Features: %d\n", filePath.c_str(), numDesc);
	isExist_EZSIFT = true;
	delete Image;
	return 1;
}

unsigned char* uchar_descriptors::GetUCHAR_descriptors() const
{
	return descs;
}

int uchar_descriptors::GetReadModeDescriptors(cv::Mat &CV_Descriptors) const
{
	if (isRead)
	{
		if (!CV_Descriptors.empty())
		{
			for (unsigned int i = 0; i<numDesc; i++)
			{
				const unsigned char* srcRow = &descs[i * featSize];
				unsigned char* descRowCV = (&CV_Descriptors.data[CV_Descriptors.step*i]);
				for (int j = 0; j<featSize; j++)
					descRowCV[j] = srcRow[j];
			}
			//CV_Descriptors =  Mat(numDesc, featSize, CV_8U, descs);
			/*for (uint y = 0; y < numDesc; y++)
				for (uint x = 0; x < featSize; x++)
					CV_Descriptors.at<CV_8U>[y][x] = (descs[y*featSize + x]);*/
		}
		else
			printf("\nEmpty Mat file.");
		return 1;
	}
	printf("\nIt is not a CV type descriptor nor Read type.");
	return 0;
}

int uchar_descriptors::ReleaseBasePointers()
{
	if(numDesc)
	{
		delete[] Xs;
		delete[] Ys;
		delete[] Sizes;
		delete[] Angles;
		numDesc = 0;
		return 1;
	}
	else
		return 0;
}

int uchar_descriptors::ReleaseData()
{
	if (isExist_CV || isExist_EZSIFT || isRead)
	{		
		delete[] descs;
		isRead = false;
		return 1;
	}
	else
		return 0;
}

int uchar_descriptors::ReleseEZSIFT()
{
	if (isExist_EZSIFT)
	{
		free(EZ_keypoints);
		isExist_EZSIFT = false;
		return 1;
	}
	else
		return 0;
}

int uchar_descriptors::ReleaseCV_Feats()
{
	if (isExist_CV)
	{
		CV_keypoints.clear();
		CV_keypoints.shrink_to_fit();

		CV_descriptors.release();
		CV_descriptors.deallocate();
		CV_descriptors.~Mat();
		//delete CV_descriptors;
		isExist_CV = false;
		return 1;
	}
	return 0;
}

int uchar_descriptors::ReleaseImgMat()
{
	if (isImMatExist)
	{
		image.release();
		image.deallocate();
		image.~Mat();
		return 1;
	}
	return 0;
}

/************************************************************************/
/* float type descriptor class											*/
/************************************************************************/
float_descriptors::~float_descriptors(void)
{
	try
	{
		flags *= ReleaseBasePointers();
		flags *= ReleaseCV_Feats();
		flags *= ReleseEZSIFT();
	}
	catch (cv::Exception e)
	{
		printf("\ntype1_descriptors: destructor error.");
	}
	
}

int float_descriptors::write_dsc() const
{
	if (dscFilePath=="")
	{
		printf("\nPath must not null.");
		return 0;
	}
	try
	{
		unsigned long long hash = dsc_magic;
		struct _iobuf* f = new FILE();
		fopen_s(&f, dscFilePath.c_str(), "wb");
		fwrite(&header, sizeof(std::string), 1, f);
		fwrite(&numDesc, sizeof(unsigned int), 1, f);
		fwrite(descs, sizeof(float), numDesc*featSize, f);
		fwrite(&hash, sizeof(unsigned long long), 1, f);
		fwrite(Xs, sizeof(float), numDesc, f);
		fwrite(Ys, sizeof(float), numDesc, f);
		fwrite(Sizes, sizeof(float), numDesc, f);
		fwrite(Angles, sizeof(float), numDesc, f);
		fclose(f);
		return 1;
	}
	catch (cv::Exception e)
	{
		printf("\nWrite dsc file error: %s", e.msg.c_str());
		return 0;
	}
}

int float_descriptors::read_dsc_v1()
{
	struct _iobuf*f;
	fopen_s(&f,dscFilePath.c_str(), "rb");
	if (f)
	{
		unsigned long long hash;
		char *cstr = new char[100];
		fread(cstr, sizeof(std::string), 1, f);
		fread(&numDesc, sizeof(unsigned int), 1, f);
		descs = new float[(numDesc+4)*featSize];
		fread(descs, sizeof(float), (numDesc)*featSize, f);
		fread(&hash, sizeof(unsigned long long), 1, f);

		if (!feof(f) && (hash == dsc_magic))
		{
			Xs = new float[numDesc];
			Ys = new float[numDesc];
			Sizes = new float[numDesc];
			Angles = new float[numDesc];
			// read the geometry information
			fread(Xs, sizeof(float), numDesc, f);
			fread(Ys, sizeof(float), numDesc, f);
			fread(Sizes, sizeof(float), numDesc, f);
			fread(Angles, sizeof(float), numDesc, f);
		}
		fclose(f);
		delete[] cstr;
		isRead = true;
		return 1;
	}
	printf("Cannot read file: %s\n", dscFilePath.c_str());
	return 0;
}

int float_descriptors::extract_EZ_SIFT()
{
	cv::Mat *Image = new cv::Mat();
	*Image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
	int *numKpts = new int();

	EZ_keypoints  = extract_ezsift(Image->data,Image->cols, Image->rows,Image->step,numKpts);
	numDesc = *numKpts;
	if (numDesc)
	{
		descs = new float[(numDesc+4)*featSize];
		Xs = new float[(numDesc+4)];
		Ys = new float[(numDesc+4)];
		Sizes = new float[(numDesc+4)];
		Angles = new float[(numDesc+4)];
		float* Xp = Xs;
		float* Yp = Ys;
		float* Sp = Sizes;
		float* Ap = Angles;
		float* d = descs;

		for (int k = 0; k < *numKpts; k++)
		{
			struct KeypointSt key = EZ_keypoints[k];
			*Xp = key.row;
			*Yp = key.col;
			*Sp = key.scale;
			*Ap = key.ori;

			for (int l = 0; l < featSize; l++)
			{
				*d = key.descrip[l];
				d++;
			}
			Xp++;
			Yp++;
			Sp++;
			Ap++;
		}
	}

	delete Image;
	return 1;
}

int float_descriptors::extract_EZ_ROOT_SIFT()
{
	cv::Mat *Image = new cv::Mat();
	*Image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
	int *numKpts = new int();

	EZ_keypoints  = extract_ezsift(Image->data,Image->cols, Image->rows,Image->step,numKpts);
	numDesc = *numKpts;
	if (numDesc)
	{
		descs = new float[(numDesc+4)*featSize];
		Xs = new float[(numDesc+4)];
		Ys = new float[(numDesc+4)];
		Sizes = new float[(numDesc+4)];
		Angles = new float[(numDesc+4)];
		float* Xp = Xs;
		float* Yp = Ys;
		float* Sp = Sizes;
		float* Ap = Angles;
		float* d = descs;
		float sum_descs = 0;

		for (int k = 0; k < *numKpts; k++)
		{
			struct KeypointSt key = EZ_keypoints[k];
			*Xp = key.row;
			*Yp = key.col;
			*Sp = key.scale;
			*Ap = key.ori;
			for (int l = 0; l < featSize; l++)
				sum_descs +=key.descrip[l];

			for (int l = 0; l < featSize; l++)
			{
				*d = sqrt(key.descrip[l] / sum_descs);
				d++;
			}
			Xp++;
			Yp++;
			Sp++;
			Ap++;
		}
	}
	Image->release();
	Image->deallocate();
	delete numKpts;
	delete Image;
	printf("\nFeature extracted:  File: - %s... Num Features: %d...", filePath.c_str(), numDesc);
	return 1;
}

float* float_descriptors::get_data() const
{
	return descs;
}

int float_descriptors::ReleaseBasePointers()
{
	if(numDesc)
	{
		delete[] Xs;
		delete[] Ys;
		delete[] Sizes;
		delete[] Angles;
		numDesc = 0;
		return 1;
	}
	else
		return 0;

}
int float_descriptors::ReleaseData()
{
	if (isExist_CV || isExist_EZSIFT || isRead)
	{
		delete[] descs;
		isRead = false;
		return 1;
	}
	return 0;
}

int float_descriptors::ReleseEZSIFT()
{
	if (isExist_EZSIFT)
	{
		free(EZ_keypoints);
		isExist_EZSIFT = false;
		return 1;
	}
	else
	{
		return 0;
	}
}

int float_descriptors::ReleaseCV_Feats()
{
	if (isExist_CV)
	{
		CV_keypoints.clear();
		//CV_keypoints.shrink_to_fit();
		CV_descriptors.release();
		//delete CV_descriptors;
		isExist_CV = false;
		return 1;
	}
	else
	{
		return 0;
	}

}
/************************************************************************/
/* other functions														*/
/************************************************************************/