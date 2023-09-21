#define NOMINMAX

#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <vector>
#include <map>
#include <filesystem>
#include <fmt/format.h>

void Windows1110Activation(int option);

void Windows7Activation(int option);

void MainMenu();

bool IsRunAsAdmin() {
    BOOL isAdmin = FALSE;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    PSID adminGroup;

    if (AllocateAndInitializeSid(&ntAuthority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0, &adminGroup)) {

        if (!CheckTokenMembership(nullptr, adminGroup, &isAdmin)) { isAdmin = FALSE; }

        FreeSid(adminGroup);
    }

    return isAdmin != 0;
}

int Confirmation(
        const std::string &text = "Are you sure you want to activate Windows?\nY - activate Windows\nN - back to menu") {
    std::string temp;
    std::cout << text << std::endl;
    std::cout << ">>> ";
    std::cin >> temp;
    std::transform(temp.begin(), temp.end(), temp.begin(), [](int c) { return std::tolower(c); });

    // 0 - yes, -1 - no
    return (temp == "y") ? 0 : -1;

}

void GenerateMessage(const std::string &title = "Text", const std::vector<std::string> &arr = {}) {
    std::cout << title << std::endl;
    for (const std::string &str: arr) { std::cout << str << std::endl; }
}

void Windows11Menu() {
    std::vector<std::string> choices = {
            "-------------------------------------------",
            "| 1. Windows 11 Home                      |",
            "| 2. Windows 11 Home Single Language      |",
            "| 3. Windows 11 Home N                    |",
            "-------------------------------------------",
            "| 4. Windows 11 Professional              |",
            "| 5. Windows 11 Professional N            |",
            "| 6. Windows 11 Professional Education    |",
            "| 7. Windows 11 Professional Education N  |",
            "-------------------------------------------",
            "| 8. Windows 11 Education                 |",
            "| 9. Windows 11 Education N               |",
            "-------------------------------------------",
            "| 10. Windows 11 Enterprise               |",
            "-------------------------------------------",
            "| 11. Windows 11 LTSC                     |",
            "| 12. Windows 11 IoT Enterprise LTSC      |",
            "-------------------------------------------"
    };
    GenerateMessage("Select choice", choices);

    int value;
    std::cout << ">>> ";
    std::cin >> value;
    Windows1110Activation(value);
}

void Windows10Menu() {
    std::vector<std::string> choices = {
            "-------------------------------------------",
            "| 1. Windows 10 Home                      |",
            "| 2. Windows 10 Home Single Language      |",
            "| 3. Windows 10 Home N                    |",
            "-------------------------------------------",
            "| 4. Windows 10 Professional              |",
            "| 5. Windows 10 Professional N            |",
            "| 6. Windows 10 Professional Education    |",
            "| 7. Windows 10 Professional Education N  |",
            "-------------------------------------------",
            "| 8. Windows 10 Education                 |",
            "| 9. Windows 10 Education N               |",
            "-------------------------------------------",
            "| 10. Windows 10 Enterprise               |",
            "-------------------------------------------",
            "| 11. Windows 10 LTSC 2019/2021           |",
            "| 12. Windows 10 IoT Enterprise LTSC      |",
            "-------------------------------------------"
    };
    GenerateMessage("Select choice", choices);

    int value;
    std::cout << ">>> ";
    std::cin >> value;
    Windows1110Activation(value);
}


void Windows7Menu() {
    std::vector<std::string> choices = {
            "--------------------------------------------",
            "| 1. Windows 7 Professional                |",
            "--------------------------------------------",
            "| 2. Windows 7 Enterprise                  |",
            "--------------------------------------------"
    };
    GenerateMessage("Select choice", choices);

    int value;
    std::cout << ">>> ";
    std::cin >> value;
    Windows7Activation(value);
}

int ActivateWindows(const std::string &productKey, const bool &isLTSC, const bool &isIoTltsc,
                    const int &i) {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    std::string ProgramPath = ((std::string) buffer).substr(0, ((std::string) buffer).find_last_of("\\/"));
    if (std::filesystem::exists(ProgramPath + "\\files")) {
        std::cout << "Unpacking SKU'S..." << std::endl;
        std::system(fmt::format(
                R"(powershell -Command "Expand-Archive '{}\files\{}.zip' -Force -DestinationPath $env:windir\system32\spp\tokens\skus\ -erroraction 'silentlycontinue')",
                ProgramPath,
                (i == 1) ? "spp10" : (i == 2) ? "spp8.1" : (i == 4) ? "spp7" : "").c_str());
        std::cout << "Registering SKU's, please wait..." << std::endl;
        std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /rilc)");
    } else {
        std::cout << "Couldn't find files directory with needed files, "
                  << "please download program from github "
                  << "https://github.com/Snaky1a/Windows-Product-Key-Switcher/releases"
                  << std::endl;
        return -1;
    }

    if (isLTSC) {
        std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /upk)");
        std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /ckms)");
        std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /cpky)");
        return 0;
    }

    if (std::system(fmt::format(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /ipk {})",
                                productKey).c_str()) == 0) {
        std::cout << "Product key installed successfully." << std::endl;
    }
    if (!isIoTltsc) {
        if (Confirmation("Do you want activate Windows with Online KMS?"
                         "\nY - yes\nN - I want to activate myself / "
                         "I will activate it later") == 0) {
            std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /skms kms.loli.best)");
            std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /ato)");
        }
    } else {
        if (Confirmation("Do you want activate Windows with HWID activation (only for IoT LTSC)?"
                         "\nY - yes\nN - I want to activate myself / "
                         "I will activate it later") == 0) {
            std::system(("\"" + ProgramPath + R"(\files\IOT_LTSC_ACTIVATION.cmd")").c_str());
        }
    }
    return 0;
}


