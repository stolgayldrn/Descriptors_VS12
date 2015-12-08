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

using namespace std;


struct Path
{
	string DataSet;
	string dscFoldName;
	string imageFolderName;
	int subFolderingLevel;
	string VocTree;
};

struct ES_params
{
	string index;
	string type;
	string url;
	string userPWD;
};

struct Image_Info
{
	string dataSet; // {basefolder}/{subFolder}/{subSubFolder}
	string dataSubSet; // {basefolder}/{subFolder}/{subSubFolder}
	string url;				// address to download
	string fileName;		// filename @database
	string descriptorType;	// unique id algorithm @MYSQL
	string source_type;		// "internet", "AA", ...
	string encoding;		// "mp4", "mpg", ...
	double height;			// belong to source video
	double width;			// belong to source video
	string path;			// disk path
	bool Import;			// true: import new image, false: for query video
	bool Query;				// true: query image
};
int getJSON_new_image(Image_Info* my_II, Path* p, json_t* my_source, string words_str);
int ES_commit(ES_params* my_ES, json_t* my_source, const char * ES_id, string fileName);
int getJSON_query_image(json_t* my_source, string words_str);
int ES_post_query(ES_params* my_ES, vector<vector<string>>& ES_results, json_t* my_source, Image_Info* my_II);
int initialization(TVoctreeVLFeat* VT, Path* p);
#endif