#include "dllinject.h"

BOOL DllInject() 
{
	DWORD pid;
	char pname[255];

	printf("\n>> Enter the process name: ");
	scanf("%s", pname);

	if ((pid = GetPid(pname)) != -1)
	{
		printf("\n[+] %ld injected!", pid);
	}
	else 
	{
		printf("\n[-] '%s' in not running!", pname);
	}

	return TRUE;
}