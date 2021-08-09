#include <fwpath.h>

void TestFWPath() {
    auto appDirPath = FWPath::getAppDataPath();
    printf("Application dir: %s\n", appDirPath.c_str());

    auto userDirPath = FWPath::getUserDataPath();
    printf("User data path: %s\n",userDirPath.c_str());
}

void TestPrependPath() {
    auto newPath = FWPath::prependPath("CMakeFiles", FWPath::dpApp);
    printf("dpApp prepended path: %s\n", newPath.c_str());

    newPath = FWPath::prependPath("CMakeFiles", FWPath::dpUser);
    printf("dpUser prepended path: %s\n", newPath.c_str());
}

int main(int, char**)
{
    TestFWPath();
    TestPrependPath();

    bool res = FWPath::createUserDataPath();
    printf("User dir exist/created: %s\n", res?"true":"false");

    res = FWPath::checkPortable();
    printf("IsPortable: %s\n", res?"true":"false");

    return 0;
}

