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

#ifndef VAS_PATH_H
#define VAS_PATH_H

#include <string>

/////////////////////////////////////////////////////////////////////////////

class FWPath
{
public:

    enum EDataPath {
        //! User config path
        dpUser = 0,
        //! App install dir path
        dpApp = 1,
        //! Auto - prefer user path if exists
        dpAuto = 2
    };

    static void setPath(const std::string &path) { m_path = path; }
    static const std::string& getPath() { return m_path; }

    //! Function check is appdir is writable
    static bool checkPortable();

    //! Function set standalone sign, forcing app to find data files in app directory
    static void setPortable();

    //! Function return standalone flag
    static bool isPortable();

    //! Function prepend \param relativePath by path where data is stored
    static std::string prependPath(const std::string &relativePath, EDataPath pathType = dpUser);

    //! Returns path to user writable directory for app configs
    static std::string getUserDataPath();

    //! Returns path to static (non writable) app data
    static std::string getAppDataPath();

    //! Function check and create if needed user data path
    static bool createUserDataPath();

    //! Function return path converted to native separators
    static std::string toNativeSeparators(const std::string &path);

    //! Function return if file exists
    static bool checkIsExists(const std::string &path);

    //! Function return if dir exists
    static bool checkIsDir(const std::string &path);

private:
    //! Path, forcely used as app root, instead of deducting
    static std::string m_path;
    //! Floag, is app folder is writable
    static bool m_isAppDirWritable;
};

#endif // VAS_PATH_H
