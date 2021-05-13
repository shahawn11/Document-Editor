#ifndef ECKeyEventHandler_h
#define ECKeyEventHandler_h

#include "ECTextViewImp.h"
#include "ECTextDocumentCtrl.h"

class ECTextDocumentCtrl;
class ECKeyEventHandler;

class ECKeyEventHandler
{
public:
    ECKeyEventHandler(ECKeyEventHandler *nextIn);
    virtual bool canHandle(const int &key);
    virtual bool Handle(const int &key, MODE &mode);
    virtual ECKeyEventHandler* GetNext();
    virtual ECTextDocumentCtrl* GetCtrl();
    virtual void SetCtrl(ECTextDocumentCtrl* in);
    virtual MODE GetMode();
    virtual void SetMode(MODE &modeIn);
private:
    ECKeyEventHandler* next;
    ECTextDocumentCtrl* docCtrl;
    MODE mode = TEXT;
};

class ctrlKeyHandler : public ECKeyEventHandler
{
public:
    ctrlKeyHandler(ECKeyEventHandler *nextIn);
    virtual bool canHandle(const int &key);
    virtual bool Handle(const int &key, MODE &mode);
};

class CharKeyHandler : public ECKeyEventHandler
{
public:
    CharKeyHandler(ECKeyEventHandler *nextIn);
    virtual bool canHandle(const int &key);
    virtual bool Handle(const int &key, MODE &mode);
};

class EnterKeyHandler : public ECKeyEventHandler
{
public:
    EnterKeyHandler(ECKeyEventHandler *nextIn);
    virtual bool canHandle(const int &key);
    virtual bool Handle(const int &key, MODE &mode);
};

class ArrowKeyHandler : public ECKeyEventHandler
{
public:
    ArrowKeyHandler(ECKeyEventHandler *nextIn);
    virtual bool canHandle(const int &key);
    virtual bool Handle(const int &key, MODE &mode);
};

class EscapeKeyHandler : public ECKeyEventHandler
{
public:
    EscapeKeyHandler(ECKeyEventHandler *nextIn);
    virtual bool canHandle(const int &key);
    virtual bool Handle(const int &key, MODE &mode);
};

class BackspaceKeyHandler : public ECKeyEventHandler
{
public:
    BackspaceKeyHandler(ECKeyEventHandler *nextIn);
    virtual bool canHandle(const int &key);
    virtual bool Handle(const int &key, MODE &mode);
};


#endif /* ECKeyEventHandler_h */
