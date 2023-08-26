#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <shellapi.h>
#include <vector>
#include <map>
#include <codecvt>
#include <filesystem>

void Windows1110Activation(int option);

void MainMenu();

bool IsRunAsAdmin() {
    BOOL isAdmin = FALSE;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    PSID adminGroup;

    if (AllocateAndInitializeSid(&ntAuthority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0, &adminGroup)) {

        if (!CheckTokenMembership(nullptr, adminGroup, &isAdmin)) {
            isAdmin = FALSE;
        }

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

void printCenteredLine(const std::string &text, char filler, int totalWidth) {
    int padding = std::max(0, (totalWidth - static_cast<int>(text.length())) / 2);
    auto remainingPadding = totalWidth - text.length() - padding;
    std::string paddingStr(padding, filler);
    std::string remainingPaddingStr(remainingPadding, filler);
    std::cout << paddingStr << text << remainingPaddingStr << std::endl;
}

void printWrappedText(const std::string &text, const int lineWidth, char filler) {
    size_t startPos = 0;
    while (startPos < text.length()) {
        size_t endPos = startPos + lineWidth;
        if (endPos < text.length() && text[endPos] != ' ') {
            while (endPos > startPos && text[endPos] != ' ') { --endPos; }
        }
        std::string line = text.substr(startPos, endPos - startPos);
        printCenteredLine(line, filler, lineWidth);
        startPos = endPos + 1;
    }
}

void GenerateMessage(const std::string &title = "Text", const std::vector<std::string> &arr = {}) {
    const int lineWidth = 120;
    const int totalWidth = std::max(lineWidth, static_cast<int>(title.length()));

    std::string separator(totalWidth, '-');

    printCenteredLine(separator, '*', totalWidth);
    printCenteredLine(title, ' ', totalWidth);

    for (const std::string &str: arr) { printWrappedText(str, lineWidth, ' '); }

    printCenteredLine(separator, '-', totalWidth);
}

void Windows11Menu() {
    std::vector<std::string> choices =
            {
                    "------------------------------------------------------------------------",
                    "| 1. Windows 11 Home to Windows 11 Professional                           |",
                    "| 2. Windows 11 Home to Windows 11 Enterprise                             |",
                    "------------------------------------------------------------------------",
                    "| 3. Windows 11 Professional to Windows 11 Home                           |",
                    "| 4. Windows 11 Professional to Windows 11 Education                      |",
                    "| 5. Windows 11 Professional to Windows 11 Enterprise                     |",
                    "| 6. Windows 11 Professional N to Windows 11 Professional                 |",
                    "------------------------------------------------------------------------",
                    "| 7. Windows 11 Education to Windows 11 Professional                      |",
                    "------------------------------------------------------------------------",
                    "| 8. Windows 11 Home to Windows 11 Professional to Windows 11 Education   |",
                    "------------------------------------------------------------------------",
                    "| 9. Windows 11 to Windows 11 LTSC                                        |",
            };
    GenerateMessage("Select choice", choices);

    int value;
    std::cout << ">>> ";
    std::cin >> value;
    Windows1110Activation(value);
}

void Windows10Menu() {
    std::vector<std::string> choices =
            {
                    "------------------------------------------------------------------------",
                    "| 1. Windows 10 Home to Windows 10 Professional                           |",
                    "| 2. Windows 10 Home to Windows 10 Enterprise                             |",
                    "------------------------------------------------------------------------",
                    "| 3. Windows 10 Professional to Windows 10 Home                           |",
                    "| 4. Windows 10 Professional to Windows 10 Education                      |",
                    "| 5. Windows 10 Professional to Windows 10 Enterprise                     |",
                    "| 6. Windows 10 Professional N to Windows 10 Professional                 |",
                    "------------------------------------------------------------------------",
                    "| 7. Windows 10 Education to Windows 10 Professional                      |",
                    "------------------------------------------------------------------------",
                    "| 8. Windows 10 Home to Windows 10 Professional to Windows 10 Education   |",
                    "------------------------------------------------------------------------",
                    "| 9. Windows 10 to Windows 10 LTSC                                        |",
            };
    GenerateMessage("Select choice", choices);

    int value;
    std::cout << ">>> ";
    std::cin >> value;
    Windows1110Activation(value);
}

/*int runSlmgr(const std::wstring &param) {
    std::wstring scriptRelativePath = L"System32\\slmgr.vbs " + param;

    WCHAR fullPath[MAX_PATH];
    swprintf(fullPath, MAX_PATH, L"%s\\%s", _wgetenv(L"windir"), scriptRelativePath.c_str());
    HINSTANCE result = ShellExecuteW(
            nullptr,
            L"open",
            L"C:\\Windows\\System32\\cscript.exe",
            fullPath,
            nullptr,
            SW_SHOWMINNOACTIVE
    );

    // 0 - false, 1 - true
    if ((INT_PTR) result <= 32) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::cout << "[ERROR] slmgr with params " << converter.to_bytes(param) << " failed. Error code "
                  << (INT_PTR) result << std::endl;
        return 0;
    } else {
        return 1;
    }
}*/


int ActivateWindows(const std::string &productKey, const bool &isLTSC) {

    std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /upk)");
    if (isLTSC) {
        char buffer[MAX_PATH];
        GetModuleFileName(nullptr, buffer, MAX_PATH);

        std::string ProgramPath = ((std::string) buffer).substr(0, ((std::string) buffer).find_last_of("\\/"));
        if (std::filesystem::exists(ProgramPath + "\\files")) {
            std::string runCommand =
                    R"(powershell -Command "Expand-Archive -Force ')" + ProgramPath + R"(\files\skus.zip'")";
            std::system(runCommand.c_str());
            runCommand = R"(powershell "Copy-Item -Path .\skus -Destination %windir%\system32\spp\tokens -Recurse")";
            std::system(runCommand.c_str());

            std::cout << "Cleanup temporally files" << std::endl;
            runCommand = "rd \"" + ProgramPath + R"("\skus /s /q)";
            std::system(runCommand.c_str());

            std::cout << "Registering SKU's, please wait..." << std::endl;
            runCommand = R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /rilc)";
            std::system(runCommand.c_str());

            std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /upk)");
            std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /ckms)");
            std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /cpky)");

            std::cout << "Activating Windows..." << std::endl;
            runCommand = R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /ipk )" + productKey;
            std::system(runCommand.c_str());
            std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /skms kms.loli.best)");
            std::system(R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /ato)");

            std::cout << "Done!" << std::endl;
            return 0;
        } else {
            std::cout << "Couldn't find files directory with needed files, "
                      << "please download program from github "
                      << "https://github.com/Snaky1a/Windows-Product-Key-Switcher/releases"
                      << std::endl;
            return -1;
        }
    }

    std::string runCommand = R"(cscript.exe //Nologo //B C:\Windows\System32\slmgr.vbs /ipk )" + productKey;
    if (std::system(runCommand.c_str()) == 0) {
        std::cout << "Activation completed." << std::endl;
    };
    return 0;
}


