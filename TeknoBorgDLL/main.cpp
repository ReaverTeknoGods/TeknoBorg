#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <cmath>

extern "C" int __declspec (dllexport) sub_100020B0(int a1, int a2, int a3, signed int a4);
extern "C" void __declspec (dllexport) fixwindowloc();
extern "C" int __declspec (dllexport) sub_417C62(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
extern "C" int __declspec(dllexport) sub_10017411();

#define RoundBy1000(a) (a / 0x1000 + ((a % 0x1000) > 0 ? 1 : 0)) * 0x1000
BOOL dword_10010FC4;
HWND handleri;
DWORD dword_10010EBC;
LONG sub_10001280;
DWORD dword_1001103C = 1;
DWORD TextStart = 0;
DWORD TextSize = 0;
DWORD FuncPTR;
DWORD FuncPTR2;
DWORD FuncPTR3;
DWORD FuncPTR4;
DWORD DLLStart = 0;
DWORD f4016EF = 0x4016EF;
DWORD f401645 = 0x401645;
DWORD f4017E4 = 0x4017E4;
DWORD f4016DB = 0x4016DB;
DWORD CenterX = 0;
DWORD CenterY = 0;
DWORD AlterVScreen = 0;
DWORD dword_1002623C = 0;
DWORD dword_1001C32C = 0;
DWORD dword_1001C304 = 0;
DWORD dword_1001C330 = 0;

int (__stdcall *dword_1001102C)(int var1, int *var2);
int (__stdcall *dword_10011018)(DWORD var1, DWORD var2, DWORD var3, DWORD var4);

void __declspec (naked) fixwindowloc()
{
	__asm
	{
		PUSH EAX
		MOV EAX, CenterX
		ADD DWORD PTR DS:[ESP+0xC], EAX
		MOV EAX, CenterY

		ADD DWORD PTR DS:[ESP+0x10], EAX
		POP EAX
		JMP AlterVScreen
	}
}

int __declspec() sub_10017411()
{
  int v0;
  int v1;
  int v2;
  unsigned int v3;
  int result;
  void *v5;

  v0 = *(DWORD*)(DLLStart + 0x2623C);
  v1 = *(DWORD*)(DLLStart + 0x1C32C);
  v2 = *(DWORD*)(DLLStart + 0x1C304);
  v3 = (unsigned int)*(DWORD*)(DLLStart + 0x1C330) >> 1;
  result = 0;
  do
  {
    v5 = (void *)v0;
    v0 += v2;
	if(IsBadWritePtr((void *)(v5),4*v3) == true)
	{
		return result;
	}
    memset(v5, 0, 4 * v3);
    --v1;
  }
  while ( v1 );
  return result;
}

int __declspec() sub_417C62(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  POINT Point;

  if(Msg == WM_PAINT)
  {
	  PAINTSTRUCT ps;
	  BeginPaint(hWnd, &ps);
	  RECT Rect;
	  GetWindowRect(hWnd, &Rect);
	  HDC v6 = GetDC(hWnd);
	  ReleaseDC(hWnd, v6);
      EndPaint(hWnd, &ps); 
	  return 1;
  }
  else if ( Msg > WM_CLOSE )
  {
    if ( Msg > WM_SYSCOMMAND )
    {
      if ( Msg > 2024 )
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
      if ( Msg == 2024 )
	  {
		  PostQuitMessage(0);
		  return 0;
	  }
      if ( Msg == WM_MOUSEMOVE )
      {
        Point.x = lParam;
        Point.y = HIWORD(lParam);
        ClientToScreen(hWnd, &Point);
		__asm
		{
			PUSH Point.y
			PUSH Point.x
			MOV EAX, DWORD PTR DS:[0x43F0E0]
			MOV ECX, DWORD PTR DS:[EAX+0x4]
			CALL f4016EF
		}
      }
      else
      {
        if ( Msg == WM_LBUTTONDOWN )
        {
          Point.x = lParam;
          Point.y = HIWORD(lParam);
		  __asm
		  {
			  PUSH Point.y
			  PUSH Point.x
			  CALL f401645
		  }
        }
        else
        {
          if ( Msg != WM_LBUTTONDBLCLK )
            return DefWindowProcA(hWnd, Msg, wParam, lParam);
		  __asm
		  {
			  MOV EAX, DWORD PTR DS:[0x43F0E0]
			  MOV ECX, DWORD PTR DS:[EAX+0x04]
			  CALL f4016DB
		  }
        }
      }
    }
    else
    {
      if ( Msg == WM_SYSCOMMAND )
      {
        if ( wParam == 61760 )
          return 1;
      }
      else
      {
        if ( Msg != WM_KEYUP )
          return DefWindowProcA(hWnd, Msg, wParam, lParam);
		__asm
		{
			PUSH wParam
			MOV EAX, DWORD PTR DS:[0x43F0E0]
			MOV ECX, DWORD PTR DS:[EAX+0x4]
			CALL f4017E4
		}
      }
    }
  }
  else
  {
    if ( Msg != WM_CLOSE )
    {
      if ( Msg != WM_DESTROY )
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
    DestroyWindow(hWnd);
  }
  return 0;
}

void main()
{
	DWORD oldprot;
	DWORD oldprot2;
	DWORD oldprot3;
	DWORD oldprot4;

	DWORD hookptr = 0x410828;
	DWORD ExecutableStart = (DWORD)GetModuleHandleA(NULL);
	DLLStart = (DWORD)GetModuleHandleA("DUKDll95.dll");

	DWORD DLLPE = DLLStart + 0x3C;
	DLLPE = *(DWORD *)DLLPE;
	DLLPE += DLLStart + 0x100;
	DWORD DLLTextSize = RoundBy1000(*(DWORD *)DLLPE);
	DLLPE+= 4;
	DWORD DLLTextStart = DLLStart + *(DWORD *)DLLPE;

	VirtualProtect((LPVOID)DLLTextStart,DLLTextSize,0x40, &oldprot3);

	dword_1002623C = DLLStart + 0x2623C;
	dword_1001C32C = DLLStart + 0x1C32C;
	dword_1001C304 = DLLStart + 0x1C304;
	dword_1001C330 = DLLStart + 0x1C330;

	DLLStart += 0x17411;
	FuncPTR4 = 	(DWORD)GetProcAddress(GetModuleHandle("TeknoBorg.dll"), "sub_10017411");
	*(WORD *)DLLStart = 0x25FF;
	DLLStart += 2;
	*(DWORD *)DLLStart = (DWORD)&FuncPTR4;
	DLLStart -= 0x17411;
	DLLStart -= 2;

	VirtualProtect((LPVOID)DLLTextStart,DLLTextSize,oldprot3, &oldprot4);
	
	DWORD DowPE = ExecutableStart + 0x3C;
	DowPE = *(DWORD *)DowPE;
	DowPE += ExecutableStart + 0x100;
	TextSize = RoundBy1000(*(DWORD *)DowPE);
	DowPE+= 4;
	TextStart = ExecutableStart + *(DWORD *)DowPE;
	FuncPTR = (DWORD)GetProcAddress(GetModuleHandle("TeknoBorg.dll"), "sub_100020B0");
	VirtualProtect((LPVOID)TextStart,TextSize,0x40, &oldprot);
	*(WORD *)hookptr = 0x15FF;
	hookptr += 2;
	*(DWORD *)hookptr = (DWORD)&FuncPTR;
	hookptr += 4;
	*(BYTE *)hookptr = 0x90;
	FuncPTR2 = (DWORD)GetProcAddress(GetModuleHandle("TeknoBorg.dll"), "sub_417C62");
	*(DWORD *)0x417B7F = FuncPTR2;

	// Res fix #1
	*(DWORD *)0x4109BF = 0x000280B8;
	*(WORD *)0x4109C3 = 0x9000;

	// Res fix #2
	*(DWORD *)0x4109cb = 0x0001E0B8;
	*(WORD *)0x4109cf = 0x9000;

	// Window Fix
	*(BYTE *)0x00417B78 = 0x08;

	// Window Blackfix
	*(DWORD *)0x004188D0 = 0x7400F883;
	*(DWORD *)0x004188D4 = 0x90A1500D;
	*(DWORD *)0x004188D8 = 0x500043E6;
	*(DWORD *)0x004188DC = 0x190415FF;
	*(BYTE *)0x004188E0 = 0x44;

	// NOCD Fix
	*(DWORD *)0x41810E = 0xB805C766;
	*(DWORD *)0x418112 = 0x2E0043EF;
	*(WORD *)0x418116 = 0xC35C;

	// Screen Location Fix
	AlterVScreen = *(DWORD *)0x44161C;
	FuncPTR3 = (DWORD)GetProcAddress(GetModuleHandle("TeknoBorg.dll"), "fixwindowloc");
	*(DWORD *)0x44161C = FuncPTR3;
	VirtualProtect((LPVOID)TextStart,TextSize,oldprot, &oldprot2);
}

#pragma optimize("",off)
BOOL __cdecl sub_10001830(HWND hWnd, int a2, int a3)
{
  BOOL result;
  struct tagRECT Rect;
  int X;
  int Y;

  result = dword_10010FC4;
  if ( !dword_10010FC4 )
  {
	//// Screen resolution
    float screen_cx = (float)GetSystemMetrics(SM_CXSCREEN);
    float screen_cy = (float)GetSystemMetrics(SM_CYSCREEN);
 
	// Centre of screen
	float screen_centre_x = screen_cx / 2.0f;
	float screen_centre_y = screen_cy / 2.0f;
	
	int X = screen_centre_x-(640/2);
	int Y = screen_centre_y-(480/2);

	CenterX = X;
	CenterY = Y;

    dword_10010FC4 = 1;
    Rect.top = 0;
    Rect.left = 0;
    Rect.right = a2 - 1;
    Rect.bottom = a3 - 1;
    AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, 0);
    SetWindowLongA(hWnd, GWL_STYLE, 0);
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, CenterX, CenterY, 0);
    ShowWindow(hWnd, 1);
    dword_10010EBC = GetWindowLongA(hWnd, -4);
    if ( !(dword_10010EBC & 0xFF000000) )
      SetWindowLongA(hWnd, -4, (LONG)sub_10001280);
    ShowCursor(0);
    result = InvalidateRect(0, 0, 1);
  }
  return result;
}

