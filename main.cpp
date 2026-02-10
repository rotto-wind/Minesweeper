#include <graphics.h>
#include <stdio.h>
#include <time.h>

#define N 10 //地图边长
#define M 50 //图片像素边长

HWND hwnd; //窗口句柄
int win; //记录翻开的个数，用来判断输赢
int map[N][N] = { 0 }; //0表示空白,-1表示地雷
int swap[N][N] = { 0 }; //0为未翻开，1为翻开，2为标记
int number[N][N] = { 0 }; //记录周围有几个地雷，若本身是地雷则为9;
int visited[N][N] = { 0 }; //服务于Show
IMAGE img[12]; //存储图片

int getnumber(int x, int y)
{
	if (map[x][y] == -1) return 9;
	int num = 0;
	for (int i = x - 1;i <= x + 1;i++)
	{
		for (int j = y - 1;j <= y + 1;j++)
		{
			if (i >= 0 && i < N && j >= 0 && j < N)
			{
				if (map[i][j] == -1) num++;
			}
		}
	}
	return num;
}
void DrawMap(int i, int j, int no)
{
	if(no >= 0 && no <=10) putimage(i * M, j * M, &img[no]);
	else putimage(i * M, j * M, &img[11]); //背景
}
void Show(int x, int y)
{
	int no = number[x][y];
	DrawMap(x, y, no);
	swap[x][y] = 1;
	if (no == 0) {
		if (x - 1 >= 0 && x - 1 < N && y >= 0 && y < N && !visited[x - 1][y]){
			visited[x - 1][y] = 1;Show(x - 1, y);
		}
		if (x + 1 >= 0 && x + 1 < N && y >= 0 && y < N && !visited[x + 1][y]) {
			visited[x + 1][y] = 1;Show(x + 1, y);
		}
		if (x >= 0 && x < N && y - 1 >= 0 && y - 1 < N && !visited[x][y - 1]) {
			visited[x][y - 1] = 1;Show(x, y - 1);
		}
		if (x >= 0 && x < N && y + 1 >= 0 && y + 1 < N && !visited[x][y + 1]) {
			visited[x][y + 1] = 1;Show(x, y + 1);
		}
	}
}
bool Play()
{
	MOUSEMSG msg;
	while (1)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN: //鼠标左键为翻开
			if (swap[msg.x / M][msg.y / M] == 0) { //原来未翻开，翻开
				swap[msg.x / M][msg.y / M] = 1;
				Show(msg.x / M, msg.y / M);
				if (map[msg.x / M][msg.y / M] == -1) //翻到炸弹
				{
					return false;
				}
			}
			else continue;
			break;
		case WM_RBUTTONDOWN: //鼠标右键为标记
			if (swap[msg.x / M][msg.y / M] == 0) //未翻开的标记
			{
				if (map[msg.x / M][msg.y / M] == -1) win++;
				else win--;
				swap[msg.x / M][msg.y / M] = 2;
				DrawMap(msg.x / M, msg.y / M, 10);
			}
			else if (swap[msg.x / M][msg.y / M] == 2) //已标记，取消标记
			{
				if (map[msg.x / M][msg.y / M] == -1) win--;
				else win++;
				swap[msg.x / M][msg.y / M] = 0;
				DrawMap(msg.x / M, msg.y / M, 11);
			}
			else continue; //已翻开的无法标记
			break;
		}
		if (win == N) return true;
	}
}
bool Game()
{
	initgraph(M * N, M * N, 0); //创建界面
	loadimage(&img[0], L"0.bmp");
	loadimage(&img[1], L"1.bmp");
	loadimage(&img[2], L"2.bmp");
	loadimage(&img[3], L"3.bmp");
	loadimage(&img[4], L"4.bmp");
	loadimage(&img[5], L"5.bmp");
	loadimage(&img[6], L"6.bmp");
	loadimage(&img[7], L"7.bmp");
	loadimage(&img[8], L"8.bmp");
	loadimage(&img[9], L"炸弹.bmp");
	loadimage(&img[10], L"标记.bmp");
	loadimage(&img[11], L"地图.bmp");

	srand(time(0)); //以当前时间作为伪随机数的参数
	for (int i = 0;i < N;i++) //每次Game重置map和swap
	{
		for (int j = 0;j < N;j++)
		{
			map[i][j] = 0;
			swap[i][j] = 0;
			number[i][j] = 0;
			visited[i][j] = 0;
		}
	}
	for (int i = 0;i < N;) //埋雷,以-1表示
	{
		int x = rand() % 10;
		int y = rand() % 10;
		if (map[x][y] == 0)
		{
			map[x][y] = -1;
			i++;
		}
	}
	for (int i = 0;i < N;i++) //绘制全背景
	{
		for (int j = 0;j <= 9;j++)
		{
			putimage(i * M, j * M, &img[11]);
			number[i][j] = getnumber(i, j);
		}
	}
	hwnd = GetHWnd();
	win = 0;
	while (1)
	{
		bool flag=Play();
		if (win == N)
		{
			MessageBox(hwnd, L"你赢了", L"Well Played", MB_OK);
			return true;
			break;
		}
		if (!flag) //失败
		{
			return false;
		}
	}
}

int main()
{
	while (1)
	{
		bool win_flag=Game();
		if (win_flag) break;
	}
	return 0;
}