#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <assert.h>
using namespace std;
int main(int argc, char** argv)
{
    filebuf* pbuf;
    ifstream filestr;
    long size;
    char* buffer;
    filestr.open(argv[1], ios::binary);
    pbuf = filestr.rdbuf();
    size = pbuf->pubseekoff(0, ios::end, ios::in);
    pbuf->pubseekpos(0, ios::in);
    buffer = new char[size];
    pbuf->sgetn(buffer, size);
    filestr.close();
    string fileName = argv[2];
    LONG PeHeadOffset = *(LONG*)(buffer +0x3c);
    cout << hex << "PeHeadOffset:0x" << PeHeadOffset <<"\n\n";
    LONG WinCertificateOffset = *(LONG*)(buffer + PeHeadOffset + 0xa8);
    cout << hex << "WIN_CECRTIFICATE_Offset:0x" << WinCertificateOffset << "\n\n";
    UINT16 length = 0;
    *((UCHAR*)&length + 1) = *(UCHAR*)(buffer + WinCertificateOffset + 0x0a);
    *((UCHAR*)&length) = *(UCHAR*)(buffer + WinCertificateOffset + 0x0b);
    cout << hex << "SEQUENCE_Length:0x" << length << "\n\n";
    length +=1;
    *(UCHAR*)(buffer + WinCertificateOffset + 0x0a) = *((UCHAR*)&length + 1);
    *(UCHAR*)(buffer + WinCertificateOffset + 0x0b) = *((UCHAR*)&length);
    HANDLE hFile = CreateFileA(fileName.data(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwWritten;
        BOOL bRet = WriteFile(hFile, buffer, 0, &dwWritten, NULL);
        assert(TRUE == bRet);
        bRet = WriteFile(hFile, buffer, size, &dwWritten, NULL);
        CloseHandle(hFile);
        if (bRet) {
            cout << "success!\n\n";
            cout <<  "file=" << argv[1] << "\nout=" << fileName << "\n";
        }
        else {
            cout << dec << "error#" << GetLastError() << "\n";
        }
    }
    else {
        cout << dec << "error#" << GetLastError() << "\n";
    }
    delete[]buffer;
    return 0;
}