#pragma optimize("",off)
int __declspec(naked) sub_100020B0(int a1, int a2, int a3, signed int a4)
{
	__asm
	{
		MOV EAX, DWORD PTR DS:[EAX]
		MOV EAX, DWORD PTR DS:[EAX]
		PUSH ECX
		MOV ECX, DWORD PTR DS:[EAX+0x54]
		MOV dword_10011018, ECX
		MOV ECX, DWORD PTR DS:[EAX+0x30]
		MOV dword_1001102C, ECX
		MOV ECX, DWORD PTR DS:[0x43EEA0]
		MOV handleri, ECX
		POP ECX
		PUSH EBP
		MOV EBP,ESP
		AND ESP,0xFFFFFFF8
		SUB ESP,0x84
		PUSH EBX
		MOV EBX,DWORD PTR SS:[EBP+0x14]
		PUSH ESI
		MOV ESI,DWORD PTR SS:[EBP+0x14]
		PUSH EDI
		MOV EDI,DWORD PTR SS:[EBP+0x10]
		PUSH EBX
		PUSH ESI
		PUSH EDI
		PUSH 0
		ADD ESP,0x10
		PUSH ESI
		PUSH EDI
		PUSH handleri
		CALL sub_10001830
		ADD ESP,0x0C
		MOV ESI,DWORD PTR SS:[EBP+0x8]
		XOR EAX,EAX
		MOV ECX,0x1F
		LEA EDI,[ESP+0x10]
		REP STOS DWORD PTR ES:[EDI]
		CMP DWORD PTR DS:[dword_1001103C],4
		SBB EAX,EAX
		LEA ECX,[ESP+0x10]
		AND EAX,0xFFFFFFF0
		PUSH ECX
		ADD EAX,0x7C
		PUSH ESI
		MOV DWORD PTR SS:[ESP+0x18],EAX
		MOV DWORD PTR SS:[ESP+0x1C],0x1006
		MOV DWORD PTR SS:[ESP+0x60],0x20
		MOV DWORD PTR SS:[ESP+0x64],0x40
		CALL dword_1001102C
		CMP DWORD PTR SS:[ESP+0x64],EBX
		JE dw0025216E
		MOV EDX,DWORD PTR SS:[ESP+0x18]
		MOV EAX,DWORD PTR SS:[ESP+0x1C]
		PUSH EBX
		PUSH EDX
		PUSH EAX
		PUSH ESI
		CALL dword_10011018
		POP EDI
		POP ESI
		POP EBX
		MOV ESP,EBP
		POP EBP
		RETN 0x10
		dw0025216E:
		POP EDI
		POP ESI
		XOR EAX,EAX
		POP EBX
		MOV ESP,EBP
		POP EBP
		RETN 0x10
	}
}


