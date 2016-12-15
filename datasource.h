#pragma once
#define CURL_STATICLIB
#pragma comment(lib,"libcurl")
#pragma comment(lib,"ws2_32")
#include <curl\curl.h>
#include <vector>
#include <array>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
namespace DataSource {

	struct PriceData
	{
		std::vector<double> open;
		std::vector<double> high;
		std::vector<double> low;
		std::vector<double> close;
		std::vector<double> volume;
		std::vector<std::string> date;
		std::vector<std::string> _time;
	};



	static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
		std::string data((const char*)ptr, (size_t)size * nmemb);
		*((std::stringstream*)stream) << data;
		return size * nmemb;
	}




	bool Google(PriceData &pricedata,const char* sname, const char* ssec = "86400", const char* sdaylength = "50") {//60 saniyelik 15gün geriden


		CURL *curl;
		CURLcode res;
		std::stringstream input;

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
			getline(input, sLine, '\n');//ilk8 bilgi satirini al 9. ilk data olmasi lazým
			if (input.eof() || input.bad() || input.fail()) {
				return FALSE;
			}
		}


	
		do
		{

			char sdate[255], unformatteddate[255], sclose[255], shigh[255], slow[255], sopen[255], svolume[255];
			if (6 != sscanf(sLine.c_str(), "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sdate, sclose, shigh, slow, sopen, svolume))
			{
				//std::cout << "bu yazilamadi: " << sLine.c_str() << std::endl;
				getline(input, sLine, '\n');
				continue;
			}


			std::string idate, itime;
			if (0 == strncmp(sdate, "a", 1))
			{

				strcpy(sdate, sdate + 1);
				strcpy(unformatteddate, sdate);

				time_t timeNow = atol(sdate);
				tm* time_info = localtime(&timeNow);

				idate = std::to_string(time_info->tm_year + 1900);
				if (time_info->tm_mon + 1 <10)idate.append("0");
				idate.append(std::to_string(time_info->tm_mon + 1));
				if (time_info->tm_mday <10)idate.append("0");
				idate.append(std::to_string(time_info->tm_mday));


				if (time_info->tm_hour <10)itime.append("0");
				itime.append(std::to_string(time_info->tm_hour));
				if (time_info->tm_min <10)itime.append("0");
				itime.append(std::to_string(time_info->tm_min));

			}
			else {
				time_t timeNow = atoi(unformatteddate) + atoi(ssec) * atoi(sdate);
				tm* time_info = localtime(&timeNow);

				idate = std::to_string(time_info->tm_year + 1900);
				if (time_info->tm_mon + 1 <10)idate.append("0");
				idate.append(std::to_string(time_info->tm_mon + 1));
				if (time_info->tm_mday <10)idate.append("0");
				idate.append(std::to_string(time_info->tm_mday));


				if (time_info->tm_hour <10)itime.append("0");
				itime.append(std::to_string(time_info->tm_hour));
				if (time_info->tm_min <10)itime.append("0");
				itime.append(std::to_string(time_info->tm_min));

			}

			pricedata.open.push_back(atof(sopen));
			pricedata.high.push_back(atof(shigh));
			pricedata.low.push_back(atof(slow));
			pricedata.close.push_back(atof(sclose));
			pricedata.volume.push_back(atoi(svolume));
			pricedata.date.push_back(idate);
			pricedata._time.push_back(itime);



			getline(input, sLine, '\n');



		} while (!input.eof());


		return TRUE;
	}


	bool Mt4Csv(PriceData &pricedata,std::string fullpath) {
		std::string sLine = "";
		std::ifstream csv;
		try {
			csv.open(fullpath);
		}
		catch (std::system_error& e) {
			return false;
		}


		while (!csv.eof())
		{
			getline(csv, sLine);
			char syear[12], smonth[12], sday[12], shour[12], sminute[12], sopen[12], shigh[12], slow[12], sclose[12], svol[12];
			if (10 != sscanf(sLine.c_str(), "%[^.].%[^.].%[^,],%[^:]:%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", syear, smonth, sday, shour, sminute, sopen, shigh, slow, sclose, svol))
			{
				//std::cout << "bu yazilamadi: " << sLine.c_str() << std::endl;
				getline(csv, sLine);
				continue;
			}


			std::string idate = syear;
			idate.append(smonth);
			idate.append(sday);
			std::string itime = shour;
			itime.append(sminute);

			pricedata.open.push_back(atof(sopen));
			pricedata.high.push_back(atof(shigh));
			pricedata.low.push_back(atof(slow));
			pricedata.close.push_back(atof(sclose));
			pricedata.volume.push_back(atoi(svol));
			pricedata.date.push_back(idate);
			pricedata._time.push_back(itime);

		}
		csv.close();


		return true;
	}

	namespace Metastock {

		struct MASTERHEADER {
			unsigned __int8 totalfx = 0;//max 0xff
			const char padding = 0;
			unsigned __int8 lastaddedfx = 0;//max 0xff 
			const char zero[50] = { 0 };
		};


		struct MASTER {
			unsigned __int8 fx = 0;
			char unknown[2] = { 0 };
			unsigned __int8 fieldssize = 24;//intradayda 7kolonsayisi*4=28
			unsigned __int8 fieldsnumber = 6;//intradayda 7kolon date-time-o-h-l-c-v
			char ___unknown[2] = { 0 };
			char secname[16] = { 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20 };
			char _unknown[2] = { 0 };
			char fdate[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
			char ldate[4] = { 0 };//YYMMDD ieee 
			char period[1] = { 'D' };//'I', 'D'
			char timeframe[2] = { 0 };//0 and 60 minutes
			char secsymbol[16] = { 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20 };
			char __unknown[1] = { 0 };
		};


		struct XMASTERHEADER {
			unsigned char reserved[4] = { 0x5d,0xfe,0x58,0x4d };
			unsigned char unknown[6] = { 0 };
			unsigned __int16 totalfx_mwd = 0;//tersi
			unsigned char _unknown[2] = { 0 };
			unsigned __int16 _totalfx_mwd = 0;//tersi
			unsigned char __unknown[2] = { 0 };
			unsigned __int16 comingasfx_mwd = { 0 };//tersi//totalmwd+255+1
			unsigned char ___unknown[130] = { 0 };
		};

		#pragma pack(1)
		struct XMASTER {
			char reserved1[1] = { 1 };
			char secsymbol[15] = { 0 };
			char secname[46] = { 0 };
			char period[1] = { 'D' };//'I', 'D'
			unsigned __int16 timeframe = 0;//0 and 60 minutes
			unsigned __int16 fx = 0;//TERSÝ//256ilki
			unsigned __int32 _unknown = { 0x3f000000 };//BF intraday 3f daily
			char ___unknown[9] = { 0 };
			char notimportantfdate[4] = { 0 };//TERSÝ 20160229
			char ____unknown[20] = { 0 };
			unsigned __int32 fdate = { 0 };//TERSÝ 20160229
			unsigned __int32 _fdate = { 0 };//TERSÝ 20160229
			char _____unknown[4] = { 0 };
			unsigned __int32 ldate = { 0 };//TERSÝ 20160315
			unsigned char _______unknown[30] = { 0 };

		};

		struct FXHEADER {
			unsigned __int16 unkown = 0;
			unsigned __int16 totalrecord = 0;//ters+1 ieee deðil// +1 boþ satýr
			char _unkown[20] = { 0 };
		};

		struct FX {
			char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
								 //	char time[4] = { 0 };//2359000 ieee
			char open[4] = { 0 };//ieee
			char high[4] = { 0 };//ieee
			char low[4] = { 0 };//ieee
			char close[4] = { 0 };//ieee
			char volume[4] = { 0 };//ieee

		};
		struct FXIHEADER {
			unsigned __int16 unkown = 0;
			unsigned __int16 totalrecord = 0;//ters+1 ieee deðil// +1 boþ satýr
			char _unkown[24] = { 0 };
		};
		struct FXI {
			char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
			char time[4] = { 0 };//2359000 ieee
			char open[4] = { 0 };//ieee
			char high[4] = { 0 };//ieee
			char low[4] = { 0 };//ieee
			char close[4] = { 0 };//ieee
			char volume[4] = { 0 };//ieee

		};

		struct Info
		{
			int totalrecord;
			std::vector<std::string> symbols;
			std::vector<std::string> filename;
			std::vector<std::string> period;

			std::string dirname;
			


		};

		bool GetInfo(std::string dirname, Info &info) {

			std::string masterfullpath = dirname;
			masterfullpath.append("\\MASTER");
			FILE *fMaster=fopen(masterfullpath.c_str(), "rb");
			if (!fMaster)return false;

			MASTERHEADER masterheader;
			if (fread(&masterheader, 1, sizeof(MASTERHEADER), fMaster) != sizeof(MASTERHEADER))return false;
				
			info.dirname = dirname;
			MASTER master;
			while (fread(&master, 1, sizeof(MASTER), fMaster) == sizeof(MASTER)) {
					/* byte swap here */

				std::string str(std::begin(master.secsymbol), std::end(master.secsymbol));
				remove_if(str.begin(), str.end(), isspace);
				info.symbols.push_back(str);
				std::string filename="F";
				filename.append(std::to_string(master.fx));
				filename.append(".dat");
				info.filename.push_back(filename);

				std::string str2(std::begin(master.period), std::end(master.period));
				info.period.push_back(str2);
			}
			fclose(fMaster);
			info.totalrecord = masterheader.totalfx;


			std::string xmasterfullpath = dirname;
			xmasterfullpath.append("\\XMASTER");
			FILE *fxMaster = fopen(xmasterfullpath.c_str(), "rb");
			if (!fxMaster)return true;
			
			XMASTERHEADER xmasterheader;
			if (fread(&xmasterheader, 1, sizeof(XMASTERHEADER), fxMaster) != sizeof(XMASTERHEADER))return true;

			XMASTER xmaster;
			while (fread(&xmaster, 1, sizeof(XMASTER), fxMaster) == sizeof(XMASTER)) {
				/* byte swap here */

				std::string str(std::begin(xmaster.secsymbol), std::end(xmaster.secsymbol));
				remove_if(str.begin(), str.end(), isspace);
				info.symbols.push_back(str);
				std::string filename = "F";
				filename.append(std::to_string(xmaster.fx));
				filename.append(".mwd");
				info.filename.push_back(filename);


				std::string str2(std::begin(xmaster.period), std::end(xmaster.period));
				info.period.push_back(str2);
			}


			fclose(fxMaster);
			info.totalrecord += xmasterheader.totalfx_mwd;
			return true;
		}


		float BasicToIEEE(unsigned char *value)
		{
			float result;
			unsigned char *msbin = (unsigned char *)value;
			unsigned char *ieee = (unsigned char *)&result;
			unsigned char sign = 0x00;
			unsigned char ieee_exp = 0x00;
			int i;
			sign = msbin[2] & 0x80;
			for (i = 0; i<4; i++) ieee[i] = 0;
			if (msbin[3] == 0) return 0;
			ieee[3] |= sign;
			ieee_exp = msbin[3] - 2;
			ieee[3] |= ieee_exp >> 1;
			ieee[2] |= ieee_exp << 7;
			ieee[2] |= msbin[2] & 0x7f;
			ieee[1] = msbin[1];
			ieee[0] = msbin[0];
			return (result);
		}
		bool BySymbol(PriceData &pricedata, Info &info,std::string symbol) {

			size_t it=0;
			for (auto i = info.symbols.begin(); i != info.symbols.end(); ++i) {
				// process i
				std::string str=*i;
				
				
				if (0 == strcmp(str.c_str(), symbol.c_str()))
				{

					std::string fxfullpath = info.dirname;
					fxfullpath.append("\\");
					fxfullpath.append(info.filename.at(it));
					FILE *ffx = fopen(fxfullpath.c_str(), "rb");
					if (!ffx)return false;

					if ("I" != info.period.at(it)) {

						FXHEADER fxheader;
						if (fread(&fxheader, 1, sizeof(FXHEADER), ffx) != sizeof(FXHEADER))return false;

						FX fx;
						while (fread(&fx, 1, sizeof(FX), ffx) == sizeof(FX)) {
							/* byte swap here */
							pricedata.open.push_back(BasicToIEEE((unsigned char*)fx.open));
							pricedata.high.push_back(BasicToIEEE((unsigned char*)fx.high));
							pricedata.low.push_back(BasicToIEEE((unsigned char*)fx.low));
							pricedata.close.push_back(BasicToIEEE((unsigned char*)fx.close));
							pricedata.volume.push_back(BasicToIEEE((unsigned char*)fx.volume));
							pricedata.date.push_back(std::to_string(BasicToIEEE((unsigned char*)fx.date) + 19000000));
							pricedata._time.push_back("000000.000000");

						}
						fclose(ffx);
					}
					else
					{
						FXIHEADER fxiheader;
						if (fread(&fxiheader, 1, sizeof(FXIHEADER), ffx) != sizeof(FXIHEADER))return false;

						FXI fxi;
						while (fread(&fxi, 1, sizeof(FXI), ffx) == sizeof(FXI)) {
							/* byte swap here */


							pricedata.open.push_back(BasicToIEEE((unsigned char*)fxi.open));
							pricedata.high.push_back(BasicToIEEE((unsigned char*)fxi.high));
							pricedata.low.push_back(BasicToIEEE((unsigned char*)fxi.low));
							pricedata.close.push_back(BasicToIEEE((unsigned char*)fxi.close));
							pricedata.volume.push_back(BasicToIEEE((unsigned char*)fxi.volume));
							pricedata.date.push_back(std::to_string(BasicToIEEE((unsigned char*)fxi.date)+19000000));
							pricedata._time.push_back(std::to_string(BasicToIEEE((unsigned char*)fxi.time)));
							
						}
						fclose(ffx);
					}
					
					//std::cout << info.filename.at(it) << " " << info.period.at(it); // this will print all the contents of *features*
				}
				++it;
			}
			return true;
		}
	}
};





