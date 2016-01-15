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

using namespace cv;
using namespace std;

descriptors::descriptors(const char* file_path, const char* dsc_path, 
	FeatureType feature): Xs(nullptr), Ys(nullptr), Sizes(nullptr), 
	Angles(nullptr), EZ_keypoints(nullptr), flags(0)
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

descriptors::descriptors(const char* dsc_path, FeatureType feature): Xs(nullptr), 
Ys(nullptr), Sizes(nullptr), Angles(nullptr), EZ_keypoints(nullptr), flags(0)
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
	catch (Exception e)
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

 int descriptors::get_KeyPoint(vector<KeyPoint> CV_Keypoints) 
 {
	if (isExist_CV)
	{
		CV_Keypoints = CV_keypoints;
		return 1;
	}
	 printf("\nIt is not a CV type descriptor.");
	 return 0;
 }

int  descriptors::get_descriptors(Mat CV_Descriptors) 
{
	if (isExist_CV)
	{
		CV_Descriptors = CV_descriptors;
		return 1;
	}
	printf("\nIt is not a CV type descriptor.");
	return 0;
}

unsigned int descriptors::get_num_descriptors() const
{
	return numDesc;
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
	}
	catch (Exception e)
	{
		printf("\ntype1_descriptors: destructor error.");
	}
	
}

int uchar_descriptors::write_dsc()
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
		unsigned int headerSize = header.size();
		char *cstr = new char[headerSize + 1];
		strcpy(cstr, header.c_str());
		fwrite(&headerSize, sizeof(unsigned int),1,f);
		fwrite(cstr, headerSize*sizeof(char*), 1, f);
		fclose(f);
		delete[] cstr;
		return 1;
	}
	catch (Exception e)
	{
		printf("\nWrite dsc file error: ", e.msg.c_str());
		return 0;
	}
}

int uchar_descriptors::write_low_dsc()
{
	if (dscFilePath == "")
	{
		printf("\nPath must not null.");
		return 0;
	}
	try
	{
		unsigned long long hash = dsc_magic;
		FILE* f = new FILE();
		fopen_s(&f, dscFilePath.c_str(), "wb");
		if (numDesc > 0)
		{
			fwrite(&numDesc, sizeof(unsigned int), 1, f);
			fwrite(descs, sizeof(unsigned char), numDesc*featSize, f);
			fwrite(&hash, sizeof(unsigned long long), 1, f);
			fwrite(Xs, sizeof(float), numDesc, f);
			fwrite(Ys, sizeof(float), numDesc, f);
			fwrite(Sizes, sizeof(float), numDesc, f);
			fwrite(Angles, sizeof(float), numDesc, f);
		}
		else
		{
			int num = 0;
			fwrite(&num, sizeof(unsigned int), 1, f);
		}
		header += "_low_v3";
		unsigned int headerSize = header.size();
		char *cstr = new char[headerSize + 1];
		strcpy(cstr, header.c_str());
		fwrite(&headerSize, sizeof(unsigned int), 1, f);
		fwrite(cstr, headerSize*sizeof(char*), 1, f);
		fclose(f);
		delete[] cstr;
		return 1;
	}
	catch (Exception e)
	{
		printf("\nWrite dsc file error: ", e.msg.c_str());
		return 0;
	}
}

