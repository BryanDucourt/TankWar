//
// Created by 微星 on 2020/3/30.
//
/**@file tank.h
 * @brief 坦克相关函数
 * @details 包含控制移动函数、坦克参数、炮弹参数、状态判定等API
 * @author Bryan Ducourt
 * @date 2020/03/30
 *
 */
#ifndef TANK_TANK_H
#define TANK_TANK_H

#include <windows.h>
#include <math.h>
#include <time.h>
#endif
#define enemyNum 5
#define Speed 8
#define enemySpeed 2
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
struct tank* enemyTankStorage=NULL;
struct tank playerTank;

/**
 * @struct tank_info
 * @brief 坦克信息结构体 ,定义储存坦克相关参数
 */
struct tank
    {
        int tkDirect;/*!<坦克面对方向*/
        int tkLocateLU[2];/*!<坦克左上角坐标*/
        int tkLocateRD[2];/*!<坦克右下角坐标*/
        int tkHp;/*!<坦克当前生命值*/
        int tkID;
        float tkCenter[2];
        BOOL move;/*!<坦克是否能向面向方向移动*/
        BOOL bomb;/*!<坦克是否开火*/
        struct tank* nextTank;
        struct tank* prevTank;
    };
/**
 * @struct bomb_info
 * @brief 炮弹信息结构体，定义储存炮弹相关参数
 */
struct bomb
    {
        int bombProducer;/*!<炮弹所属坦克id*/
        int bmDirect;/*!<炮弹飞行方向*/
        int bmLocateLU[2];/*!<炮弹左上角坐标*/
        int bmLocateRD[2];/*!<炮弹右下角坐标*/
        BOOL bmMoveCheck;/*!<炮弹是否能继续飞行*/
        float bmCenter[2];
        struct bomb *nextBomb;
        struct bomb *prevBomb;
    };
struct bomb* bombStorage=NULL;
void tkCtrl(struct tank *tankCtrl, int direct, BOOL ifBombFire);
struct bomb* addBomb(struct bomb* stpBombStorage, struct bomb *newBomb);
void createEnemyTank();
void createPlayerTank();
struct bomb* deleteBomb(struct bomb* stpBomb);
void movBomb();
void movAiTank(struct tank* tankCtrl);
BOOL MovCheck(struct tank * tkCheck);
BOOL bombMovCheck(struct bomb* bmCheck);
struct tank* deleteTank(struct tank* stpTank);
void tankHitCheck(struct tank* stpTank);
void aiFireCheck(struct tank *plrTank);
/**
 * @brief 增加炮弹
 * @param stpBombStorage 炮弹库指针
 * @param newBomb 存储待增加炮弹数据
 * @return
 */
struct bomb* addBomb(struct bomb* stpBombStorage, struct bomb *newBomb)
    {
        struct bomb * temp;
        temp=stpBombStorage;
        while (stpBombStorage->nextBomb != NULL)
            {
                stpBombStorage=stpBombStorage->nextBomb;
            }
        stpBombStorage->nextBomb=newBomb;
        newBomb->prevBomb=stpBombStorage;
        return temp;
    }
    /**
     * @brief 创建玩家坦克
     * @param stpTank 玩家坦克指针
     * @return
     */
    void createPlayerTank()
    {
        playerTank.tkLocateRD[0]=550;
        playerTank.tkLocateRD[1]=580;
        playerTank.tkLocateLU[0]=460;
        playerTank.tkLocateLU[1]=490;
        playerTank.tkDirect=DOWN;
        playerTank.tkHp=3;
        playerTank.tkID=0;
        playerTank.tkCenter[0]=(playerTank.tkLocateLU[0]+playerTank.tkLocateRD[0])/2;
        playerTank.tkCenter[1]=(playerTank.tkLocateLU[1]+playerTank.tkLocateRD[1])/2;

    }
    /**
     * @brief 创建敌方坦克
     * @param stpTank 敌方坦克库指针
     * @return
     */
