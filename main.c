/**
 * @file main.c
 * @brief 主函数
 * @details 初始化游戏内容，预加载地图
 * @author Bryan Ducourt
 * @date 2020/04/01
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <afxres.h>
#include "draw.h"
//todo 20/04/01 初始化函数，坦克模型设计，炸弹库系统设计
//done 20/04/02 坦克模型设计，坦克移动，画面绘制
//todo 20/04/14 添加炮弹线程
#define WM_SKIP (WM_USER+0)
BOOL bombFire=FALSE;
HANDLE bombThread,msg_ProcStart,msg_ProcFinish;
int kbDirect=0,bombFlag=0;
DWORD WINAPI bombProc(LPVOID lpParameter);
DWORD WINAPI tankMovProc(LPVOID lpParameter);
DWORD WINAPI tankFireProc(LPVOID lpParameter);
void init();



LRESULT CALLBACK WndProc (HWND,UINT,WPARAM,LPARAM);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR lpCmdLine,int nCmdShow)
    {
        FreeConsole();
        extern struct bomb* bombStorage;
        struct bomb* temp=(struct bomb*)malloc(sizeof(struct bomb));
        bombStorage=temp;
        static TCHAR tcWndClassName[]=TEXT("mainWnd");
        HWND hWnd;
        MSG msg;
        WNDCLASS wndclass;
        msg_ProcStart=CreateEventA(NULL,TRUE,FALSE,NULL);
        msg_ProcFinish=CreateEventA(NULL,FALSE,FALSE,NULL);
        init();



        wndclass.style=CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc=WndProc;
        wndclass.cbClsExtra=0;
        wndclass.cbWndExtra=0;
        wndclass.hInstance=hInstance;
        wndclass.hIcon=LoadIconA(NULL,IDI_APPLICATION);
        wndclass.lpszClassName=tcWndClassName;
        wndclass.lpszMenuName=NULL;
        wndclass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
        wndclass.hCursor=LoadCursorA(NULL,IDC_ARROW);
        RegisterClass(&wndclass);
        hWnd=CreateWindow(tcWndClassName,TEXT("tank war"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,918,947,NULL,NULL,hInstance,NULL);
        CreateThread(NULL,0,tankMovProc,NULL,0,NULL);
        CreateThread(NULL,0,tankFireProc,NULL,0,NULL);
        ShowWindow(hWnd,nCmdShow);
        UpdateWindow(hWnd);
        while (GetMessageA(&msg,NULL,0,0))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        return msg.wParam;
    }
    LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
        {
            HDC hdc;
            PAINTSTRUCT ps;
            RECT rect;
            GetClientRect(hwnd,&rect);
            switch (message)
                {
                    case WM_CREATE:
                        bombThread=CreateThread(NULL, 0, bombProc, (LPVOID)hwnd,0, NULL);
                        YellowBrush=CreateSolidBrush(RGB(255,255,0));
                        BlackBrush=CreateSolidBrush(RGB(0,0,0));
                        RedBrush=CreateSolidBrush(RGB(255,0,0));
                        GreenBrush=CreateSolidBrush(RGB(0,255,0));
                        WhiteBrush=CreateSolidBrush(RGB(255,255,255));

                        break;
                    case WM_PAINT:
                        SetEvent(msg_ProcStart);
                        hdc=BeginPaint(hwnd,&ps);
                        HDC hdcMem=CreateCompatibleDC(hdc);
                        HBITMAP bitmapMem=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
                        SelectObject(hdcMem,bitmapMem);
                        //aiFireCheck();
                        movBomb();

                        tankHitCheck(enemyTankStorage);
                        tankHitCheck(&playerTank);
                        drawPlayerTank(&playerTank,hdcMem);
                        drawEnemyTank(enemyTankStorage,hdcMem);
                        drawBomb(hdcMem);
                        WaitForSingleObject(msg_ProcFinish,50);
                        BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
                        EndPaint(hwnd,&ps);
                        DeleteDC(hdcMem);
                        DeleteObject(bitmapMem);
                        bombFlag=0;
                        break;
                    case WM_KEYDOWN:
                        bombFlag=1;
                        bombFire=FALSE;
                        switch (wParam)
                            {
                                case VK_UP:
                                    kbDirect=UP;
                                break;
                                case VK_DOWN:
                                    kbDirect=DOWN;
                                break;
                                case VK_RIGHT:
                                    kbDirect=RIGHT;
                                break;
                                case VK_LEFT:
                                    kbDirect=LEFT;
                                break;
                                case VK_SPACE:
                                    bombFire=TRUE;
                                    kbDirect=5;
                                    break;
                            }
                            tkCtrl(&playerTank,kbDirect,bombFire);
                        InvalidateRect(hwnd,NULL,FALSE);
                        Sleep(30);
                        kbDirect=NULL;
                        break;
                    case WM_DESTROY:
                        PostQuitMessage(0);
                        break;
                    default:
                        return DefWindowProcA(hwnd,message,wParam,lParam);
                }
            return 0;
        }
        /**
         *@brief 初始化函数，负责初始化炮弹库、玩家单位和电脑单位 todo 以及障碍物系统
         *
         */
void init()
    {
        createPlayerTank();
        createEnemyTank();
        bombStorage->bmLocateRD[0]=0;
        bombStorage->bmLocateRD[1]=0;
        bombStorage->bmLocateLU[0]=0;
        bombStorage->bmLocateLU[1]=0;
        bombStorage->bombProducer=2;
        bombStorage->bmDirect=0;
        bombStorage->nextBomb=NULL;
        bombStorage->prevBomb=NULL;             
        bombStorage->bmMoveCheck=FALSE;
    }
    /**
     * @brief 用于维持画面刷新
     * @param lpParameter
     * @return
     */
DWORD WINAPI bombProc(LPVOID lpParameter)
    {
        HWND hwnd=(HWND)lpParameter;
        int i=1;
        while(i==1)
            {
                switch (bombFlag)
                    {
                        case 1:
                            break;
                        case 0:
                            InvalidateRect(hwnd, NULL, FALSE);
                        Sleep(30);
                    }

            }
    }
    /**
     * @brief 用于控制电脑单位移动
     * @details WM_PAINT消息开始执行时发送msg_ProcStart消息，线程接收到消息后开始进行运算，运算完成后重置msg_ProcStart消息并发送msg_ProcFinish消息
     * @param lpParameter
     * @return
     */
DWORD WINAPI tankMovProc(LPVOID lpParameter)
    {
        while(1)
            {
                WaitForSingleObject(msg_ProcStart, INFINITE);

                aiFireCheck(&playerTank);
                //printf("1\n");
                ResetEvent(msg_ProcStart);
                SetEvent(msg_ProcFinish);

            }

    }
    /**
     * @brief 控制电脑单位开火间隔
     * @param lpParameter
     * @return
     */
DWORD WINAPI tankFireProc(LPVOID lpParameter)
    {
        struct tank* temp=enemyTankStorage;
        while (1)
            {
                while (enemyTankStorage!=NULL)
                    {
                        enemyTankStorage->bomb=TRUE;
                        enemyTankStorage=enemyTankStorage->nextTank;
                    }
                enemyTankStorage=temp;
                Sleep(2000);
            }
    }