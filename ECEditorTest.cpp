// Test code for editor
#include "ECTextViewImp.h"
#include "ECCommand.h"
#include "ECObserver.h"
#include "ECEditorView.h"
#include "ECTextDocumentCtrl.h"
#include "ECKeyEventHandler.h"
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace  std;

int myCounter = 0;

int main(int argc, char *argv[])
{
    //
    ECTextViewImp ViewImp;
    ECEditorView wndTest = ECEditorView(ViewImp);
    ECTextDocument doc(ViewImp, wndTest);
    ECTextDocumentCtrl &docCtrl = doc.GetCtrl();
    ECTextDocumentCtrl *Ctrl_pointer = &docCtrl;
    ViewImp.Attach(Ctrl_pointer);

    ECKeyEventHandler h1 = ECKeyEventHandler(NULL);
    ECKeyEventHandler* KeyHandler = &h1;
    KeyHandler->SetCtrl(Ctrl_pointer);
    
    CharKeyHandler h2 = CharKeyHandler(KeyHandler);
    CharKeyHandler* CharKey = &h2;
    CharKey->SetCtrl(Ctrl_pointer);
    
    ctrlKeyHandler h3 = ctrlKeyHandler(CharKey);
    ctrlKeyHandler* ctrlKey = &h3;
    ctrlKey->SetCtrl(Ctrl_pointer);
    
    EnterKeyHandler h4 = EnterKeyHandler(ctrlKey);
    EnterKeyHandler* EnterKey = &h4;
    EnterKey->SetCtrl(Ctrl_pointer);
    
    ArrowKeyHandler h5 = ArrowKeyHandler(EnterKey);
    ArrowKeyHandler* ArrowKey = &h5;
    ArrowKey->SetCtrl(Ctrl_pointer);
    
    EscapeKeyHandler h6 = EscapeKeyHandler(ArrowKey);
    EscapeKeyHandler* EscapeKey = &h6;
    EscapeKey->SetCtrl(Ctrl_pointer);

    BackspaceKeyHandler h7 = BackspaceKeyHandler(EscapeKey);
    BackspaceKeyHandler* BackspaceKey = &h7;
    BackspaceKey->SetCtrl(Ctrl_pointer);
    Ctrl_pointer->SetHandler(BackspaceKey);
    
    // add a status bar
    wndTest.AddStatusRow("Text", "", true);

    char* filename = argv[1];
    fstream file;
    file.open(filename,ios::in); //open a file to perform read operation using file object
    if (file){   //checking whether the file exists
      string row;
      int y=0;
      while(getline(file, row))
      {
          wndTest.AddRow(row);
          doc.AddRow(y, row);
          y++;
      }
    }
    wndTest.SetCursorX(0);
    wndTest.SetCursorY(0);

    file.close();

    docCtrl.DisplayRows();

    wndTest.Show();

    file.open(filename, ios::out | ios::trunc);
    for(int i=0;i<doc.GetDocLen();++i)
    {
        file << doc.GetRow(i) << endl;
    }

    file.close();
    return 0;
}
