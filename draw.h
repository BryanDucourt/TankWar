//
// Created by 微星 on 2020/4/2.
//
/**
 * @file draw.h
 * @brief 绘图相关函数
 * @details 包含各个方向坦克绘制、砖块绘制、双缓冲、炮弹绘制等api
 * @author Bryan Ducourt
 * @date 2020/04/02
 */
#ifndef TANK_DRAW_H
#define TANK_DRAW_H

#include "tank.h"
#include <windows.h>
#endif //TANK_DRAW_H
static HBRUSH YellowBrush;
static HBRUSH BlackBrush;
static HBRUSH RedBrush;
static HBRUSH GreenBrush;
static HBRUSH WhiteBrush;
BOOL drawEnemyTank (struct tank* enemyTank,HDC hdc);
BOOL drawPlayerTank (struct tank* plrTank,HDC hdc);
BOOL drawBomb(HDC hdc);
HDC hdcPlayerTank,hdcEnemyTank,hdcBomb;
HBITMAP bitmapPlayerTank,bitmapEnemyTank,bitmapBomb;

/**
 * @brief 绘制玩家坦克
 * @param plrTank 玩家坦克指针
 * @return
 */
BOOL drawPlayerTank (struct tank* plrTank,HDC hdc)
    {
        switch (plrTank->tkDirect)
            {
                case UP:
                        bitmapPlayerTank=LoadImageA(NULL,TEXT("..\\art material\\player\\player_tank_up.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                        break;
                case DOWN:
                        bitmapPlayerTank=LoadImageA(NULL,TEXT("..\\art material\\player\\player_tank_down.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                        break;
                case LEFT:
                        bitmapPlayerTank=LoadImageA(NULL,TEXT("..\\art material\\player\\player_tank_left.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                        break;
                case RIGHT:
                        bitmapPlayerTank=LoadImageA(NULL,TEXT("..\\art material\\player\\player_tank_right.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                        break;
            }
        hdcPlayerTank=CreateCompatibleDC(hdc);

        SelectObject(hdcPlayerTank,bitmapPlayerTank);
        BitBlt(hdc,plrTank->tkLocateLU[0],playerTank.tkLocateLU[1],plrTank->tkLocateRD[0],plrTank->tkLocateRD[1],hdcPlayerTank,0,0,SRCCOPY);
    }
BOOL drawEnemyTank (struct tank* enemyTank,HDC hdc)
    {
        struct tank * temp;
        temp=enemyTank;
        do
            {
               if (enemyTank->tkHp != 0)
                   {
                       switch (enemyTank->tkDirect)
                           {
                               case UP:
                                   bitmapEnemyTank=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_tank_up.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                                   break;
                               case DOWN:
                                   bitmapEnemyTank=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_tank_down.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                                   break;
                               case LEFT:
                                   bitmapEnemyTank=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_tank_left.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                                   break;
                               case RIGHT:
                                   bitmapEnemyTank=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_tank_right.bmp"),IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
                                   break;
                           }
                       hdcEnemyTank=CreateCompatibleDC(hdc);
                       SelectObject(hdcEnemyTank,bitmapEnemyTank);
                       BitBlt(hdc,enemyTank->tkLocateLU[0],enemyTank->tkLocateLU[1],enemyTank->tkLocateRD[0],enemyTank->tkLocateRD[1],hdcEnemyTank,0,0,SRCCOPY);
                       enemyTank = enemyTank->nextTank;
                   }
                else
                    enemyTank=deleteTank(enemyTank);
            }while (enemyTank != NULL);
        enemyTank=temp;
    }
BOOL drawBomb(HDC hdc)
    {
        hdcBomb=CreateCompatibleDC(hdc);
        struct bomb * temp;
        temp=bombStorage;
        SelectObject(hdc,BlackBrush);
        do
            {
                switch (bombStorage->bombProducer)
                    {
                        case 0:
                            switch (bombStorage->bmDirect)
                                {
                                    case UP:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\player\\player_bomb_up.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                    case DOWN:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\player\\player_bomb_down.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                    case RIGHT:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\player\\player_bomb_right.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                    case LEFT:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\player\\player_bomb_left.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                }
                        break;
                        case 1:
                            switch (bombStorage->bmDirect)
                                {
                                    case UP:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_bomb_up.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                    case DOWN:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_bomb_down.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                    case RIGHT:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_bomb_right.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                    case LEFT:
                                        bitmapBomb=LoadImageA(NULL,TEXT("..\\art material\\enemy\\enemy_bomb_left.bmp"),IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
                                    break;
                                }
                        break;
                    }
                SelectObject(hdcBomb,bitmapBomb);
                BitBlt(hdc,bombStorage->bmLocateLU[0],bombStorage->bmLocateLU[1],bombStorage->bmLocateRD[0],bombStorage->bmLocateRD[1],hdcBomb,0,0,SRCCOPY);
                bombStorage=bombStorage->nextBomb;
            }while (bombStorage != NULL);
        bombStorage=temp;
    }