void createEnemyTank()
    {
        extern struct tank* enemyTankStorage;
        struct tank* head,* tail;
        head=(struct tank*)malloc(sizeof(struct tank));
        //tail=(struct tank*)malloc(sizeof(struct tank));

        head->tkLocateLU[0]=0;
        head->tkLocateLU[1]=0;
        head->tkLocateRD[0]=90;
        head->tkLocateRD[1]=90;
        head->tkDirect=DOWN;
        head->tkHp=3;
        head->bomb=FALSE;
        head->nextTank=NULL;
        head->prevTank=NULL;
        head->tkID=1;
        head->tkCenter[0]=(head->tkLocateLU[0]+head->tkLocateRD[0])/2;
        head->tkCenter[1]=(head->tkLocateLU[1]+head->tkLocateRD[1])/2;
        for (int i=1;i<=4;i++)
            {
                struct tank* temp;
                temp=head;
                while(head->nextTank!=NULL)
                    {
                        head=head->nextTank;
                    }
                struct tank* newTank;
                newTank=(struct tank*)malloc(sizeof(struct tank));
                int length=i*202;
                newTank->tkLocateLU[0]=0+length;
                newTank->tkLocateLU[1]=0;
                newTank->tkLocateRD[0]=90+length;
                newTank->tkLocateRD[1]=90;
                newTank->tkDirect=DOWN;
                newTank->tkHp=3;
                newTank->bomb=TRUE;
                newTank->tkID=1;
                newTank->nextTank=NULL;
                newTank->tkCenter[0]=(newTank->tkLocateLU[0]+newTank->tkLocateRD[0])/2;
                newTank->tkCenter[1]=(newTank->tkLocateLU[1]+newTank->tkLocateRD[1])/2;
                newTank->prevTank=head;
                head->nextTank=newTank;
                head=temp;
            }
        enemyTankStorage=head;
    }
/**
 * @brief 控制坦克移动、开火
 * @param tankCtrl  被操作坦克指针
 * @param direct 键盘接收的方向信息
 * @param ifBombFire 用户是否开火
 *
 */
