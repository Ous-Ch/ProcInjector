#include "pid.h"

DWORD GetPid(char *pname)
{
	HANDLE hsnapshot;	// handle to process snapshot 
	PROCESSENTRY32 ps;	// process info struct
	BOOL running = FALSE;	// ture if process is running

	// take a snapshot of all running process in the system
	hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hsnapshot == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Invalid handle value!", GetLastError());
		return -1;
	}

	ps.dwSize = sizeof(PROCESSENTRY32);	// set dwsize to size of process struct

	if (!Process32First(hsnapshot, &ps))
	{
		fprintf(stderr, "No process found!", GetLastError());
		CloseHandle(hsnapshot);	// close snapshot handle
		return -1;
	}

	do
	{
		//printf("\n %-4d | %-30s ", ps.th32ProcessID, ps.szExeFile) ; // list of all running process
		if (!strcmp(ps.szExeFile, pname))	// 0 if equal
		{
			printf("\n[+] %s | PID: %-4d ", ps.szExeFile, ps.th32ProcessID);
			CloseHandle(hsnapshot);
			return ps.th32ProcessID;
		}

	} while (Process32Next(hsnapshot, &ps));

	CloseHandle(hsnapshot);
	return -1;	// process is not running

}