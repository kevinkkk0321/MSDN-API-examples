void Scan_disk()
{
	/*start scan disk steps*/
	printf("---start scan disk steps---\n");
	PBasicInfo info_buffer = (PBasicInfo)malloc(sizeof(BasicInfo));
	PDiskList diskList = (PDiskList)malloc(sizeof(DiskList));
	DTA_DEVICE_TYPE devType = DEVICE_TYPE_UNKNOWN;
	int diskIndex = 0;
	int	found = 0;
	ULONG ret = 0;
	char custom_name[55];
	ZeroMemory(custom_name, 55);
	ZeroMemory(diskList, sizeof(DiskList));
	

	for (UCHAR i = 0; i < MAX_DISKS; i++)
	{
		int external_drive = 0;
		InterfaceAPI* api = new InterfaceAPI();
		ZeroMemory(info_buffer, sizeof(BasicInfo));
		devType = DEVICE_TYPE_UNKNOWN;
		ret = api->examDisk(i, devType);
		if (ret == ERR_NO_ERROR)
		{
			if (devType == DEVICE_TYPE_NVME)
			{
				/* PhysicalDriveX */
				printf("found NVMe drive");	
				found = 1;				
			}
		}
		delete api;
	}
	
	if (found == 0)
	{
		if (info_buffer)
			free(info_buffer);
		if (diskList)
			free(diskList);
		return ERR_DISK_NO_DISK;
	}
	
}
