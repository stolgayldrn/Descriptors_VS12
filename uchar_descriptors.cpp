#include "uchar_descriptors.h"
/** @brief The destructor*/
UcharDescriptors::~UcharDescriptors(void)
{
	try
	{
		flags *= releaseBasePointers();
		flags *= releaseData();
		flags *= releaseOpencvFeats();
		flags *= releseEZSIFT();
		flags *= releaseImgMat();
	}
	catch (cv::Exception e)
	{
		printf("\ntype1_descriptors: destructor error.");
	}
}
/** @brief Writing the feature, keypoints and path of the descriptor as a dsc file.*/
int UcharDescriptors::writeDSC()
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
		fwrite(&headerSize, sizeof(unsigned int), 1, f);
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

/** @brief Reading the feature, keypoints and path of the descriptor from a dsc file.*/
int UcharDescriptors::readDSC()
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
		unsigned int headerSize = 0;
		fread(&headerSize, sizeof(unsigned int), 1, f);
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

/** @brief Reading the feature, keypoints and path of the descriptor from a dsc file.

@note Old version: the version with string type header. 
*/
int UcharDescriptors::readDSC__v1()
{
	FILE *f;
	fopen_s(&f, dscFilePath.c_str(), "rb");
	if (f)
	{
		unsigned long long hash;
		char *cstr = new char[100];
		fread(cstr, sizeof(std::string), 1, f);
		fread(&numDesc, sizeof(unsigned int), 1, f);
		descs = new unsigned char[(numDesc + 4)*featSize];
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

/** @brief */
void UcharDescriptors::recursiveExtractAKAZE(cv::Mat* Image, int rec, double threshold)
{
	CV_keypoints.clear();
	CV_descriptors.release();
	threshold = (numDesc<MIN_FEATURE_SIZE) ? threshold / 3 : threshold * 2;
	cv::Ptr<cv::AKAZE> akazeRec = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 0, 3, threshold, 4, 6, cv::KAZE::DIFF_PM_G2);
	akazeRec->detectAndCompute(*Image, cv::noArray(), CV_keypoints, CV_descriptors);
	numDesc = CV_descriptors.rows;
	akazeRec->clear();
	akazeRec.release();
	rec++;
	if (((numDesc < MIN_FEATURE_SIZE) || (numDesc > MAX_FEATURE_SIZE)) && (rec < 10))
		recursiveExtractAKAZE(Image, rec, threshold);
}

/** @brief */
void UcharDescriptors::setResizeImage(bool reSize)
{
	resize = reSize;
}

void UcharDescriptors::resizeImage(cv::Mat* Image, double maxSize)
{

	if (Image->rows > maxSize || Image->cols > maxSize)
	{
		int h = Image->rows;
		int w = Image->cols;

		/*int hss = floor(h / maxSize);
		if (hss<2) hss = 2;
		else if (hss<4) hss = 4;
		else if (hss<8) hss = 8;
		else if (hss<16) hss = 16;
		else hss = 32;

		int wss = floor(w / maxSize);
		if (wss<2) wss = 2;
		else if (wss<4) wss = 4;
		else if (wss<8) wss = 8;
		else if (wss<16) wss = 16;
		else wss = 32;*/

		double hs = h / maxSize;
		double ws = w / maxSize;
		try{
			if (h > w)
				cv::resize(*Image, *Image, cv::Size(w / hs, h / hs), 0, 0, IMAGE_RESIZE_ALG);
			else
				cv::resize(*Image, *Image, cv::Size(w / ws, h / ws), 0, 0, IMAGE_RESIZE_ALG);
		}
		catch (std::exception e)
		{
			printf("\nMain:::Resizing Error:%s", e.what());
		}
	}
}

int UcharDescriptors::extractAKAZE()
{
	featSize = 61;
	cv::Mat *Image = new cv::Mat();

	std::string myFilePath = filePath;
	try
	{
		if (isExist_OpencvMatImage)
			*Image = image;
		else
			*Image = cv::imread(myFilePath, cv::IMREAD_GRAYSCALE);
		if (resize)
		{
			try
			{
				resizeImage(Image, MAX_IMAGE_SIZE);
			}
			catch (std::exception e)
			{
				printf("\nExtract AKAZE:::Resizing Error:%s", e.what());
			}
		}
		Image->copyTo(image);
		isExist_OpencvMatImage = true;
	}
	catch (std::exception e)
	{
		printf("\nExtract AKAZE:::imread error:%s", e.what());
	}

	if (Image->cols >= featSize && Image->rows >= featSize)
	{
		height = Image->rows;
		width = Image->cols;
		cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 0, 3, 0.01, 4, 6, cv::KAZE::DIFF_PM_G2);
		//cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();
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
		double threshold = 0.0005;
		if (numDesc < MIN_FEATURE_SIZE || numDesc > MAX_FEATURE_SIZE)
			recursiveExtractAKAZE(Image, rec, threshold);
		if (numDesc)
		{
			descs = new unsigned char[(numDesc + 4)*featSize];
			Xs = new float[(numDesc + 4)];
			Ys = new float[(numDesc + 4)];
			Sizes = new float[(numDesc + 4)];
			Angles = new float[(numDesc + 4)];
			unsigned char *d = descs;
			for (unsigned int k = 0; k<numDesc; k++)
			{
				cv::KeyPoint key = CV_keypoints[k];
				Xs[k] = key.pt.x;
				Ys[k] = key.pt.y;
				Sizes[k] = key.size;
				Angles[k] = key.angle;
				for (int j = 0; j < featSize; ++j)
				{
					int intdesc = int(CV_descriptors.data[j + (k*featSize)]);
					intdesc = (intdesc > 255) ? 255 : intdesc;
					*d = unsigned char(intdesc);
					d++;
				}
			}
			//printf("Feature extracted:  File: - %s... Num Features: %d\n", filePath.c_str(), numDesc);
		}
		else
		{
			printf("\nError at extractAKAZE:::: Unable to extract descriptors in frame %s.", filePath.c_str());
			delete Image;
			return 2;
		}
		akaze->clear();
		akaze.release();
	}
	else
	{
		printf("\nError at extractAKAZE:::: Image is too small or corrupted: %s. ", filePath.c_str());
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



int UcharDescriptors::extractEZSIFT()
{
	cv::Mat *Image = new cv::Mat();
	std::string myFilePath = filePath;
	try
	{
		if (isExist_OpencvMatImage)
			*Image = image;
		else
			*Image = cv::imread(myFilePath, cv::IMREAD_GRAYSCALE);
		if (resize)
			resizeImage(Image, MAX_IMAGE_SIZE);

		Image->copyTo(image);
		isExist_OpencvMatImage = true;
	}
	catch (std::exception e)
	{
		printf("\nExtract AKAZE:::imread error:%s", e.what());
	}
	auto *numKpts = new int();

	EZ_keypoints = extract_ezsift(Image->data, Image->cols, Image->rows, Image->step, numKpts);
	numDesc = *numKpts;
	if (numDesc)
	{
		descs = new unsigned char[(numDesc + 4)*featSize];
		Xs = new float[(numDesc + 4)];
		Ys = new float[(numDesc + 4)];
		Sizes = new float[(numDesc + 4)];
		Angles = new float[(numDesc + 4)];
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

unsigned char* UcharDescriptors::getUcharDescriptors() const
{
	return descs;
}

int UcharDescriptors::getReadModeDescriptors(cv::Mat &CV_Descriptors) const
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
		}
		else
			printf("\nEmpty Mat file.");
		return 1;
	}
	printf("\nIt is not a CV type descriptor nor Read type.");
	return 0;
}

int UcharDescriptors::releaseBasePointers()
{
	if (numDesc)
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

int UcharDescriptors::releaseData()
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

int UcharDescriptors::releseEZSIFT()
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

int UcharDescriptors::releaseOpencvFeats()
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

int UcharDescriptors::releaseImgMat()
{
	if (isExist_OpencvMatImage)
	{
		image.release();
		image.deallocate();
		image.~Mat();
		return 1;
	}
	return 0;
}


/************************************************************************/
/* other functions														*/
/************************************************************************/

void findMatches(UcharDescriptors &descriptor_1, UcharDescriptors &descriptor_2, std::vector<cv::DMatch >& good_matches)
{
	cv::Mat feats_1, feats_2;
	feats_1 = descriptor_1.getOpencvDescriptors();
	feats_2 = descriptor_2.getOpencvDescriptors();

	// Match with FLANN
	std::vector<cv::DMatch > matches;
	if (feats_1.type() == 0)  feats_1.convertTo(feats_1, CV_32F);
	if (feats_2.type() == 0)  feats_2.convertTo(feats_2, CV_32F);

	float gm_dist = 400;

	if (feats_1.type() == feats_2.type() && feats_1.cols == feats_2.cols)
		findFlannBasedGoodMatches(feats_1, feats_2, matches, good_matches, gm_dist);
}

int findIntersectedFeatures(std::string imgPath, cv::Mat img1, UcharDescriptors& descriptor_1, std::vector<cv::DMatch >& inMatches)
{
	int status = 0;
	auto img2 = cv::imread(imgPath.c_str());
	auto img3 = cv::imread(imgPath.c_str());

	UcharDescriptors::resizeImage(&img2, 800);
	UcharDescriptors::resizeImage(&img3, 600);

	UcharDescriptors descriptor_2(imgPath.c_str(), img2, "", AKAZE_FEATS);
	UcharDescriptors descriptor_3(imgPath.c_str(), img3, "", AKAZE_FEATS);

	try
	{
		status = descriptor_1.extractAKAZE();
		status = descriptor_2.extractAKAZE();
		status = descriptor_3.extractAKAZE();
	}
	catch (cv::Exception e)
	{
		printf("\nError at findIntersectedFeatures functions.");
		return status;
	}
	if (status && (descriptor_1.getNumOfDescriptors()> 0) &&
		(descriptor_2.getNumOfDescriptors()> 0) &&
		(descriptor_3.getNumOfDescriptors()> 0))
	{
		std::vector<cv::DMatch > gm_12, gm_13, gm_23;
		findMatches(descriptor_1, descriptor_2, gm_12);
		findMatches(descriptor_1, descriptor_3, gm_13);
		findMatches(descriptor_2, descriptor_3, gm_23);

		std::vector<int> intersectedMatches;
		int last = 0;
		for (int i = 0; i < gm_12.size(); i++)
		{
			if (gm_12[i].queryIdx < gm_13[last].queryIdx)
				continue;
			for (int k = last; k < gm_13.size(); k++)
			{
				if (gm_12[i].queryIdx == gm_13[k].queryIdx)
				{
					intersectedMatches.push_back(gm_12[i].queryIdx);
					inMatches.push_back(gm_12[i]);
					last = k;
					continue;
				}
			}
		}
	}
	else status = -1;
	return status;
}

int findFlannBasedGoodMatches(cv::Mat &descriptors_1, cv::Mat &descriptors_2, std::vector< cv::DMatch > &matches, std::vector< cv::DMatch > &good_matches, float gm_distance)
{
	int status = -1;
	cv::FlannBasedMatcher matcher;
	good_matches.clear();

	if (descriptors_1.rows>4 && descriptors_2.rows>4)
	{
		matcher.match(descriptors_1, descriptors_2, matches);

		if (matches.size()>0)
		{
			//-- Quick calculation of max and min distances between keypoints
			double max_dist = 0; double min_dist = matches[0].distance;

			for (int i = 0; i < matches.size(); i++)
			{
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}

			for (int i = 0; i < matches.size(); i++)
			{
				if (matches[i].distance <= max(2 * min_dist, gm_distance))
					good_matches.push_back(matches[i]);
			}
		}

		matcher.clear();
	}

	return status;
}