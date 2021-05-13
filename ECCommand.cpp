//
//  ECCommand.cpp
//  
//
//  Created by Yufeng Wu on 2/26/20.
//
//

#include "ECCommand.h"

// ******************************************************
// Implement command history

ECCommandHistory :: ECCommandHistory() 
{
  // your code goes here
  vector<ECCommand*> undoStack;
  vector<ECCommand*> redoStack;
}

ECCommandHistory :: ~ECCommandHistory()
{
  // your code goes here
  undoStack.clear();
  redoStack.clear();
}
bool ECCommandHistory :: Undo()
{
  if(undoStack.size() > 0)
  {
    undoStack.back()->UnExecute();
    redoStack.push_back(undoStack.back());
    undoStack.pop_back();
    return true;
  }
  else return false;
}
bool ECCommandHistory :: Redo()
{
  if(redoStack.size() > 0)
  {
    redoStack.back()->Execute();
    undoStack.push_back(redoStack.back());
    redoStack.pop_back();
    return true;
  }
  else return false;
}
void ECCommandHistory :: ExecuteCmd( ECCommand *pCmd )
{
  redoStack.clear();
  pCmd->Execute();
  undoStack.push_back(pCmd);
}
