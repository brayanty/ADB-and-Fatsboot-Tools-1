#include <windows.h>
#include <commdlg.h> // Biblioteca necesaria para GetOpenFileName
#include <iostream>

// Muestra el cuadro de diálogo de apertura de archivos
bool openFile(std::wstring& filePath) {
    OPENFILENAMEW ofn;       // Estructura para la configuración del cuadro de diálogo
    WCHAR szFile[260];       // Buffer para el nombre del archivo

    // Inicializa la estructura OPENFILENAMEW
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = L'\0'; // Inicializa con una cadena vacía
    ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
    ofn.lpstrFilter = L"Todos los archivos\0*.*\0Archivos de texto\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Muestra el cuadro de diálogo de apertura de archivos
    if (GetOpenFileNameW(&ofn) == TRUE) {
        // Ruta del archivo seleccionada exitosamente
        filePath = ofn.lpstrFile;
        return true;
    } else {
        // Error o cancelación por el usuario
        std::cerr << "No se seleccionó ningún archivo o hubo un error." << std::endl;
        return false;
    }
}
