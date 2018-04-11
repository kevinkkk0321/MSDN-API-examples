//// Start to create process as user ////
void StartCreateProcessAsUser(wchar_t action_path)
{
	HANDLE hToken = 0;
	HANDLE phToken = NULL;
	DWORD SessionId;
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	bool result;
	bool bResult;
	LPVOID pEnv = NULL;	

	//
	SessionId = WTSGetActiveConsoleSessionId();

	bResult = WTSQueryUserToken(SessionId, &hToken);
	if (!bResult) {return;
	};

	// Convert the impersonation token to a primary token
	bResult = DuplicateTokenEx(hToken, 0, NULL, SecurityAnonymous, TokenPrimary, &phToken);
	if (!bResult) {return;
	};
	CloseHandle(hToken);

	//SetTokenInformation(hToken, TokenSessionId, &SessionId, sizeof(DWORD));
	bResult = CreateEnvironmentBlock(&pEnv, phToken, false);
	if (!bResult) {return;
	};

	memset(&sInfo, 0, sizeof(STARTUPINFO));
	sInfo.cb = sizeof(STARTUPINFO);
	sInfo.lpDesktop = L"WinSta0\\Default";
	sInfo.wShowWindow = SW_SHOW;
	sInfo.dwFlags = STARTF_USESHOWWINDOW;


	//parameter = 1 --> locked issue

	bResult = CreateProcessAsUser(phToken,
		NULL,
		action_path, //aaa;
		NULL,
		NULL,
		FALSE,
		CREATE_UNICODE_ENVIRONMENT | CREATE_NEW_CONSOLE,  //CREATE_NO_WINDOW //CREATE_NEW_CONSOLE
		pEnv,
		NULL,
		&sInfo,
		&pInfo);
	


	//for debug
	if (!bResult) {return;
	};

	if (pEnv)
	{
		DestroyEnvironmentBlock(pEnv);
	}

	return;
}