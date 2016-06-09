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

#ifndef ES_IMAGE_H
#define  ES_IMAGE_H

#include "TVoctree.h"
#include "TVoctreeVLFeat.h"
#include "curl/curl.h"
#include "jansson.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <memory>
#include "vld.h"

#define QUERY_RETURN_SIZE  25
#define MAX_DESCRIPTOR_SIZE_QUERY 7000

struct Path
{
	std::string DataSet; 
	std::string dscFoldName;
	std::string imgFoldName;
	std::string VocTree;
	std::string corruptedFilesFolder;
	std::string logPath;
	int subFolderingLevel;
};

struct ELKParams
{
	std::string index;
	std::string type;
	std::string url;
	std::string userPWD;
};

struct ImageInfo
{
	std::string dataSet;		// {basefolder}/{subFolder}/{subSubFolder}
	std::string dataSubSet;		// {basefolder}/{subFolder}/{subSubFolder}
	std::string url;			// address to download
	std::string fileName;		// filename @database
	std::string descriptorType;	// unique id algorithm @MYSQL
	std::string source_type;	// "internet", "AA", ...
	std::string encoding;		// "mp4", "mpg", ...
	std::string path;			// disk path
	double height;				// belong to source video
	double width;				// belong to source video
	int numDescs;				// num of Descriptors
	bool Import;				// true: import new image, false: for query video
	bool Query;					// true: query image
};
int getJsonForNewImage(ImageInfo thisII, Path thisP, std::string words_string, json_t* json_source);
int commitJsonToELK(ELKParams elastic_params, json_t* json_source, std::string file_name, const char* elastic_ID);
int getJsonForQueryImage(std::string words_string, std::string words_field_name, json_t* json_source);
int postQueryToELK(json_t* json_source, ImageInfo thisII, ELKParams elastic_params, std::vector<std::vector<std::string>>& file_name_list);
int postQueryToELK(ELKParams elastic_params, json_t* json_source, ImageInfo thisII, std::vector<std::string>& file_names_list,
                   std::vector<std::string>& dsc_paths_list, std::vector<float> & scores_list, int & total_num_of_returns);
int VocTreeInit(TVoctreeVLFeat* VT, Path thisP);
#endif