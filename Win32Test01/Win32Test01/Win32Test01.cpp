// Win32Test01.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Win32Test01.h"
#include <ocidl.h>
#include <olectl.h>

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32TEST01, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32TEST01));

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32TEST01));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32TEST01);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#define BUTTON_ID1 0
#define BUTTON_ID2 1

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   // �{�^���������Ă݂��肵�āB
   CreateWindow(
		TEXT("BUTTON") , TEXT("Kitty") ,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		0 , 0 , 100 , 50 ,
		hWnd , (HMENU)BUTTON_ID1 , hInstance , NULL
	);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// �i�o�d�f�W�J�e�X�g
//LPVOID loadImage(char *);
LPVOID loadImage(void);
LPVOID g_lpBmp;
LPBITMAPINFO g_bmiImage;
LPVOID g_lpPixels;

void draw(HWND hWnd) {
	/* �摜�t�@�C������DIB���쐬 */
	g_lpBmp = loadImage();

	if (g_lpBmp != NULL) {
		MessageBox(hWnd , TEXT("�Ƃꂽ���ۂ��I") , TEXT("����") , MB_OK);

		/* DIB�̃w�b�_�|�C���^��ݒ� */
		g_bmiImage = (LPBITMAPINFO)g_lpBmp;

		/* DIB�̃s�N�Z����|�C���^��ݒ� */
		g_lpPixels = (LPBYTE)g_lpBmp + sizeof(BITMAPINFO);

	} else {
		MessageBox(hWnd , TEXT("NULL!") , TEXT("����") , MB_OK);
	}
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR tcStr[128];

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		// �{�^���Ȃ�E�E�E�B
		case BUTTON_ID1:
			draw(hWnd);
//			MessageBox(hWnd , TEXT("BUTTON_ID1") , TEXT("Kitty") , MB_OK);
//			ShowWindow( hWnd, SW_MINIMIZE );
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	// �L�[�{�[�h�Ȃ�E�E�E�B
	case WM_KEYDOWN:
		wsprintf(tcStr , TEXT("Kitty on your lap\nwPalam = %Xh") , wParam);
//		MessageBox(NULL , tcStr , TEXT("Key Event") , MB_OK);
		//InvalidateRect();

		//�ĕ`��
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...

		/* �摜��ǂݍ���DIB������Ε\�� */
		if (g_lpBmp != NULL) {
			SetDIBitsToDevice(hdc, 100, 100, g_bmiImage->bmiHeader.biWidth, g_bmiImage->bmiHeader.biHeight, 0, 0, 0, g_bmiImage->bmiHeader.biHeight, g_lpPixels, g_bmiImage, DIB_RGB_COLORS);
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// �i�o�d�f�C���[�W�����[�h����
LPVOID loadImage(void) {

	IStream * isImage;
	IPicture * ipicImage;
	HANDLE hFile;
	HGLOBAL hBuf;
	LPVOID lpBuf;
	OLE_HANDLE hObj = NULL;

	HBITMAP hBmp;
	BITMAP bmpInfo;
	LPBITMAPINFO lpbiBmp;
	LPBYTE lpBmp;

	DWORD dwFileSize, dwLoadSize;

	/* �摜�t�@�C���I�[�v�� */
//	hFile = CreateFile(lpszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hFile = CreateFile(TEXT("marubatsu.jpg"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	/* �t�@�C���T�C�Y�擾 */
	dwFileSize = GetFileSize(hFile, NULL);

	/* �摜�t�@�C���f�[�^�i�[�p�������u���b�N�m�� */
	hBuf = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);

	/* �������u���b�N�����b�N���A�h���X���擾 */
	lpBuf = GlobalLock(hBuf);

	/* �摜�t�@�C���̃f�[�^���������u���b�N�ɓǂݍ��� */
	ReadFile(hFile, lpBuf, GetFileSize(hFile, NULL), &dwLoadSize, NULL);

	CloseHandle(hFile);

	/* �������u���b�N�̃��b�N���� */
	GlobalUnlock(hBuf);

	/* �������u���b�N����IStream���쐬 */
	CreateStreamOnHGlobal(hBuf, TRUE, &isImage);

	/* �摜�t�@�C���̃X�g���[������摜�I�u�W�F�N�g���쐬 */
//	OleLoadPicture(isImage, dwFileSize, FALSE, &IID_IPicture, (LPVOID*)&ipicImage);
	OleLoadPicture(isImage, dwFileSize, TRUE, IID_IPicture, (LPVOID*)&ipicImage);

	/* �X�g���[����� */
//	isImage->lpVtbl->Release(isImage);
	isImage->Release();

	/* �������u���b�N��� */
	GlobalFree(hBuf);

	/* �摜�I�u�W�F�N�g�̃n���h�����擾 */
//	ipicImage->lpVtbl->get_Handle(ipicImage, &hObj);
	ipicImage->get_Handle(&hObj);

	hBmp = (HBITMAP)hObj;

	/* BITMAP�����擾 */
	GetObject(hBmp, sizeof(BITMAP), &bmpInfo);

	/* DIB�p�������m�� */
	lpBmp = (LPBYTE)GlobalAlloc(GPTR, sizeof(BITMAPINFO) + (bmpInfo.bmWidth * bmpInfo.bmWidth * 4));

	/* 32�r�b�gDIB�pBITMAPINFO�\���̐ݒ� */
	lpbiBmp = (LPBITMAPINFO)lpBmp;
	lpbiBmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiBmp->bmiHeader.biWidth = bmpInfo.bmWidth;
	lpbiBmp->bmiHeader.biHeight = bmpInfo.bmHeight;
	lpbiBmp->bmiHeader.biPlanes = 1;
	lpbiBmp->bmiHeader.biBitCount = 32;

	/* DIB�s�N�Z������擾 */
	GetDIBits(GetDC(0), hBmp, 0, bmpInfo.bmHeight, lpBmp + sizeof(BITMAPINFO), lpbiBmp, DIB_RGB_COLORS); 

//	ipicImage->lpVtbl->Release(ipicImage);
	ipicImage->Release();

	return lpBmp;
}