void tkCtrl(struct tank *tankCtrl, int direct, BOOL ifBombFire)
    {
        extern struct bomb* bombStorage;
        struct bomb* newBomb;
        int bombLU[2],bombRD[2];
        tankCtrl->move=MovCheck(tankCtrl);
        if (tankCtrl->move==TRUE || direct!=tankCtrl->tkDirect)
            {
                switch (direct)
                    {
                        case UP:
                            tankCtrl->tkDirect = 0;
                        tankCtrl->tkLocateLU[1] -= Speed;
                        tankCtrl->tkLocateRD[1] -= Speed;
                        break;
                        case DOWN:
                            tankCtrl->tkDirect = 1;
                        tankCtrl->tkLocateLU[1] += Speed;
                        tankCtrl->tkLocateRD[1] += Speed;
                        break;
                        case RIGHT:
                            tankCtrl->tkDirect = 2;
                        tankCtrl->tkLocateLU[0] += Speed;
                        tankCtrl->tkLocateRD[0] += Speed;
                        break;
                        case LEFT:
                            tankCtrl->tkDirect = 3;
                        tankCtrl->tkLocateLU[0] -= Speed;
                        tankCtrl->tkLocateRD[0] -= Speed;
                        break;
                    }
                tankCtrl->tkCenter[0]=(tankCtrl->tkLocateRD[0]+tankCtrl->tkLocateLU[0])/2;
                tankCtrl->tkCenter[1]=(tankCtrl->tkLocateRD[1]+tankCtrl->tkLocateLU[1])/2;
            }
                if (ifBombFire == TRUE)
                    {
                        newBomb = (struct bomb *) malloc(sizeof(struct bomb));

                        newBomb->bmDirect = tankCtrl->tkDirect;
                        newBomb->bombProducer = 0;
                        newBomb->nextBomb = NULL;
                        switch (tankCtrl->tkDirect)
                            {
                                case UP:
                                    bombLU[0] = tankCtrl->tkLocateLU[0] + 30;
                                bombLU[1] = tankCtrl->tkLocateLU[1] - 30;
                                bombRD[0] = tankCtrl->tkLocateLU[0] + 60;
                                bombRD[1] = tankCtrl->tkLocateLU[1];
                                break;
                                case DOWN:
                                    bombLU[0] = tankCtrl->tkLocateRD[0] - 60;
                                bombLU[1] = tankCtrl->tkLocateRD[1];
                                bombRD[0] = tankCtrl->tkLocateRD[0] - 30;
                                bombRD[1] = tankCtrl->tkLocateRD[1] + 30;
                                break;
                                case RIGHT:
                                    bombLU[0] = tankCtrl->tkLocateRD[0];
                                bombLU[1] = tankCtrl->tkLocateRD[1] - 60;
                                bombRD[0] = tankCtrl->tkLocateRD[0] + 30;
                                bombRD[1] = tankCtrl->tkLocateRD[1] - 30;
                                break;
                                case LEFT:
                                    bombLU[0] = tankCtrl->tkLocateLU[0] - 30;
                                bombLU[1] = tankCtrl->tkLocateLU[1] + 30;
                                bombRD[0] = tankCtrl->tkLocateLU[0];
                                bombRD[1] = tankCtrl->tkLocateLU[1] + 60;
                                break;
                            }
                        newBomb->bmLocateLU[0] = bombLU[0];
                        newBomb->bmLocateLU[1] = bombLU[1];
                        newBomb->bmLocateRD[0] = bombRD[0];
                        newBomb->bmLocateRD[1] = bombRD[1];
                        newBomb->bmMoveCheck = bombMovCheck(newBomb);
                        newBomb->bmCenter[0]=(newBomb->bmLocateLU[0]+newBomb->bmLocateRD[0])/2;
                        newBomb->bmCenter[1]=(newBomb->bmLocateLU[1]+newBomb->bmLocateRD[1])/2;
                        bombStorage = addBomb(bombStorage, newBomb);
                    }


    }
    /**
     * @brief 炮弹移动函数
     * @details 首先判断炮弹下一时刻位置上是否有单位，根据判断结果进行移动。若存在单位则炮弹删除，否则炮弹移动到下一个位置
     */
void movBomb()
    {
        int speed;
        struct bomb *temp;
        temp=bombStorage;
        //bombStorage=bombStorage->nextBomb;
        do
            {
                switch (bombStorage->bmMoveCheck=bombMovCheck(bombStorage))
                    {
                        case TRUE:
                            switch (bombStorage->bombProducer)
                                {
                                    case 0:
                                        speed=Speed;
                                    break;
                                    case 1:
                                        speed=enemySpeed+2;
                                    break;
                                }
                            switch (bombStorage->bmDirect)
                                {
                                    case UP:
                                        bombStorage->bmLocateLU[1]-=speed;
                                        bombStorage->bmLocateRD[1]-=speed;
                                        bombStorage->bmCenter[1]-=speed;
                                    break;
                                    case DOWN:
                                        bombStorage->bmLocateLU[1]+=speed;
                                        bombStorage->bmLocateRD[1]+=speed;
                                        bombStorage->bmCenter[1]+=speed;
                                    break;
                                    case RIGHT:
                                        bombStorage->bmLocateLU[0]+=speed;
                                        bombStorage->bmLocateRD[0]+=speed;
                                        bombStorage->bmCenter[0]+=speed;
                                    break;
                                    case LEFT:
                                        bombStorage->bmLocateLU[0]-=speed;
                                        bombStorage->bmLocateRD[0]-=speed;
                                        bombStorage->bmCenter[0]-=speed;
                                    break;

                                }
                        break;
                        case FALSE:
                            bombStorage=deleteBomb(bombStorage);
                            break;
                    }
                    bombStorage=bombStorage->nextBomb;
            }while (bombStorage!=NULL);
        bombStorage=temp;
    }
    /**
     * @brief 判断用户坦克是否可以移动
     * @brief 判断坦克下一时刻位置上是否存在障碍物，若存在，则停留在当前位置，否则移动到下一位置
     * @param tkCheck 用户坦克指针
     * @return 可移动返回TRUE，否则返回FALSE
     */
