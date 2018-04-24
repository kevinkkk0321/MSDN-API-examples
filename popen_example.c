/*popen_example*/

void popen_example()
{
	char psBuffer[1024];
	char test[2048] = "running command full path";
	memset(test, '\0', sizeof(test));
	FILE *pPipe = _popen(test, "r");
	if (!pPipe)	return 0;
	else printf("_popen success\n");
	
	while (!feof(pPipe)) {
		if (fgets(psBuffer, 1024, pPipe) != NULL)
			strcat(test, psBuffer);
	}
	_pclose(pPipe);
}
