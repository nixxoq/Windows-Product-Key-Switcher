#define NOMINMAX
#define w10productkey "W269N-WFGWX-YVC9B-4J6C9-T83GX"
#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <shellapi.h>
#include <vector>

void WindowsActivation();

int Confirmation()
{
	std::string temp;
	std::cout << "Are you sure you want to activate Windows?\nY - activate Windows\nN - back to menu" << std::endl;
	std::cout << ">>> ";
	std::cin >> temp;
	std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c) { return std::tolower(c); });

	// 0 - yes, -1 - no
	std::cout << temp << std::endl;
	return 0 ? temp == "y" : -1;

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
	WindowsActivation();
}


int ActivateWindows(const std::string productkey) {
	std::string productKey = "/ipk " + productkey;

	HINSTANCE hInstance = ShellExecuteA(nullptr, "open", "C:\\Windows\\System32\\slmgr.vbs", productKey.c_str(), nullptr, SW_HIDE);
	if ((intptr_t)hInstance > 32)
	{
		WaitForSingleObject(hInstance, INFINITE);
		CloseHandle(hInstance);
		return 0;
	}
	else
	{
		DWORD error = GetLastError();
		printf("Error: %d\n", error);
		return 1;
	}
}


void WindowsActivation()
{
	Confirmation();
	//ActivateWindows(w10productkey);
}


int main()
{
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

	std::cout << "Press any key to exit..." << std::endl;
	std::cin.ignore();
	std::cin.get();

	return 0;
}