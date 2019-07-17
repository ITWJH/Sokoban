#include <stdio.h>
#include <conio.h>//����̨�����������Ҫ�Ƕ��ڼ��̲�����һЩ����
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
void readMap( char(*map)[]);//��ȡtext�ļ�������������ݳ�ʼ������
void printMap(char(*map)[]);//��ӡ��ͼ
int getNewMap(char select, char (*map)[], int *x, int *y);
void hideCursor();//���ع�� 
COORD getCursorPositon(COORD * pCoord); // ��ȡ���λ�� ,COORD��һ��λ�ýṹ�� ,����X,Y 
void flushXYOnCmdWindow(short x, short y, char c);//��С�ڿ��������� 
//cell erase;
//cell paint;
int main()
{
//	system("cls"); 
    char map[9][column];//�洢ԭʼ��ͼ��Ϣ 
    char select;
    int x = 1, y = 1; //��¼�����λ��
    int flag = 0;
    _Bool flag1 = true;
	hideCursor();//�������ع�꺯�� 
    readMap(map);//��ȡ��ͼ 
    printMap(map);//��ӡ��ͼ 
	printf("���������ѡ�� W:���� S:����  A������  D:����  G:����\n");
    while(flag1)
    {
        select = getch();
        if(select == 'G' || select == 'g')
        {
            printf("��Ϸ������");
            break;
        }
        flag = getNewMap(select, map, &x, &y);//���ú���,�����ͼ,����λ�� 
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
        printf("�ļ���ʧ�ܣ�");
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
                *(*(map + i) + j) = '#'; //����ǽ��
                break;
            case 50: //  2
                *(*(map + i) + j) = 'o'; //��������
                break;
            case 51:  // 3
                *(*(map + i) + j) = ' ';
                break;
            case 52:  // 4
                *(*(map + i) + j) = '$'; //����Ŀ�ĵ�
                break;
            case 53:  // 5
                *(*(map + i) + j) = 'B'; //��������
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
    	case 65://a ,���ͷ 
    	case 97:
    	case 75:
    		numx = 0;
    		numy = 1;
    		break;
    	case 87://w  ,�ϼ�ͷ 
    	case 119:
    	case 72:
    		numx = 1;
    		numy = 0;
    		break;
    	case 83://s  ,�¼�ͷ 
    	case 115:
    	case 80:
    		numx = -1;
    		numy = 0;
    		break;
    	case 68://d  ,�Ҽ�ͷ 
    	case 100:
    	case 77:
    		numx = 0;
    		numy = -1;
    		break;
	}
    if(map[*x - numx][*y - numy] == '#')
		return flag; //�ж��Ƿ�Ϊǽ �������ǽ��������switch
    else
    {
        if(map[*x - numx][*y - numy] == 'B') //�ж��Ƿ�Ϊ����
        {
            if(map[*x - (2*numx)][*y - (2*numy)] == '#'){
				return flag; //���Ϊǽ�ڣ�������switch
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
                    flag = 1;//1�������óɹ�����Ϸ������
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
        else //��Ϊ����
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
void hideCursor(){//���ع�� 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);//�����ڸ�cci��ֵ 
	cci.bVisible = false;//��ֵ֮��ı�ɼ��� 
	SetConsoleCursorInfo(hOut, &cci);//�������� 
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
	// �޸Ĺ��λ�� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	putchar(c);
	// ������ 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), oldPos);
}







