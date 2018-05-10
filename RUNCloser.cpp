//---------------------------------------------------------------------------
#include <vcl.h>
//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
USERES("RUNCloser.res");
USEFORM("Unit1.cpp", Form1);
USEFORM("..\..\FORM_TEMPLATES\About_frm.cpp", AboutForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR CommandLine, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "RUNCLoser";
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(TAboutForm), &AboutForm);
         Application->Run();
        }
        catch (...)
        {
        }
        return 0;
}
//---------------------------------------------------------------------------
