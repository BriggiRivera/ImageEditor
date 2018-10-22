
// EditorFotos.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"		// Símbolos principales


// CEditorFotosApp:
// Consulte la sección EditorFotos.cpp para obtener información sobre la implementación de esta clase
//

class CEditorFotosApp : public CWinApp
{
public:
	CEditorFotosApp();

// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CEditorFotosApp theApp;