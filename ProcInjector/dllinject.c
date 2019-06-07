#include "dllinject.h"

#define DLLPATH "D:\\msgbox.dll"

BOOL DllInject() 
{
	DWORD pid;
	char pname[255];
	HANDLE hprocess = NULL;
	DWORD SizeDllPath = strlen(DLLPATH)+1;	// +1 for the /0
	LPVOID lpBaseAddr;	// generic pointer void* 
	DWORD bytewriten;
	FARPROC AddrFunc;
	HANDLE hremoteThread;

	EnableDebugPrivilege();

	printf("\n>> Enter the process name: ");
	scanf("%s", pname);

	printf(">> Dll path: %s", DLLPATH);

	if ((pid = GetPid(pname)) != -1)
	{
		// 01 Get the process Handle
		if ((hprocess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION|
			PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE, FALSE, pid)) != NULL) 
		{
			printf("\n[+] Get process handle", pid);
			printf("\t\t\tOK");

			// 02 allocate dllpath to process memory 
			if ((lpBaseAddr = VirtualAllocEx(hprocess, NULL, SizeDllPath, MEM_COMMIT, PAGE_READWRITE)) != NULL)
			{
				printf("\n[+] Allocate %d Bytes of Memory", SizeDllPath);
				printf("\t\tOK");	

				// 03 write dllpath to the allocated space
				if (WriteProcessMemory(hprocess, lpBaseAddr,DLLPATH, SizeDllPath, &bytewriten))
				{
					printf("\n[+] Write dllpath to Memory %ld", bytewriten);
					printf("\t\tOK");

					// 04.1 Get address of LoadLibraryA Funciton from Kernel32.dll 
					if ((AddrFunc = GetAddrFunc("kernel32.dll", "LoadLibraryA")) != NULL)
					{
						//printf("\n@ of LoadLibraryA = 0x%X", AddrFunc);

						// 04.2 Load and exec the Dll
						if (hremoteThread = CreateRemoteThread(hprocess, NULL, 0, (LPTHREAD_START_ROUTINE)AddrFunc, lpBaseAddr, 0, NULL))
						{
							WaitForSingleObject(hremoteThread, INFINITE);

							printf("\n[+] dll injected with sucess!");
							printf("\t\tOK");
							CloseHandle(hremoteThread);
						}else 
						{
							printf("\n[-] dll injection failed!");
							printf("\t\tError %ld", GetLastError());
						}
						
					}else
					{
						printf("\n[-] Fatal Error! %ld", GetLastError());
					}

				} else 
				{
					printf("\n[-] Write dllpath to Memory");
					printf("\t\tError %ld\n", GetLastError());
				}

			} else 
			{
				printf("\n[-] Allocating %d Bytes of Memory", SizeDllPath);
				printf("\t\tError %ld\n", GetLastError());
			}
			CloseHandle(hprocess);
		}else 
		{
			printf("\n[-] Get process handle", pid);
			printf("\t\tError %ld\n", GetLastError());
		}

	}
	else 
	{
		printf("\n[-] '%s' is not running!", pname);
	}

	return TRUE;
}

// Get address of a fucntion from dll module
FARPROC GetAddrFunc(char *dllModuleName, char *funcName)
{
	HMODULE hkerneldll;
	FARPROC AddrFunc;

	if ((hkerneldll = GetModuleHandleA(dllModuleName)) != NULL)
	{
		if ((AddrFunc = GetProcAddress(hkerneldll, funcName)) != NULL)
		{
			return AddrFunc;
		}
	}

	FreeLibrary(hkerneldll);

	return NULL; 
}

void EnableDebugPrivilege()
{
	TOKEN_PRIVILEGES priv;
	HANDLE htoken;
	LUID luid;	// locally Unique Identifier

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &htoken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	priv.PrivilegeCount = 1;
	priv.Privileges[0].Luid = luid;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(htoken, FALSE, &priv, sizeof(priv), NULL, NULL);

	CloseHandle(htoken);
}
