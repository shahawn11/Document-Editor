editor: ECTextViewImp.o ECEditorView.o ECCommand.o ECKeyEventHandler.o ECTextDocumentCtrl.o ECEditorTest.o
	g++ ECTextViewImp.o ECEditorView.o ECCommand.o ECKeyEventHandler.o ECTextDocumentCtrl.o ECEditorTest.o -o editor

ECTextViewImp.o: ECTextViewImp.cpp
	g++ -c ECTextViewImp.cpp

ECEditorView.o: ECEditorView.cpp
	g++ -c ECEditorView.cpp

ECCommand.o: ECCommand.cpp
	g++ -c ECCommand.cpp

ECKeyEventHandler.o: ECKeyEventHandler.cpp
	g++ -c ECKeyEventHandler.cpp

ECTextDocumentCtrl.o: ECTextDocumentCtrl.cpp
	g++ -c ECTextDocumentCtrl.cpp

ECEditorTest.o: ECEditorTest.cpp
	g++ -c ECEditorTest.cpp

clean: 
	rm *.o editor