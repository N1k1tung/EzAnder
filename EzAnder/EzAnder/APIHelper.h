// a class to ease WinApi calls
#include <Windows.h>

typedef enum MouseButtons {
	MBLeft,
	MBMiddle,
	MBRight
};

class APIHelper {
	HWND _hwnd;
public:
	APIHelper(HWND hwnd = 0); // all calls will be performer on set wnd

	POINT searchPixel(int left, int top, int w, int h, DWORD color);
	void click(MouseButtons btn, int x, int y);
	void keyPress(char key);
};