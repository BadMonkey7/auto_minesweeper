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
	// ���
	void push(int element) {
		data[last++] = element;
		last %= 1000;
		cnt++;
	};
	// ����Ԫ��
	int top(void) {
		return data[head];
	};
	// ����
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
	// ������Χ��ֱ����Ч
	if (x<0 || x >n || y <0 || y > m)
		return 0;
	// �㵽����
	if (mine[x][y])
		return -1;
	// ��Ч���
	if (GamePanel[x][y] != -1)
		return 0;
	/* ������� */
	// ����
	int dx[8] = { -1,-1,-1,0,0,1,1,1 };
	int dy[8] = { 0,1,-1,1,-1,0,1,-1 };
	// ����
	myQueue q;
	// ������ʼ��
	q.push(x);
	q.push(y);
	while (!q.isEmpty()) {
		//���Ӵ�
		// ÿ�η��ֵĵ���
		int cnt = 0;
		// ����ɨ�����ĵ������
		int xx = q.pop();
		int yy = q.pop();
		// ����8������ȷ��������Χ��ը����
		for (int i = 0; i < 8; i++) {
			int nx = xx + dx[i];
			int ny = yy + dy[i];
			// ����ը��
			if (nx >= 0 && nx < n && ny >= 0 && ny < m && mine[nx][ny])
				cnt++;

		}
		// ��ը��,�鿴��������һ������
		if (cnt) {
			// �������ʵ��Ҳ����
			GamePanel[xx][yy] = cnt;
			continue;
		}
		else {
			GamePanel[xx][yy] = 0;
			// ��ΧһȦ��ӣ�������grid
			for (int i = 0; i < 8; i++) {
				int nx = xx + dx[i];
				int ny = yy + dy[i];
				// ���Ӳ����ظ���ӣ�ֻ���û�д򿪵ĸ���
				if (nx >= 0 && nx < n && ny >= 0 && ny < m && GamePanel[nx][ny] == -1) {
					q.push(nx);
					q.push(ny);
					// ���ø��ӱ�ʶΪ���򿪣����ǲ�ȷ����Χ�Ƿ���ը��
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
// ������
void machine(int GamePanel[30][30], int n, int m, int &x, int &y)
{
	// ��¼���ֵ��׵�����
	int boom[30][30];
	for(int i=0;i<30;i++){
        for(int j=0;j<30;j++){
            boom[i][j]=0;
        }
	}
	// 8������
	int dx[8] = { -1,0,1,-1,1,-1,0,1 };
	int dy[8] = { 1,1,1,0,0,-1,-1,-1 };
	// �ռ���Ϣ�����µ���״̬���� boom
	int update = 0;
	while(1){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(GamePanel[i][j]!=0 && GamePanel[i][j]!=-1){
                    // ���ܰ���������
                    int boomNum = GamePanel[i][j];
                    // ������Χ��֪������
                    int found = 0;
                    // û�д򿪵ĸ�����
                    int unopen = 0;
                    for(int k=0;k<8;k++){
                        int nx = i + dx[k];
                        int ny = j + dy[k];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < m)
                            if(GamePanel[nx][ny]==-1 && boom[nx][ny]==0)
                                unopen++;
                            // ���ֵ�����
                            if(boom[nx][ny])
                                found++;
                    }
                    if(unopen==0)
                        continue;
                    // �����µ��ף����µ���״̬
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
	// ����boom����Ϣ����������
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
        if(GamePanel[i][j]==-1||GamePanel[i][j]==0)
            continue;
        int xx = i;
        int yy = j;
        // ��Χ������
        int boomNum = GamePanel[xx][yy];
        // ��֪������
        int found = 0;
        // û�д򿪵ĸ�����
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
            // ���ѡ�� һ��û�򿪵ĸ���
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
	printf("���������У����Ե�\n");

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
				if (win == 1)		//ɨ�׳ɹ�����Ϸ����
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
				else if (win == -1)		//ɨ��ʧ�ܣ���Ϸ����
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

		if (win == 0)	//�����400����Ϸ��δ��������������Ŀǰ�򿪵ĸ�����
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

	printf("\n����Ϊ %d.\n\n", finalscore/10);
	system("pause");
	return 0;
}
