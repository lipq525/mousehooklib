#pragma once

#define WM_MY_MOUSEMOVE (WM_USER+10)

// CMyMoueHook ����Ŀ��

class AFX_EXT_CLASS CMyMouseHook:public CObject
{
public:
    CMyMouseHook();
    virtual ~CMyMouseHook();

public:
    BOOL StartHook(HWND hWnd); //��װ���Ӻ���
    BOOL StopHook(HWND hWnd); //ж�ع��Ӻ���
};

