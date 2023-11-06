#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "resource.h"

WNDPROC EditWndProc1;
WNDPROC EditWndProc2;

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK EditProcForDialog(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_GETDLGCODE:
			return (DLGC_WANTALLKEYS | CallWindowProc(EditWndProc1, hWnd, msg, wParam, lParam));
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			MessageBox(hWnd, TEXT("Enter Keyが押されました。"), szClassName, 0);
			SetFocus(hWnd);
			return 0;
		}
		break;
	default:
		break;
	}
	return CallWindowProc(EditWndProc1, hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK EditProcForWindow(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			MessageBox(hWnd, TEXT("Enter Keyが押されました。"), szClassName, 0);
			SetFocus(hWnd);
			return 0;
		}
		break;
	default:
		break;
	}
	return CallWindowProc(EditWndProc2, hWnd, msg, wParam, lParam);
}

INT_PTR CALLBACK DialogProc(HWND hDlg, unsigned msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo;
	switch (msg)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		EditWndProc1 = (WNDPROC)SetWindowLongPtr(GetWindow(hCombo, GW_CHILD), GWLP_WNDPROC, (LONG_PTR)EditProcForDialog);
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo;
	switch (msg)
	{
	case WM_CREATE:

		DialogBox(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

		hCombo = CreateWindow(TEXT("COMBOBOX"), TEXT("変換"), WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 10, 10, 256, 256, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		EditWndProc2 = (WNDPROC)SetWindowLongPtr(GetWindow(hCombo, GW_CHILD), GWLP_WNDPROC, (LONG_PTR)EditProcForWindow);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				TCHAR szText[256];
				GetWindowText(hCombo, szText, 256);
				MessageBox(hWnd, szText, TEXT("変換"), MB_OK);
			}
			else if (HIWORD(wParam) == CBN_EDITCHANGE)
			{
				TCHAR szText[256];
				GetWindowText(hCombo, szText, 256);
				MessageBox(hWnd, szText, TEXT("変換"), MB_OK);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
