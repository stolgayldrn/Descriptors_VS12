#include "ES_image.h"

/***********************************************************/
/** @brief Create JSON with JANSSON.LIB to import new image at Elastic Search Index

@param thisII INPUT:: ImageInfo type input parameters
@param thisP INPUT:: Path type input parameters
@param words_string INPUT:: Concatenated visual words
@param json_source OUTPUT:: The pointer for JSON
*/
int getJsonForNewImage(ImageInfo thisII, Path thisP, std::string words_string, json_t* json_source)
{
	try{
		json_object_set_new(json_source, "data_set", json_string(thisII.dataSet.c_str()));
		json_object_set_new(json_source, "data_sub_set", json_string(thisII.dataSubSet.c_str()));
		json_object_set_new(json_source, "url", json_string(thisII.url.c_str()));
		json_object_set_new(json_source, "file_name", json_string(thisII.fileName.c_str()));
		json_object_set_new(json_source, "descriptor_type", json_string(thisII.descriptorType.c_str()));
		json_object_set_new(json_source, "source_type", json_string(thisII.source_type.c_str()));
		json_object_set_new(json_source, "encoding", json_string(thisII.encoding.c_str()));
		json_object_set_new(json_source, "height", json_string(int2string(thisII.height).c_str()));
		json_object_set_new(json_source, "width", json_string(int2string(thisII.width).c_str()));
		json_object_set_new(json_source, "num_of_descs", json_string(int2string(thisII.numDescs).c_str()));
		json_object_set_new(json_source, "disk_path", json_string(thisII.path.c_str()));
		json_object_set_new(json_source, "words_string", json_string(words_string.c_str()));
	}
	catch (std::exception e){
		std::cout << "# ERR: Elasticsearch Exception in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
	}
	if (!json_is_object(json_source)){
		fprintf(stderr, "error: commits is not an array\n");
		return 0;
	}
	return 1;
}

