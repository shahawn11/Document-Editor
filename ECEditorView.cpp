#include "ECTextDocumentCtrl.h"
#include "ECTextViewImp.h"

ECEditorView :: ECEditorView(ECTextViewImp &viewIn) : viewImp(viewIn) {}
void ECEditorView :: Show() { viewImp.Show(); }
void ECEditorView :: Refresh() {viewImp.Refresh(); }
int ECEditorView :: GetPressedKey() { return viewImp.GetPressedKey(); }
void ECEditorView :: InitRows() { return viewImp.InitRows(); }
void ECEditorView :: AddRow(const std::string &strRow) { viewImp.AddRow(strRow); }
int ECEditorView :: GetCursorX() const { return viewImp.GetCursorX(); }
int ECEditorView :: GetCursorY() const { return viewImp.GetCursorY(); }
void ECEditorView :: SetCursorX(int pos) { viewImp.SetCursorX(pos); }
void ECEditorView :: SetCursorY(int pos) { viewImp.SetCursorY(pos); }
void ECEditorView :: AddStatusRow( const std::string &statusMsgLeft, const std::string &statusMsgRight, bool fBlackBackground ) 
{
     viewImp.AddStatusRow(statusMsgLeft, statusMsgRight, fBlackBackground); 
}
void ECEditorView :: ClearStatusRow()
{
    viewImp.ClearStatusRows();
}
void ECEditorView :: SetColor(int row, int colBegin, int colEnd, TEXT_COLOR clr) 
{
    viewImp.SetColor(row, colBegin, colEnd, clr); 
}
int ECEditorView :: GetRowNumInView() const { return viewImp.GetRowNumInView(); }