
// EditorFotos.h: archivo de encabezado principal para la aplicaci�n PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"		// S�mbolos principales


// CEditorFotosApp:
// Consulte la secci�n EditorFotos.cpp para obtener informaci�n sobre la implementaci�n de esta clase
//

class CEditorFotosApp : public CWinApp
{
public:
	CEditorFotosApp();

// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementaci�n

	DECLARE_MESSAGE_MAP()
};

extern CEditorFotosApp theApp;