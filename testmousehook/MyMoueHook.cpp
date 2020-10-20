//#include "stdafx.h"
#include "pch.h"
#include "MyMouseHook.h"

#pragma data_seg("mydata")
HWND hWnd_Mouse=NULL; //������Ҫ���������Ϣ�Ĵ��ھ��,�����������Ϣ�������ⷢ��һ����Ϣ���ô���
#pragma data_seg()

#pragma comment(linker,"/SECTION:mydata,RWS")

HHOOK hHook=NULL; //��깳�Ӿ��
HINSTANCE hInstanceHook=NULL; //���뱣��DLLʵ�����

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
    return CallNextHookEx(hHook,nCode,wParam,lParam); //�������ݹ�����Ϣ
}

// CMyMoueHook

CMyMouseHook::CMyMouseHook()
{
}

CMyMouseHook::~CMyMouseHook()
{
    //	StopHook();
}


// CMyMoueHook ��Ա����


BOOL CMyMouseHook::StartHook(HWND hWnd)
{
    if(hHook==NULL)
    {
        hHook=SetWindowsHookExW(WH_GETMESSAGE,(HOOKPROC)MouseProc,hInstanceHook,0);
        if(hHook!=NULL)
        {
            hWnd_Mouse=hWnd; //������Ҫ���������Ϣ�Ĵ��ھ��
            return TRUE;
        }
    }else
    {
        if(hWnd_Mouse!=NULL)
        {
            if(hWnd_Mouse==hWnd)
            {
                return FALSE; //�ô��ھ���Ѿ�hook��
            }else
            {
                hWnd_Mouse=hWnd; //������Ҫ���������Ϣ�Ĵ��ھ��,�������Ḳ��ǰһ�����
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
