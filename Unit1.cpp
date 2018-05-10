//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "./../../FORM_TEMPLATES/About_frm.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"

TForm1 *Form1;
//----------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
Semaphore = NULL;
}
//----------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
if (Semaphore!=NULL)
   {
    CloseHandle(Semaphore); Semaphore = NULL;
   }
// START CZYLI NIE ROB SEMAFORA
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
if (Semaphore!=NULL)
   {
    CloseHandle(Semaphore); Semaphore = NULL;
   }
Semaphore = CreateSemaphore(NULL, 1, 1, "TCloserPro_pauza");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
if (Semaphore!=NULL)
   {
    CloseHandle(Semaphore); Semaphore = NULL;
   }
Semaphore = CreateSemaphore(NULL, 1, 1, "TCloserPro_stop");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{
Application->Restore(); Application->Minimize();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
BitBtn1Click(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton4Click(TObject *Sender)
{
AboutForm->ShowModal();    
}
//---------------------------------------------------------------------------

