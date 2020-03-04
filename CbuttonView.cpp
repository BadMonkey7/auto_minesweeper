

#include "stdafx.h"
#include "Cbutton.h"
#include "CbuttonDoc.h"
#include "CbuttonView.h"
#include <time.h>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDB_ZLH 0

IMPLEMENT_DYNCREATE(CCbuttonView, CView)

BEGIN_MESSAGE_MAP(CCbuttonView, CView)
	//{{AFX_MSG_MAP(CCbuttonView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_COMMAND(ID_MENU_CAPTION, OnMenuCaption)
	ON_COMMAND(ID_MENU_NEW, OnMenuNew)
	ON_COMMAND(ID_MENU_OUT, OnMenuOut)
	ON_COMMAND(ID_MACHINE, OnMachine)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

class myQueue {
private:
	int data[100];
	int head = 0;
	int last = 0;
	int cnt = 0;
public:
	// 入队
	void push(int element) {
		data[last++] = element;
		last %= 40;
		cnt++;
	};
	// 队首元素
	int top(void) {
		return data[head];
	};
	// 出队
	int pop(void) {
		int tp = data[head];
		head = (head + 1) % 40;
		cnt--;
		return tp;
	};
	bool isEmpty(void) {
		return cnt == 0;
	};
};
/******************************************************************************************************************/
/************************************************需要添加的两个函数如下********************************************/
int CCbuttonView::RefreshGamePanel(int GamePanel[30][30], int x, int y, int mine[30][30], int n, int m, int k)
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

/********************************************************************************/
void CCbuttonView::machine(int GamePanel[30][30], int n, int m, int &x, int &y)
{
	// 记录发现地雷的坐标
	int boom[30][30];
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			boom[i][j] = 0;
		}
	}
	// 8个方向
	int dx[8] = { -1,0,1,-1,1,-1,0,1 };
	int dy[8] = { 1,1,1,0,0,-1,-1,-1 };
	// 收集信息，更新地雷状态数组 boom
	int update = 0;
	while (1) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (GamePanel[i][j] != 0 && GamePanel[i][j] != -1) {
					// 四周包含地雷数
					int boomNum = GamePanel[i][j];
					// 计算周围已知地雷数
					int found = 0;
					// 没有打开的格子数
					int unopen = 0;
					for (int k = 0; k < 8; k++) {
						int nx = i + dx[k];
						int ny = j + dy[k];
						if (nx >= 0 && nx < n && ny >= 0 && ny < m)
							if (GamePanel[nx][ny] == -1 && boom[nx][ny] == 0)
								unopen++;
						// 发现地雷数
						if (boom[nx][ny])
							found++;
					}
					if (unopen == 0)
						continue;
					// 发现新地雷，更新地雷状态
					if (unopen == boomNum - found) {
						for (int k = 0; k < 8; k++) {
							int nx = i + dx[k];
							int ny = j + dy[k];
							if (nx >= 0 && nx < n && ny >= 0 && ny < m && GamePanel[nx][ny] == -1 && boom[nx][ny] == 0)
								boom[nx][ny] = 1;
						}
					}
				}
			}
		}
		int tot = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (boom[i][j])
					tot++;
			}
		}
		if (tot == update)
			break;
		update = tot;
	}
	// 利用boom中信息，做出决策
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (GamePanel[i][j] == -1 || GamePanel[i][j] == 0)
				continue;
			int xx = i;
			int yy = j;
			// 周围地雷数
			int boomNum = GamePanel[xx][yy];
			// 已知地雷数
			int found = 0;
			// 没有打开的格子数
			int unopen = 0;
			for (int i = 0; i < 8; i++) {
				int nx = xx + dx[i];
				int ny = yy + dy[i];
				if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
					if (boom[nx][ny])
						found++;
					if (GamePanel[nx][ny] == -1 && boom[nx][ny] == 0)
						unopen++;
				}
			}
			if (found == boomNum || unopen + found == boomNum) {
				// 随机选择 一个没打开的格子
				for (int i = 0; i < 8; i++) {
					int nx = xx + dx[i];
					int ny = yy + dy[i];
					if (nx > 0 && nx < n && ny > 0 && ny < m && boom[nx][ny] == 0 && GamePanel[nx][ny] == -1) {
						x = nx;
						y = ny;
						return;
					}
				}
			}
		}
	}
	while (1) {
		x = rand() % n;
		y = rand() % m;
		if (boom[x][y] == 0 && GamePanel[x][y] == -1)
			break;
	}
	return;
};
/************************************************需要添加的两个函数如上********************************************/
/******************************************************************************************************************/
















