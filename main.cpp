#include "helpers2.h"
#include <opencv2/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include "descriptors.h"
#include <ctime>
#include <omp.h>
#include "ES_image.h"
#include "TVoctree.h"
#include "TVoctreeVLFeat.h"

using namespace std;
using namespace cv;

void write_csv(vector<vector<string>> testSet)
{
	ofstream myfile;
	string CSV_Path = "flicker10k.csv";
	myfile.open(CSV_Path.c_str());
	for (int i = 0; i < testSet.size(); i++){
		string lineStr = "";
		for (int ii = 0; ii < 11; ii++){
			lineStr += testSet[i][ii] + ";";
		}
		lineStr += "\n";
		myfile << lineStr;
		
	}
}

void release_all(string& dscPath, Image_Info* myIm, unsigned* vwI, json_t* myJSON, string& vwS, const char* ES_id)
{
	delete[] vwI;
	json_decref(myJSON);
	delete myIm;
	dscPath = "";
	vwS = "";
	delete ES_id;
}

int main()
{
	Path *myPath = new Path;
	ES_params *myES = new ES_params;
	TVoctreeVLFeat* VT = new TVoctreeVLFeat;
	vector<string> imageList;
	vector<string> dscList;
	vector<vector<string>> testSet;
	string dscFoldPath;
	clock_t start = clock();
	double duration;
	unsigned int err_count = 0;

	myPath->dscFoldName = "images0/dsc_akaze2/0";
	myPath->DataSet = "C:/ImageSearch/flicker10K";
	myPath->imageFolderName = "";
	myPath->subFolderingLevel = 2;
	myPath->VocTree = "C:/ImageSearch/VT_Trees/VT_flicker100K_AKAZE_small_tree_S2_P.dat";

	myES->index = "image_search_akaze2";
	myES->type = "akaze";
	myES->url = "http://172.16.10.175:9200";
	myES->userPWD = "";

	initialization(VT, myPath);
	get_directory_images(myPath->DataSet.c_str(), imageList);
	dscFoldPath = myPath->DataSet + "/" + myPath->dscFoldName;
	pathControl(dscFoldPath);
	get_directory_dsc(dscFoldPath.c_str(), dscList);

	//omp_set_dynamic(0);     // Explicitly disable dynamic teams
	omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
	//#pragma omp parallel for
	for (int m = 0, n = 0; m < dscList.size(); m++)
	{
		//string imagePath = myPath->DataSet + "/" + imageList[m];
		string imagePath = myPath->DataSet + "/" + "no_image";
		string dscPath = dscFoldPath + "/" + dscList[m];
		Image_Info* myIm = new Image_Info;

		myIm->dataSet = "flicker10K";
		myIm->dataSubSet = "";
		myIm->descriptorType = "akaze";
		myIm->encoding = "jpg";
		myIm->fileName = dscList[m].substr(0, dscList[m].length() - 4);
		myIm->height = 0.0;
		myIm->width = 0.0;
		myIm->Import = "false";
		myIm->Query = "true";
		myIm->path = imagePath;
		myIm->source_type = "flicker1M";

		if (is_dsc_file(dscPath.c_str()))
		{
			/*uchar_descriptors my_desc(imagePath.c_str(), dscPath.c_str(), AKAZE_FEATS);
			my_desc.extract_AKAZE_feats();
*/
			uchar_descriptors my_desc( dscPath.c_str(), AKAZE_FEATS);
			my_desc.read_dsc();

			unsigned int * vwI = new unsigned int[my_desc.get_num_descriptors()];
			json_t* myJSON = json_object();
			string vwS = "";
			const char * ES_id = new char;

			if (my_desc.get_num_descriptors() > 0)
			{
				try
				{
					VT->quantize_multi(vwI, my_desc.get_data(), my_desc.get_num_descriptors(), 61);

					for (int s = 0; s < my_desc.get_num_descriptors(); s++)
						vwS += " " + int2string(int(vwI[s]));

					if (vwS != "")
					{
						getJSON_query_image(myJSON, vwS);
						ES_post_query(myES, testSet, myJSON, myIm);
					}
				}
				catch (exception e)
				{
					printf("\nElasticSearch:::commit error:%s", e.what());
				}
			}
			try
			{
				release_all(dscPath, myIm, vwI, myJSON, vwS, ES_id);
			}
			catch (exception e)
			{
				printf("\nElasticSearch:::release error:%s", e.what());
			}
		}
		if (m% 100 == 0)
		{
			double num = (m*100.0 / dscList.size());
			printf("\rProcess Rate = %.2f%%", num);
		}
	}
	write_csv(testSet);
	duration = clock() - start;
	printf(":::Duration: %.2f, Error count: %d", duration, err_count);
	return 0;
}
int mainIMP()
{
	Path *myPath = new Path;
	ES_params *myES = new ES_params;

	myPath->dscFoldName = "dsc_akaze2";
	myPath->DataSet = "D:/Data/DataSet16035.AA.20140925";
	myPath->imageFolderName = "";
	myPath->subFolderingLevel = 0;
	myPath->VocTree = "D:/Data/VT_flicker100K_AKAZE2_feats_small_tree.dat";

	myES->index = "image_search_test";
	myES->type = "akaze";
	myES->url = "http://services.maviucak.com:9200";
	myES->userPWD = "";

	TVoctreeVLFeat* VT = new TVoctreeVLFeat;
	initialization(VT, myPath);

	vector<string> imageList;
	get_directory_images(myPath->DataSet.c_str(), imageList);

	string dscFoldPath = myPath->DataSet + "/" + myPath->dscFoldName;
	pathControl(dscFoldPath);
	vector<string> dscList;
	get_directory_dsc(dscFoldPath.c_str(), dscList);

	clock_t start;
	start = clock();
	double duration;
	unsigned int err_count = 0;

	//omp_set_dynamic(0);     // Explicitly disable dynamic teams
	omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
	//#pragma omp parallel for
	for (int m = 0, n = 0; m < imageList.size(); m++)
	{
		if (imageList[m] != dscList[n].substr(0, dscList[n].length() - 4))
		{
			string imagePath = myPath->DataSet + "/" + imageList[m];
			string dscPath = dscFoldPath + "/" + imageList[m] + ".dsc";

			Image_Info* myIm = new Image_Info;
			myIm->dataSet = "DataSet16035.AA.20140925";
			myIm->dataSubSet = "";
			myIm->descriptorType = "akaze";
			myIm->encoding = "jpg";
			myIm->fileName = imageList[m];
			myIm->height = 0.0;
			myIm->width = 0.0;
			myIm->Import = "true";
			myIm->Query = "false";
			myIm->path = imagePath;
			myIm->source_type = "AA";

			if (is_image_file(imagePath.c_str()))
			{
				uchar_descriptors my_desc(imagePath.c_str(), dscPath.c_str(), AKAZE_FEATS);
				if (my_desc.extract_AKAZE_feats())
					my_desc.write_dsc();

				unsigned int * vwI = new unsigned int[my_desc.get_num_descriptors()];
				json_t* ImJSON = json_object();
				string vwS = "";
				const char * ES_id = new char;

				if (my_desc.get_num_descriptors() > 0)
				{
					try
					{
						VT->quantize_multi(vwI, my_desc.get_data(), my_desc.get_num_descriptors(), 61);

						for (int s = 0; s < my_desc.get_num_descriptors(); s++)
							vwS += " " + int2string((int)vwI[s]);

						getJSON_new_image(myIm, myPath, ImJSON, vwS);
						if (!ES_commit(myES, ImJSON, ES_id, myIm->fileName))
							err_count++;
					}
					catch (exception e)
					{
						printf("\nElasticSearch:::commit error:%s", e.what());
					}
				}

				try
				{
					delete[] vwI;
					json_decref(ImJSON);
					delete myIm;
					dscPath = "";
					vwS = "";
					delete ES_id;
				}
				catch (exception e)
				{
					printf("\nElasticSearch:::release error:%s", e.what());
				}
			}
			if (m % 100 == 0)
			{
				double num = (m*100.0 / dscList.size());
				printf("\rProcess Rate = %.2f%%", num);
			}

		}
		else
		{
			n++;
		}
	}
	duration = clock() - start;
	printf(":::Duration: %.2f, Error count: %d", duration, err_count);
	return 0;
}

