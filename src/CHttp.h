#pragma once

#include <curl/curl.h>

typedef struct{ 
    size_t size;
    char *memory;
	bool done;
	long responsecode;
	CURL* easyhandle;
	bool autodestroy;
} HttpData;

class CHttp
{
	CURLM* multi_handle; 

	public:
		CHttp();
		~CHttp();
		int Update();
		HttpData* AddRequest(const char* uri, bool AutoDestroyData);
		void CancelRequest(HttpData *data);
		void DestroyData(HttpData **data);
};