/*************************************以下代码不用管********************************************/
CCbuttonView::CCbuttonView()
{

	// TODO: add construction code here
	srand(int(time(0)));
	minenum=50;
	for(i=0;i<20;i++)
		for(j=0;j<20;j++)
		{	
			mine[i][j]=0;
			GamePanel[i][j]=-1;
		}

	for(int k=0;k<minenum;k++)
	{
		i=rand()%20;
		j=rand()%20;
		while(mine[i][j]!=0)
		{
			i=rand()%20;
			j=rand()%20;
		}
		mine[i][j]=1;
	}
	x=0;y=0;
	id=0;
	i=0;j=0;
	showmine=-1;
	win=0;
	current_j=-1;
	current_i=-1;
}

CCbuttonView::~CCbuttonView()
{
}


void CCbuttonView::OnMenuNew() 
{
	// TODO: Add your command handler code here
	for(i=0;i<20;i++)
		for(j=0;j<20;j++)
		{	
			mine[i][j]=0;
			GamePanel[i][j]=-1;
			::ShowWindow(cbutton[i][j],SW_SHOW);
		}
		
		for(int k=0;k<minenum;k++)
		{
			i=rand()%20;
			j=rand()%20;
			while(mine[i][j]!=0)
			{
				i=rand()%20;
				j=rand()%20;
			}
			mine[i][j]=1;
		}
		x=0;y=0;
		id=0;
		i=0;j=0;
		win=0;
		showmine=-1;
		current_i=-1;
		current_j=-1;
		CRect rect1;
		GetClientRect(&rect1);
		InvalidateRect(rect1);

}
void CCbuttonView::OnMenuOut() 
{
	// TODO: Add your command handler code here
	 
	//PostQuitMessage(0);
	exit(0) ;
}


BOOL CCbuttonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCbuttonView drawing

void CCbuttonView::OnDraw(CDC* pDC)
{
	CCbuttonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCbuttonView printing

BOOL CCbuttonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCbuttonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCbuttonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCbuttonView diagnostics

#ifdef _DEBUG
void CCbuttonView::AssertValid() const
{
	CView::AssertValid();
}

void CCbuttonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCbuttonDoc* CCbuttonView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCbuttonDoc)));
	return (CCbuttonDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCbuttonView message handlers

int CCbuttonView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
	id = 0;
	x=0;y=0;
	int realid;
	for(i=0;i<20;i++)
	 {
		 for(j=0;j<20;j++)
		 {
			 if (id == 0) realid = 1;
			 else realid = id;
			  cbutton[i][j].Create("",WS_CHILD | WS_VISIBLE,CRect(x,y,x+20,y+20),this,realid);
			  x+=20;
			  id+=20;
		 }
		 x=0;
		 y+=20;
	 }
     return 0;
}

void CCbuttonView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	
	CView::OnLButtonDown(nFlags, point);
}


void CCbuttonView::OnRButtonDown(UINT nFlags, CPoint point)			 
{
	// TODO: Add your message handler code here and/or call default
	/*for(int k=0;k<20;k++)
	{
	  ::ShowWindow(cbutton[i_lei[k]][j_lei[k]],SW_HIDE);
	}*/
	showmine*=-1;
		CClientDC dc(this);
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP_LEI);
	if(showmine==1)
	{
	
		CBrush brush(&bitmap);
		//cbutton[0][0].SetBitmap();
		//dc.FillRect(CRect(point.x,point.y,point.x+20,point.y+20),&brush);
		
		for(i=0;i<20;i++)
		{
			 for(j=0;j<20;j++)
			 {
				 if(mine[i][j]==1)
				 {
					 dc.FillRect(CRect(j*20,i*20,j*20+20,i*20+20),&brush);
				 }
			 }
		}
	}
	else{
	
		CRect rect1;
		GetClientRect(&rect1);
		InvalidateRect(rect1);
	}
	//cbutton[0][0].SetBitmap(bitmap);

	CView::OnRButtonDown(nFlags, point);
}