void Windows1110Activation(int option) {
    // Windows 10/11
    std::map<std::string, std::string> w10_keys =
            {
                    {"1",  "W269N-WFGWX-YVC9B-4J6C9-T83GX"}, // professional
                    {"2",  "NPPR9-FWDCX-D2C8J-H872K-2YT43"}, // enterprise
                    {"3",  "TX9XD-98N7V-6WMQ6-BX7FG-H8Q99"}, // home
                    {"4",  "NPPR9-FWDCX-D2C8J-H872K-2YT43"}, // education
                    {"5",  "M7XTQ-FN8P6-TTKYV-9D4CC-J462D"}, // LTSC,
                    {"10", "QPM6N-7J2WJ-P88HH-P3YRH-YY74H"}, // IoT LTSC
            };


    int result = Confirmation();
    if (result == -1) { MainMenu(); }
    else {
        if (option <= 6 or option == 9) {
            auto res = w10_keys.find(
                    std::to_string((option == 5) ? 2 : (option == 6) ? 1 : (option == 9) ? 5 : option));
            if (res != w10_keys.end()) { ActivateWindows(res->second, (option == 9)); }
        } else { std::cout << "This option is not implemented. Wait new release :)" << std::endl; }
    }
}


void MainMenu() {
    std::system("cls");
    std::vector<std::string> menu =
            {
                    "---------------------",
                    "| 1. Windows 11 menu  |",
                    "| 2. Windows 10 menu  |",
                    "| 3. Windows 8 menu   |",
                    "| 4. Windows 7 menu   |",
            };

    int value;
    GenerateMessage("Select choice", menu);

    std::cout << ">>> ";
    std::cin >> value;

    // TODO: complete this code (2)
    if (value == 1) { Windows11Menu(); }
    else if (value == 2) { Windows10Menu(); }
    else { std::cout << "This option is not implemented. Wait new release! :)" << std::endl; }

    std::cout << "Press any key to exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();
    exit(0);
}


int main() {
    if (IsRunAsAdmin()) {
        MainMenu();
    } else {
        if (IsRunAsAdmin()) {
            MainMenu();
        } else {
            if (Confirmation("Run program with administrative rights\nY - Run\nN - Exit") != -1) {
                char buffer[MAX_PATH];
                GetModuleFileName(nullptr, buffer, MAX_PATH);
                std::string runCommand =
                        R"(powershell -Command "Start-Process ')" + (std::string) buffer + "' -Verb RunAs";
                std::system(runCommand.c_str());
            } else { exit(0); }
        }
    }
}