#define UNICODE
#define _UNICODE

#include <windows.h>
#include <commdlg.h> // Biblioteca necesaria para GetOpenFileName
#include <wchar.h>
#include <iostream>
#include <string>
#include "openFile.h"
#include "define.h"
#include "wstrinToWchar_t.h"

/* Datos de la aplicación */
typedef struct stDatos {
	char Lista[6][80]; // Valores de los comboboxes
	char Item[3][80];  // Opciones elegidas
} DATOS;

static DATOS Datos;
int i;

WNDCLASSEX wc; /* A properties struct of our window */

using namespace std;
std::wstring fileToFlash;
LPCWSTR fileToFlashConvert;



bool wstringToWcharT(std::wstring wideStr) {
	// Crear un búfer para almacenar la cadena de caracteres anchos (wide)
	wchar_t* buffer = new wchar_t[wideStr.length() + 1];
	// Copiar la cadena wstring al búfer wchar_t
	wcscpy(buffer, wideStr.c_str());
	// Asignar el puntero del búfer a fileToFlashConvert
	fileToFlashConvert = buffer;
	// Liberar la memoria asignada para el búfer
	delete[] buffer;

	return true;
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {

		case WM_CREATE: {
			wc.hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
			strcpy(Datos.Item[0], "a");
			strcpy(Datos.Item[1], "c");
			strcpy(Datos.Item[2], "e");
			for (i = 0; i < 6; i++) {
				sprintf(Datos.Lista[i], "%c) Opción %c", 'a' + i, 'A' + i);
			}

			// Crear ComboBox
			HWND hComboBox = CreateWindowW(L"COMBOBOX", NULL, CBS_DROPDOWN | WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			                               50, 19, 300, 99, hwnd, (HMENU)9, NULL, NULL);

			// Crear edit para la ubicacion del archivo
			HWND hEditFileToFlash = CreateWindowW(L"EDIT", NULL, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			                                      50, 50, 300, 25, hwnd, (HMENU)ID_EDITTIFLASH, NULL, NULL);

			CreateWindowW(L"BUTTON", L"Browse", WS_VISIBLE | WS_CHILD | WS_BORDER,
			              350, 50, 40, 25, hwnd, (HMENU)ID_BROWSE, NULL, NULL);

			// Añadir cadenas al ComboBox
			SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Flash to ADB Sideload");
			SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"2");
			SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"3");

			// Seleccionar elemento por defecto
			SendMessage(hComboBox, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)Datos.Item[0]);

			// Crear primer botón de prueba
			CreateWindowW(L"BUTTON", L"Salir", WS_VISIBLE | WS_CHILD,
			              400, 19, 80, 25, hwnd, (HMENU)ID_EXIT, NULL, NULL);

			// Crear segundo botón de prueba
			CreateWindowW(L"BUTTON", L"Flash", WS_VISIBLE | WS_CHILD,
			              500, 19, 80, 25, hwnd, (HMENU)ID_FLASH, NULL, NULL);

			break;
		}

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case ID_EXIT: {
					if (MessageBoxW(hwnd, L"Deseas Salir", L"Aviso", MB_OKCANCEL | MB_ICONEXCLAMATION ) == IDOK) {
						PostQuitMessage(0);
					}
					break;
				}
				case ID_BROWSE: {
					if (openFile(fileToFlash)) {
						SetWindowTextW(GetDlgItem(hwnd, ID_EDITTIFLASH), fileToFlash.c_str());
						wstringToWcharT(fileToFlash);
						MessageBox(hwnd,fileToFlashConvert,L"w", MB_OKCANCEL | MB_ICONEXCLAMATION);
					} else {
						MessageBoxW(hwnd, L"No se seleccionó ningún archivo o hubo un error.", L"Error", MB_OK | MB_ICONERROR);
					}
					break;
				}
				case ID_FLASH: {
					// Acción para el botón Flash
					MessageBoxW(hwnd, fileToFlash.c_str(), L"Archivo a flashear", MB_OK);
					break;
				}
			}
			break;
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hwnd;   /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg;     /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"WindowClass";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);    /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"WindowClass", L"Caption", WS_VISIBLE | WS_OVERLAPPED,
	                      CW_USEDEFAULT, /* x */
	                      CW_USEDEFAULT, /* y */
	                      640,           /* width */
	                      480,           /* height */
	                      NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	/*
	    This is the heart of our program where all input is processed and
	    sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
	    this loop will not produce unreasonably high CPU usage
	*/
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg);  /* Send it to WndProc */
	}
	return msg.wParam;
}
