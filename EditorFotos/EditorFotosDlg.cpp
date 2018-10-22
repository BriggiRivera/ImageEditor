
// EditorFotosDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "EditorFotos.h"
#include "EditorFotosDlg.h"
#include "afxdialogex.h"
#include "BMP.h"
#define cimg_use_jpeg
#include "CImg.h"
#include "PixelRGB.h"
#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace cimg_library;

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cuadro de diálogo de CEditorFotosDlg



CEditorFotosDlg::CEditorFotosDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditorFotosDlg::IDD, pParent)
	, rutaArchivo(_T(""))
	, escalas_wavelet(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorFotosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, groupID, groupArea);
	DDX_Text(pDX, IDC_EDIT1, rutaArchivo);
	DDX_Control(pDX, groupID2, groupArea2);
	DDX_Text(pDX, CAMPO1, escalas_wavelet);
	DDV_MinMaxInt(pDX, escalas_wavelet, 1, 64);
}

BEGIN_MESSAGE_MAP(CEditorFotosDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CEditorFotosDlg::OnBnClickedOk)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
//	ON_COMMAND(IDR_MENU1, &CEditorFotosDlg::OnOpciones)
ON_COMMAND(IDR_MENU1, &CEditorFotosDlg::OnIdrMenu1)
ON_COMMAND(ID_OPCIONES_WAVELET, &CEditorFotosDlg::OnOpcionesWavelet)
END_MESSAGE_MAP()

// Controladores de mensaje de CEditorFotosDlg

BOOL CEditorFotosDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Establecer el icono para este cuadro de diálogo.  El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	ShowWindow(SW_MAXIMIZE);

	// TODO: agregar aquí inicialización adicional

	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

void CEditorFotosDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono.  Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CEditorFotosDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CEditorFotosDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/*
void CEditorFotosDlg::OnBnClickedOk()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	
	CDC* cdc = this->groupArea.GetDC();

	brg::BMP bmp("D:\\Maestría\\CG\\Briggi\\MostrarBMP\\Pruebas\\Sor Asunta.bmp");

	GetDlgItem(groupID)->MoveWindow(0, 0, bmp.getWidth(), bmp.getHeight(), TRUE);

	for (int i = 0; i < bmp.getWidth(); i++)
	{
		for (int j = 0; j < bmp.getHeight(); j++)
		{
			brg::PixelRGB2 pixel = bmp.getPixel(i, j);
			cdc->SetPixel(i, bmp.getHeight() - j - 1, RGB(pixel.r, pixel.g, pixel.b));
		}
	}
}*/

void CEditorFotosDlg::OnBnClickedOk()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	UpdateData(true);
	CStringA charstr(rutaArchivo);
	brg::BMP image((const char *)charstr);

	int size = image.getWidth() * image.getHeight();
	CDC* cdc = this->groupArea.GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(cdc);
	vector<COLORREF> bitmap(size, RGB(255, 255, 255));
	
	GetDlgItem(groupID)->MoveWindow(0, 50, image.getWidth(), image.getHeight(), TRUE);
	

	for (int i = 0; i<image.getWidth(); i++)
	{
		for (int j = 0; j<image.getHeight(); j++)
		{
			brg::PixelRGB& pixel = image.getPixel(i, j);
			bitmap[(image.getHeight()-1-j)*image.getWidth() + i] = 0x00000000 | pixel.r << 16 | pixel.g << 8 | pixel.b;
		}
	}
	
	CBitmap bmp;
	bmp.CreateBitmap(image.getWidth(), image.getHeight(), 1, 32, &bitmap[0]);
	dcMem.SelectObject(&bmp);

	cdc->TransparentBlt(
		0, 0, image.getWidth(), image.getHeight(), &dcMem,
		0, 0, image.getWidth(), image.getHeight(), RGB(255, 255, 255));
	
	dcMem.DeleteDC();
}

void CEditorFotosDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode)
	{
	case SB_TOP:
		m_iSrcY = 0;
		break;
	case SB_BOTTOM:
		m_iSrcY = INT_MAX;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_iSrcY = nPos;
		break;

	case SB_PAGEDOWN:
	case SB_LINEDOWN:
		if (m_iSrcY >= INT_MAX)
		{
			m_iSrcY = INT_MAX;
			return;
		}
		m_iSrcY += 10;
		break;
	case SB_PAGEUP:
	case SB_LINEUP:
		if (m_iSrcY <= (-INT_MAX))
		{
			m_iSrcY = 0;
			return;
		}
		m_iSrcY -= 10;
		break;
	}
	m_VScrollBar.SetScrollPos(m_iSrcY);
	Invalidate();
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CEditorFotosDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode)
	{
	case SB_TOP:
		m_iSrcX = 0;
		break;
	case SB_BOTTOM:
		m_iSrcX = INT_MAX;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_iSrcX = nPos;
		break;
	case SB_PAGERIGHT:
	case SB_LINERIGHT:
		if (m_iSrcX >= INT_MAX)
		{
			m_iSrcX = INT_MAX;
			return;
		}
		m_iSrcX += 10;
		break;
	case SB_PAGELEFT:
	case SB_LINELEFT:
		if (m_iSrcX <= (-INT_MAX))
		{
			m_iSrcX = 0;
			return;
		}
		m_iSrcX -= 10;
		break;
	}

	m_HScrollBar.SetScrollPos(m_iSrcX);
	Invalidate();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditorFotosDlg::OnSize(UINT nType, int cx, int cy)
{
}

void CEditorFotosDlg::OnIdrMenu1()
{
}

void mostrarImagen(CEditorFotosDlg* window, CStatic& area, int group, string fileName)
{
	brg::BMP image(fileName.c_str());

	int size = image.getWidth() * image.getHeight();
	CDC* cdc = area.GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(cdc);
	vector<COLORREF> bitmap(size, RGB(255, 255, 255));

	//window->GetDlgItem(group)->MoveWindow(0, 50, image.getWidth(), image.getHeight(), TRUE);

	for (int i = 0; i<image.getWidth(); i++)
	{
		for (int j = 0; j<image.getHeight(); j++)
		{
			brg::PixelRGB& pixel = image.getPixel(i, j);
			bitmap[(image.getHeight() - 1 - j)*image.getWidth() + i] = 0x00000000 | pixel.r << 16 | pixel.g << 8 | pixel.b;
		}
	}

	CBitmap bmp;
	bmp.CreateBitmap(image.getWidth(), image.getHeight(), 1, 32, &bitmap[0]);
	dcMem.SelectObject(&bmp);

	cdc->TransparentBlt(
		0, 0, image.getWidth(), image.getHeight(), &dcMem,
		0, 0, image.getWidth(), image.getHeight(), RGB(255, 255, 255));

	dcMem.DeleteDC();
}

/*
	Maestria CS UNSA - Briggi Rivera Guillen
	Prof. Juan Carlos Gutierrez Caceres
*/

void CEditorFotosDlg::OnOpcionesWavelet()
{
	// TODO: Agregue aquí su código de controlador de comandos
	//MessageBoxW(L"Text Here", 0);

	// TODO: Agregue aquí su código de controlador de notificación de control
	UpdateData(true);
	CStringA charstr(rutaArchivo);
	string file = (const char *)charstr;
	string fileSaved = file;
	fileSaved.replace(fileSaved.find(".bmp"), sizeof("_procesada.bmp") - 1, "_procesada.bmp");

	CImg<unsigned char> wavelet(file.c_str());
	wavelet.resize(512, 512);
	wavelet.haar(false, escalas_wavelet);
	wavelet.save(fileSaved.c_str());
	
	CImg<unsigned char> imageInput(file.c_str());
	imageInput.resize(512, 512);
	imageInput.save(file.c_str());
	
	mostrarImagen(this, this->groupArea, groupID, file);
	mostrarImagen(this, this->groupArea2, groupID2, fileSaved);
}