BOOL MovCheck(struct tank * tkCheck)
    {
        switch (tkCheck->tkDirect)
            {
                case UP:
                    if (tkCheck->tkLocateLU[1]<=Speed)
                        return FALSE;
                    else
                        return TRUE;

                case DOWN:
                    if (900-tkCheck->tkLocateRD[1]<=Speed)
                        return FALSE;
                    else
                        return TRUE;

                case RIGHT:
                    if (900-tkCheck->tkLocateRD[0]<=Speed)
                        return FALSE;
                    else
                        return TRUE;

                case LEFT:
                    if (tkCheck->tkLocateLU[0]<=Speed)
                        return FALSE;
                    else
                        return TRUE;
            }
    }
    /**
     * @brief 判断炮弹是否可以移动
     * @details 逻辑同坦克判断
     * @param bmCheck
     * @return 可移动返回TRUE，否则返回FALSE
     */
BOOL bombMovCheck(struct bomb* bmCheck)
    {
        switch (bmCheck->bmDirect)
            {
                case UP:
                    if(bmCheck->bmLocateLU[1]-Speed<=0)
                        return FALSE;
                    else
                        return TRUE;
                case DOWN:
                    if(bmCheck->bmLocateRD[1]+Speed>=900)
                        return FALSE;
                    else
                        return TRUE;
                case RIGHT:
                    if(bmCheck->bmLocateRD[0]+Speed>=900)
                        return FALSE;
                    else
                        return TRUE;
                case LEFT:
                    if(bmCheck->bmLocateLU[0]-Speed<=0)
                        return FALSE;
                    else
                        return TRUE;

            }

    }
    /**
     * @brief 删除失效炮弹（命中/出界）
     * @param stpBomb 待删除炮弹指针
     * @return 待删除炮弹前一炮弹指针
     */
struct bomb* deleteBomb(struct bomb* stpBomb)
    {
        if (stpBomb->prevBomb != NULL)
            {
                struct bomb *tempPrev, *tempNext;
                tempNext = stpBomb->nextBomb;
                tempPrev = stpBomb->prevBomb;
                if(tempNext!=NULL)
                    {
                        tempNext->prevBomb = tempPrev;
                    }
                tempPrev->nextBomb = tempNext;
                free(stpBomb);
                return tempPrev;
            }
        else
            return stpBomb;
    }
struct tank* deleteTank(struct tank* stpTank)
    {
        struct tank* tempNext,* tempPrev;
        tempNext=stpTank->nextTank;
        tempPrev=stpTank->prevTank;
        if (tempNext!=NULL)
            {
                tempNext->prevTank=tempPrev;
            }
        if (tempPrev!=NULL)
            {
                tempPrev->nextTank=tempNext;
            }
            free(stpTank);
        return tempPrev;
    }
    /**
     * @brief 判断坦克是否被击中
     * @details 敌人和玩家共用 首先判断当前坦克为玩家或是电脑，而后遍历炮弹库判断是否被击中。双方只会被对方发出的炮弹击中
     * @param stpTank
     */
