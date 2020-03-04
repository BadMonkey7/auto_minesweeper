#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
using namespace std;


/********************************************************************************/
/********************************corecode******************************/

class myQueue {
private:
	int data[1000];
	int head = 0;
	int last = 0;
	int cnt = 0;
public:
	// 入队
	void push(int element) {
		data[last++] = element;
		last %= 1000;
		cnt++;
	};
	// 队首元素
	int top(void) {
		return data[head];
	};
	// 出队
	int pop(void) {
		int tp = data[head];
		head = (head + 1) % 1000;
		cnt--;
		return tp;
	};
	bool isEmpty(void) {
		return cnt == 0;
	};
};

int RefreshGamePanel(int GamePanel[30][30], int x, int y, int mine[30][30], int n, int m, int k)
{
	// 超出范围，直接无效
	if (x<0 || x >n || y <0 || y > m)
		return 0;
	// 点到地雷
	if (mine[x][y])
		return -1;
	// 无效点击
	if (GamePanel[x][y] != -1)
		return 0;
	/* 正常点击 */
	// 方向
	int dx[8] = { -1,-1,-1,0,0,1,1,1 };
	int dy[8] = { 0,1,-1,1,-1,0,1,-1 };
	// 队列
	myQueue q;
	// 设置起始点
	q.push(x);
	q.push(y);
	while (!q.isEmpty()) {
		//格子打开
		// 每次发现的地雷
		int cnt = 0;
		// 弹出扫描中心的坐标点
		int xx = q.pop();
		int yy = q.pop();
		// 遍历8个方向，确定格子周围的炸弹数
		for (int i = 0; i < 8; i++) {
			int nx = xx + dx[i];
			int ny = yy + dy[i];
			// 发现炸弹
			if (nx >= 0 && nx < n && ny >= 0 && ny < m && mine[nx][ny])
				cnt++;

		}
		// 有炸弹,查看队列中下一个格子
		if (cnt) {
			// 这个格子实际也打开了
			GamePanel[xx][yy] = cnt;
			continue;
		}
		else {
			GamePanel[xx][yy] = 0;
			// 周围一圈入队，并更新grid
			for (int i = 0; i < 8; i++) {
				int nx = xx + dx[i];
				int ny = yy + dy[i];
				// 格子不能重复添加，只添加没有打开的格子
				if (nx >= 0 && nx < n && ny >= 0 && ny < m && GamePanel[nx][ny] == -1) {
					q.push(nx);
					q.push(ny);
					// 将该格子标识为被打开，但是不确定周围是否有炸弹
					GamePanel[nx][ny] = 0;
				}
			}
		}
	}
	int tot = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (GamePanel[i][j] == -1)
				tot++;
		}
	}
	if (tot <= k)
		return 1;
	else
		return 0;
};
// 机器人
void machine(int GamePanel[30][30], int n, int m, int &x, int &y)
{
	// 记录发现地雷的坐标
	int boom[30][30];
	for(int i=0;i<30;i++){
        for(int j=0;j<30;j++){
            boom[i][j]=0;
        }
	}
	// 8个方向
	int dx[8] = { -1,0,1,-1,1,-1,0,1 };
	int dy[8] = { 1,1,1,0,0,-1,-1,-1 };
	// 收集信息，更新地雷状态数组 boom
	int update = 0;
	while(1){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(GamePanel[i][j]!=0 && GamePanel[i][j]!=-1){
                    // 四周包含地雷数
                    int boomNum = GamePanel[i][j];
                    // 计算周围已知地雷数
                    int found = 0;
                    // 没有打开的格子数
                    int unopen = 0;
                    for(int k=0;k<8;k++){
                        int nx = i + dx[k];
                        int ny = j + dy[k];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < m)
                            if(GamePanel[nx][ny]==-1 && boom[nx][ny]==0)
                                unopen++;
                            // 发现地雷数
                            if(boom[nx][ny])
                                found++;
                    }
                    if(unopen==0)
                        continue;
                    // 发现新地雷，更新地雷状态
                    if(unopen==boomNum-found){
                        for(int k=0;k<8;k++){
                            int nx = i + dx[k];
                            int ny = j + dy[k];
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m && GamePanel[nx][ny]==-1 && boom[nx][ny]==0)
                                boom[nx][ny]=1;
                            }
                    }
                }
            }
        }
    int tot = 0;
	for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(boom[i][j])
               tot++;
        }
	}
	if(tot==update)
        break;
    update = tot;
	}
	// 利用boom中信息，做出决策
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
        if(GamePanel[i][j]==-1||GamePanel[i][j]==0)
            continue;
        int xx = i;
        int yy = j;
        // 周围地雷数
        int boomNum = GamePanel[xx][yy];
        // 已知地雷数
        int found = 0;
        // 没有打开的格子数
        int unopen = 0;
        for(int i=0;i<8;i++){
            int nx = xx + dx[i];
            int ny = yy + dy[i];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m){
                    if(boom[nx][ny])
                        found++;
                    if(GamePanel[nx][ny]==-1 && boom[nx][ny]==0)
                        unopen++;
            }
        }
        if(found==boomNum||unopen+found==boomNum){
            // 随机选择 一个没打开的格子
            for(int i=0;i<8;i++){
                int nx = xx + dx[i];
                int ny = yy + dy[i];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && boom[nx][ny]==0 && GamePanel[nx][ny]==-1){
                        x = nx;
                        y = ny;
                        return ;
                    }
                }
            }
        }
    }
    int cnt = 0;
	while(1){
        x = rand()%n;
        y = rand()%m;
        if(boom[x][y]==0&&GamePanel[x][y]==-1){
            if(cnt>10)
                return;
            cnt++;
            int flag = 1;
            for(int k=0;k<8;k++){
                int nx = x+dx[k];
                int ny = y+dy[k];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m){
                    if(GamePanel[nx][ny]>2){
                        flag = 0;
                        break;
                    }
                }
            }
            if(flag)
                break;
        }
	}
	return;
}
/********************************corecode******************************/
/********************************************************************************/



