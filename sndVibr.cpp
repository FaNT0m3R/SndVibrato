// sndVibr.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "sndVibr.h"

#include "MainFrm.h"
#include "sndVibrDoc.h"
#include "sndVibrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSndVibrApp

								   //карта сообщений приложения. У нас он обрабатывает только открытие файла
BEGIN_MESSAGE_MAP(CSndVibrApp, CWinApp)
	//{{AFX_MSG_MAP(CSndVibrApp)

		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSndVibrApp construction
CSndVibrApp::CSndVibrApp()
{}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Создаём класс программы. Единственная глобальная переменная, но без неё никак
CSndVibrApp theApp;



/////////////////////////////////////////////////////////////////////////////
// Стандартная инициализация
BOOL CSndVibrApp::InitInstance()
{
	Enable3dControls();			// Call this when using MFC in a shared DLL
	
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;				  
	pDocTemplate = new CSingleDocTemplate(		   //создаём документ, вид для него, и главный фрейм
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSndVibrDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSndVibrView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	//показываем главное окно программы	
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}
