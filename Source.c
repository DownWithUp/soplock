/*
    soplock - Simple Opportunistic lock tool
    Based on work by James Forshaw from Google Project Zero
*/
#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    OVERLAPPED	overlapped;
    HANDLE      hFile;
    LPCSTR      lpszPath;
    DWORD       dwBytesOut;
    DWORD       dwFlags;

    if (argc != 2){
        printf("Usage: soplock [Full File Path]\n");
        return(-1);
	}

    lpszPath = argv[1];
    while (TRUE) {
    /*
        Overlapped required for FSCTL_REQUEST_OPLOCK_LEVEL_1, according to MSDN
    */
    overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, "Lock");
    hFile = CreateFileA(lpszPath, GENERIC_READ | SYNCHRONIZE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
    DeviceIoControl(hFile, FSCTL_REQUEST_OPLOCK_LEVEL_1, NULL, 0, NULL, 0, NULL, &overlapped);
    WaitForSingleObject(overlapped.hEvent, INFINITE);
    printf("Lock was triggerd when %s was accessed\nPress ENTER to contine...", lpszPath);
    getchar();
    CloseHandle(hFile);
    CloseHandle(overlapped.hEvent);
    Sleep(10); // Sleep seems to be required to properly reset the loop
    }
    return(0);
}
