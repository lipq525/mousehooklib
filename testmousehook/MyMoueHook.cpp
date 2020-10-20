//#include "stdafx.h"
#include "pch.h"
#include "MyMouseHook.h"

#pragma data_seg("mydata")
HWND hWnd_Mouse=NULL; //设置需要捕获鼠标消息的窗口句柄,即所有鼠标消息都会另外发送一份消息到该窗口
#pragma data_seg()

#pragma comment(linker,"/SECTION:mydata,RWS")

HHOOK hHook=NULL; //鼠标钩子句柄
HINSTANCE hInstanceHook=NULL; //插入保存DLL实例句柄

extern "C" LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode < 0)
    {
        CallNextHookEx(hHook, nCode, wParam, lParam);
        return 0;
    }
    if(nCode>=0)
    {
        LPMSG msg=(LPMSG)lParam;
        if(msg->message==WM_MOUSEMOVE || msg->message==WM_NCMOUSEMOVE)
        {
            PostMessage(hWnd_Mouse, WM_MY_MOUSEMOVE, 0, 0);
            //			PostMessage(hWnd_Mouse, WM_MOUSEMOVE, 0, 0);
        }
    }
    return CallNextHookEx(hHook,nCode,wParam,lParam); //继续传递钩子信息
}

// CMyMoueHook

CMyMouseHook::CMyMouseHook()
{
}

CMyMouseHook::~CMyMouseHook()
{
    //	StopHook();
}


// CMyMoueHook 成员函数


BOOL CMyMouseHook::StartHook(HWND hWnd)
{
    if(hHook==NULL)
    {
        hHook=SetWindowsHookExW(WH_GETMESSAGE,(HOOKPROC)MouseProc,hInstanceHook,0);
        if(hHook!=NULL)
        {
            hWnd_Mouse=hWnd; //设置需要捕获鼠标消息的窗口句柄
            return TRUE;
        }
    }else
    {
        if(hWnd_Mouse!=NULL)
        {
            if(hWnd_Mouse==hWnd)
            {
                return FALSE; //该窗口句柄已经hook了
            }else
            {
                hWnd_Mouse=hWnd; //设置需要捕获鼠标消息的窗口句柄,这样将会覆盖前一个句柄
                return TRUE;
            }
        }
    }

    return FALSE; //failed to set hook
}

BOOL CMyMouseHook::StopHook(HWND hWnd)
{
    if(hWnd!=hWnd_Mouse || hWnd==NULL){return FALSE;}
    BOOL bResult=UnhookWindowsHookEx(hHook);
    if(bResult){hWnd_Mouse=NULL;hHook=NULL;}
    return bResult;
}