int uchar_descriptors::read_dsc()
{
	FILE *f;
	string  myFilePath = dscFilePath;
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

int uchar_descriptors::read_dsc_v1()
{
	FILE *f;
	fopen_s(&f,dscFilePath.c_str(), "rb");
	if (f)
	{
		unsigned long long hash;
		char *cstr = new char[100];
		fread(cstr, sizeof(string), 1, f);
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

void uchar_descriptors::recursive_extract_akaze(Mat* Image, int rec, float threshold)
{
	CV_keypoints.clear();
	CV_descriptors.release();
	threshold /= 10;
	Ptr<AKAZE> akazeRec = AKAZE::create(AKAZE::DESCRIPTOR_MLDB, 0, 3, threshold, 4, 4);
	akazeRec->detectAndCompute(*Image, noArray(), CV_keypoints, CV_descriptors);
	numDesc = CV_descriptors.rows;
	akazeRec->clear();
	akazeRec.release();
	rec++;
	if ((numDesc < 10) && (rec < 10))
		recursive_extract_akaze(Image, rec, threshold);
}

int uchar_descriptors::extract_AKAZE_feats()
{
	//CV_descriptors = new Mat();
	featSize = 61;
	Mat *Image = new Mat();
	string myFilePath = filePath;
	try
	{
		*Image = imread(myFilePath, IMREAD_GRAYSCALE);
	}
	catch (exception e)
	{
		printf("\nExtract AKAZE:::imread error:%s", e.what());
	}

	if ( Image->cols>=featSize && Image->rows>=featSize)
	{
		Ptr<AKAZE> akaze = AKAZE::create();
		try
		{
			akaze->detectAndCompute(*Image, noArray(), CV_keypoints, CV_descriptors);
		}
		catch (exception e)
		{
			printf("\nExtract AKAZE:::detect and compute error:%s", e.what());
		}

		numDesc = CV_descriptors.rows;
		int rec = 1;
		float threshold = 0.001;
		if (numDesc < 10)
		{
			recursive_extract_akaze(Image, rec, threshold);
		}
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
				KeyPoint key = CV_keypoints[k];
				Xs[k] = key.pt.x;
				Ys[k] = key.pt.y;
				Sizes[k] = key.size;
				Angles[k] = key.angle;
				for(int j = 0; j < featSize; ++j)
				{
					int intdesc = int(CV_descriptors.data[j+(k*featSize)]);
					if( intdesc > 255 )
						intdesc = 255;
					*d=unsigned char(intdesc);
					d++;
				}
			}
			//printf("Feature extracted:  File: - %s... Num Features: %d\n", filePath.c_str(), numDesc);
		}
		else
		{
			printf("\nError at extract_AKAZE_feats:::: Unable to extract descriptors in frame %s.", filePath.c_str());
			
			delete Image;
			return 0;
		}
		akaze->clear();
		akaze.release();
	}
	else
	{
		printf("\nError at extract_AKAZE_feats:::: Frame is too small : %s. ", filePath.c_str());
		
		delete Image;
		return 0;
	}

	Image->empty();
	Image->release();
	Image->deallocate();
	isExist_CV = true;
	delete Image;
	return 1;
}

int uchar_descriptors::extract_AKAZE_low_feats()
{
	//CV_descriptors = new Mat();
	featSize = 61;
	Mat *Image = new Mat();
	string myFilePath = filePath;
	try
	{
		*Image = imread(myFilePath, IMREAD_GRAYSCALE);
	}
	catch (exception e)
	{
		printf("\nExtract AKAZE:::imread error:%s", e.what());
	}

	if (Image->cols >= featSize && Image->rows >= featSize)
	{
		Ptr<AKAZE> akaze = AKAZE::create(AKAZE::DESCRIPTOR_MLDB, 0, 3, 0.008, 40, 4);
		try
		{
			akaze->detectAndCompute(*Image, noArray(), CV_keypoints, CV_descriptors);
		}
		catch (exception e)
		{
			printf("\nExtract AKAZE:::detect and compute error:%s", e.what());
		}

		numDesc = CV_descriptors.rows;
		int rec = 1;
		float threshold = 0.008;
		if (numDesc < 10)
		{
			recursive_extract_akaze(Image, rec, threshold);
		}
		if (numDesc)
		{
			descs = new unsigned char[(numDesc )*featSize];
			Xs = new float[(numDesc )];
			Ys = new float[(numDesc )];
			Sizes = new float[(numDesc )];
			Angles = new float[(numDesc )];
			unsigned char *d = descs;
			for (unsigned int k = 0; k<numDesc; k++)
			{
				KeyPoint key = CV_keypoints[k];
				Xs[k] = key.pt.x;
				Ys[k] = key.pt.y;
				Sizes[k] = key.size;
				Angles[k] = key.angle;
				for (int j = 0; j < featSize; ++j)
				{
					int intdesc = int(CV_descriptors.data[j + (k*featSize)]);
					if (intdesc > 255)
						intdesc = 255;
					*d = unsigned char(intdesc);
					d++;
				}
			}
			//printf("Feature extracted:  File: - %s... Num Features: %d\n", filePath.c_str(), numDesc);
		}
		else
		{
			printf("\nError at extract_AKAZE_feats:::: Unable to extract descriptors in frame %s.", filePath.c_str());

			delete Image;
			return 0;
		}
		akaze->clear();
	}
	else
	{
		printf("\nError at extract_AKAZE_feats:::: Frame is too small : %s. ", filePath.c_str());

		delete Image;
		return 0;
	}

	Image->empty();
	Image->release();
	Image->deallocate();
	isExist_CV = true;
	delete Image;
	return 1;
}

int uchar_descriptors::extract_EZ_SIFT()
{
	Mat *Image = new Mat();
	string myFilePath = filePath;
	*Image = imread(myFilePath,IMREAD_GRAYSCALE);
	int *numKpts = new int();

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
	printf("Feature extracted:  File: - %s... Num Features: %d\n", filePath.c_str(), numDesc);

	isExist_EZSIFT = true;
	delete Image;
	return 1;
}

unsigned char* uchar_descriptors::get_data() const
{
	return descs;
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
	{
		return 0;
	}
}

int uchar_descriptors::ReleaseCV_Feats()
{
	if (isExist_CV)
	{
		CV_keypoints.clear();
		CV_descriptors.release();
		CV_keypoints.shrink_to_fit();
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
	catch (Exception e)
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
		fwrite(&header, sizeof(string), 1, f);
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
	catch (Exception e)
	{
		printf("\nWrite dsc file error: ", e.msg.c_str());
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
		fread(cstr, sizeof(string), 1, f);
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
	Mat *Image = new Mat();
	*Image = imread(filePath,IMREAD_GRAYSCALE);
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
	Mat *Image = new Mat();
	*Image = imread(filePath,IMREAD_GRAYSCALE);
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