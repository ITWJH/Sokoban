#include <stdio.h>
#include <conio.h>//控制台输入输出，主要是对于键盘操作的一些函数
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#define column 24
struct step{
	short x;
	short y;
	char c;
};
typedef struct step cell;
void readMap( char(*map)[]);//读取text文件，将里面的内容初始化数组
void printMap(char(*map)[]);//打印地图
int getNewMap(char select, char (*map)[], int *x, int *y);
void hideCursor();//隐藏光标 
COORD getCursorPositon(COORD * pCoord); // 获取光标位置 ,COORD是一个位置结构体 ,包含X,Y 
void flushXYOnCmdWindow(short x, short y, char c);//在小黑框界面中清除 
//cell erase;
//cell paint;
int main()
{
//	system("cls"); 
    char map[9][column];//存储原始地图信息 
    char select;
    int x = 1, y = 1; //记录人物的位置
    int flag = 0;
    _Bool flag1 = true;
	hideCursor();//调用隐藏光标函数 
    readMap(map);//读取地图 
    printMap(map);//打印地图 
	printf("请输入你的选择 W:向上 S:向下  A：向左  D:向右  G:结束\n");
    while(flag1)
    {
        select = getch();
        if(select == 'G' || select == 'g')
        {
            printf("游戏结束！");
            break;
        }
        flag = getNewMap(select, map, &x, &y);//调用函数,传入地图,人物位置 
        if(flag == 1)
        {
        	system("cls");
            printMap(map);
            printf("Winner!\nGame  over!");
            break;
        }
    }

}
void readMap(char (*map)[column])
{
    FILE *fp;
    int i, j;
    char c;
    fp = fopen("map.txt", "r");
    if(fp == NULL)
    {
        printf("文件打开失败！");
        exit(1);
    }
    c = fgetc(fp);
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < column; j++)
        {
            switch((int)c)
            {
            case 49:  // 1
                *(*(map + i) + j) = '#'; //代表墙壁
                break;
            case 50: //  2
                *(*(map + i) + j) = 'o'; //代表人物
                break;
            case 51:  // 3
                *(*(map + i) + j) = ' ';
                break;
            case 52:  // 4
                *(*(map + i) + j) = '$'; //代表目的地
                break;
            case 53:  // 5
                *(*(map + i) + j) = 'B'; //代表箱子
                break;
            }
            c = fgetc(fp);
        }
    }
    fclose(fp);
}
void printMap(char(*map)[column])
{
    int i, j;
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < column - 1; j++)
        {
            printf("%c", *(*(map + i) + j));
        }
        putchar('\n');
    }
}
int  getNewMap(char select, char (*map)[column], int *x, int *y)
{
    int flag = 0;
    int numx = 0, numy = 0;
    switch((int)select){
    	case 65://a ,左箭头 
    	case 97:
    	case 75:
    		numx = 0;
    		numy = 1;
    		break;
    	case 87://w  ,上箭头 
    	case 119:
    	case 72:
    		numx = 1;
    		numy = 0;
    		break;
    	case 83://s  ,下箭头 
    	case 115:
    	case 80:
    		numx = -1;
    		numy = 0;
    		break;
    	case 68://d  ,右箭头 
    	case 100:
    	case 77:
    		numx = 0;
    		numy = -1;
    		break;
	}
    if(map[*x - numx][*y - numy] == '#')
		return flag; //判断是否为墙 ，如果是墙，则跳出switch
    else
    {
        if(map[*x - numx][*y - numy] == 'B') //判断是否为箱子
        {
            if(map[*x - (2*numx)][*y - (2*numy)] == '#'){
				return flag; //如果为墙壁，则跳出switch
			}
            else
            {
                if(map[*x - (2*numx)][*y - (2*numy)] == '$')
                {
                    map[*x][*y] = ' ';
                    flushXYOnCmdWindow(*y, *x, ' ');
                    map[*x-=numx][*y-=numy] = 'o';
                    flushXYOnCmdWindow(*y, *x, 'o');
                    map[*x-numx][*y - numy] = 'B';
                    flushXYOnCmdWindow(*y - numy,*x-numx, 'B');
                    flag = 1;//1代表你获得成功，游戏结束！
                    return flag;
                }
                else
                {
                    map[*x][*y] = ' ';
                    flushXYOnCmdWindow(*y, *x, ' ');
                    map[*x-=numx][*y-=numy] = 'o';
                    flushXYOnCmdWindow(*y, *x, 'o');
                    map[*x-numx][*y-numy] = 'B';
                    flushXYOnCmdWindow( *y-numy,*x-numx, 'B');
                    return flag;
                }
            }
        }
        else //不为箱子
        {
        	if(map[*x - numx][*y - numy] == '$')return flag;
            map[*x][*y] = ' ';
            flushXYOnCmdWindow(*y, *x, ' ');
            map[*x-=numx][*y-=numy] = 'o';
            flushXYOnCmdWindow(*y, *x, 'o');
            return flag;
        }
    }
}
void hideCursor(){//隐藏光标 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);//类似于给cci赋值 
	cci.bVisible = false;//赋值之后改变可见性 
	SetConsoleCursorInfo(hOut, &cci);//重新设置 
}
COORD getCursorPositon(COORD * pCoord){
	HANDLE gh_std_out;
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
	(*pCoord).X = bInfo.dwCursorPosition.X;
	(*pCoord).Y = bInfo.dwCursorPosition.Y;
}
void flushXYOnCmdWindow(short x, short y, char c){
	COORD pos;
	COORD oldPos;
	getCursorPositon(&oldPos);
	pos.X = x;
	pos.Y = y;
	// 修改光标位置 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	putchar(c);
	// 光标回正 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), oldPos);
}







