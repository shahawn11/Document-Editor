#ifndef ECTextDocumentCtrl_h
#define ECTextDocumentCtrl_h

#include "ECCommand.h"
#include "ECObserver.h"
#include "ECTextViewImp.h"
#include "ECEditorView.h"
#include <vector>

class ECTextDocument;
class ECKeyEventHandler;
class ctrlKeyHandler;
class CharKeyHandler;
class EnterKeyHandler;
class ArrowKeyHandler;

enum MODE{
    TEXT = 0, /* Text */
    FIND = 1, /* Find */
    REPLACE = 2 /* Replace */
};

class InsertCharAt_cmd : public ECCommand
{
public:
    InsertCharAt_cmd(ECTextDocument &doc, int x, int y, char &charIn);
    void Execute();
    void UnExecute();
private:
    ECTextDocument &docu;
    int _x,_y;
    char input;
};

class RemoveCharAt_cmd : public ECCommand
{
public:
    RemoveCharAt_cmd(ECTextDocument &doc, int x, int y);
    void Execute();
    void UnExecute();
private:
    ECTextDocument &docu;
    int _x,_y;
    char lastChar;
    int rowLen=0;
};

class Enter_cmd : public ECCommand
{
public:
    Enter_cmd(ECTextDocument &doc, int x, int y);
    void Execute();
    void UnExecute();
private:
    ECTextDocument &docu;
    int _x,_y;
    int flag = 0;
};

class Replace_cmd : public ECCommand
{
public:
    Replace_cmd(ECTextDocument &doc, int x, int y, std::string &in, std::string &out);
    void Execute();
    void UnExecute();
private:
    ECTextDocument &docu;
    std::string stringIn;
    std::string stringOut;
    int page;
    int pageLen;
    std::vector<int> rows;
    std::vector<int> cols;
};

// **********************************************************
// Controller for text document

class ECTextDocumentCtrl : public ECObserver
{
public:
    ECTextDocumentCtrl(ECTextDocument &docIn, ECTextViewImp &ViewImpIn, ECEditorView &viewIn);          // conroller constructor takes the document as input
    virtual ~ECTextDocumentCtrl();
    void InsertChar(char &charIn);    // insert a list of characters starting at position
    void RemoveChar();                            // remove a segment of characters  of lenToRemove starting from pos                              // Lowercase the text of lenToLoer starting from pos
    bool Undo();                                                            // undo any change you did to the text
    bool Redo();                                                            // redo the change to the text
    void Update();
    void Quit();
    void Enter();
    void Left();
    void Right();
    void Up();
    void Down();
    int getX();
    int getY();
    void DisplayRows();
    void SetHandler(ECKeyEventHandler *handlerIn);
    void CheckPage();
    int GetPage();
    void SetPage(int pageIn);
    void SetMode(MODE modeIn);
    void SearchInsert(char &charIn);
    void SearchRemove();
    void ReplaceInsert(char &charIn);
    void ReplaceRemove();
    void Find();
    void Replace();

private:
    ECTextDocument &doc;
    ECCommandHistory histCmds;
    ECTextViewImp &ViewImp;
    ECEditorView &view;
    ECKeyEventHandler *BackspaceKey;
    int x = 0;
    int y = 0;
    int pageLen;
    int page = 0;
    std::string search_string;
    std::string replace_string;

};

// **********************************************************
// Document for text document

class ECTextDocument
{
public:
    ECTextDocument(ECTextViewImp &ViewImpIn, ECEditorView &viewIn);
    virtual ~ECTextDocument();
    ECTextDocumentCtrl &GetCtrl();          // return document controller
    int GetDocLen() const;
    int GetRowLen(int row) const;
    char GetCharAt(int x, int y) const;          // get char at current position
    void InsertCharAt(int x, int y, char ch);    // insert a single char at position
    void RemoveCharAt(int x, int y);             // erase a single char at position
    void AddRow(int y, const std::string &strRow);
    void ConcatRow(int y, const std::string &strIn);
    void DeleteRow(int y);
    void Replace(int x, int y, std::string &strIn, std::string &strOut);
    std::string substring(int x, int y, int len);
    std::string GetRow(int y);
private:
    ECTextDocumentCtrl docCtrl;
    std::vector<std::string> document;
};

#endif /* ECTextDocumentCtrl_h */
