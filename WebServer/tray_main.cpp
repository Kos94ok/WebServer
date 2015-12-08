
#include "stdafx.h"
#include "tray.h"

void cTrayIcon::OnMessage(UINT uMsg)
{
	switch (uMsg)
	{
	case WM_LBUTTONUP:
		
		break;
	case WM_RBUTTONUP:
		// here your call GetCursorPos() and track a popupmenu at that position
		break;
	}
}