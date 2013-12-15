#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "PE.h"
STARTUPINFO si;
PROCESS_INFORMATION pi;
_CONTEXT mycontext;
PEStruct FilePEFile;
int RunTo(DWORD Address, DWORD Mode, DWORD Eip);
DWORD MyLoadLibraryA = 0;
int LoadHookDLL()
{
	MyLoadLibraryA = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	char backbuf[256];
	LPSTR filelocation = "TeknoBorg.dll";
	DWORD OEP = FilePEFile.image_nt_headers.OptionalHeader.AddressOfEntryPoint + FilePEFile.image_nt_headers.OptionalHeader.ImageBase;
	DWORD TextLocation = OEP + 0x0C;
	DWORD CallLoadLibraryA = MyLoadLibraryA - (OEP + 10);
	ReadProcessMemory(pi.hProcess,(LPVOID)OEP,backbuf,256,0);
	WriteProcessMemory(pi.hProcess,(LPVOID)OEP,"\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xEB\xFE",0x0C,0);
	WriteProcessMemory(pi.hProcess,(LPVOID)(OEP + 1), &TextLocation,4,0);
	WriteProcessMemory(pi.hProcess,(LPVOID)(OEP + 6), &CallLoadLibraryA,4,0);
	WriteProcessMemory(pi.hProcess,(LPVOID)TextLocation,filelocation,13,0);
	GetThreadContext(pi.hThread, &mycontext);
	Sleep(1000);
	if(!RunTo(OEP + 10,0,0))
	{
		printf("Failed to Load DLL!");
		return 0;
	}
	if(mycontext.Eax == 0)
	{
		printf("Failed to Load DLL!");
		return 0;
	}
	GetThreadContext(pi.hThread, &mycontext);
	mycontext.Eip = FilePEFile.image_nt_headers.OptionalHeader.AddressOfEntryPoint + FilePEFile.image_nt_headers.OptionalHeader.ImageBase;
	SetThreadContext(pi.hThread,&mycontext);
	WriteProcessMemory(pi.hProcess,(LPVOID)OEP,backbuf,256,0);
	printf("DLL Loaded!\n");// %08x\n", mycontext.Eax);
	return 1;
}
int RunTo(DWORD Address, DWORD Mode, DWORD Eip)
{
	char tempbuf[4];
	if(Eip != 0)
	{
		GetThreadContext(pi.hThread,&mycontext);
		mycontext.Eip = Eip;
		SetThreadContext(pi.hThread,&mycontext);
	}
	ReadProcessMemory(pi.hProcess,(LPVOID)Address,tempbuf,4,0);
	WriteProcessMemory(pi.hProcess,(LPVOID)Address,"\xEB\xFE",2,0);
	ResumeThread(pi.hThread);
	while(GetThreadContext(pi.hThread, &mycontext))
	{
		if(Mode == 1) WriteProcessMemory(pi.hProcess,(LPVOID)Address,"\xEB\xFE",2,0);
		if(mycontext.Eip == Address) break;
		Sleep(100);
	}
	SuspendThread(pi.hThread);
	if(!GetThreadContext(pi.hThread, &mycontext)) return 0;
	WriteProcessMemory(pi.hProcess,(LPVOID)Address,tempbuf,4,0);
	return 1;
}
void lowercase(char string[])
{
   int  i = 0;

   while ( string[i] )
   {
      string[i] = tolower(string[i]);
      i++;
   }

   return;
}
int main(int argc, char *argv[])
{
	WIN32_FIND_DATA filedata;
	HANDLE hFind;
	char szDir[512];
	char szDir2[512];
	char homo[256];
	printf("TeknoBorg - Star Trek Borg Modernizer\nFor Borg.exe only!\nhttp://www.teknogods.com - Modding Gurus!\nSupported OS: Windows XP x86/x64, Windows Vista x86/x64, Windows 7 x86/x64\nCode: Reaver\n");
	GetCurrentDirectoryA(400, szDir);
	sprintf_s(szDir2, "%s\\TeknoBorg.dll", szDir);
	hFind = FindFirstFile("borg.exe", &filedata);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		printf("Unable to find borg.exe");
		_getch();
		return 1;
	}
	else
	{
			printf("borg.exe found, starting process\n");
			FilePEFile = getPEFileInformation("borg.exe");
				if( !CreateProcess( "borg.exe", // No module name (use command line). 
				argv[1],			  // Command line.
				NULL,             // Process handle not inheritable. 
				NULL,             // Thread handle not inheritable. 
				FALSE,            // Set handle inheritance to FALSE. 
				CREATE_SUSPENDED | CREATE_NEW_PROCESS_GROUP, // suspended creation flags. 
				NULL,             // Use parent's environment block. 
				NULL,             // Use parent's starting directory. 
				&si,              // Pointer to STARTUPINFO structure.
				&pi )             // Pointer to PROCESS_INFORMATION structure.
				)
					{
						printf("Failed to load process!");
						_getch();
						return 1;
					}
			mycontext.ContextFlags = 0x00010000+1+2+4+8+0x10;
			GetThreadContext(pi.hThread, &mycontext);
			Sleep(1000);
			if(!RunTo(FilePEFile.image_nt_headers.OptionalHeader.AddressOfEntryPoint + FilePEFile.image_nt_headers.OptionalHeader.ImageBase,1,0))
			{
				printf("Failed to run the process");
				TerminateProcess(pi.hProcess, 0);
				_getch();
				return 1;
			}
			sprintf(homo, "Tekno%08x", pi.dwProcessId ^ 0x60);
			CreateMutex(0, true, homo);
			if(!LoadHookDLL()) 
			{
				TerminateProcess(pi.hProcess, 0);
				_getch();
				return 1;
			}
			printf("Resuming game!\n");
			ResumeThread(pi.hThread);
			while(GetThreadContext(pi.hThread, &mycontext)) Sleep(2000);
			printf("Reaver thanks you for using this tool!\n");
			_getch();

	}
}