void tankHitCheck(struct tank* stpTank)
    {
        struct bomb * bmTemp=bombStorage;
        struct tank * tkTemp;
        if (bmTemp->nextBomb!=NULL)
            {
                bmTemp=bmTemp->nextBomb;
                do
                    {
                        tkTemp = stpTank;
                        switch (tkTemp->tkID)
                            {
                                case 1:
                                    if(bmTemp->bombProducer==0)
                                        {
                                            do
                                                {
                                                    if (abs(bmTemp->bmCenter[0] - tkTemp->tkCenter[0]) <= 60 &&
                                                        abs(bmTemp->bmCenter[1] - tkTemp->tkCenter[1]) <= 60)
                                                        {
                                                            tkTemp->tkHp--;
                                                            if (tkTemp->tkHp == 0)
                                                                tkTemp = deleteBomb(tkTemp);
                                                            deleteBomb(bmTemp);
                                                        }
                                                    tkTemp = tkTemp->nextTank;
                                                } while (tkTemp != NULL);
                                        }
                                break;
                                case 0:
                                    if(bmTemp->bombProducer==1)
                                        {
                                            do
                                                {
                                                    if (abs(bmTemp->bmCenter[0] - tkTemp->tkCenter[0]) <= 60 &&
                                                        abs(bmTemp->bmCenter[1] - tkTemp->tkCenter[1]) <= 60)
                                                        {
                                                            tkTemp->tkHp--;
                                                            if (tkTemp->tkHp == 0)
                                                                tkTemp = deleteBomb(tkTemp);
                                                            deleteBomb(bmTemp);
                                                        }
                                                    tkTemp = tkTemp->nextTank;
                                                } while (tkTemp != NULL);
                                        }
                            }
                        bmTemp = bmTemp->nextBomb;
                    } while (bmTemp != NULL);
            }

    }
    /**
     * @brief 判断电脑是否开火
     * @details 遍历坦克库，根据电脑单位与玩家单位的位置关系判断是否开火或转向，当两单位之间中心点x坐标或y坐标之差的绝对值小于单位碰撞体积时，电脑单位开火
     * @param plrTank
     */
