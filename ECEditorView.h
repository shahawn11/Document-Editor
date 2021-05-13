#include "ECTextDocumentCtrl.h"
#include "ECTextViewImp.h"

#ifndef ECEditorView_h
#define ECEditorView_h

class ECEditorView
{
public:
    ECEditorView(ECTextViewImp &viewIn);
    void Show();
    void Refresh();
    int GetPressedKey();
    void InitRows();
    void AddRow(const std::string &strRow);
    int GetCursorX() const;
    int GetCursorY() const;
    void SetCursorX(int pos);
    void SetCursorY(int pos);
    void AddStatusRow( const std::string &statusMsgLeft, const std::string &statusMsgRight, bool fBlackBackground );
    void ClearStatusRow();
    void SetColor(int row, int colBegin, int colEnd, TEXT_COLOR clr);
    int GetRowNumInView() const;

private:
    ECTextViewImp &viewImp;
};


#endif /* ECEditorView_h */