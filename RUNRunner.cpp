//---------------------------------------------------------------------------
#include <windows.h>
#pragma hdrstop
#include <condefs.h>
//---------------------------------------------------------------------------
#include "../Koperek.VCL/LIB/DLLDatabase/TSoft_Database.h"
#pragma link "../Koperek.VCL/LIB/DLLDatabase/TSoft_Database.obj"
#pragma link "../Koperek.VCL/LIB/DLLIO/TSoft_IO.obj"

//---------------------------------------------------------------------------
#define brakDYSKUwSTACJI (HANDLE)INVALID_HANDLE_VALUE
//---------------------------------------------------------------------------
USERES("RUNRunner.res");
//---------------------------------------------------------------------------
DWORD CheckLastError(VOID)
{
char lpMsgBuf[256]; MessageBeep(MB_ICONEXCLAMATION);
//
FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
              NULL,GetLastError(),
              MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
              &lpMsgBuf[0],sizeof(lpMsgBuf),NULL);
return MessageBox(NULL,lpMsgBuf,"Problem z...",
                  MB_OKCANCEL + MB_DEFBUTTON1 + MB_ICONERROR);
}
//---------------------------------------------------------------------------

DWORD CheckLastMessage(VOID)
{
for (HANDLE Semaphore; ( Semaphore = OpenSemaphore(SYNCHRONIZE, false, "RUNCloser_pauza") )!=NULL; )
    {
     CloseHandle(Semaphore); Sleep(5000);
    }
for (HANDLE Semaphore; ( Semaphore = OpenSemaphore(SYNCHRONIZE, false, "RUNCloser_stop") )!=NULL; )
    {
     CloseHandle(Semaphore); return 1;
    }
return 0;
}
//---------------------------------------------------------------------------

BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lParam)
{
SendMessage(hwnd, WM_DESTROY, 0, 0); return true;
}
//---------------------------------------------------------------------------

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR LiniaPolecen, int)
{
//
// blokada kilkukrotnego startu
//
HANDLE hInstanceMutex = ::CreateMutex(NULL,TRUE,"RUNDrive_mutex");
if (GetLastError() == ERROR_ALREADY_EXISTS)
   {
    if (hInstanceMutex)
        CloseHandle(hInstanceMutex);
    return MessageBoxA(0,"Próbujesz uruchamiaæ kilkakrotnie. Nie mogê na to pozwoliæ!","SSTSOFT.RUNRunner(R) 1.0",MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
   }
//
// ustawienie prorytetu
//
char temp[256];
SetPriorityClass(GetCurrentProcess(),IDLE_PRIORITY_CLASS);
//
// start
//
if (strstr(LiniaPolecen,"/h")!=NULL)
   {
    return MessageBoxA(0,"Autor, Stanislaw Stasiak dziekuje za kopiowanie i rozpowszechnianie programu!","SSTSOFT.Runner(R) 1.0",MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
   }
char CurrentDirectory[256];
GetCurrentDirectoryA(256,CurrentDirectory); if (CurrentDirectory[strlen(CurrentDirectory)-1]=='\\') CurrentDirectory[strlen(CurrentDirectory)-1] = '\0';
char ThisFile[256];
strcat(strcpy(ThisFile,CurrentDirectory),"\\RUNRunner.ini");
char Bufor[256];
GetPrivateProfileStringA("Bufor","Text","Clipboard:",Bufor,256,ThisFile);
char Naped[256];
GetPrivateProfileStringA("Naped","Text","D:",Naped,256,ThisFile);
char NazwaProgramuSzukacza[256];
GetPrivateProfileStringA("NazwaProgramuSzukacza","Text","Koperek.exe",NazwaProgramuSzukacza,256,ThisFile);
/*if (strcmp(NazwaProgramuSzukacza,strExtractFileName(temp, NazwaProgramuSzukacza))==0)
   {
    strInsert(NazwaProgramuSzukacza,0,strcat(strcpy(temp,CurrentDirectory),"\\"));
   }*/
char ListaParametrowProgramuSzukacza[256];
GetPrivateProfileStringA("ListaParametrowProgramuSzukacza","Text","/listuj /break{t} /maska{*.avi;*.asf;*.mpg;*.mpe;*.mpeg;*wmv;*.mp3;*.wma} /argts{\"\"\"[NazwaProgramuOdtwarzacza]\"\" [ListaParametrowProgramuOdtwarzacza] \"\"#file\"\"\"} \"[Bufor]\"",ListaParametrowProgramuSzukacza,256,ThisFile);
char NazwaProgramuOdtwarzacza[256];
GetPrivateProfileStringA("NazwaProgramuOdtwarzacza","Text","wmplayer.exe",NazwaProgramuOdtwarzacza,256,ThisFile);
/*if (strcmp(NazwaProgramuOdtwarzacza,strExtractFileName(temp, NazwaProgramuOdtwarzacza))==0)
   {
    strInsert(NazwaProgramuOdtwarzacza,0,strcat(strcpy(temp,CurrentDirectory),"\\"));
   } */
char ListaParametrowProgramuOdtwarzacza[256];
GetPrivateProfileStringA("ListaParametrowProgramuOdtwarzacza","Text","/Play",ListaParametrowProgramuOdtwarzacza,256,ThisFile);
//
//---------------------------------------------------------------------------
//
strExchange(ListaParametrowProgramuSzukacza,"[NazwaProgramuOdtwarzacza]",NazwaProgramuOdtwarzacza);
strExchange(ListaParametrowProgramuSzukacza,"[ListaParametrowProgramuOdtwarzacza]",ListaParametrowProgramuOdtwarzacza);
strExchange(ListaParametrowProgramuSzukacza,"[CurrentDirectory]",CurrentDirectory);
strExchange(ListaParametrowProgramuSzukacza,"[Bufor]",Bufor);
//
//---------------------------------------------------------------------------
//
// interwa³ sprawdzania
//
int Interval = GetPrivateProfileIntA("Interval","Int",2000,ThisFile);
int Timeout  = GetPrivateProfileIntA("Timeout","Int",20000,ThisFile);
//
TSoft::Database *RUNLista = new TSoft::Database;
//
unsigned long VolumeSerialNumber1 = 0;
unsigned long VolumeSerialNumber2 = 0;
//
LPPROCESS_INFORMATION lpProcessInformation = new PROCESS_INFORMATION;
LPSTARTUPINFO lpStartupInformation = new STARTUPINFO;
setmem(lpStartupInformation, sizeof(STARTUPINFO), 0);
lpStartupInformation->cb = sizeof(STARTUPINFO);
//
// pokazuj lub nie Okno o braku dysku (no raczej nie?)
//
SetErrorMode(SEM_FAILCRITICALERRORS);
//
// start co 5 sekund sprawdzaj tyle czasu zajmuje wyjêcie i w³o¿enie (szybkie)
//
for (; !CheckLastMessage(); Sleep(Interval))
    {
     if (GetVolumeInformation(Naped,NULL,0,&VolumeSerialNumber2,NULL,NULL,NULL,0))
        {
        if (VolumeSerialNumber1 !=VolumeSerialNumber2)
           {
            VolumeSerialNumber1  =VolumeSerialNumber2;
            //
            // umieœæ dla twórcy nazwê Ÿród³ow¹ w schowku
            //
            if (OpenClipboard(NULL))
               {EmptyClipboard();
                SetClipboardData(CF_TEXT, strDup(Naped));
                CloseClipboard();}
            else
                return 0;//
            // uruchom twórcê listy
            //
            char ExecuteLine[256];
            strcat(ExecuteLine, "\"");
            strcat(ExecuteLine, NazwaProgramuSzukacza);
            strcat(ExecuteLine, "\" ");
            strcat(ExecuteLine, ListaParametrowProgramuSzukacza);
            if (CreateProcess(NazwaProgramuSzukacza, ExecuteLine,NULL,NULL,false,NORMAL_PRIORITY_CLASS,NULL,NULL,lpStartupInformation,lpProcessInformation))
               {
                WaitForInputIdle(lpProcessInformation->hProcess,Timeout);
                for (DWORD dwExitStatus = STILL_ACTIVE; dwExitStatus==STILL_ACTIVE; Sleep(Interval))
                    {
                     GetExitCodeProcess(lpProcessInformation->hProcess, &dwExitStatus);
                    }
                CloseHandle(lpProcessInformation->hProcess);
                CloseHandle(lpProcessInformation->hThread);
               }
            // wyci¹gam ze schowka
            strcpy(temp,Bufor);
            if (strstr(strlwr(temp), "clipboard:")!=NULL)
               {
                RUNLista->Text[LINES] = "";
                if (OpenClipboard(0))
                   {
                    if (IsClipboardFormatAvailable(CF_TEXT))
                       {RUNLista->Text[LINES] = (char*)GetClipboardData(CF_TEXT);
                        if (strcmp(RUNLista->Text[LINES],Naped)==0) // tzn ¿e nie zadzia³a³o
                            RUNLista->Text[LINES] = "";
                       }
                    CloseClipboard();
                   }
                }
            else
                {
                 RUNLista->Clr();
                 RUNLista->Alias = Bufor;
                 RUNLista->Read();
                }
            for (int i = 0; i < RUNLista->Count && !CheckLastMessage(); i++)
                {
                 if (CreateProcess(NazwaProgramuOdtwarzacza, RUNLista->Items[i]->text,NULL,NULL,false,NORMAL_PRIORITY_CLASS,NULL,strExtractFilePath(temp, NazwaProgramuOdtwarzacza),lpStartupInformation,lpProcessInformation))
                    {
                     WaitForInputIdle(lpProcessInformation->hProcess,Timeout);
                     for (DWORD dwExitStatus = STILL_ACTIVE; dwExitStatus==STILL_ACTIVE; Sleep(Interval))
                         {
                          if (!GetVolumeInformation(Naped,NULL,0,&VolumeSerialNumber2,NULL,NULL,NULL,0))
                             {VolumeSerialNumber2 = 0; TerminateProcess(lpProcessInformation->hProcess, 0);} // wywoluje funkcje zamykania odtwarzacza
                          GetExitCodeProcess(lpProcessInformation->hProcess, &dwExitStatus);
                         }
                     CloseHandle(lpProcessInformation->hProcess);
                     CloseHandle(lpProcessInformation->hThread);

                     for (int r = 0; (VolumeSerialNumber1 != VolumeSerialNumber2); Sleep(Interval),r+=Interval)
                         {
                          if (GetVolumeInformation(Naped,NULL,0,&VolumeSerialNumber2,NULL,NULL,NULL,0) || r >= Timeout)
                             {
                              if ((VolumeSerialNumber1 != VolumeSerialNumber2) || r >= Timeout) i = -1; // inna plyta lub minê³o wiêcej ni¿ timeout, wiêc wychodzê byu urucjhomiæ jeszcze raz
                              else break;
                             }
                         }
                    }
                }
            // koniec ifa uruchamiania
           }
         // koniec ifa dysku information
        }
     else VolumeSerialNumber1 = 0;
}
delete lpProcessInformation;
delete lpStartupInformation;
//
free((char*)CurrentDirectory);
free((char*)Bufor);
free((char*)Naped);
//
// zamykam program
//
if (hInstanceMutex)
    CloseHandle(hInstanceMutex);
return 0;
}
//---------------------------------------------------------------------------
