void getMDfive(char *filename) {

	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789abcdef";

	hFile = CreateFile(filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwStatus = GetLastError();
		printf("Error opening file %s\nError: %d\n", filename, dwStatus);
		return 0;
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		return 0;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return 0;
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
		&cbRead, NULL))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			dwStatus = GetLastError();
			printf("CryptHashData failed: %d\n", dwStatus);
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return 0;
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		printf("ReadFile failed: %d\n", dwStatus);
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return 0;
	}
	cbHash = MD5LEN;
	int i = 0;
	int counter = 0;
	char record1[20];
	char record2[20];
	char record[40];
	ZeroMemory(record1, 20);
	ZeroMemory(record2, 20);
	ZeroMemory(record, 40);
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		printf("MD5 hash of file %s is: ", filename);
		for (DWORD i = 0; i < cbHash; i++)
		{
			printf("%c%c", rgbDigits[rgbHash[i] >> 4],
				rgbDigits[rgbHash[i] & 0xf]);
			record1[i] = rgbDigits[rgbHash[i] >> 4];
			record2[i] = rgbDigits[rgbHash[i] & 0xf];
		}
		printf("\n");
	}
	else
	{
		dwStatus = GetLastError();
		printf("CryptGetHashParam failed: %d\n", dwStatus);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);

	//return dwStatus;
	for (int i = 0; (record1[i] != NULL && record2[i] != NULL); i++)
	{
		record[counter] = record1[i];
		counter++;
		record[counter] = record2[i];
		counter++;
	}
	char info_record[MAX_number];
	ZeroMemory(info_record, MAX_number);
	ostringstream oss;
	string MD5_string;
	for (int i = 0; record[i] != NULL; i++)
	{
		oss << record[i];
		MD5_string = oss.str();
	}
	printf("\n");
	cout<< MD5_string; //MD5_string = MD5
	//
	//char *MD5_cstr = NULL;
	//MD5_cstr = strdup(MD5_string.c_str());
	//return MD5_cstr;
}
