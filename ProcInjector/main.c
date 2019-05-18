#include "dllinject.h"

int main(int argc, char **argv)
{
	int techNum;

	puts("-----------------------------------");
	puts("      [> PROCESS INJECTOR <]"        );
	puts("      [>    By OUS-CH     <]"        );
	puts("-----------------------------------");	

	printf("[ 0 ] CLASSIC DLL INJECTION\n");
	printf("[ 1 ] PROCESS HOLOWING\n");
	printf("[ 2 ] HOOK INJECTION\n");

	printf("\nChoose an injection technique : ");
	scanf("%d", &techNum);

	switch (techNum)
	{
		case 0: 
			DllInject();
			break;
		case 1:
			printf("Process Holowing\n");
			break;
		case 2: 
			printf("Hook injection\n");
			break;
		default: printf("sorry! no such technique!");
			break;
	}

	printf("\n\nDone!\n");
	getchar();
	getchar();
	return 0;

}