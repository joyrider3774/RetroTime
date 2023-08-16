#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include "urlencode.h"
#include "hmacsha1.h"

#undef SKIP_PEER_VERIFICATION 
#undef SKIP_HOSTNAME_VERIFICATION

std::string GetSubmitScoreUrl(std::string secretkey, unsigned int gameid, unsigned int score, std::string playername)
{
	std::string data = playername + std::to_string(score) + std::to_string(gameid) + secretkey;
	const char * key = secretkey.c_str();
	unsigned char hash[SHA1_HASH_SIZE];
	hmac_sha1(key, secretkey.length(), data.c_str(), sizeof(char) * data.length(), hash, SHA1_HASH_SIZE);
	std::ostringstream hexhash;
	hexhash.fill('0');
	hexhash << std::hex;
	for (int i = 0; i < SHA1_HASH_SIZE; i++)
	{
		hexhash << std::setw(2) << int((unsigned char) hash[i]);
	}
	std::string url = "https://scores.joyrider3774.xyz/AddScore.php?game=" + std::to_string(gameid) + "&score=" + std::to_string(score) + "&name=" + url_encode(playername) + "&hash=" + hexhash.str();
	return url;
}

bool SubmitScore(std::string secretkey, unsigned int gameid, unsigned int score, std::string playername)
{
	CURL *curl;
	CURLcode res;
	bool result = true;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) 
	{
		std::string url = GetSubmitScoreUrl(secretkey, gameid, score, playername);
		std::cout << url << std::endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
#ifdef SKIP_PEER_VERIFICATION
	/*
		* If you want to connect to a site who is not using a certificate that is
		* signed by one of the certs in the CA bundle you have, you can skip the
		* verification of the server's certificate. This makes the connection
		* A LOT LESS SECURE.
		*
		* If you have a CA cert for the server stored someplace else than in the
		* default bundle, then the CURLOPT_CAPATH option might come handy for
		* you.
		*/
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
	/*
		* If the site you are connecting to uses a different host name that what
		* they have mentioned in their server certificate's commonName (or
		* subjectAltName) fields, libcurl will refuse to connect. You can skip
		* this check, but this will make the connection less secure.
		*/
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",	curl_easy_strerror(res));
			result = false;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return result;
}