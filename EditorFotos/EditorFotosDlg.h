
// EditorFotosDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
#include "afxmenubutton.h"


// Cuadro de diálogo de CEditorFotosDlg
class CEditorFotosDlg : public CDialogEx
{
// Construcción
public:
	CEditorFotosDlg(CWnd* pParent = NULL);	// Constructor estándar

// Datos del cuadro de diálogo
	enum { IDD = IDD_EDITORFOTOS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CScrollBar m_HScrollBar;
	CScrollBar m_VScrollBar;
	SCROLLINFO m_horz, m_vert;

	int m_iSrcX;
	int m_iSrcY;

public:
	afx_msg void OnBnClickedOk();
	CStatic groupArea;
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnSize(UINT nType, int cx, int cy);
	CString rutaArchivo;
	CMFCMenuButton MenuGeneral;
	afx_msg void OnBnClickedMenu();
//	afx_msg void OnOpciones();
	afx_msg void OnIdrMenu1();
	afx_msg void OnOpcionesWavelet();
	CStatic groupArea2;
	int escalas_wavelet;
	afx_msg void OnGlobalesFiltromediana();
	afx_msg void OnGlobalEcualizaci32775();
};
