
void PrintFileVersion(TCHAR *pszFilePath)
{
	DWORD               dwSize = 0;
	BYTE                *pbVersionInfo = NULL;
	VS_FIXEDFILEINFO    *pFileInfo = NULL;
	UINT                puLenFileInfo = 0;

	// Get the version information for the file requested
	dwSize = GetFileVersionInfoSize(pszFilePath, NULL);
	if (dwSize == 0)
	{
		printf("Error in GetFileVersionInfoSize: %d\n", GetLastError());
		return;
	}

	pbVersionInfo = new BYTE[dwSize];

	if (!GetFileVersionInfo(pszFilePath, 0, dwSize, pbVersionInfo))
	{
		printf("Error in GetFileVersionInfo: %d\n", GetLastError());
		delete[] pbVersionInfo;
		return;
	}

	if (!VerQueryValue(pbVersionInfo, _T((LPCSTR)"\\"), (LPVOID*)&pFileInfo, &puLenFileInfo))
	{
		printf("Error in VerQueryValue: %d\n", GetLastError());
		delete[] pbVersionInfo;
		return;
	}

	// pFileInfo->dwFileVersionMS is usually zero. However, you should check
	// this if your version numbers seem to be wrong

	printf("File Version: %d.%d.%d.%d\n",
		(pFileInfo->dwFileVersionMS >> 16) & 0xff,
		(pFileInfo->dwFileVersionMS >> 0) & 0xff,
		(pFileInfo->dwFileVersionLS >> 16) & 0xff,
		(pFileInfo->dwFileVersionLS >> 0) & 0xff

	);

	// pFileInfo->dwProductVersionMS is usually zero. However, you should check
	// this if your version numbers seem to be wrong.
	//printf("Product Version: %d.%d.%d.%d\n",
	//(pFileInfo->dwProductVersionLS >> 16) & 0xff,
	//(pFileInfo->dwProductVersionLS >> 0) & 0xff,
	//(pFileInfo->dwProductVersionLS >> 8) & 0xff,
	//(pFileInfo->dwProductVersionLS >> 0) & 0xff
	//);
}
