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

#define QUERY_RETURN_SIZE  10


struct Path
{
	std::string DataSet; 
	std::string dscFoldName;
	std::string dscFoldName2;
	std::string imgFoldName;
	std::string VocTree;
	std::string VocTreeLow;
	int subFolderingLevel;
};

struct ELK_params
{
	std::string index;
	std::string type;
	std::string url;
	std::string userPWD;
};

struct Image_Info
{
	std::string dataSet; // {basefolder}/{subFolder}/{subSubFolder}
	std::string dataSubSet; // {basefolder}/{subFolder}/{subSubFolder}
	std::string url;				// address to download
	std::string fileName;		// filename @database
	std::string descriptorType;	// unique id algorithm @MYSQL
	std::string source_type;		// "internet", "AA", ...
	std::string encoding;		// "mp4", "mpg", ...
	std::string path;			// disk path
	double height;			// belong to source video
	double width;			// belong to source video
	bool Import;			// true: import new image, false: for query video
	bool Query;				// true: query image
};
int GetJSON__NewImage(Image_Info my_II, Path p, json_t* my_source, std::string words_str);
int GetJSON__NewImage(Image_Info my_II, Path p, json_t* my_source, std::string words_str, std::string low_words_str);
int ELK__Commit(ELK_params my_ES, json_t* my_source, const char * ES_id, std::string fileName);
int GetJSON__QueryImage(json_t* my_source, std::string words_str, std::string wordsType);
int GetJSON__QueryLowImage(json_t* my_source, std::string words_str);
int ELK_PostQuery(ELK_params my_ES, std::vector<std::vector<std::string>>& ES_results, json_t* my_source, Image_Info my_II);
int ELK_PostQuery(ELK_params my_ES, json_t* my_source, Image_Info my_II, std::vector<std::string>& fileNamesV,
                  std::vector<std::string>& dscPathsV, std::vector<float> & scoresV, int &totalNumELK);
int VocTreeInit(TVoctreeVLFeat* VT, Path p);
int VocTreeInit(TVoctreeVLFeat* VT, TVoctreeVLFeat* VT_low, Path p);
#endif