// 参考代码：https://www.bilibili.com/video/BV1HZ4y1978a
// 参考代码：https://blog.csdn.net/XIeTTTT/article/details/119190669

#include <windows.h>
#include <stdio.h>
#include<string>

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0)
	{
		//找到它下一个窗口，类名为WorkerW,隐藏它
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);

		return FALSE;
	}
	return TRUE;
}

void MiniSizeAllWindows() 
{
	char szTitle[MAX_PATH] = { 0 };
	char szClass[MAX_PATH] = { 0 };
	int nMaxCount = MAX_PATH;

	LPSTR lpClassName = szClass;
	LPSTR lpWindowName = szTitle;

	while (true)
	{
		HWND hwnd = GetForegroundWindow();//使hwnd代表最前端的窗口
		GetWindowTextA(hwnd, lpWindowName, nMaxCount);
		GetClassNameA(hwnd, lpClassName, nMaxCount);
		std::string str = (std::string)lpClassName;

		if (str.find("TrayWnd") != std::string::npos) {
			break;
		}
		PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0); //最小化窗口
	}
}

int main(int argc, char *argv[])
{

	LPCWSTR lpParameter = L" 视频.mp4 -noborder -x 1920 -y 1080 -volume 10 -loop 0"; //-an:静音 -fs:全屏 -volume vol[0-100] 声音大小
	STARTUPINFO si{ 0 };
	PROCESS_INFORMATION pi{ 0 };
	MiniSizeAllWindows(); //最小化所有窗口 不需要注释即可
	//if (CreateProcess(L"D:\\Program Files\\ffmpeg\\bin\\ffplay.exe", (LPWSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	if (CreateProcess(L"ffplay.exe", (LPWSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		Sleep(6500);//ms
		HWND hProgman = FindWindow(L"Progman", 0);
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);
		HWND hFfplay = FindWindow(L"SDL_app", 0);
		SetParent(hFfplay, hProgman);
		EnumWindows(EnumWindowsProc, 0);
	}

	return 0;
}

