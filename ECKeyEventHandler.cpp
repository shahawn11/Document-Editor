#include "ECKeyEventHandler.h"
#include "ECTextDocumentCtrl.h"
#include "ECTextViewImp.h"

ECKeyEventHandler :: ECKeyEventHandler(ECKeyEventHandler *nextIn) : next(nextIn), docCtrl(nullptr) {}
bool ECKeyEventHandler :: canHandle(const int &key) {return next->canHandle(key);}
bool ECKeyEventHandler :: Handle(const int &key, MODE &mode) {return next->Handle(key, mode);}
ECKeyEventHandler* ECKeyEventHandler :: GetNext() {return next;}
ECTextDocumentCtrl* ECKeyEventHandler :: GetCtrl() {return docCtrl;}
void ECKeyEventHandler :: SetCtrl(ECTextDocumentCtrl* in) { docCtrl = in;}
MODE ECKeyEventHandler :: GetMode() { return mode; }
void ECKeyEventHandler :: SetMode(MODE &modeIn) { mode = modeIn; }

ctrlKeyHandler :: ctrlKeyHandler(ECKeyEventHandler *nextIn) : ECKeyEventHandler(nextIn) {}
bool ctrlKeyHandler :: canHandle(const int &key) 
{
    if (key >= 1 && key <= 8 || key >= 17 && key <= 26) return true;
    else return false;
}
bool ctrlKeyHandler :: Handle(const int &key, MODE &mode) 
{
    if (this->canHandle(key) == false) {GetNext()->Handle(key, mode); return false;}
    switch (key)
    {
    case CTRL_Q:
        GetCtrl()->Quit();
        break;
    case CTRL_Z:
        GetCtrl()->Undo();
        break;
    case CTRL_Y:
        GetCtrl()->Redo();
        break;
    case CTRL_F:
        GetCtrl()->SetMode(FIND);
        break;
    default:
        break;
    }
    return true;
}

CharKeyHandler :: CharKeyHandler(ECKeyEventHandler *nextIn) : ECKeyEventHandler(nextIn) {}
bool CharKeyHandler :: canHandle(const int &key) {return true;}
bool CharKeyHandler :: Handle(const int &key, MODE &mode) 
{
    if (this->canHandle(key) == false) {GetNext()->Handle(key, mode); return false;}
    char keyChar = char(key);
    if (mode == FIND)
    {
        if (keyChar == '/') GetCtrl()->SetMode(REPLACE);
        else GetCtrl()->SearchInsert(keyChar);
    }
    else if (mode == REPLACE) GetCtrl()->ReplaceInsert(keyChar);
    else GetCtrl()->InsertChar(keyChar);
    return true;
}

EnterKeyHandler :: EnterKeyHandler(ECKeyEventHandler *nextIn) : ECKeyEventHandler(nextIn) {}
bool EnterKeyHandler :: canHandle(const int &key) 
{
    if(key == 13) return true;
    else return false;
}
bool EnterKeyHandler :: Handle(const int &key, MODE &mode)
{
    if (this->canHandle(key) == false) {GetNext()->Handle(key, mode); return false;}
    if (mode == TEXT) GetCtrl()->Enter();
    else if(mode == FIND) GetCtrl()->Find();
    else GetCtrl()->Replace();
    return true;
}


ArrowKeyHandler :: ArrowKeyHandler(ECKeyEventHandler *nextIn) : ECKeyEventHandler(nextIn) {}
bool ArrowKeyHandler :: canHandle(const int &key) 
{
    if(key >= 1000 && key <= 1003) return true;
    else return false;
}
bool ArrowKeyHandler :: Handle(const int &key, MODE &mode)
{
    if (this->canHandle(key) == false) {GetNext()->Handle(key, mode); return false;}
    switch (key)
    {
    case ARROW_LEFT:
        GetCtrl()->Left();
        break;
    case ARROW_RIGHT:
        GetCtrl()->Right();
        break;
    case ARROW_UP:
        GetCtrl()->Up();
        break;
    case ARROW_DOWN:
        GetCtrl()->Down();
        break;
    default:
        break;
    }
    return true;
}

EscapeKeyHandler :: EscapeKeyHandler(ECKeyEventHandler *nextIn) : ECKeyEventHandler(nextIn) {}
bool EscapeKeyHandler :: canHandle(const int &key)
{
    if(key == ESC) return true;
    else return false;
}
bool EscapeKeyHandler :: Handle(const int &key, MODE &mode)
{
    if (this->canHandle(key) == false) {GetNext()->Handle(key, mode); return false;}
    if(mode == REPLACE) GetCtrl()->SetMode(FIND);
    else if(mode == FIND) GetCtrl()->SetMode(TEXT);
    return true;
}

BackspaceKeyHandler :: BackspaceKeyHandler(ECKeyEventHandler *nextIn) : ECKeyEventHandler(nextIn) {}
bool BackspaceKeyHandler :: canHandle(const int &key) 
{
    if(key == 127) return true;
    else return false;
}
bool BackspaceKeyHandler :: Handle(const int &key, MODE &mode)
{
    if (this->canHandle(key) == false) {GetNext()->Handle(key, mode); return false;}
    if (mode == TEXT) GetCtrl()->RemoveChar();
    else if (mode == FIND) GetCtrl()->SearchRemove();
    else GetCtrl()->ReplaceRemove();
    return true;
}