void Windows1110Activation(int option) {
    // Windows 10/11
    std::map<std::string, std::string> w10_keys = {
            {"1",  "TX9XD-98N7V-6WMQ6-BX7FG-H8Q99"}, // Home
            {"2",  "7HNRX-D7KGG-3K4RQ-4WPJ4-YTDFH"}, // Home Single Language
            {"3",  "3KHY7-WNT83-DGQKR-F7HPR-844BM"}, // Home N
            {"4",  "W269N-WFGWX-YVC9B-4J6C9-T83GX"}, // Pro
            {"5",  "MH37W-N47XK-V7XM9-C7227-GCQG9"}, // Pro N
            {"6",  "6TP4R-GNPTD-KYYHQ-7B7DP-J447Y"}, // Pro Edu
            {"7",  "YVWGF-BXNMC-HTQYQ-CPQ99-66QFC"}, // Pro Edu N,
            {"8",  "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2"}, // Edu
            {"9",  "2WH4N-8QGBV-H22JP-CT43Q-MDWWJ"}, // Edu N
            {"10", "NPPR9-FWDCX-D2C8J-H872K-2YT43"}, // Enterprise
            {"11", "M7XTQ-FN8P6-TTKYV-9D4CC-J462D"}, // LTSC 2021
            {"12", "QPM6N-7J2WJ-P88HH-P3YRH-YY74H"}, // IoT LTSC
    };

    if (Confirmation() == -1) { MainMenu(); }
    else {
        auto res = w10_keys.find(std::to_string(option));
        if (res != w10_keys.end()) { ActivateWindows(res->second, (option == 11), (option == 12), 1); }
    }
}

void Windows7Activation(int option) {
    // Windows 10/11
    std::map<std::string, std::string> w10_keys = {
            {"1", "FJ82H-XT6CR-J8D7P-XQJJ2-GPDD4"}, // Professional
            {"2", "33PXH-7Y6KF-2VJC9-XBBR8-HVTHH"}, // Enterprise
    };

    if (Confirmation() == -1) { MainMenu(); }
    else {
        auto res = w10_keys.find(std::to_string(option));
        if (res != w10_keys.end()) { ActivateWindows(res->second, false, false, 4); }
    }
}


void MainMenu() {
    std::system("cls");
    std::vector<std::string> menu = {
            "-----------------------",
            "| 1. Windows 11 menu  |",
            "| 2. Windows 10 menu  |",
            "| 3. Windows 8 menu   |",
            "| 4. Windows 7 menu   |",
            "-----------------------"
    };

    int value;
    GenerateMessage("Select choice", menu);

    std::cout << ">>> ";
    std::cin >> value;

    // TODO: complete this code (2)
    if (value == 1) { Windows11Menu(); }
    else if (value == 2) { Windows10Menu(); }
    else if (value == 4) { Windows7Menu(); }
    else { std::cout << "This option is not implemented. Wait new release! :)" << std::endl; }
    std::cout << "Press any key to exit from program..." << std::endl;
    std::cin.ignore();
    std::cin.get();
    exit(0);
}


int main() {
    if (IsRunAsAdmin()) {
        int result = std::system(std::string(
                "powershell -Command \"$PSVersionTable.PSVersion.Major -ge 5\" | findstr False >NUL || exit /B 1").c_str());
        // 0 - powershell 5+ needed
        if (result == 0) {
            std::cout << "For using this program, you need install Powershell 5.1 version or higher"
                         "\nFollow this link: https://learn.microsoft.com/en-us/powershell/scripting"
                         "/windows-powershell/wmf/setup/install-configure?view=powershell-5.1#install"
                         "-wmf-51-for-windows-server-2008-r2-and-windows-7"
                      << std::endl;
            std::cout << "Press any key to exit from program..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            exit(0);
        }
        MainMenu();
    } else {
        if (Confirmation("Run program with administrative rights\nY - Run\nN - Exit") != -1) {
            char buffer[MAX_PATH];
            GetModuleFileName(nullptr, buffer, MAX_PATH);
            std::string runCommand = fmt::format("powershell -Command \"Start-Process '{}' -Verb RunAs\"",
                                                 (std::string) buffer);
            std::system(runCommand.c_str());
        } else { exit(0); }
    }
}