/** @brief Committing JSON to Elastic Search 

@param elastic_params INPUT:: Holding access parameters for elastic search index
@param json_source INPUT:: The pointer for JSON
@param file_name INPUT:: File name
@param elastic_ID OUTPUT:: Elastic Search Item ID
*/
int commitJsonToELK(ELKParams elastic_params, json_t* json_source, std::string file_name, const char* elastic_ID)
{
	CURL *curl = curl_easy_init();
	//char *userPWD = "writer:writeme";

	std::string ES_new_object_url = elastic_params.url + "/" + elastic_params.index + "/" + elastic_params.type;
	struct curl_slist *headers = NULL;
	size_t json_flags = 0;
	/* set content type */
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	// Response information.
	int httpCode(0);
	std::unique_ptr<std::string> httpData(new std::string());

	if (curl) {
		try{
			//CURLcode res;
			/* set curl options */
			curl_easy_setopt(curl, CURLOPT_URL, ES_new_object_url.c_str());
			//curl_easy_setopt(curl, CURLOPT_USERPWD, userPWD);
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			char *JDumps = json_dumps(json_source, json_flags);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, JDumps);
			// Hook up data handling function.
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
			// Hook up data container (will be passed as the last parameter to the
			// callback handling function).  Can be any pointer type, since it will
			// internally be passed as a void pointer.
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

			//res = curl_easy_perform(curl);
			curl_easy_perform(curl);
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
			/* cleanup curl handle */
			curl_easy_cleanup(curl);
			/* free headers */
			curl_slist_free_all(headers);
			//
			delete JDumps;
		}
		catch (std::exception e){
			printf("\nElasticsearch error: %s", e.what());
			return 0;
		}
	}

	if (httpCode == 201){
		json_t *rootRes;
		json_error_t error;
		try
		{
			rootRes = json_loads(httpData->data(), 0, &error);
			if (!rootRes){
				fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
				return 1;
			}
			const char *str;
			int created = -1;
			json_unpack(rootRes, "{s:s}", "_id", &str);
			json_unpack(rootRes, "{s:b}", "created", &created);
			// ReSharper disable once CppAssignedValueIsNeverUsed
			elastic_ID = str;
			//if (strcmp(str2, "true")==0)
			if (!created)
			{
				printf("ERROR AT INSERT");
			}
			//printf("\nES::::New image is committed to image_search_test\\akaze\\%s.", fileName.c_str());
			/*httpData->clear();
			httpData->shrink_to_fit();*/
			json_decref(rootRes);
			return 1;
		}
		catch (std::exception e){
			printf("\nElasticsearch error: %s", e.what());
			return 0;
		}
	}
	else{
		printf("\nElasticsearch error: httpCode, %d", httpCode);
		return 0;
	}
}
/** @brief Create JSON with JANSSON.LIB for postting query to Elastic Search Index

@param words_string INPUT:: Concatenated visual words
@param words_field_name INPUT:: The field name for "words string" in the Elastic Search Index 
@param json_source OUTPUT:: The pointer for JANSSON.LIB based JSON pointer
*/
int getJsonForQueryImage(std::string words_string, std::string words_field_name, json_t* json_source)
{
	json_t *query = json_object();
	json_t *match = json_object();
	try	{
		json_object_set_new(match, words_field_name.c_str(), json_string(words_string.c_str()));
		json_object_set_new(query, "match", match);
		json_object_set_new(json_source, "size", json_real(QUERY_RETURN_SIZE));
		json_object_set_new(json_source, "query", query);
		return 1;
	}
	catch (std::exception e){
		std::cout << "# ERR: Elasticsearch Exception in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << ", ES State: " << e.what() << " )" << std::endl;
		return 0;
	}
}
/** @brief Posting created query json object to the Elastic Search Index 

@param json_source INPUT:: The pointer for JSON
@param thisII INPUT:: ImageInfo type input parameters
@param elastic_params INPUT:: Holding access parameters for elastic search index
@param file_name_list OUTPUT:: Name list of returned files from Elastic Search
*/
int postQueryToELK(json_t* json_source, ImageInfo thisII, ELKParams elastic_params, std::vector<std::vector<std::string>>& file_name_list)
{
	std::vector<std::string> ES_result;
	ES_result.push_back(thisII.fileName.c_str());
	CURL *curl = curl_easy_init();
	//char *userPWD = "writer:writeme";
	std::string ES_new_object_url = elastic_params.url + "/" + elastic_params.index + "/" + "_search";
	struct curl_slist *headers = NULL;
	size_t json_flags = 0;
	/* set content type */
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	// Response information.
	int httpCode(0);
	std::unique_ptr<std::string> httpData(new std::string());
	if (curl){
		try{
			//CURLcode res;
			char *jDumps = json_dumps(json_source, json_flags);
			/* set curl options */
			curl_easy_setopt(curl, CURLOPT_URL, ES_new_object_url.c_str());
			//curl_easy_setopt(curl, CURLOPT_USERPWD, userPWD);
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jDumps);
			// Hook up data handling function.
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
			// Hook up data container (will be passed as the last parameter to the callback handling function).  
			// Can be any pointer type, since it will internally be passed as a void pointer.
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

			//res = curl_easy_perform(curl);
			curl_easy_perform(curl);
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
			/* cleanup curl handle */
			curl_easy_cleanup(curl);
			/* free headers */
			curl_slist_free_all(headers);
			/* clear json dumps */
			delete jDumps;
		}
		catch (std::exception e){
			printf("\nElasticsearch error: %s", e.what());
			return 0;
		}
	}
	if (httpCode == 200){
		json_t *rootRes, *hits, *hits2;
		json_error_t error;
		rootRes = json_loads(httpData->data(), 0, &error);
		if (!rootRes){
			fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
			return 0;
		}
		hits = json_object_get(rootRes, "hits");
		hits2 = json_object_get(hits, "hits");
		if (!json_is_array(hits2)){
			fprintf(stderr, "error: hits2 is not an array\n");
			return 0;
		}
		else{
			for (int i = 0; i < QUERY_RETURN_SIZE; i++){
				if (i < json_array_size(hits2)){
					json_t *data, *sourceRes, *image_ID;
					data = json_array_get(hits2, i);
					const char *message_text;
					if (!json_is_object(data)){
						fprintf(stderr, "error: commit data %d is not an object\n", i + 1);
						return 1;
					}
					sourceRes = json_object_get(data, "_source");
					image_ID = json_object_get(sourceRes, "file_name");
					message_text = json_string_value(image_ID);
					ES_result.push_back(message_text);
					//releases
					json_object_clear(data);
					json_object_clear(sourceRes);
					json_object_clear(image_ID);
				}
				else{
					ES_result.push_back("-1");
				}
			}
			//releases			
			json_object_clear(rootRes);
			json_object_clear(hits);
			json_object_clear(hits2);
			file_name_list.push_back(ES_result);
			ES_result.clear();
			return 1;
		}
	}
	else{
		return 0;
	}
}
/** @brief Posting the query json object to the Elastic Search Index 
and returning results of images names and scores 

@param elastic_params INPUT:: Holding access parameters for elastic search index
@param json_source INPUT:: The pointer for JSON
@param thisII INPUT:: ImageInfo type input parameters
@param file_names_list OUTPUT:: Name list of returned files from Elastic Search in order to descanding Elastic Search tf-idf score
@param dsc_paths_list OUTPUT:: Address list of returned files
@param scores_list OUTPUT:: Elastic Search tf-idf scores list of returned files
@param total_num_of_returns OUTPUT::Total number of returned files
*/
int postQueryToELK(ELKParams elastic_params, json_t* json_source, ImageInfo thisII, std::vector<std::string>& file_names_list,
                   std::vector<std::string>& dsc_paths_list, std::vector<float> & scores_list, int & total_num_of_returns)
{
	CURL *curl = curl_easy_init();
	//char *userPWD = "writer:writeme";
	std::string ES_new_object_url = elastic_params.url + "/" + elastic_params.index + "/" + elastic_params.type + "/" +"_search";
	struct curl_slist *headers = NULL;
	size_t json_flags = 0;
	/* set content type */
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	// Response information.
	int httpCode(0);
	std::unique_ptr<std::string> httpData(new std::string());
	if (curl){
		try{
			//CURLcode res;
			/* set curl options */
			curl_easy_setopt(curl, CURLOPT_URL, ES_new_object_url.c_str());
			//curl_easy_setopt(curl, CURLOPT_USERPWD, userPWD);
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			char *jsonDump = json_dumps(json_source, json_flags);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonDump);
			// Hook up data handling function.
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
			// Hook up data container (will be passed as the last parameter to the callback handling function).  
			// Can be any pointer type, since it will internally be passed as a void pointer.
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

			//res = curl_easy_perform(curl);
			curl_easy_perform(curl);
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
			/* cleanup curl handle */
			curl_easy_cleanup(curl);
			/* free headers */
			curl_slist_free_all(headers);
			//Clear JSON
			free(jsonDump);
			//delete jsonDump;
		}
		catch (std::exception e){
			printf("\nElasticsearch error: %s", e.what());
			return 0;
		}
	}
	if (httpCode == 200){
		try{
			json_t *rootRes, *hits, *hits2, *totalNum;
			json_error_t error;
			rootRes = json_loads(httpData->data(), 0, &error);
			if (!rootRes){
				fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
				for (int n = 0; n < QUERY_RETURN_SIZE; n++)
				{
					file_names_list.push_back("-1");
					dsc_paths_list.push_back("-1");
					scores_list.push_back(-1);
				}
				return 0;
			}
			hits = json_object_get(rootRes, "hits");
			totalNum = json_object_get(hits, "total");
			total_num_of_returns = json_integer_value(totalNum);
			hits2 = json_object_get(hits, "hits");
			if (!json_is_array(hits2)){
				fprintf(stderr, "error: hits2 is not an array\n");
				for (int n = 0; n < QUERY_RETURN_SIZE; n++)
				{
					file_names_list.push_back("-1");
					dsc_paths_list.push_back("-1");
					scores_list.push_back(-1);
				}
				return 0;
			}

			for (int i = 0; i < QUERY_RETURN_SIZE; i++){
				if (i < json_array_size(hits2)){
					json_t *data, *sourceRes, *image_ID, *dscPathJSON, *scoreJSON;
					data = json_array_get(hits2, i);
					const char *fileNameStr, *dscPathStr;
					double scoreChar;
					if (!json_is_object(data)){
						fprintf(stderr, "error: commit data %d is not an object\n", i + 1);
						return 1;
					}
					sourceRes	= json_object_get(data, "_source");
					image_ID	= json_object_get(sourceRes, "file_name");
					fileNameStr = json_string_value(image_ID);
					file_names_list.push_back(fileNameStr);

					dscPathJSON	= json_object_get(sourceRes, "disk_path");
					dscPathStr	= json_string_value(dscPathJSON);
					dsc_paths_list.push_back(dscPathStr);

					scoreJSON = json_object_get(data, "_score");
					scoreChar = json_real_value(scoreJSON);
					scores_list.push_back(scoreChar);
					//releases
					json_object_clear(scoreJSON);
					json_object_clear(sourceRes);
					json_object_clear(dscPathJSON);
					json_object_clear(data);
					json_object_clear(image_ID);
				}
				else{
					for (int n = 0; n < QUERY_RETURN_SIZE; n++)
					{
						file_names_list.push_back("100");
						dsc_paths_list.push_back("100");
						scores_list.push_back(000);
					}
				}
			}
			//releases	
			json_object_clear(totalNum);
			json_object_clear(hits2);
			json_object_clear(hits);
			json_decref(rootRes);
			return 1;
		}
		catch (std::exception e){
			printf("\nElasticsearch error: %s", e.what());
			return 0;
		}
	}
	printf("\nElasticsearch error: Connection Error___   ");
	return 0;
}
/** @brief Initialize VLFeat based Vocabulary Tree model with address from Path object

@param VT The Vocabulary Tree Model
@param thisP INPUT:: Path type input parameters
*/
int VocTreeInit(TVoctreeVLFeat* VT, Path thisP)
{
	try{
		VT->init_read(thisP.VocTree.c_str());
		return 1;
	}
	catch (std::exception e){
		printf("\nInitialization error: %s", e.what());
		return 0;
	}
}


