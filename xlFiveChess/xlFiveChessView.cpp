// xlFiveChessView.cpp : implementation of the CxlFiveChessView class
//

#include "stdafx.h"
#include "xlFiveChess.h"

#include "xlFiveChessDoc.h"
#include "xlFiveChessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// 这里开始是自己写的代码

#pragma comment(lib, "winmm.lib")
#include "mmsystem.h"
#include "MainFrm.h"
#define SIZE 15   //棋盘大小，设为 15*15
#define WIDTH 35  //格子边长
POINT crtpoint;  //当前点
int crtplayer=0;  //当前玩家，1黑棋，-1白棋，0禁止
int chess[SIZE][SIZE];  //用于存储棋局的二维数组，1黑棋，-1白棋，0空白
int chessed=0;  //已走步数
POINT winline[2];  //胜利后五子线的首尾两点
CClientDC* p;
//客户区设备指针，不知道怎么得到，OnDraw() 里好不容易有个，就存为全局变量备用
bool playsound=true;  //音效开关



POINT PntTrans(int x, int y){  //逻辑坐标(从(1,1)到(SIZE,SIZE))转换为物理坐标

    POINT newpoint;

    newpoint.x=WIDTH*(x-1)+(WIDTH+1)/2+10;  //10为微调
    newpoint.y=WIDTH*(y-1)+(WIDTH+1)/2+0;

    return newpoint;
}

POINT PntTrans(POINT point){  //搞个函数重载，算是 C++ 用过了
    return PntTrans(point.x, point.y);
}

POINT PntTransA(POINT point){  //物理坐标转为逻辑坐标

    POINT pnt, rpnt;
    for(pnt.y=1;pnt.y<=SIZE;pnt.y++){
        for(pnt.x=1;pnt.x<=SIZE;pnt.x++){
            rpnt=PntTrans(pnt);
            if((point.x>rpnt.x-(WIDTH-1)/2&&point.x<rpnt.x+(WIDTH-1)/2)
                &&(point.y>rpnt.y-(WIDTH-1)/2&&point.y<rpnt.y+(WIDTH-1)/2)
              ){

                return pnt;
            }
        }
    }

    //本来好像可以直接算出的，可是我始终得不到正确的值
    //所以只好穷举检查

    pnt.x=0;
    pnt.y=0;
    return pnt;
}

void ClearUnit(POINT pnt){  //清除一个点

    POINT point;
    point=PntTrans(pnt);

    CPen pen;
    pen.CreatePen(PS_SOLID, 1, RGB(255, 255,255));
    CPen *oldpen=(CPen*)p->SelectObject(&pen);
    for(int i=0;i<WIDTH;i++){
        p->MoveTo(point.x-(WIDTH+1)/2, point.y-(WIDTH-1)/2+i);
        p->LineTo(point.x+(WIDTH+1)/2, point.y-(WIDTH-1)/2+i);
    }
    p->SelectObject(pen);
}

void DrawBGUnit(POINT pnt){  //画一个点的背景格子(一个十字)

    POINT point;
    point=PntTrans(pnt);

    ClearUnit(pnt);

    CPen pen;
    pen.CreatePen(PS_SOLID, 3, RGB(193, 210, 238));
    CFont font;
    font.CreateFont(12, 0, 0, 0,
                    FW_NORMAL,                 // Weight 
                    FALSE,                     // 斜体
                    FALSE,                     // 下划线
                    FALSE,                     // 删除线
                    ANSI_CHARSET,              // nCharSet
                    OUT_DEFAULT_PRECIS,        // nOutPrecision
                    CLIP_DEFAULT_PRECIS,       // nClipPrecision
                    DEFAULT_QUALITY,           // nQuality
                    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                    "Tahoma");                 // lpszFacename
                    //上面这个 MSDN 中抄的

    CString str;
    CPen *oldpen=(CPen*)p->SelectObject(&pen);
    p->MoveTo(point);
    if(pnt.x>1){
        p->LineTo(point.x-(WIDTH+1)/2, point.y);
        p->MoveTo(point);
    } else {  //如果是 x=1，不画格子写出数字
        CFont* oldfont=p->SelectObject(&font);
        str.Format("%d", SIZE+1-pnt.y);
        p->TextOut(point.x-24, point.y-6, str);
        p->SelectObject(oldfont);
    }
    if(pnt.y>1){
        p->LineTo(point.x, point.y-(WIDTH-1)/2);
        p->MoveTo(point);
    }
    if(pnt.x<SIZE){
        p->LineTo(point.x+(WIDTH+1)/2, point.y);
        p->MoveTo(point);
    }
    if(pnt.y<SIZE){
        p->LineTo(point.x, point.y+(WIDTH-1)/2);
       p->MoveTo(point);
    } else {  //同上，y=SIZE，写出坐标
        CFont* oldfont=p->SelectObject(&font);
        str.Format("%c", pnt.x-1+'A');
        p->TextOut(point.x-4, point.y+16, str);
        p->SelectObject(oldfont);
    }
    p->SelectObject(oldpen);
}

