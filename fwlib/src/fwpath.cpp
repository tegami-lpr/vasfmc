#include "fwpath.h"

//#include <thread>


#include <processthreadsapi.h>
#include <userenv.h>
#include <windows.h>


//QString VasPath::m_path=".";
std::string FWPath::m_path;

bool FWPath::m_isStandalone = false;

//--------------------------------------------------------------------------------------------------------------------//

//QString VasPath::prependPath(const QString &relativePath)
//{
//    QString ret;
//    QString tmpPath = QDir::fromNativeSeparators(relativePath);
//    if (relativePath.isEmpty()) ret = m_path;
//    else ret = m_path + (tmpPath[0] == '/' ? "" : "/") +  relativePath;
//    return ret.trimmed();
//}


std::string FWPath::prependPath(const std::string &relativePath, FWPath::EDataPath pathType) {
//    std::string dataPath;
//    do {
//        if (!m_path.isEmpty()) {
//            dataPath = m_path;
//            break;
//        }
//
//        if (isStandalone() || pathType == dpApp) {
//            dataPath = getAppDataPath();
//            break;
//        }
//
//        if (pathType == dpAuto) {
//            QString userDirPath = prependPath(relativePath, dpUser);
//            QFileInfo info(userDirPath);
//            if (info.exists()) return userDirPath;
//
//            QString appDirPath = prependPath(relativePath, dpApp);
//            info.setFile(appDirPath);
//            if (info.exists()) return appDirPath;
//
//            //if all fail, then return path relative to user dir
//            return userDirPath;
//        }
//
//        dataPath = getUserDataPath();
//
//    } while(false);
//
//    QString ret;
//    QString tmpPath = QDir::fromNativeSeparators(relativePath);
//    if (relativePath.isEmpty()) ret = dataPath;
//    else ret = dataPath + (tmpPath[0] == '/' ? "" : "/") +  relativePath;
//    return ret.trimmed();
    return std::string();
}

//--------------------------------------------------------------------------------------------------------------------//

std::string FWPath::getUserDataPath() {
    if (!m_path.empty()) return m_path;
    if (isStandalone()) return getAppDataPath();


    std::string userPath;
    HANDLE tokenHandle;
    HANDLE processHandle = GetCurrentProcess();

    bool res = OpenProcessToken(processHandle, TOKEN_READ, &tokenHandle);
    do {
        if (!res) break;
        char pBuf[256];
        DWORD len = sizeof(pBuf);
        memset(pBuf, 0, len);
        res = GetUserProfileDirectory(tokenHandle, pBuf, &len);
        if (!res) break;
        userPath = std::string(pBuf);
    } while(false);

    CloseHandle(tokenHandle);
    return userPath;
}

//--------------------------------------------------------------------------------------------------------------------//

std::string FWPath::getAppDataPath() {
    char pBuf[256];
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

bool FWPath::checkUserDataPath() {
//    QDir dir(getUserDataPath());
//    if (dir.exists()) return true;
//    return dir.mkpath(dir.absolutePath());
}

//--------------------------------------------------------------------------------------------------------------------//

void FWPath::setStandalone() {
    m_isStandalone = true;
}

bool FWPath::isStandalone() {
    return m_isStandalone;
}

void FWPath::checkStanalone() {
//    QDir dir(getAppDataPath());
//    QFile tmpFile(dir.absolutePath() + "/tmp.tmp");
//    bool res = tmpFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Unbuffered);
//    m_isStandalone = res;
//    tmpFile.close();
//    tmpFile.remove();
}
