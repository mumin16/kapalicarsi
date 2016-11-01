#pragma once
#include "windows.h"
#include "ddeml.h"
#include "stdio.h"
#include "dde.h"
#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> secdata;



DWORD idInst;
class dde
{
private:
	
	HCONV hConv;

public:
	
	char szItem[1024];
	dde() : szItem("GBPJPY")
	{
		//DDE Initialization
		UINT iReturn;
		iReturn = DdeInitialize(&idInst, (PFNCALLBACK)DdeCallback,
			APPCLASS_STANDARD | APPCMD_CLIENTONLY, 0);
		if (iReturn != DMLERR_NO_ERROR)
		{
			printf("DDE Initialization Failed: 0x%04x\n", iReturn);
	
			
		}
		
		//DDE Connect to Server using given AppName and topic.
		HSZ hszService, hszTopic;
		hszService = DdeCreateStringHandle(idInst, "MT4", 0);
		hszTopic = DdeCreateStringHandle(idInst, "QUOTE", 0);
		hConv = DdeConnect(idInst, hszService, hszTopic, NULL);
		DdeFreeStringHandle(idInst, hszService);
		DdeFreeStringHandle(idInst, hszTopic);
		if (hConv == NULL)
		{
			printf("DDE Connection Failed.\n");
			DdeUninitialize(idInst);
		}

		
		HSZ hszItem = DdeCreateStringHandle(idInst, szItem, 0);
		HDDEDATA hData = DdeClientTransaction(NULL, 0, hConv, hszItem, CF_TEXT,	XTYP_ADVSTART/*XTYP_REQUEST*/, TIMEOUT_ASYNC/*5000*/, NULL);
		DdeFreeStringHandle(idInst, hszItem);
	}

	virtual ~dde()
	{
		//DDE Disconnect and Uninitialize.
		DdeDisconnect(hConv);
		DdeUninitialize(idInst);
	}
	static HDDEDATA CALLBACK DdeCallback(UINT uType, UINT uFmt, HCONV hconv, HSZ hsz1, HSZ hsz2, HDDEDATA hdata, DWORD dwData1, DWORD dwData2)
	{

		return 0;
	}
	void DDERequest()
	{


		HSZ hszItem = DdeCreateStringHandle(idInst, szItem, 0);
		HDDEDATA hData = DdeClientTransaction(NULL, 0, hConv, hszItem, CF_TEXT, XTYP_REQUEST, 50, NULL);
		DdeFreeStringHandle(idInst, hszItem);
		if (hData == NULL)
		{
			printf("Request failed: %s\n", szItem);
			//reconnect();
		}
		else
		{
			char szResult[1024];

			if (DdeGetData(hData, (unsigned char *)szResult, 1024, 0))
			{

				if (secdata.size() == 0)
				{
					secdata.push_back(szResult);
					char date[12], time[12], bid[12];
					sscanf(szResult, "%[^ ] %[^ ] %[^ ]", date, time, bid);
					close.push_back(atof(bid));
					std::cout << szResult << std::endl;
				}
				else
				{
					if (strcmp(secdata.back().c_str(), szResult) != 0)
					{
						
						secdata.push_back(szResult);
						char date[12], time[12], bid[12];
						sscanf(szResult, "%[^ ] %[^ ] %[^ ]", date, time, bid);
						close.push_back(atof(bid));
						std::cout << szResult << std::endl;
					}
				}

			Sleep(50);

			}


		}
	}


};