#pragma optimize("",on)
LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {

   // Get event information
   PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT) lParam;
 
   bool eat = true;
 
   // Screen resolution
   static float screen_cx = static_cast<float>( GetSystemMetrics(SM_CXSCREEN) );
   static float screen_cy = static_cast<float>( GetSystemMetrics(SM_CYSCREEN) );
 
   // Centre of screen
   static float screen_centre_x = screen_cx / 2.0f;
   static float screen_centre_y = screen_cy / 2.0f;
 
   // Calculate distance away from centre of screen
   float dx = p->pt.x - screen_centre_x;
   float dy = p->pt.y - screen_centre_y;
 
   float dist = sqrt(dx * dx + dy * dy);
 
	int X = screen_centre_x-(640/2);
	int Y = screen_centre_y-(480/2);

   // Check if cursor is more than 300px away from centre of screen
   if(p->pt.x < X && p->pt.y < Y)
   {
	// Trap cursor
      SetCursorPos(
         /* X */ int( X ),
         /* Y */ int( Y )
      );
	  eat = true;
   }
   else if(p->pt.x < X)
   {
	  SetCursorPos(
         /* X */ int( X ),
         /* Y */ int( p->pt.y )
      );
	  eat = true;
   }
   else if(p->pt.y < Y)
   {
	   	         // Trap cursor
      SetCursorPos(
         /* X */ int( p->pt.x ),
         /* Y */ int( Y )
      );
	  eat = true;
   }
   if(p->pt.x > X+640 && p->pt.y > Y+480)
   {
	// Trap cursor
      SetCursorPos(
         /* X */ int( X+640 ),
         /* Y */ int( Y+480 )
      );
	  eat = true;
   }
   else if(p->pt.x > X+640)
   {
	  SetCursorPos(
         /* X */ int( X+640 ),
         /* Y */ int( p->pt.y )
      );
	  eat = true;
   }
   else if(p->pt.y > Y+480)
   {
	// Trap cursor
      SetCursorPos(
         /* X */ int( p->pt.x ),
         /* Y */ int( Y+480 )
      );
	  eat = true;
   } 
   return (eat ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{  
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
			main();
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}