void DrawChessman(POINT point, int flag){  //在一个点画棋子，flag 1黑棋，-1白棋


    POINT pnt=PntTrans(point);

    //这一段书上弄错了没例子，这章的课件也没弄过来
    //就抄了 MSDN 中一个例子的代码
    CBitmap bmp;
    if(flag==1){
        bmp.LoadBitmap(IDB_BMP_BLACK);
    }else if(flag==-1){
        bmp.LoadBitmap(IDB_BMP_WHITE);
    }
    BITMAP bmpInfo;
    bmp.GetBitmap(&bmpInfo);
    CDC dcMemory;
    dcMemory.CreateCompatibleDC(p);
    CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);
    p->BitBlt(pnt.x-12, pnt.y-12, bmpInfo.bmWidth, bmpInfo.bmHeight,
              &dcMemory, 0, 0, SRCCOPY);
    dcMemory.SelectObject(pOldBitmap);

}

void ReDrawUnit(POINT pnt){  //重画某点
    DrawBGUnit(pnt);  //先重画背景
    int thechess=chess[pnt.x-1][pnt.y-1];
    if(thechess!=0){  //如果有棋子，棋子也重画
        DrawChessman(pnt, thechess);
    }
}

void DrawBG(){  //画整个背景(棋盘)
    
    POINT point;

    for(point.y=1;point.y<=SIZE;point.y++){
        for(point.x=1;point.x<=SIZE;point.x++){
            DrawBGUnit(point);
        }
    }

}

void PutWinLine(int x1, int y1, int x2, int y2){

    winline[0].x=x1;
    winline[0].y=y1;
    winline[1].x=x2;
    winline[1].y=y2;
}

void DrawWinLine(){  //画出五子线
    if(winline[0].x!=0&&winline[0].y!=0&&winline[1].x!=0&&winline[1].y!=0){
        CPen pen;
        pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        CPen *oldpen=(CPen*)p->SelectObject(&pen);
        p->MoveTo(PntTrans(winline[0]));
        p->LineTo(PntTrans(winline[1]));
        p->SelectObject(oldpen);
    }
}

void ReDraw(){  //重画整个棋局
    
    POINT point;

    for(point.y=1;point.y<=SIZE;point.y++){
        for(point.x=1;point.x<=SIZE;point.x++){
            ReDrawUnit(point);
        }
    }
    DrawWinLine();

}

bool IsWin(POINT point){  //判断是否有玩家胜出

    int i, s1=0, s2=0;

    //判断横向
    for(i=1;i<=4&&point.x-i>=1&&chess[point.x-1-i][point.y-1]==crtplayer;i++){
        s1++;
        if(s1+s2==4){  //当前点还有一个本方棋子，所以是 4 而不是 5
            PutWinLine(point.x-s1, point.y, point.x+s2, point.y);
            DrawWinLine();
            return true;
        }
    }
    for(i=1;i<=4&&point.x+i<=SIZE&&chess[point.x-1+i][point.y-1]==crtplayer;i++){
        s2++;
        if(s1+s2==4){
            PutWinLine(point.x-s1, point.y, point.x+s2, point.y);
            DrawWinLine();
            return true;
        }
    }

    s1=0, s2=0;

    //判断纵向
    for(i=1;i<=4&&point.y-i>=1&&chess[point.x-1][point.y-1-i]==crtplayer;i++){
        s1++;
        if(s1+s2==4){
            PutWinLine(point.x, point.y-s1, point.x, point.y+s2);
            DrawWinLine();
            return true;
        }
    }
    for(i=1;i<=4&&point.y+i<=SIZE&&chess[point.x-1][point.y-1+i]==crtplayer;i++){
        s2++;
        if(s1+s2==4){
            PutWinLine(point.x, point.y-s1, point.x, point.y+s2);
            DrawWinLine();
            return true;
        }
    }

    s1=0, s2=0;

    //左上到右下
    for(i=1;i<=4&&point.x-i>=1&&point.y-i>=1&&chess[point.x-1-i][point.y-1-i]==crtplayer;i++){
        s1++;
        if(s1+s2==4){
            PutWinLine(point.x-s1, point.y-s1, point.x+s2, point.y+s2);
            DrawWinLine();
            return true;
        }
    }
    for(i=1;i<=4&&point.x+i<=SIZE&&point.y+i<=SIZE&&chess[point.x-1+i][point.y-1+i]==crtplayer;i++){
        s2++;
        if(s1+s2==4){
            PutWinLine(point.x-s1, point.y-s1, point.x+s2, point.y+s2);
            DrawWinLine();
            return true;
        }
    }

    s1=0, s2=0;

    //左下到右上
    for(i=1;i<=4&&point.x-i>=1&&point.y+i<=SIZE&&chess[point.x-1-i][point.y-1+i]==crtplayer;i++){
        s1++;
        if(s1+s2==4){
            PutWinLine(point.x-s1, point.y+s1, point.x+s2, point.y-s2);
            DrawWinLine();
            return true;
        }
    }
    for(i=1;i<=4&&point.x+i<=SIZE&&point.y-i>=1&&chess[point.x-1+i][point.y-1-i]==crtplayer;i++){
        s2++;
        if(s1+s2==4){
            PutWinLine(point.x-s1, point.y+s1, point.x+s2, point.y-s2);
            DrawWinLine();
            return true;
        }
    }

    //这样判断似乎有些繁琐，不过我只能想到这步了

    return false;
}

