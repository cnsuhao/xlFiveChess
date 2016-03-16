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
// ���￪ʼ���Լ�д�Ĵ���

#pragma comment(lib, "winmm.lib")
#include "mmsystem.h"
#include "MainFrm.h"
#define SIZE 15   //���̴�С����Ϊ 15*15
#define WIDTH 35  //���ӱ߳�
POINT crtpoint;  //��ǰ��
int crtplayer=0;  //��ǰ��ң�1���壬-1���壬0��ֹ
int chess[SIZE][SIZE];  //���ڴ洢��ֵĶ�ά���飬1���壬-1���壬0�հ�
int chessed=0;  //���߲���
POINT winline[2];  //ʤ���������ߵ���β����
CClientDC* p;
//�ͻ����豸ָ�룬��֪����ô�õ���OnDraw() ��ò������и����ʹ�Ϊȫ�ֱ�������
bool playsound=true;  //��Ч����



POINT PntTrans(int x, int y){  //�߼�����(��(1,1)��(SIZE,SIZE))ת��Ϊ��������

    POINT newpoint;

    newpoint.x=WIDTH*(x-1)+(WIDTH+1)/2+10;  //10Ϊ΢��
    newpoint.y=WIDTH*(y-1)+(WIDTH+1)/2+0;

    return newpoint;
}

POINT PntTrans(POINT point){  //����������أ����� C++ �ù���
    return PntTrans(point.x, point.y);
}

POINT PntTransA(POINT point){  //��������תΪ�߼�����

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

    //�����������ֱ������ģ�������ʼ�յò�����ȷ��ֵ
    //����ֻ����ټ��

    pnt.x=0;
    pnt.y=0;
    return pnt;
}

