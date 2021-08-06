#include <fwpath.h>

void TestFWPath() {
    auto appDirPath = FWPath::getAppDataPath();
    printf("Application dir: %s\n", appDirPath.c_str());

    auto userDirPath = FWPath::getUserDataPath();
    printf("User data path: %s\n",userDirPath.c_str());
}


int main(int, char**)
{
    TestFWPath();

    return 0;
}