void StartGame(){  //开始游戏
    
    DrawBG();  //画背景

    for(int i=0;i<SIZE;i++){  //棋局数据清零
        for(int j=0;j<SIZE;j++){
            chess[i][j]=0;
        }
    }
    
    PutWinLine(0, 0, 0, 0);
    
    crtpoint.x=(SIZE+1)/2;
    crtpoint.y=(SIZE+1)/2;  //设置当前点为中心那个点
    chessed=0;  //设置当前已走步数为 0

    CString str;
    CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    CStatusBar *pStatus=&pFrame->m_wndStatusBar;
    //初始化状态栏信息
    str="";
    pStatus->SetPaneText(3, str);
    str.Format("当前方: 黑棋");
    pStatus->SetPaneText(2, str);
    str.Format("已走 %d 步, 剩余 %d 步", chessed, SIZE*SIZE-chessed);
    pStatus->SetPaneText(1, str);

    crtplayer=1;  //黑棋先走

}
//自己写的部分到此，主要代码都集中在这里
//好像用的都是些纯C，连一个类都没自定义过……
//不过能实现预想中的功能就好
/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessView

IMPLEMENT_DYNCREATE(CxlFiveChessView, CView)

BEGIN_MESSAGE_MAP(CxlFiveChessView, CView)
	//{{AFX_MSG_MAP(CxlFiveChessView)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MENU_START, OnMenuStart)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	ON_COMMAND(ID_MENU_SOUND, OnMenuSound)
	ON_UPDATE_COMMAND_UI(ID_MENU_SOUND, OnUpdateMenuSound)
	ON_COMMAND(ID_MENU_HELP, OnMenuHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessView construction/destruction

CxlFiveChessView::CxlFiveChessView()
{
	// TODO: add construction code here

}

CxlFiveChessView::~CxlFiveChessView()
{
}

BOOL CxlFiveChessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessView drawing

void CxlFiveChessView::OnDraw(CDC* pDC)
{
	CxlFiveChessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    p=new CClientDC(this);  //将这个指针存到全局变量 p 中

    //画右边的图
    CBitmap bmp;
    bmp.LoadBitmap(IDB_BMP_BG);
    BITMAP bmpInfo;
    bmp.GetBitmap(&bmpInfo);
    CDC dcMemory;
    dcMemory.CreateCompatibleDC(p);
    CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);
    p->BitBlt(550, 10, bmpInfo.bmWidth, bmpInfo.bmHeight,
              &dcMemory, 0, 0, SRCCOPY);
    dcMemory.SelectObject(pOldBitmap);

    ReDraw();  //重画棋局
}

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessView diagnostics

#ifdef _DEBUG
void CxlFiveChessView::AssertValid() const
{
	CView::AssertValid();
}

void CxlFiveChessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CxlFiveChessDoc* CxlFiveChessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CxlFiveChessDoc)));
	return (CxlFiveChessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessView message handlers

void CxlFiveChessView::OnMouseMove(UINT nFlags, CPoint point) 
{
    CString str;
    CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    CStatusBar *pStatus=&pFrame->m_wndStatusBar;
    //下面要设置状态栏文本，这三行书里抄的，不懂什么意思
    //只知道搞出个 m_wndStatusBar 拿来用

    if(crtplayer!=0){  //如果游戏正在进行
        POINT pnt=PntTransA(point);  //pnt是逻辑点，point是物理点
//        if(pnt.x!=crtpoint.x||pnt.y!=crtpoint.y){  //如果鼠标移动改变了当前点            
            ReDrawUnit(crtpoint);  //重画上个点的棋局
            if(pnt.x>=1&&pnt.y>=1&&pnt.y<=SIZE&&pnt.y<=SIZE&&chess[pnt.x-1][pnt.y-1]==0){
            //如果新的点为有效点，并且该点没有棋子
                crtpoint=pnt;  //置当前点为这个新的点
                DrawChessman(crtpoint, crtplayer);  //在新的点的位置画出棋子(示意用)
                //置状态栏信息
                str.Format("当前位置: %c%d", pnt.x-1+'A', SIZE+1-pnt.y);
                pStatus->SetPaneText(3, str);
                switch(crtplayer){
                case 1:
                    str.Format("当前方: 黑棋");break;
                case -1:
                    str.Format("当前方: 白棋");break;
                }
                pStatus->SetPaneText(2, str);


            }
//        }
    }
	
	CView::OnMouseMove(nFlags, point);
}

void CxlFiveChessView::OnMenuStart() 
{

    StartGame();  //开始游戏
}

void CxlFiveChessView::OnLButtonUp(UINT nFlags, CPoint point)
{   //鼠标左键放开作为棋子落下的依据
    CString str;
    CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    CStatusBar *pStatus=&pFrame->m_wndStatusBar;
    //同上，下面要改变状态栏文本

    if(crtplayer!=0){  //如果游戏正在进行中
        POINT pnt=PntTransA(point);  //得到逻辑坐标        
        if(pnt.x>=1&&pnt.y>=1&&pnt.y<=SIZE&&pnt.y<=SIZE){  //如果当前点有效
        //两个条件写起来太长，分开了一下
            if(chess[pnt.x-1][pnt.y-1]==0){  //如果当前点没有棋子
                chess[pnt.x-1][pnt.y-1]=crtplayer;  //改变棋局数据，表明下了棋子
                DrawChessman(pnt, crtplayer);  //在棋盘上画出棋子
                chessed++;  //已走步数+1
                str.Format("已走 %d 步, 剩余 %d 步", chessed, SIZE*SIZE-chessed);
                pStatus->SetPaneText(1, str);  //更新状态栏信息
                if(playsound){  //如果音效开，播放下棋声
                    PlaySound(MAKEINTRESOURCE(IDR_WAVE_CHESS), AfxGetInstanceHandle(), SND_RESOURCE|SND_ASYNC);
                }
                if(IsWin(pnt)){  //如果有玩家获胜
                    if(playsound){  //如果音效开，播放胜利声
                        PlaySound(MAKEINTRESOURCE(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE|SND_ASYNC);
                    }
                    //信息框显示玩家获胜
                    if(crtplayer==1){
                        MessageBox("黑方取得了本局胜利！", "胜利", MB_OK|MB_ICONASTERISK);
                    }else if(crtplayer==-1){
                        MessageBox("白方取得了本局胜利！", "胜利", MB_OK|MB_ICONASTERISK);
                    }
                    crtplayer=0;  //禁止继续下棋
                }else if(chessed==SIZE*SIZE){  //如果棋盘下满且没人获胜
                    //显示平局信息
                    MessageBox("双方实力相当，平局。", "平局", MB_OK|MB_ICONASTERISK);
                    crtplayer=0;  //禁止继续下棋
                }else{
                    //既没玩家胜出也没平局
                    crtplayer=-crtplayer;  //交换玩家
                }
//            }else{
//                MessageBox("这里已经下过棋了，选别的地方吧~");
            //这个框框跳出来似乎会让人有点烦，就不跳了
            }
        }

    }
	
	CView::OnLButtonUp(nFlags, point);
}

void CxlFiveChessView::OnMenuExit() 
{
	PostQuitMessage(0);  //退出，那个 SDK 例子里抄的
}

void CxlFiveChessView::OnMenuSound() 
{
	playsound=!playsound;  //改变音效开关状态
	
}

void CxlFiveChessView::OnUpdateMenuSound(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(playsound?1:0);  //改变音效菜单选中状态
}

void CxlFiveChessView::OnMenuHelp() 
{
	ShellExecute(m_hWnd, "open", "help.chm", NULL, NULL, SW_SHOW);  //打开帮助文件
    //这个 ShellExecute() 很早以前玩“易语言”(跟VB有点相通)的时候用过
    //原来这里用这些 API 不用自己查声明和参数表
}
