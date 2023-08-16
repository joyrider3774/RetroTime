#include <curl/curl.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include "CHttp.h"

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){// cURL callback function [read in datastream to memory]
    // This prototype is provided by cURL, with an argument at the end for our data structure.
    // This function is repeatedly called by cURL until there is no more data in the data stream; *ptr [it is assumed cURL handles memory management for this pointer].
    size_t realsize = size * nmemb;// The number of bytes in the datastream [there is no NULL char]

    HttpData *mem = (HttpData *)userdata;
    char *tmp = (char *)std::realloc(mem->memory, mem->size + realsize + 1);// We add 1 for the NULL char.

    if (tmp == NULL){
        printf("Not Enough Memory, realloc returned NULL.\n");
        exit(EXIT_FAILURE);
    }

    mem->memory = tmp;
    std::memcpy(&(mem->memory[mem->size]), ptr, realsize);// Starting at the last element copy in datastream
    mem->size += realsize;// The actual size is realsize + 1, however realsize gives us the location of the last element.
    mem->memory[mem->size] = 0;// The datastream doesn't include a NULL char, so we zeroize the last element.
    // We overwrite the NULL char {the zeroized element} on the next callback iteration, if any.

    return realsize;// cURL crosschecks the datastream with this return value.
}

CHttp::CHttp()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	multi_handle = curl_multi_init();
}

CHttp::~CHttp()
{
	curl_multi_cleanup(multi_handle);
	curl_global_cleanup();
}

void CHttp::CancelRequest(HttpData *data)
{
	if(!data)
		return;
	
	if(!data->easyhandle)
		return;
	
	curl_multi_remove_handle(multi_handle, data->easyhandle);
	curl_easy_cleanup(data->easyhandle);
}

void CHttp::DestroyData(HttpData **data)
{
	if(!data)
		return;

	if(!(*data))
		return;
	
	if(!(*data)->memory)
		return;

	free((*data)->memory);
	free((*data));
	*data = NULL;
}

int CHttp::Update()
{
	CURLMsg *msg;
    int queued;
	int handle_count;
	curl_multi_perform(multi_handle, &handle_count);	
	//check for finished calls & cleanup
	do {
		msg = curl_multi_info_read(multi_handle, &queued);
		if(msg) 
		{
			if(msg->msg == CURLMSG_DONE) 
			{
				HttpData *data;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &data);
				curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &data->responsecode);
				data->done = true;
				curl_multi_remove_handle(multi_handle, msg->easy_handle);
				curl_easy_cleanup(msg->easy_handle);
				if (data->autodestroy)
					DestroyData(&data);
			}
		}
    } while(msg);
	return handle_count;
}

HttpData* CHttp::AddRequest(const char* uri, bool AutoDestroyData)
{
	HttpData *Result = (HttpData *) std::malloc(sizeof(HttpData));
	if (!Result)
		return NULL;
	Result->done = false;
	Result->memory = (char *) std::malloc(1);              // Initialize the memory component of the structure.
    Result->size = 0;
	Result->responsecode = 0l;
	Result->easyhandle = curl_easy_init();
	Result->autodestroy = AutoDestroyData;
	if(Result->easyhandle)
	{
		curl_easy_setopt(Result->easyhandle, CURLOPT_URL, uri);
		curl_easy_setopt(Result->easyhandle, CURLOPT_WRITEFUNCTION, write_callback);// The callback function to write data to.
    	curl_easy_setopt(Result->easyhandle, CURLOPT_WRITEDATA, (void *)Result);// Send the address of the data struct to callback func.
    	curl_easy_setopt(Result->easyhandle, CURLOPT_PRIVATE, (void *)Result);// set also as private pointer so we can access it and set done flag
		curl_multi_add_handle(multi_handle, Result->easyhandle);
	}
	else
	{
		Result->memory[0] = '\0';
		Result->done = true;
	}
	return Result;
}