int main()
{
	int GamePanel[30][30];
	int temp[30][30];
	int mine[30][30];
	int iii, ii, n, m, k, i, j, minenum, x, y, win = 0;
	int score, bonus, finalscore = -999;
	n = 20;
	m = 20;
	minenum = 50;
	srand(int(time(0)));
	printf("正在评测中，请稍等\n");

	for (iii = 0; iii<10; iii++)
	{
		for (i = 0; i<n; i++)
			for (j = 0; j<m; j++)
			{
				GamePanel[i][j] = -1;
				mine[i][j] = 0;
			}
		for (k = 0; k<minenum; k++)
		{
			i = rand() % 20;
			j = rand() % 20;
			while (mine[i][j] != 0)
			{
				i = rand() % 20;
				j = rand() % 20;
			}
			mine[i][j] = 1;
		}

		int cnt = 0;

		for (ii = 0; ii<400; ii++)
		{
			int t1, t2;
			for (t1 = 0; t1 < 20; t1++)
				for (t2 = 0; t2 < 20; t2++)
					temp[t1][t2] = GamePanel[t1][t2];

			machine(temp, n, m, x, y); 

			if (GamePanel[x][y] == -1)
			{
				cnt++;
				win = RefreshGamePanel(GamePanel, x, y, mine, n, m, minenum);
				if (win == 1)		//扫雷成功，游戏结束
				{
					if (cnt < 80) bonus = 100;
					else if (cnt < 90) bonus = 90;
					else if (cnt < 100) bonus = 80;
					else if (cnt < 120) bonus = 70;
					else if (cnt < 150) bonus = 50;
					else if (cnt < 200) bonus = 30;
					else if (cnt < 250) bonus = 10;
					else bonus = 0;
					score = 500 + bonus;
					break;
				}
				else if (win == -1)		//扫雷失败，游戏结束
				{
					score = 0;
					for (i = 0; i<n; i++)
						for (j = 0; j<m; j++)
						{
							if (GamePanel[i][j] != -1)
								score++;
						}
					break;
				}
			}

		} //end for ii

		if (win == 0)	//点击超400次游戏仍未结束，分数等于目前打开的格子数
		{
			score = 0;
			for (i = 0; i<n; i++)
				for (j = 0; j<m; j++)
				{
					if (GamePanel[i][j] != -1)
						score++;
				}
		}
		cout<<score/10<<endl;
		if (score>finalscore) finalscore = score;
	}	// end for iii

	printf("\n分数为 %d.\n\n", finalscore/10);
	system("pause");
	return 0;
}
