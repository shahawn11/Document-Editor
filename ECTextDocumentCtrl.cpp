#include "ECTextDocumentCtrl.h"
#include "ECKeyEventHandler.h"
#include <iostream>
#include <cctype>
#include <math.h>

using namespace std;

// **********************************************************
// Commands

// your code goes here 

static const char * EnumStrings[] = { "Text", "Find:", "Replace:" };
const char * getTextForEnum( int enumVal )
{
  return EnumStrings[enumVal];
}

InsertCharAt_cmd :: InsertCharAt_cmd(ECTextDocument &doc, int x, int y, char &charIn) : docu(doc)
{
  _x = x;
  _y = y;
  input = charIn;
}
void InsertCharAt_cmd :: Execute()
{
  docu.InsertCharAt(_x,_y, input);
}
void InsertCharAt_cmd :: UnExecute()
{
  docu.RemoveCharAt(_x,_y);
}

RemoveCharAt_cmd :: RemoveCharAt_cmd(ECTextDocument &doc, int x, int y) : docu(doc)
{
  _x = x;
  _y = y;
}
void RemoveCharAt_cmd :: Execute()
{
  if (_x > 0)
  {
    lastChar = docu.GetCharAt(_x-1,_y);
    docu.RemoveCharAt(_x-1,_y);
  }
  else if(_y > 0)
  {
    rowLen = docu.GetRowLen(_y-1);
    docu.ConcatRow(_y-1, docu.GetRow(_y));
    if(_y<=docu.GetDocLen()) docu.DeleteRow(_y);
  }
}
void RemoveCharAt_cmd :: UnExecute()
{
  if(_x > 0) docu.InsertCharAt(_x-1, _y, lastChar);
  else if(_y > 0)
  {
    int len = docu.GetRowLen(_y-1);
    string s = docu.substring(rowLen, _y-1, len - rowLen);
    for(int i = 0; i < len - rowLen; ++i)
    {
      docu.RemoveCharAt(rowLen, _y-1);
    } 
    docu.AddRow(_y,s);
  }
}

Enter_cmd :: Enter_cmd(ECTextDocument &doc, int x, int y) : docu(doc)
{
  _x = x;
  _y = y;
}
void Enter_cmd :: Execute()
{
  if(docu.GetDocLen() == 0) docu.AddRow(_y,"");
  else if(_y >= docu.GetDocLen()) {docu.AddRow(_y,""); flag = 1;}
  string s;
  if(_x < docu.GetRowLen(_y)) 
  {
    int len = docu.GetRowLen(_y);
    s = docu.substring(_x, _y, len - _x);
    for(int i = 0; i < len - _x; ++i)
    {
      docu.RemoveCharAt(_x, _y);
    } 
  }
  docu.AddRow(_y+1, s);
}
void Enter_cmd :: UnExecute()
{
  docu.ConcatRow(_y, docu.GetRow(_y+1));
  docu.DeleteRow(_y+1);
  if(flag ==1) docu.DeleteRow(_y+1);
}

Replace_cmd :: Replace_cmd(ECTextDocument &doc, int pageIn, int pageLenIn, std::string &in, std::string &out) : docu(doc), stringIn(in), stringOut(out)
{
  page = pageIn;
  pageLen = pageLenIn;
}
void Replace_cmd :: Execute()
{
  rows.clear();
  cols.clear();
  for(int i = (page * pageLen); i < min(docu.GetDocLen(),pageLen*(page+1)); ++i)
  {
    string::size_type start = 0;
    while ((start = docu.GetRow(i).find(stringIn, start)) != string::npos) {
        rows.push_back(i);
        cols.push_back(start);
        docu.Replace(start, i, stringIn, stringOut);
        if(!stringOut.empty()) start += max(stringIn.size(), stringOut.size());
    }
  }
}
void Replace_cmd :: UnExecute()
{
  for(int i = rows.size()-1; i >=0; --i)
  {
    docu.Replace(cols[i], rows[i], stringOut, stringIn);
  }
}
// **********************************************************
// Controller for text document

ECTextDocumentCtrl :: ECTextDocumentCtrl(ECTextDocument &docIn, ECTextViewImp &ViewImpIn, ECEditorView &viewIn) : doc(docIn), ViewImp(ViewImpIn), view(viewIn), BackspaceKey(nullptr)
{
  pageLen = view.GetRowNumInView()-1;
}

