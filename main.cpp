#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>

#include <Dbghelp.h>

using namespace std;

#pragma auto_inline (off)
#pragma comment( lib, "DbgHelp" )

#include "crashhandler.h"

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    HANDLE lhDumpFile = CreateFile(L"DumpFile.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;

    loExceptionInfo.ExceptionPointers = ExceptionInfo;
    loExceptionInfo.ThreadId = GetCurrentThreadId();
    loExceptionInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);

    CloseHandle(lhDumpFile);

    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)MyUnhandledExceptionFilter);
    QApplication a(argc, argv);
    DxCrashHandler crashhandler;

    MainWindow w;
    w.show();
    return a.exec();
}
