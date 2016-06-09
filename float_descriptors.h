/*
Copyright (C) 2015-2020 S.Tolga Yildiran.
All rights reserved.

This file is part of Tolga Yildiran Video Search library and is made available under
the terms of the BSD license (see the COPYING file).
*/
/************************************************************************/
/* Tolga Yildiran														*/
/* 24/05/2016 															*/
/* stolgayldrn@gmail.com												*/
/************************************************************************/

#ifndef FLOAT_DESCRIPTORS_H
#define FLOAT_DESCRIPTORS_H

#include "descriptors.h"


class float_descriptors : public Descriptors
{
public:
	float_descriptors(const char* file_path, const char* dsc_path, FeatureType feature) :Descriptors(file_path, dsc_path, feature), descs(nullptr)
	{
		header += "type2_";
		header += "v2_";
	};
	float_descriptors(const char* dsc_path, FeatureType feature) :Descriptors(dsc_path, feature), descs(nullptr)
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