ECTextDocumentCtrl :: ~ECTextDocumentCtrl()
{
}

void ECTextDocumentCtrl :: SetHandler(ECKeyEventHandler* handlerIn)
{
  BackspaceKey = handlerIn;
}

void ECTextDocumentCtrl :: InsertChar(char &charIn)
{
  // your code
  InsertCharAt_cmd* insert = new InsertCharAt_cmd(doc, x,  y, charIn);
  histCmds.ExecuteCmd(insert);
  x++;
  view.SetCursorX(x);

  DisplayRows();
}

void ECTextDocumentCtrl :: RemoveChar()
{
  // your code
  if (y <= doc.GetDocLen())
  {
    RemoveCharAt_cmd* remove = new RemoveCharAt_cmd(doc, x, y);
    histCmds.ExecuteCmd(remove);
    if (x > 0) x--;
    else if(y > 0)
    {
      y--;
      while (y>doc.GetDocLen()) Up();
      x = doc.GetRowLen(y);
    }
    view.SetCursorX(x);
    view.SetCursorY(y-(pageLen*page));
    CheckPage(); 
  }
}

bool ECTextDocumentCtrl :: Undo()
{
  // your code
  histCmds.Undo();
  DisplayRows();
  ViewImp.ClearColor();
  
  while (y>=doc.GetDocLen()) Up();
  while (x>doc.GetRowLen(y)) Left();

  return true;
}

bool ECTextDocumentCtrl :: Redo()
{
  // your code
  histCmds.Redo();
  DisplayRows();

  while (y>doc.GetDocLen()) Up();
  while (x>doc.GetRowLen(y)) Left();

  return true;
}

void ECTextDocumentCtrl :: Update()
{
  int key = ViewImp.GetPressedKey();
  MODE mode = BackspaceKey->GetMode();
  BackspaceKey->Handle(key, mode);
}

void ECTextDocumentCtrl :: Quit()
{
  ViewImp.Quit();
}

void ECTextDocumentCtrl :: Enter()
{
  Enter_cmd* enter = new Enter_cmd(doc, x, y);
  histCmds.ExecuteCmd(enter);
  y++;
  x = 0;
  view.SetCursorX(x);
  view.SetCursorY(y-(pageLen*page));
  CheckPage();
}

void ECTextDocumentCtrl :: Left() 
{
  if(x > 0)
  {
    x--;
  }
  else if(y > 0)
  {
    y--;
    x = doc.GetRowLen(y);
  }
  view.SetCursorX(x);
  view.SetCursorY(y-(pageLen*page));
  CheckPage();
}
void ECTextDocumentCtrl :: Right() 
{
  if(x < doc.GetRowLen(y))
  {
    x++;
  }
  else if(y < doc.GetDocLen()-1)
  {
    y++;
    x = 0;
  }
  view.SetCursorX(x);
  view.SetCursorY(y-(pageLen*page));
  CheckPage();
}
void ECTextDocumentCtrl :: Up() 
{
  if(y > 0)
  {
    if (x > doc.GetRowLen(y-1)) x = doc.GetRowLen(y-1);
    y--;
    view.SetCursorX(x);
    view.SetCursorY(y-(pageLen*page));
  }
  CheckPage();
}
void ECTextDocumentCtrl :: Down() 
{
  if(y < doc.GetDocLen()-1)
  {
    if (x > doc.GetRowLen(y+1)) x = doc.GetRowLen(y+1);
    y++;
    view.SetCursorX(x);
    view.SetCursorY(y-(pageLen*page));
  }
  CheckPage();
}

int ECTextDocumentCtrl :: getX()
{
  return x;
}
int ECTextDocumentCtrl :: getY()
{
  return y;
}

void  ECTextDocumentCtrl :: CheckPage()
{
  if (y > ((page+1) * pageLen)-1) {page++; y = page*pageLen; view.SetCursorY(y-(pageLen*page));}
  else if (y < page * pageLen) {page--; y = ((page+1)*pageLen-1); view.SetCursorY(y-(pageLen*page));}
  DisplayRows();
}
int ECTextDocumentCtrl :: GetPage()
{
  return page;
}

void ECTextDocumentCtrl :: SetPage(int pageIn)
{
  page = pageIn;
}

void ECTextDocumentCtrl :: DisplayRows()
{
  view.InitRows();

  for(int i = (page * pageLen); i < min(doc.GetDocLen(),pageLen*(page+1)); ++i)
  {
    view.AddRow(doc.GetRow(i));
  }
  view.Refresh();
}