void aiFireCheck(struct tank* plrTank)
    {
        srand(time(NULL));
        struct tank* temp;
        temp=enemyTankStorage;
        while(enemyTankStorage!=NULL)
            {

                float distance=sqrtf(pow(enemyTankStorage->tkCenter[0]-plrTank->tkCenter[0],2)+pow(enemyTankStorage->tkCenter[1]-plrTank->tkCenter[1],2));
                if (distance<=500)
                    {
                        if ((abs(enemyTankStorage->tkCenter[0] - plrTank->tkCenter[0]) <= 90 ||
                             abs(enemyTankStorage->tkCenter[1] - plrTank->tkCenter[1]) <= 90) &&
                            enemyTankStorage->bomb == TRUE)
                            {
                                if (abs(enemyTankStorage->tkCenter[0] - plrTank->tkCenter[0]) >
                                    abs(enemyTankStorage->tkCenter[1] - plrTank->tkCenter[1]))
                                    {
                                        if (plrTank->tkCenter[0] > enemyTankStorage->tkCenter[0])
                                            enemyTankStorage->tkDirect = RIGHT;
                                        else
                                            enemyTankStorage->tkDirect = LEFT;
                                    } else if (abs(enemyTankStorage->tkCenter[0] - plrTank->tkCenter[0]) <
                                               abs(enemyTankStorage->tkCenter[1] - plrTank->tkCenter[1]))
                                    {
                                        if (plrTank->tkCenter[1] > enemyTankStorage->tkCenter[1])
                                            enemyTankStorage->tkDirect = DOWN;
                                        else
                                            enemyTankStorage->tkDirect = UP;
                                    }
                                struct bomb *newBomb;
                                newBomb = (struct bomb *) malloc(sizeof(struct bomb));
                                newBomb->bmDirect = enemyTankStorage->tkDirect;
                                newBomb->nextBomb = NULL;
                                newBomb->prevBomb = NULL;
                                newBomb->bombProducer = 1;
                                switch (enemyTankStorage->tkDirect)
                                    {
                                        case UP:
                                            newBomb->bmLocateLU[0] = enemyTankStorage->tkLocateLU[0] + 30;
                                        newBomb->bmLocateLU[1] = enemyTankStorage->tkLocateLU[1] - 30;
                                        newBomb->bmLocateRD[0] = enemyTankStorage->tkLocateLU[0] + 60;
                                        newBomb->bmLocateRD[1] = enemyTankStorage->tkLocateLU[1];
                                        break;
                                        case DOWN:
                                            newBomb->bmLocateLU[0] = enemyTankStorage->tkLocateRD[0] - 60;
                                        newBomb->bmLocateLU[1] = enemyTankStorage->tkLocateRD[1];
                                        newBomb->bmLocateRD[0] = enemyTankStorage->tkLocateRD[0] - 30;
                                        newBomb->bmLocateRD[1] = enemyTankStorage->tkLocateRD[1] + 30;
                                        break;
                                        case RIGHT:
                                            newBomb->bmLocateLU[0] = enemyTankStorage->tkLocateRD[0];
                                        newBomb->bmLocateLU[1] = enemyTankStorage->tkLocateRD[1] - 60;
                                        newBomb->bmLocateRD[0] = enemyTankStorage->tkLocateRD[0] + 30;
                                        newBomb->bmLocateRD[1] = enemyTankStorage->tkLocateRD[1] - 30;
                                        break;
                                        case LEFT:
                                            newBomb->bmLocateLU[0] = enemyTankStorage->tkLocateLU[0] - 30;
                                        newBomb->bmLocateLU[1] = enemyTankStorage->tkLocateLU[1] + 30;
                                        newBomb->bmLocateRD[0] = enemyTankStorage->tkLocateLU[0];
                                        newBomb->bmLocateRD[1] = enemyTankStorage->tkLocateLU[1] + 60;
                                        break;
                                    }
                                newBomb->bmCenter[0] = (newBomb->bmLocateRD[0] + newBomb->bmLocateLU[0]) / 2;
                                newBomb->bmCenter[1] = (newBomb->bmLocateRD[1] + newBomb->bmLocateLU[1]) / 2;
                                bombStorage = addBomb(bombStorage, newBomb);
                                enemyTankStorage->bomb = FALSE;
                            }
                    }
                else
                    {
                        int nDirect=rand()%4;
                        switch (nDirect)
                            {
                                case 0:
                                case 1:
                                    enemyTankStorage->tkDirect=DOWN;
                                break;
                                case 2:
                                    enemyTankStorage->tkDirect=RIGHT;
                                break;
                                case 3:
                                    enemyTankStorage->tkDirect=LEFT;
                                break;
                            }

                    }
                movAiTank(enemyTankStorage);
                enemyTankStorage=enemyTankStorage->nextTank;
            }
        enemyTankStorage=temp;
    }
void movAiTank(struct tank* tankCtrl)
    {
        tankCtrl->move=MovCheck(tankCtrl);
        if (tankCtrl->move==TRUE)
            {

                switch (tankCtrl->tkDirect)
                    {
                        case UP:
                            tankCtrl->tkLocateLU[1] -= enemySpeed;
                            tankCtrl->tkLocateRD[1] -= enemySpeed;
                        break;
                        case DOWN:
                            tankCtrl->tkLocateLU[1] += enemySpeed;
                            tankCtrl->tkLocateRD[1] += enemySpeed;
                        break;
                        case RIGHT:
                            tankCtrl->tkLocateLU[0] += enemySpeed;
                            tankCtrl->tkLocateRD[0] += enemySpeed;
                        break;
                        case LEFT:
                            tankCtrl->tkLocateLU[0] -= enemySpeed;
                            tankCtrl->tkLocateRD[0] -= enemySpeed;
                        break;
                    }
                tankCtrl->tkCenter[0]=(tankCtrl->tkLocateRD[0]+tankCtrl->tkLocateLU[0])/2;
                tankCtrl->tkCenter[1]=(tankCtrl->tkLocateRD[1]+tankCtrl->tkLocateLU[1])/2;
            }
    }