void ClearUnit(POINT pnt){  //���һ����

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

void DrawBGUnit(POINT pnt){  //��һ����ı�������(һ��ʮ��)

    POINT point;
    point=PntTrans(pnt);

    ClearUnit(pnt);

    CPen pen;
    pen.CreatePen(PS_SOLID, 3, RGB(193, 210, 238));
    CFont font;
    font.CreateFont(12, 0, 0, 0,
                    FW_NORMAL,                 // Weight 
                    FALSE,                     // б��
                    FALSE,                     // �»���
                    FALSE,                     // ɾ����
                    ANSI_CHARSET,              // nCharSet
                    OUT_DEFAULT_PRECIS,        // nOutPrecision
                    CLIP_DEFAULT_PRECIS,       // nClipPrecision
                    DEFAULT_QUALITY,           // nQuality
                    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                    "Tahoma");                 // lpszFacename
                    //������� MSDN �г���

    CString str;
    CPen *oldpen=(CPen*)p->SelectObject(&pen);
    p->MoveTo(point);
    if(pnt.x>1){
        p->LineTo(point.x-(WIDTH+1)/2, point.y);
        p->MoveTo(point);
    } else {  //����� x=1����������д������
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
    } else {  //ͬ�ϣ�y=SIZE��д������
        CFont* oldfont=p->SelectObject(&font);
        str.Format("%c", pnt.x-1+'A');
        p->TextOut(point.x-4, point.y+16, str);
        p->SelectObject(oldfont);
    }
    p->SelectObject(oldpen);
}

void DrawChessman(POINT point, int flag){  //��һ���㻭���ӣ�flag 1���壬-1����


    POINT pnt=PntTrans(point);

    //��һ������Ū����û���ӣ����µĿμ�ҲûŪ����
    //�ͳ��� MSDN ��һ�����ӵĴ���
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

void ReDrawUnit(POINT pnt){  //�ػ�ĳ��
    DrawBGUnit(pnt);  //���ػ�����
    int thechess=chess[pnt.x-1][pnt.y-1];
    if(thechess!=0){  //��������ӣ�����Ҳ�ػ�
        DrawChessman(pnt, thechess);
    }
}

void DrawBG(){  //����������(����)
    
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

void DrawWinLine(){  //����������
    if(winline[0].x!=0&&winline[0].y!=0&&winline[1].x!=0&&winline[1].y!=0){
        CPen pen;
        pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        CPen *oldpen=(CPen*)p->SelectObject(&pen);
        p->MoveTo(PntTrans(winline[0]));
        p->LineTo(PntTrans(winline[1]));
        p->SelectObject(oldpen);
    }
}

void ReDraw(){  //�ػ��������
    
    POINT point;

    for(point.y=1;point.y<=SIZE;point.y++){
        for(point.x=1;point.x<=SIZE;point.x++){
            ReDrawUnit(point);
        }
    }
    DrawWinLine();

}

bool IsWin(POINT point){  //�ж��Ƿ������ʤ��

    int i, s1=0, s2=0;

    //�жϺ���
    for(i=1;i<=4&&point.x-i>=1&&chess[point.x-1-i][point.y-1]==crtplayer;i++){
        s1++;
        if(s1+s2==4){  //��ǰ�㻹��һ���������ӣ������� 4 ������ 5
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

    //�ж�����
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

    //���ϵ�����
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

    //���µ�����
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

    //�����ж��ƺ���Щ������������ֻ���뵽�ⲽ��

    return false;
}

void StartGame(){  //��ʼ��Ϸ
    
    DrawBG();  //������

    for(int i=0;i<SIZE;i++){  //�����������
        for(int j=0;j<SIZE;j++){
            chess[i][j]=0;
        }
    }
    
    PutWinLine(0, 0, 0, 0);
    
    crtpoint.x=(SIZE+1)/2;
    crtpoint.y=(SIZE+1)/2;  //���õ�ǰ��Ϊ�����Ǹ���
    chessed=0;  //���õ�ǰ���߲���Ϊ 0

    CString str;
    CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    CStatusBar *pStatus=&pFrame->m_wndStatusBar;
    //��ʼ��״̬����Ϣ
    str="";
    pStatus->SetPaneText(3, str);
    str.Format("��ǰ��: ����");
    pStatus->SetPaneText(2, str);
    str.Format("���� %d ��, ʣ�� %d ��", chessed, SIZE*SIZE-chessed);
    pStatus->SetPaneText(1, str);

    crtplayer=1;  //��������

}
//�Լ�д�Ĳ��ֵ��ˣ���Ҫ���붼����������
//�����õĶ���Щ��C����һ���඼û�Զ��������
//������ʵ��Ԥ���еĹ��ܾͺ�
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

    p=new CClientDC(this);  //�����ָ��浽ȫ�ֱ��� p ��

    //���ұߵ�ͼ
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

    ReDraw();  //�ػ����
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
    //����Ҫ����״̬���ı������������ﳭ�ģ�����ʲô��˼
    //ֻ֪������� m_wndStatusBar ������

    if(crtplayer!=0){  //�����Ϸ���ڽ���
        POINT pnt=PntTransA(point);  //pnt���߼��㣬point�������
//        if(pnt.x!=crtpoint.x||pnt.y!=crtpoint.y){  //�������ƶ��ı��˵�ǰ��            
            ReDrawUnit(crtpoint);  //�ػ��ϸ�������
            if(pnt.x>=1&&pnt.y>=1&&pnt.y<=SIZE&&pnt.y<=SIZE&&chess[pnt.x-1][pnt.y-1]==0){
            //����µĵ�Ϊ��Ч�㣬���Ҹõ�û������
                crtpoint=pnt;  //�õ�ǰ��Ϊ����µĵ�
                DrawChessman(crtpoint, crtplayer);  //���µĵ��λ�û�������(ʾ����)
                //��״̬����Ϣ
                str.Format("��ǰλ��: %c%d", pnt.x-1+'A', SIZE+1-pnt.y);
                pStatus->SetPaneText(3, str);
                switch(crtplayer){
                case 1:
                    str.Format("��ǰ��: ����");break;
                case -1:
                    str.Format("��ǰ��: ����");break;
                }
                pStatus->SetPaneText(2, str);


            }
//        }
    }
	
	CView::OnMouseMove(nFlags, point);
}

void CxlFiveChessView::OnMenuStart() 
{

    StartGame();  //��ʼ��Ϸ
}

void CxlFiveChessView::OnLButtonUp(UINT nFlags, CPoint point)
{   //�������ſ���Ϊ�������µ�����
    CString str;
    CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    CStatusBar *pStatus=&pFrame->m_wndStatusBar;
    //ͬ�ϣ�����Ҫ�ı�״̬���ı�

    if(crtplayer!=0){  //�����Ϸ���ڽ�����
        POINT pnt=PntTransA(point);  //�õ��߼�����        
        if(pnt.x>=1&&pnt.y>=1&&pnt.y<=SIZE&&pnt.y<=SIZE){  //�����ǰ����Ч
        //��������д����̫�����ֿ���һ��
            if(chess[pnt.x-1][pnt.y-1]==0){  //�����ǰ��û������
                chess[pnt.x-1][pnt.y-1]=crtplayer;  //�ı�������ݣ�������������
                DrawChessman(pnt, crtplayer);  //�������ϻ�������
                chessed++;  //���߲���+1
                str.Format("���� %d ��, ʣ�� %d ��", chessed, SIZE*SIZE-chessed);
                pStatus->SetPaneText(1, str);  //����״̬����Ϣ
                if(playsound){  //�����Ч��������������
                    PlaySound(MAKEINTRESOURCE(IDR_WAVE_CHESS), AfxGetInstanceHandle(), SND_RESOURCE|SND_ASYNC);
                }
                if(IsWin(pnt)){  //�������һ�ʤ
                    if(playsound){  //�����Ч��������ʤ����
                        PlaySound(MAKEINTRESOURCE(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE|SND_ASYNC);
                    }
                    //��Ϣ����ʾ��һ�ʤ
                    if(crtplayer==1){
                        MessageBox("�ڷ�ȡ���˱���ʤ����", "ʤ��", MB_OK|MB_ICONASTERISK);
                    }else if(crtplayer==-1){
                        MessageBox("�׷�ȡ���˱���ʤ����", "ʤ��", MB_OK|MB_ICONASTERISK);
                    }
                    crtplayer=0;  //��ֹ��������
                }else if(chessed==SIZE*SIZE){  //�������������û�˻�ʤ
                    //��ʾƽ����Ϣ
                    MessageBox("˫��ʵ���൱��ƽ�֡�", "ƽ��", MB_OK|MB_ICONASTERISK);
                    crtplayer=0;  //��ֹ��������
                }else{
                    //��û���ʤ��Ҳûƽ��
                    crtplayer=-crtplayer;  //�������
                }
//            }else{
//                MessageBox("�����Ѿ��¹����ˣ�ѡ��ĵط���~");
            //�������������ƺ��������е㷳���Ͳ�����
            }
        }

    }
	
	CView::OnLButtonUp(nFlags, point);
}

void CxlFiveChessView::OnMenuExit() 
{
	PostQuitMessage(0);  //�˳����Ǹ� SDK �����ﳭ��
}

void CxlFiveChessView::OnMenuSound() 
{
	playsound=!playsound;  //�ı���Ч����״̬
	
}

void CxlFiveChessView::OnUpdateMenuSound(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(playsound?1:0);  //�ı���Ч�˵�ѡ��״̬
}

void CxlFiveChessView::OnMenuHelp() 
{
	ShellExecute(m_hWnd, "open", "help.chm", NULL, NULL, SW_SHOW);  //�򿪰����ļ�
    //��� ShellExecute() ������ǰ�桰�����ԡ�(��VB�е���ͨ)��ʱ���ù�
    //ԭ����������Щ API �����Լ��������Ͳ�����
}
