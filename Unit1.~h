//----------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//----------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:
    TPanel *Panel1;
    TSpeedButton *SpeedButton2;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton9;
    TSpeedButton *SpeedButton4;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
    void __fastcall SpeedButton4Click(TObject *Sender);
private:
public:
        HANDLE Semaphore;
	virtual __fastcall TForm1(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TForm1 *Form1;
//----------------------------------------------------------------------------
#endif
