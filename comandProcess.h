bool adbToFlash(LPCSTR command){
    // Estructuras de datos para el proceso
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Limpiar las estructuras
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Crear el proceso
    if (!CreateProcessW(L"ADBTOFASTBOOT/adb.exe ",   // ADB
        (LPWSTR)command,        // Comando a ejecutar
        NULL,                  // No hay un proceso padre, así que usaré NULL
        NULL,                  // No necesitamos un identificador de proceso secundario
        FALSE,                 // No heredar las manijas del proceso principal
        0,                     // Sin banderas adicionales
        NULL,                  // Sin variables de entorno específicas (heredar las del proceso principal)
        NULL,                  // No hay un directorio de inicio específico
        &si,                   // Información de inicio
        &pi                    // Información del proceso
    )) {
        
        return false;
    }

    // Esperar a que el proceso hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Cerrar los manejadores
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
}