int mainQuerry()
{
	Path *myPath = new Path;
	ES_params *myES = new ES_params;

	myPath->dscFoldName = "dsc_akaze2";
	myPath->DataSet = "D:/Data/flicker10K";
	myPath->imageFolderName = "image";
	myPath->subFolderingLevel = 2;
	myPath->VocTree = "D:/Data/VT_flicker100K_AKAZE2_feats_small_tree.dat";

	myES->index = "image_search_test";
	myES->type = "akaze";
	myES->url = "http://services.maviucak.com:9200";
	myES->userPWD = "";

	TVoctreeVLFeat* VT = new TVoctreeVLFeat;
	initialization(VT, myPath);

	//string dataPath = "D:/Data/flicker1M";
	vector<string> dataList;
	get_folder_list(myPath->DataSet.c_str(), dataList);
	vector<vector<string>> testSet;

	for (int m = 0; m < dataList.size(); m++)
	{
		clock_t start;
		start = clock();
		double duration;

		string folderPath = myPath->DataSet + "/" + dataList[m];
		vector<string> foldersList;
		get_folder_list((folderPath + "/" + myPath->dscFoldName).c_str(), foldersList);
		//pathControl(folderPath + myPath->dscFoldName);

		omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
		//#pragma omp parallel for
		for (int i = 0; i < foldersList.size(); i++)
		{
			//vector<string> imageList;
			//string subFolderPath = folderPath + "/images/" + foldersList[i];
			vector<string> dscList;
			string dsc_folderPath = folderPath + "/" + myPath->dscFoldName + "/" + foldersList[i];
			get_directory_dsc(dsc_folderPath.c_str(), dscList);

			//omp_set_dynamic(0);     // Explicitly disable dynamic teams
			//omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
			//#pragma omp parallel for
			//for (int k = 0; k < dscList.size(); k++)
			for (int k = 0; k < 200; k++)
			{
				//start = clock();
				//string imagePath = subFolderPath + "/" + imageList[k];
				//string dscPath = dsc_folderPath + "/" + imageList[k] + ".dsc";
				string dscPath = dsc_folderPath + "/" + dscList[k];

				Image_Info* myIm = new Image_Info;
				myIm->dataSet = "flicker10K";
				myIm->dataSubSet = dataList[m] + "/" + foldersList[i] + "/";
				myIm->descriptorType = "akaze";
				myIm->encoding = "jpg";
				myIm->fileName = dscList[k].substr(0, dscList[k].length() - 4);
				myIm->height = 0.0;
				myIm->width = 0.0;
				myIm->Import = "true";
				myIm->Query = "false";
				myIm->path = dscPath;
				myIm->source_type = "internet";

				if (is_dsc_file(dscPath.c_str()))
				{
					uchar_descriptors readedIM(dscPath.c_str(), AKAZE_FEATS);
					readedIM.read_dsc();
					unsigned char * vwC = new unsigned char[readedIM.get_num_descriptors()];
					unsigned int * vwI = new unsigned int[readedIM.get_num_descriptors()];
					json_t* myJSON = json_object();
					vwC = readedIM.get_data();
					string vwS = "";

					VT->quantize_multi(vwI, vwC, readedIM.get_num_descriptors(), 61);

					for (int s = 0; s < readedIM.get_num_descriptors(); s++)
					{
						if (vwI[s] != 5000)
							vwS += " " + int2string((int)vwI[s]);
					}

					//const char * ES_id = new char;
					//getJSON_new_image(myIm, myPath, ImJSON, vwS);
					//ES_commit(myES, ImJSON, ES_id, myIm->fileName);

					if (vwS != "")
					{
						getJSON_query_image(myJSON, vwS);
						ES_post_query(myES, testSet, myJSON, myIm);

					}

					//delete[] vwC;
					delete[] vwI;
					json_decref(myJSON);
					delete myIm;
					dscPath = "";
					vwS = "";
					//delete ES_id;
				}
				if (k % 10 == 0)
				{
					double num = ((m * 10) + i + (k*1.0 / 100));
					printf("\rProcess Rate = %.2f%%", num);
				}
			}
			dscList.clear();
			dscList.shrink_to_fit();
			/*imageList.clear();
			imageList.shrink_to_fit();*/
		}
		foldersList.clear();
		foldersList.shrink_to_fit();
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		printf(":::Duration: %f", duration);
	}
	ofstream myfile;
	string CSV_Path = ".csv";
	myfile.open(CSV_Path.c_str());
	for (int i = 0; i < testSet.size(); i++){
		string lineStr = "";
		for (int ii = 0; ii < 11; ii++){
			lineStr += testSet[i][ii] + ";";
		}
		lineStr += "\n";
		myfile << lineStr;
	}


	return 0;
}