void ECTextDocumentCtrl :: SetMode(MODE modeIn) 
{ 
  BackspaceKey->SetMode(modeIn); 
  view.ClearStatusRow();
  if (modeIn == FIND) view.AddStatusRow(getTextForEnum(modeIn) + search_string, "", true);
  else if (modeIn == REPLACE) view.AddStatusRow(getTextForEnum(modeIn) + search_string + ":" + replace_string, "", true);
  else {view.AddStatusRow("Text", "", true); ViewImp.ClearColor();}
  view.Refresh();
}

void ECTextDocumentCtrl :: SearchInsert(char &charIn)
{
  search_string.insert(search_string.size(), 1, charIn);
  view.ClearStatusRow();
  view.AddStatusRow("Find:" + search_string, "", true);
  view.Refresh();
}

void ECTextDocumentCtrl :: SearchRemove()
{
  if (search_string.length() > 0) search_string.pop_back();
  view.ClearStatusRow();
  view.AddStatusRow("Find:" + search_string, "", true);
  view.Refresh();
}

void ECTextDocumentCtrl :: ReplaceInsert(char &charIn)
{
  replace_string.insert(replace_string.size(), 1, charIn);
  view.ClearStatusRow();
  view.AddStatusRow("Replace:" + search_string + ":" + replace_string, "", true);
  view.Refresh();
}

void ECTextDocumentCtrl :: ReplaceRemove()
{
  if (replace_string.length() > 0) replace_string.pop_back();
  view.ClearStatusRow();
  view.AddStatusRow("Replace:" + search_string + ":" + replace_string, "", true);
  view.Refresh();
}

void ECTextDocumentCtrl :: Find()
{
  if(!search_string.empty()){
    ViewImp.ClearColor();
    for(int i = (page * pageLen); i < min(doc.GetDocLen(),pageLen*(page+1)); ++i)
    {
      string::size_type start = 0;
      while ((start = doc.GetRow(i).find(search_string, start)) != string::npos) {
          view.SetColor(i-(pageLen*page),start,start+search_string.length()-1,TEXT_COLOR_RED);
          start += search_string.length();
      }
    }
    view.Refresh();
  }
}

void ECTextDocumentCtrl :: Replace()
{
  if(!search_string.empty()){
    Replace_cmd* replace = new Replace_cmd(doc, page, pageLen, search_string, replace_string);
    histCmds.ExecuteCmd(replace);
    DisplayRows();
    search_string = replace_string;
    SetMode(FIND);
    ViewImp.ClearColor();
    while (y>doc.GetDocLen()) Up();
    while (x>doc.GetRowLen(y)) Left();
    Find();
  }
}

// **********************************************************
// Document for text document


ECTextDocument :: ECTextDocument(ECTextViewImp &ViewImpIn, ECEditorView &viewIn) : docCtrl(*this, ViewImpIn, viewIn)
{
}

ECTextDocument :: ~ECTextDocument()
{
}

ECTextDocumentCtrl & ECTextDocument :: GetCtrl()
{
  return docCtrl;
}

int ECTextDocument :: GetDocLen() const { return document.size(); }

int ECTextDocument :: GetRowLen(int row) const { return document[row].size();}

char ECTextDocument :: GetCharAt(int x, int y) const
{
  return document[y][x];
}

void ECTextDocument :: InsertCharAt(int x, int y, char ch)
{
  // your code here
  if (y >= document.size()) AddRow(y, "");
  document[y].insert(x, 1, ch);
}

void ECTextDocument :: RemoveCharAt(int x, int y)
{
  // your code here
  document[y].erase(document[y].begin()+x);
}

void ECTextDocument :: AddRow(int y, const std::string &strRow)
{
  document.insert(document.begin()+y, strRow);
}

void ECTextDocument :: ConcatRow(int y, const std::string &strIn)
{
  document[y] = document[y] + strIn;
}
void ECTextDocument :: DeleteRow(int y)
{
  document.erase(document.begin()+y);
}

void ECTextDocument :: Replace(int x, int y, std::string &strIn, std::string &strOut)
{
  document[y].erase(x, strIn.length());
  document[y].insert(x, strOut);
}

string ECTextDocument :: substring(int x, int y, int len)
{
  return document[y].substr(x,len);
}

string ECTextDocument :: GetRow(int y)
{
  return document[y];
}
