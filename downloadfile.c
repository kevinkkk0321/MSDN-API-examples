HRESULT downloadfiles(char *target_URL, char *path, char *info_name) {

	// create Info file
	FILE* file;
	char filepath[256];
	char filename[256];
	const char null[5] = "NULL";
	const char downloadinfo[19] = "downloadfiles.info";
	if (strncmp(info_name, null, sizeof(info_name)) == 0) strncpy(filename, downloadinfo, sizeof(filename));
	else  strncpy(filename, info_name, sizeof(filename));
	//strncpy(filename, downloadinfo, sizeof(downloadinfo));
	ZeroMemory(filepath, 256);

	strcat_s(filepath, pwd);
	strcat_s(filepath, filename);

	cout << "info filepath: "<< filepath << endl;
	//
	char targeturl[512];
	strcpy(targeturl, target_URL);
	char slash[2] = "\\";
	char *p;
	char *targetfilename;


	char *token = strtok_s(target_URL, "\/", &p);
	while ((token != NULL)) {
		targetfilename = token;			
		token = strtok_s(NULL, "\/", &p);
	}

	char add[100];
	strcpy(add, targetfilename);
	strcpy(target_file, path);
	strcat(target_file, slash);
	strcat(target_file, add);
	cout << "download filepath: " << target_file << endl;
	UtilityUpdater* updater = new UtilityUpdater();

	//URLDownload_parameter
	URLdown th1;
	th1.pCaller = NULL;
	th1.szURL = targeturl;
	th1.szFileName = target_file;
	th1.dwReserved = NULL;
	th1.lpfnCB = updater;
	th1.status = ERR_INET_DOWNLOAD_OK;

	BOOL Internet_check;
	int no_data_count = 0;
	ULONG old_transferred = 0;
	HANDLE handle1 = CreateThread(NULL, 0, URLDownload, &th1, 0, NULL);	

	// internet corruption circumstance  
	while (1) {
		Sleep(1500);
		// no internet at first 
		int sleep_count = 0;
		DWORD lpdwFlags;
		Internet_check = IsNetworkAlive(&lpdwFlags);
		if ((updater->progress) == 0 && Internet_check == FALSE)
		{
			adjust(filepath); Sleep(2000);
			return ERR_INET_DOWNLOAD_FAILURE;
		}		
		// no transferred data issue
		//printf("old Transferred : %lu byte(s)\n", old_transferred);
		//printf("new Transferred : %lu byte(s)\n", updater->getTransferred());		
		if ((updater->getTransferred() - old_transferred) == 0)
			return ERR_INET_DOWNLOAD_FAILURE; 
		old_transferred = updater->getTransferred();
		
		if (((updater->progress) == 100) ) break;
	}	

	CloseHandle(handle1);
	CloseHandle(handle2);
	
	return th1.status;
}

DWORD WINAPI URLDownload(PVOID param1) {
	PURLDownload_parameter URLparam = (PURLDownload_parameter) param1;
	DeleteUrlCacheEntry(URLparam->szURL);
	downloadcheck = URLDownloadToFile( URLparam->pCaller, URLparam->szURL, URLparam->szFileName, URLparam->dwReserved, URLparam->lpfnCB);
	if (downloadcheck == S_OK) { URLparam->status = ERR_INET_DOWNLOAD_OK; }
	else if (downloadcheck == INET_E_DOWNLOAD_FAILURE) { URLparam->status = ERR_INET_DOWNLOAD_FAILURE; }
	else {  URLparam->status = ERR_INET_DOWNLOAD_OUTMEM; }
	return 0;
}