int mainImport()
{
	Path *myPath = new Path;
	ES_params *myES = new ES_params;

	myPath->dscFoldName = "dsc_akaze2";
	myPath->DataSet = "D:/Data/flicker1M";
	myPath->imageFolderName = "image";
	myPath->subFolderingLevel = 2;
	myPath->VocTree = "D:/Data/VT_flicker100K_AKAZE2_feats_small_tree.dat";

	myES->index = "image_search_test";
	myES->type = "akaze";
	myES->url = "http://services.maviucak.com:9200";
	myES->userPWD = "";

	TVoctreeVLFeat* VT = new TVoctreeVLFeat;
	initialization(VT, myPath);

	//string dataPath = "D:/Data/flicker1M";
	vector<string> dataList;
	get_folder_list(myPath->DataSet.c_str(), dataList);

	for (int m = 0; m < 10; m++)
	{
		clock_t start;
		start = clock();
		double duration;

		string folderPath = myPath->DataSet + "/" + dataList[m];
		vector<string> foldersList;
		get_folder_list((folderPath + "/" + myPath->dscFoldName).c_str(), foldersList);
		//pathControl(folderPath + myPath->dscFoldName);

		omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
		//#pragma omp parallel for
		for (int i = 0; i < foldersList.size(); i++)
		{
			//vector<string> imageList;
			//string subFolderPath = folderPath + "/images/" + foldersList[i];
			vector<string> dscList;
			string dsc_folderPath = folderPath + "/" + myPath->dscFoldName + "/" + foldersList[i];
			get_directory_dsc(dsc_folderPath.c_str(), dscList);

			//omp_set_dynamic(0);     // Explicitly disable dynamic teams
			//omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
			//#pragma omp parallel for
			for (int k = 0; k < dscList.size(); k++)
			{
				//start = clock();
				//string imagePath = subFolderPath + "/" + imageList[k];
				//string dscPath = dsc_folderPath + "/" + imageList[k] + ".dsc";
				string dscPath = dsc_folderPath + "/" + dscList[k];

				Image_Info* myIm = new Image_Info;
				myIm->dataSet = "flicker1M";
				myIm->dataSubSet = dataList[m] + "/" + foldersList[i] + "/";
				myIm->descriptorType = "akaze";
				myIm->encoding = "jpg";
				myIm->fileName = dscList[k].substr(0, dscList[k].length() - 4);
				myIm->height = 0.0;
				myIm->width = 0.0;
				myIm->Import = "true";
				myIm->Query = "false";
				myIm->path = dscPath;
				myIm->source_type = "internet";

				if (is_dsc_file(dscPath.c_str()))
				{
					uchar_descriptors readedIM(dscPath.c_str(), AKAZE_FEATS);
					readedIM.read_dsc();
					unsigned char * vwC = new unsigned char[readedIM.get_num_descriptors()];
					unsigned int * vwI = new unsigned int[readedIM.get_num_descriptors()];
					json_t* ImJSON = json_object();
					vwC = readedIM.get_data();
					string vwS = "";

					VT->quantize_multi(vwI, vwC, readedIM.get_num_descriptors(), 61);

					for (int s = 0; s < readedIM.get_num_descriptors(); s++)
						vwS += " " + int2string((int)vwI[s]);

					//const char * ES_id = new char;
					getJSON_new_image(myIm, myPath, ImJSON, vwS);
					//ES_commit(myES, ImJSON, ES_id, myIm->fileName);

					//delete[] vwC;
					delete[] vwI;
					json_decref(ImJSON);
					delete myIm;
					dscPath = "";
					vwS = "";
					//delete ES_id;
				}
				if (k % 1000 == 0)
				{
					double num = ((m * 10) + i + (k*1.0 / 10000));
					printf("\rProcess Rate = %.2f%%", num);
				}
			}
			dscList.clear();
			dscList.shrink_to_fit();
			/*imageList.clear();
			imageList.shrink_to_fit();*/
		}
		foldersList.clear();
		foldersList.shrink_to_fit();
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		printf(":::Duration: %f", duration);
	}


	return 0;
}
