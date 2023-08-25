#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <shellapi.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <locale>
#include <codecvt>

void Windows1110Activation(int option);
void MainMenu();

int Confirmation()
{
	std::string temp;
	std::cout << "Are you sure you want to activate Windows?\nY - activate Windows\nN - back to menu" << std::endl;
	std::cout << ">>> ";
	std::cin >> temp;
	std::transform(temp.begin(), temp.end(), temp.begin(), [](int c) { return std::tolower(c); });

	// 0 - yes, -1 - no
	return (temp == "y") ? 0 : -1;

}

void printCenteredLine(const std::string& text, char filler, int totalWidth)
{
	int padding = std::max(0, (totalWidth - static_cast<int>(text.length())) / 2);
	auto remainingPadding = totalWidth - text.length() - padding;
	std::string paddingStr(padding, filler);
	std::string remainingPaddingStr(remainingPadding, filler);
	std::cout << paddingStr << text << remainingPaddingStr << std::endl;
}

void printWrappedText(const std::string& text, const int lineWidth, char filler)
{
	size_t startPos = 0;
	while (startPos < text.length())
	{
		size_t endPos = startPos + lineWidth;
		if (endPos < text.length() && text[endPos] != ' ') { while (endPos > startPos && text[endPos] != ' ') { --endPos; } }
		std::string line = text.substr(startPos, endPos - startPos);
		printCenteredLine(line, filler, lineWidth);
		startPos = endPos + 1;
	}
}

void GenerateMessage(std::string title = "Text", std::vector<std::string> arr = {})
{
	const int lineWidth = 120;
	const int totalWidth = std::max(lineWidth, static_cast<int>(title.length()));

	std::string separator(totalWidth, '-');

	printCenteredLine(separator, '*', totalWidth);
	printCenteredLine(title, ' ', totalWidth);

	for (const std::string& str : arr) { printWrappedText(str, lineWidth, ' '); }

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


int RunSlmgr(std::wstring param) {
	std::wstring scriptRelativePath = L"System32\\slmgr.vbs " + param;

	WCHAR fullPath[MAX_PATH];
	swprintf(fullPath, MAX_PATH, L"%s\\%s", _wgetenv(L"windir"), scriptRelativePath.c_str());
	HINSTANCE result = ShellExecuteW(
		NULL,
		L"open",
		L"C:\\Windows\\System32\\cscript.exe",
		fullPath,
		NULL,
		SW_SHOWMINNOACTIVE
	);

	// 0 - false, 1 - true
	if ((INT_PTR)result <= 32)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::cout << "[ERROR] slmgr with params " << converter.to_bytes(param) << " failed. Error code " << (INT_PTR)result << std::endl;
		return 0;
	}
	else
	{
		return 1;
	}
}


int ActivateWindows(const std::wstring productkey, bool ltsc) {
	std::wstring productKey = L"/ipk " + productkey;

	RunSlmgr(L"/upk");
	if (ltsc)
	{
		RunSlmgr(L"/rilc");
		RunSlmgr(L"/upk");
		RunSlmgr(L"/ckms");
		RunSlmgr(L"/cpky");
	}

	if (RunSlmgr(productKey) == 1)
	{
		std::cout << "Activation completed." << std::endl;
	};
	return 0;
}


void Windows1110Activation(int option)
{
	// Windows 10/11
	std::map<std::string, std::wstring> w10_keys =
	{
		{"1", L"W269N-WFGWX-YVC9B-4J6C9-T83GX"}, // professional
		{"2", L"NPPR9-FWDCX-D2C8J-H872K-2YT43"}, // enterprise
		{"3", L"TX9XD-98N7V-6WMQ6-BX7FG-H8Q99"}, // home
		{"4", L"NPPR9-FWDCX-D2C8J-H872K-2YT43"}, // education
		{"5", L"M7XTQ-FN8P6-TTKYV-9D4CC-J462D"}, // ltsc
	};


	int result = Confirmation();
	if (result == -1)
	{
		MainMenu();
	}
	else
	{
		if (option <= 6)
		{
			auto res = w10_keys.find(std::to_string((option == 5) ? 2 : (option == 6) ? 1 : option));
			if (res != w10_keys.end()) { ActivateWindows(res->second, false); }
		}
		else
		{
			std::cout << "This option is not implemented. Wait new release :)" << std::endl;
		}
	}
}


void MainMenu()
{
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


int main()
{
	MainMenu();
	return 0;
}