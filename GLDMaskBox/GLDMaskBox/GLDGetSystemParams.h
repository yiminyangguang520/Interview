#ifndef GLDGETSYSTEMPATH_H
#define GLDGETSYSTEMPATH_H

#include <tchar.h>
#include <Shlobj.h>
#include <windows.h>

#include <QString>

#define INFO_BUFFER_SIZE 32767

QString temp            = "%TEMP%";
QString winDir          = "%WINDIR%";
QString winSysDir       = "%WINSYSDIR%";
QString userAppData     = "%USERAPPDATA%";
QString commonAppData   = "%COMMONAPPDATA%";
QString userDocuments   = "%USERDOCUMENTS%";
QString commonDocuments = "%COMMONDOCUMENTS%";

/**
 * @brief 获取临时目录,即%TEMP%
 * @return
 */
QString getTempPath()
{
    DWORD dwRetVal = 0;
    TCHAR lpTempPathBuffer[MAX_PATH];

    dwRetVal = ::GetTempPath(MAX_PATH, lpTempPathBuffer);

    return QString::fromWCharArray(lpTempPathBuffer).replace("\\", "/");
}

/**
 * @brief 获取window目录,即%WINDIR%
 * @return
 */
QString getWindowDir()
{
    TCHAR infoBuf[INFO_BUFFER_SIZE];
    GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE);

    return QString::fromWCharArray(infoBuf).replace("\\", "/");
}

/**
 * @brief 获取系统目录,即%WINSYSDIR%
 * @return
 */
QString getSystemDir()
{
    TCHAR infoBuf[INFO_BUFFER_SIZE];
    GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE);

    return QString::fromWCharArray(infoBuf).replace("\\", "/");
}

/**
 * @brief 获取系统参数
 * @param 标示
 * @return
 */
QString getSysParams(int flag)
{
    TCHAR szPath[MAX_PATH];
    LPITEMIDLIST pidl;
    LPMALLOC pShellMalloc;

    QString str;

    if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
    {
        if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, flag, &pidl)))
        {
            if (SHGetPathFromIDList(pidl, szPath))
            {
                str = QString::fromWCharArray(szPath).replace("\\", "/");
            }
        }
    }

    CoTaskMemFree(pidl);

    return str;
}

void replaceAllParams(QString & source)
{
    if (source.startsWith(temp))
    {
        source.replace(0, temp.length(), getTempPath());
    }
    else if (source.startsWith(winDir))
    {
        source.replace(0, winDir.length(), getWindowDir());
    }
    else if (source.startsWith(winSysDir))
    {
        source.replace(0, winSysDir.length(), getSystemDir());
    }
    else if (source.startsWith(userAppData))
    {
        source.replace(0, userAppData.length(), getSysParams(CSIDL_APPDATA));
    }
    else if (source.startsWith(commonAppData))
    {
        source.replace(0, commonAppData.length(), getSysParams(CSIDL_COMMON_APPDATA));
    }
    else if (source.startsWith(userDocuments))
    {
        source.replace(0, userDocuments.length(), getSysParams(CSIDL_PERSONAL));
    }
    else if (source.startsWith(commonDocuments))
    {
        source.replace(0, commonDocuments.length(), getSysParams(CSIDL_COMMON_DOCUMENTS));
    }
}

#endif // GLDGETSYSTEMPATH_H
