#include "fwpath.h"

#include <processthreadsapi.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <windows.h>


#include <sys/types.h>
#include <sys/stat.h>

#include "strutils.h"

std::string FWPath::m_path;

bool FWPath::m_isAppDirWritable = false;

//--------------------------------------------------------------------------------------------------------------------//

std::string FWPath::prependPath(const std::string &relativePath, FWPath::EDataPath pathType) {
    std::string dataPath;
    do {
        if (!m_path.empty()) {
            dataPath = m_path;
            break;
        }

        if (isPortable() || pathType == dpApp) {
            dataPath = getAppDataPath();
            break;
        }

        if (pathType == dpAuto) {
            std::string userDirPath = prependPath(relativePath, dpUser);
            if (checkIsDir(userDirPath)) return userDirPath;

            std::string appDirPath = prependPath(relativePath, dpApp);
            if (checkIsDir(appDirPath)) return appDirPath;

            //if all fail, then return path relative to user dir
            return userDirPath;
        }

        dataPath = getUserDataPath();
    } while(false);

    std::string ret;
    std::string tmpPath = toNativeSeparators(relativePath);
    if (relativePath.empty()) ret = dataPath;
    else ret = dataPath + (tmpPath[0] == '\\' ? "" : "\\") +  relativePath;

    return trim_copy(ret);
}

//--------------------------------------------------------------------------------------------------------------------//

std::string FWPath::getUserDataPath() {
    if (!m_path.empty()) return m_path;
    if (isPortable()) return getAppDataPath();

    CHAR szPath[MAX_PATH];
    if (! SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_APPDATA, nullptr, 0, szPath))) return std::string();

    PathAppend( szPath, TEXT("\\FWFmc") );

    return std::string(szPath);
}

//--------------------------------------------------------------------------------------------------------------------//

std::string FWPath::getAppDataPath() {
    char pBuf[MAX_PATH];
    size_t len = sizeof(pBuf);
    memset(pBuf, 0, len);
    DWORD ret = GetModuleFileName(nullptr, pBuf, len);
    if (ret < 1) return std::string();

    std::string filePath = std::string(pBuf);
    auto lastSepPos = filePath.find_last_of('\\');
    filePath = filePath.erase(lastSepPos);

    return filePath;
}

//--------------------------------------------------------------------------------------------------------------------//

bool FWPath::createUserDataPath() {

    auto userDir = getUserDataPath();
    if (userDir.empty()) return false;

    bool res = checkIsDir(userDir);
    if (res) return true;

    res = CreateDirectoryA(getUserDataPath().c_str(), nullptr);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

void FWPath::setPortable() {
    m_isAppDirWritable = true;
}

bool FWPath::isPortable() {
    return m_isAppDirWritable;
}

bool FWPath::checkPortable() {
    if (m_isAppDirWritable) return true;

    m_isAppDirWritable = false;
    auto tmpFilePath = prependPath("tmp.tmp", dpApp);
    auto fileHandle = CreateFile(tmpFilePath.c_str(), (GENERIC_READ | GENERIC_WRITE), 0, nullptr, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_ARCHIVE, nullptr);
    do {
        CloseHandle(fileHandle);
        if (fileHandle == INVALID_HANDLE_VALUE) break;
        DeleteFile(tmpFilePath.c_str());
        m_isAppDirWritable = true;
    } while(false);

    return m_isAppDirWritable;
}

//--------------------------------------------------------------------------------------------------------------------//

std::string FWPath::toNativeSeparators(const std::string &path) {
    auto tmpPath = path;
    for (char & p : tmpPath) {
        if (p == '/') p = '\\';
    }
    return tmpPath;
}

//--------------------------------------------------------------------------------------------------------------------//

bool FWPath::checkIsExists(const std::string &path) {
    struct stat info {};
    return (stat(path.c_str(), &info) == 0);
}

//--------------------------------------------------------------------------------------------------------------------//

bool FWPath::checkIsDir(const std::string &path) {
    struct stat info {};
    auto res = stat(path.c_str(), &info);
    if (res != 0) return false;

    return ( info.st_mode & (unsigned short)S_IFDIR );
}
