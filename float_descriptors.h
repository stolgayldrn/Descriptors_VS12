#ifndef FLOAT_DESCRIPTORS_H
#define FLOAT_DESCRIPTORS_H

#include "descriptors.h"


class float_descriptors : public descriptors
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

