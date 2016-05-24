#include "float_descriptors.h"



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
	if (dscFilePath == "")
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
	fopen_s(&f, dscFilePath.c_str(), "rb");
	if (f)
	{
		unsigned long long hash;
		char *cstr = new char[100];
		fread(cstr, sizeof(std::string), 1, f);
		fread(&numDesc, sizeof(unsigned int), 1, f);
		descs = new float[(numDesc + 4)*featSize];
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

	EZ_keypoints = extract_ezsift(Image->data, Image->cols, Image->rows, Image->step, numKpts);
	numDesc = *numKpts;
	if (numDesc)
	{
		descs = new float[(numDesc + 4)*featSize];
		Xs = new float[(numDesc + 4)];
		Ys = new float[(numDesc + 4)];
		Sizes = new float[(numDesc + 4)];
		Angles = new float[(numDesc + 4)];
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

	EZ_keypoints = extract_ezsift(Image->data, Image->cols, Image->rows, Image->step, numKpts);
	numDesc = *numKpts;
	if (numDesc)
	{
		descs = new float[(numDesc + 4)*featSize];
		Xs = new float[(numDesc + 4)];
		Ys = new float[(numDesc + 4)];
		Sizes = new float[(numDesc + 4)];
		Angles = new float[(numDesc + 4)];
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
				sum_descs += key.descrip[l];

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
