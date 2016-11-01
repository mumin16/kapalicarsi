#pragma once

#define CURL_STATICLIB
#include <curl\curl.h>
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#if defined(_MSC_VER) && defined(_WIN64)
#pragma comment(lib,"x64/libcurl")
#pragma comment(lib,"Ws2_32")
#else 
#pragma comment(lib,"libcurl")
#pragma comment(lib,"Ws2_32")
#endif // _MSC_VER && _WIN64

#include <sstream> 


#include <time.h>       /* time_t, struct tm, time, localtime */

#include <string>
#include <vector>
#include <iostream>
#include <fstream> 



struct FX
{
	double open, high, low, close;
	int volume;
	std::string date, time;

};

FX fx;
std::vector<FX> fxs;

class googlefinance
{
public:
	std::vector<double> close;
	googlefinance() {}
	~googlefinance() {}
	bool download(const char* sname, const char* ssec = "86400", const char* sdaylength = "50");//60 saniyelik 15gün geriden 60=1dk 300=5dk 900=15dk 1800=30dk 3600=1saat 86400=gunluk max 50gun
private:
	static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
		std::string data((const char*)ptr, (size_t)size * nmemb);
		*((std::stringstream*)stream) << data;
		return size * nmemb;
	}
};






bool googlefinance::download(const char* sname, const char* ssec, const char* sdaylength) {//60 saniyelik 15gün geriden


	CURL *curl;
	CURLcode res;
	std::stringstream input;
	input.clear();
	char buffer[250];
	sprintf(buffer, "http://www.google.com/finance/getprices?&i=%s&p=%sd&q=%s", ssec, sdaylength, sname);

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);//60saniye
		curl_easy_setopt(curl, CURLOPT_URL, buffer);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &input);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

	}
	if (res != CURLE_OK)return FALSE;

	std::string sLine = "";
	for (size_t i = 0; i < 9; i++)
	{
		getline(input, sLine, '\n');//ilk8 bilgi satirini al 9. ilk data olmasi lazım
		if (input.eof() || input.bad() || input.fail()) {
			return FALSE;
		}
	}



	do
	{

		char sdate[255], unformatteddate[255], sclose[255], shigh[255], slow[255], sopen[255], svolume[255];
		if (6 != sscanf(sLine.c_str(), "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sdate, sclose, shigh, slow, sopen, svolume))
		{
			std::cout << "bu yazilamadi: " << sLine.c_str() << std::endl;
			getline(input, sLine, '\n');
			continue;
		}



		fx.open = atof(sopen);
		fx.high = atof(shigh);
		fx.low = atof(slow);
		fx.close = atof(sclose);
		fx.volume = atoi(svolume);

		close.push_back(atof(sclose));


		if (0 == strncmp(sdate, "a", 1))
		{

			strcpy(sdate, sdate + 1);
			strcpy(unformatteddate, sdate);

			time_t timeNow = atol(sdate);
			tm* time_info = localtime(&timeNow);

			fx.date = std::to_string(time_info->tm_year + 1900);
			if (time_info->tm_mon + 1 <10)fx.date.append("0");
			fx.date.append(std::to_string(time_info->tm_mon + 1));
			if (time_info->tm_mday <10)fx.date.append("0");
			fx.date.append(std::to_string(time_info->tm_mday));


			if (time_info->tm_hour <10)fx.time.append("0");
			fx.time.append(std::to_string(time_info->tm_hour));
			if (time_info->tm_min <10)fx.time.append("0");
			fx.time.append(std::to_string(time_info->tm_min));
			fxs.push_back(fx);
		}
		else {
			time_t timeNow = atoi(unformatteddate) + atoi(ssec) * atoi(sdate);
			tm* time_info = localtime(&timeNow);

			std::string idate = std::to_string(time_info->tm_year + 1900);
			if (time_info->tm_mon + 1 <10)idate.append("0");
			idate.append(std::to_string(time_info->tm_mon + 1));
			if (time_info->tm_mday <10)idate.append("0");
			idate.append(std::to_string(time_info->tm_mday));

			std::string itime;
			if (time_info->tm_hour <10)itime.append("0");
			itime.append(std::to_string(time_info->tm_hour));
			if (time_info->tm_min <10)itime.append("0");
			itime.append(std::to_string(time_info->tm_min));
			fx.date = idate;
			fx.time = itime;
			fxs.push_back(fx);
		}




		//std::cout << sname<< ": " <<fx.date << " - " << fx.time << " o: "<<fx.open <<" h: "<< fx.high << " c: "<< fx.close <<" l: "<< fx.low << " v: "<< fx.volume << std::endl;

		fx = { 0 };

		getline(input, sLine, '\n');



	} while (!input.eof());

	//std::cout << fxs.size();

	input.clear();

	return TRUE;
}