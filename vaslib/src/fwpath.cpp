///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2005-2008 Martin Boehme
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
///////////////////////////////////////////////////////////////////////////////

#include "vas_path.h"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>

//QString VasPath::m_path=".";
QString VasPath::m_path;

bool VasPath::m_isStandalone = false;

/////////////////////////////////////////////////////////////////////////////

//QString VasPath::prependPath(const QString &relativePath)
//{
//    QString ret;
//    QString tmpPath = QDir::fromNativeSeparators(relativePath);
//    if (relativePath.isEmpty()) ret = m_path;
//    else ret = m_path + (tmpPath[0] == '/' ? "" : "/") +  relativePath;
//    return ret.trimmed();
//}


QString VasPath::prependPath(const QString &relativePath, VasPath::EDataPath pathType) {
    QString dataPath;
    do {
        if (!m_path.isEmpty()) {
            dataPath = m_path;
            break;
        }

        if (isStandalone() || pathType == dpApp) {
            dataPath = getAppDataPath();
            break;
        }

        if (pathType == dpAuto) {
            QString userDirPath = prependPath(relativePath, dpUser);
            QFileInfo info(userDirPath);
            if (info.exists()) return userDirPath;

            QString appDirPath = prependPath(relativePath, dpApp);
            info.setFile(appDirPath);
            if (info.exists()) return appDirPath;

            //if all fail, then return path relative to user dir
            return userDirPath;
        }

        dataPath = getUserDataPath();

    } while(false);

//    if (m_path.isEmpty()) {
//        dataPath = getUserDataPath();
//        if (pathType == dpApp) {
//            dataPath = getAppDataPath();
//        }
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
//    } else {
//        dataPath = m_path;
//    }

    QString ret;
    QString tmpPath = QDir::fromNativeSeparators(relativePath);
    if (relativePath.isEmpty()) ret = dataPath;
    else ret = dataPath + (tmpPath[0] == '/' ? "" : "/") +  relativePath;
    return ret.trimmed();
}

/////////////////////////////////////////////////////////////////////////////

QString VasPath::getUserDataPath() {
    if (!m_path.isEmpty()) return m_path;
    if (isStandalone()) return getAppDataPath();
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

/////////////////////////////////////////////////////////////////////////////

QString VasPath::getAppDataPath() {
    return QApplication::applicationDirPath();
}

/////////////////////////////////////////////////////////////////////////////

bool VasPath::checkUserDataPath() {
    QDir dir(getUserDataPath());
    if (dir.exists()) return true;
    return dir.mkpath(dir.absolutePath());
}

/////////////////////////////////////////////////////////////////////////////

void VasPath::setStandalone() {
    m_isStandalone = true;
}

bool VasPath::isStandalone() {
    return m_isStandalone;
}

void VasPath::checkStanalone() {
    QDir dir(getAppDataPath());
    QFile tmpFile(dir.absolutePath() + "/tmp.tmp");
    bool res = tmpFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Unbuffered);
    m_isStandalone = res;
    tmpFile.close();
    tmpFile.remove();
}