void CCbuttonView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
    CBrush brush(RGB(255,255,255));
	dc.SelectObject(&brush);
	dc.FillRect(CRect(0,0,800,600),&brush);

	CPen pen(PS_SOLID,1,RGB(180,180,180));
 
	for( i=0;i<420;i+=20)
	{
		dc.SelectObject(&pen);
		dc.MoveTo(i,0);
		dc.LineTo(i,440);
		dc.MoveTo(0,i);
		dc.LineTo(400,i);
	}
				
    COLORREF col=dc.SetBkColor(RGB(255,255,255));
	dc.SetTextColor(RGB(0,0,255));

	if(win==0)
	{

		for(i=0;i<20;i++)
            for(j=0;j<20;j++)
			{
				if(GamePanel[i][j]==0)
					::ShowWindow(cbutton[i][j],SW_HIDE);
				else if(GamePanel[i][j]>0 && GamePanel[i][j]<9)
				{
					::ShowWindow(cbutton[i][j],SW_HIDE);
					str.Format("%d",GamePanel[i][j]);
					dc.SetTextColor(RGB(0,0,255));
					dc.DrawText(str,CRect(j*20,i*20,j*20+20,i*20+20),DT_CENTER);
				}
			}
	}
	else if(win==-1 || win==1)
	{
		
		for(i=0;i<20;i++)
            for(j=0;j<20;j++)
			{
				if(GamePanel[i][j]==0)
					::ShowWindow(cbutton[i][j],SW_HIDE);
				else if(GamePanel[i][j]>0 && GamePanel[i][j]<9)
				{
					::ShowWindow(cbutton[i][j],SW_HIDE);
					str.Format("%d",GamePanel[i][j]);
					dc.SetTextColor(RGB(0,0,255));
					dc.DrawText(str,CRect(j*20,i*20,j*20+20,i*20+20),DT_CENTER);
				}
				CBitmap bitmap,bitmap1;
				bitmap.LoadBitmap(IDB_BITMAP_LEI);
				CBrush brush(&bitmap);
		
				if(mine[i][j]==1)
				{
					::ShowWindow(cbutton[i][j],SW_HIDE);
					
					if(win==-1 && current_i==i && current_j==j)
					{
						bitmap1.LoadBitmap(IDB_REDMINE_BITMAP);
						CBrush brush1(&bitmap1);
						dc.FillRect(CRect(j*20,i*20,j*20+20,i*20+20),&brush1);
					}
					else dc.FillRect(CRect(j*20,i*20,j*20+20,i*20+20),&brush);

				}

			}
	}
	// Do not call CView::OnPaint() for painting messages
}


void CCbuttonView::OnCancelMode() 
{
	CView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CCbuttonView::OnMenuCaption() 
{
	// TODO: Add your command handler code here

  MessageBox("简易版扫雷游戏图形界面框架","扫雷");
}


BOOL CCbuttonView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
    
	int ii,jj,k,ButtonCmdId=LOWORD(wParam);
	if (ButtonCmdId == 1)
	{
		ii = 0;
		jj = 0;
	}
	else
	{
		ii = (ButtonCmdId / 400);
		jj = (ButtonCmdId - 400 * ii) / 20;
	}

		/************************************************************************/
		/***/ win=RefreshGamePanel(GamePanel,ii,jj,mine,20,20,minenum); /********/
        /************************************************************************/

		CRect rect1;
		GetClientRect(&rect1);
		current_j=jj;
		current_i=ii;
		
		if(win==-1 || win==1)
		{
			InvalidateRect(rect1);
		
			if(win==-1)
				MessageBox("你输了！点击确定重新开始","扫雷",MB_ICONEXCLAMATION);
			else MessageBox("恭喜，你赢了！点击确定重新开始","扫雷",MB_ICONINFORMATION);
			for(i=0;i<20;i++)
				for(j=0;j<20;j++)
				{	
					mine[i][j]=0;
					GamePanel[i][j]=-1;
					::ShowWindow(cbutton[i][j],SW_SHOW);
				}
			
			for(int k=0;k<minenum;k++)
			{
				i=rand()%20;
				j=rand()%20;
				while(mine[i][j]!=0)
				{
					i=rand()%20;
					j=rand()%20;
				}
				mine[i][j]=1;
			}
			x=0;y=0;
			id=0;
			i=0;j=0;
			win=0;
			showmine=-1;
			current_i=-1;
			current_j=-1;
			machinestart=false;
		}
		//InvalidateRect(rect1);
		{
			CClientDC dc(this); // device context for painting
			for(i=0;i<20;i++)
				for(j=0;j<20;j++)
				{
					if(GamePanel[i][j]==0)
						::ShowWindow(cbutton[i][j],SW_HIDE);
					else if(GamePanel[i][j]>0 && GamePanel[i][j]<9)
					{
						::ShowWindow(cbutton[i][j],SW_HIDE);
						str.Format("%d",GamePanel[i][j]);
						dc.SetTextColor(RGB(0,0,255));
						dc.DrawText(str,CRect(j*20,i*20,j*20+20,i*20+20),DT_CENTER);
					}
				}
			
		}
	
	return CView::OnCommand(wParam, lParam);
}

void CCbuttonView::OnMachine() 
{
	// TODO: Add your command handler code here
	int x=-1,y=-1;
	machinestart=true;
	int tim=50,count=0;
	while(machinestart && count<400)
	{
		/*************************************************/
		/*********/machine(GamePanel,20,20, x,y);/********/
		/*************************************************/
		if (x<0 || x>19 || y<0 || y>19) break;
		count++;
		SendMessage(WM_COMMAND,MAKEWPARAM(x*400+20*y,BN_CLICKED),0); 
		Sleep(tim);
	}
}