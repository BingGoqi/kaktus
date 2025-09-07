#include <windows.h>
#include <string>

//��������صĴ��ھ��
HWND WorkerW1 = nullptr;
HWND WorkerW2 = nullptr;
HWND Program = nullptr;

//�������
//EnumWindows�Ļص�����
inline BOOL CALLBACK EnumWindowsProc(_In_ HWND TopHandle, _In_ LPARAM topparamhandle)
{
	if (TopHandle == nullptr) return false;
	HWND def = FindWindowEx(TopHandle, nullptr, L"SHELLDLL_DefView", nullptr);
	if (def != nullptr)
	{
		wchar_t str[256];
		GetClassName(TopHandle, str, 256);
		std::wstring s1 = str;//��鴰���Ƿ��WorkerW
		if (s1 == L"WorkerW")
		{
			WorkerW1 = TopHandle;
			return false;
		}
		//ShowWindow(TopHandle, SW_HIDE);
	}
	return true;
}
//��������Ļ���
//��ȡ������صľ��
bool SetDeskEnvironment()
{
	//��ȡProgram���
	EnumWindows(EnumWindowsProc, (LPARAM)nullptr);
	Program = FindWindow(L"Progman", L"Program Manager");
	if (WorkerW1 == nullptr)
	{
		SendMessage(Program, 0x052c, 0x0D, 0);
		SendMessage(Program, 0x052c, 0x0D, 1);//������Ϣ����Windows����Worker
		EnumWindows(EnumWindowsProc, (LPARAM)nullptr);
	}
	Program = FindWindow(L"Progman", L"Program Manager");
	WorkerW2 = FindWindowEx(0, WorkerW1, L"WorkerW", nullptr);
	return true;
}
int main()
{
	SetDeskEnvironment();
	ShowWindow(WorkerW2, 1);
	HWND hWall = FindWindow(L"Notepad", L"MKDL.txt - ���±�");
	long wl = GetWindowLong(hWall, GWL_STYLE);
	wl &= ~WS_POPUP;
	wl |= WS_CHILD;
	SetWindowLong(hWall, GWL_STYLE, wl);
	//SetWindowLong(WorkerW1,GWL_EXSTYLE,GetWindowLong(WorkerW1,GWL_EXSTYLE)|WS_EX_LAYERED|WS_EX_TRANSPARENT);
	//SetLayeredWindowAttributes(WorkerW2, 0, 255, LWA_ALPHA);
	SetParent(hWall, WorkerW2);
	printf("%lx", (long)WorkerW1);
	return (long)WorkerW2;
}