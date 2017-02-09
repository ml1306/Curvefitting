#include "stdafx.h"
#include "LineChartCtrl.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrl

CLineChartCtrl::CLineChartCtrl()
{
	RGB_BK   = RGB(0,64,64);
	RGB_TITLE= RGB(250,0,250);
	RGB_UNIT = RGB(250,250,0);
	RGB_FONT = RGB(255,255,255);
	RGB_COOR = RGB(204,200,50);
	RGB_LINE = RGB(0,180,0);
	RGB_VP   = RGB(200,12,12);
	RGB_SCALE= RGB(0,255,255);
	RGB_SD   = RGB(100,100,0);
	m_VP	 = FALSE;
	m_Regress= FALSE;
	m_SD     = FALSE;
	x_Decimal= 0;
	y_Decimal=4;
	GetColor();
	AddChart();
	ActiveChart=0;
	m_Index = -1;
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
    memcpy(&m_Logfont, &(ncm.lfMessageFont), sizeof(LOGFONT));

	CLineChartCtrl::RegisterWndClass(AfxGetInstanceHandle());
}

void CLineChartCtrl::GetColor()
{
	F_Color = fopen("Setting\\Colorset.ini","r+b");
	COLORREF G_Color;
	if(!F_Color)return;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_BK = G_Color;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_TITLE = G_Color;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_UNIT = G_Color;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_FONT = G_Color;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_COOR = G_Color;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_LINE = G_Color;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_VP = G_Color;
	fread(&G_Color,1,sizeof(COLORREF),F_Color);RGB_SCALE = G_Color;
	fclose(F_Color);
}


CLineChartCtrl::~CLineChartCtrl()
{
	CutInTp();
}


BEGIN_MESSAGE_MAP(CLineChartCtrl, CWnd)
	//{{AFX_MSG_MAP(CLineChartCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CLineChartCtrl::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName = "LINE_CHART_CTRL"; // matches class name in client
	wc.hInstance = hInstance;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = NULL;
	wc.hbrBackground = (HBRUSH) ::GetStockObject(LTGRAY_BRUSH);
	wc.style = CS_GLOBALCLASS; // To be modified
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	return (::RegisterClass(&wc) != 0);
}


BOOL CLineChartCtrl::AddChart(int Index,
						 LPCTSTR TableName,  //图表名称
						 LPCTSTR x_Unit,     //图表X轴单位
						 LPCTSTR y_Unit)     //图表y轴单位
{
	if(GetPosition(Index))
	{
		m_Index = Index;
		InitChart(m_Index,TableName,x_Unit,y_Unit);
		return TRUE;
	}
	CData* pItem = new CData;
	if(TableName)strcpy(pItem->TableName,TableName);
	else strcpy(pItem->TableName,"");
	if(x_Unit)strcpy(pItem->x_Unit,x_Unit);
	else strcpy(pItem->x_Unit,"");
	if(y_Unit)strcpy(pItem->y_Unit,y_Unit);
	else strcpy(pItem->y_Unit,"");
	pItem->x_Min = 0;
	pItem->x_Max = 0;
	pItem->y_Min = 0;
	pItem->y_Max = 3.0;
	pItem->m_Point = 0;
	pItem->Y_MaxMin = FALSE;
	pItem->Y_Max=0;
	pItem->Y_Min=0;
	pItem->x_Decimal = x_Decimal;
	pItem->y_Decimal = y_Decimal;
	pItem->m_Num  = 0;
	pItem->Index = Index;
	m_Index = pItem->Index;
	m_Pt.AddTail(pItem);
	return TRUE;
}

void CLineChartCtrl::InitGraph()
{
	CPaintDC dc(this); // device context for painting
	if (m_MemDC.GetSafeHdc() == NULL)
	{
		GetClientRect(rcClient);
		m_MemDC.CreateCompatibleDC(&dc);
		m_Bitmap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
		m_MemDC.SelectObject(m_Bitmap);
		m_MemDC.SelectStockObject(NULL_PEN);
		CBrush Brush;
		CBrush *OldBrush;
		Brush.CreateSolidBrush(RGB_BK);
		OldBrush=m_MemDC.SelectObject(&Brush);
		m_MemDC.Rectangle(rcClient);
		m_MemDC.SelectObject(OldBrush);
		DeleteObject(Brush);
		DrawChart();
	}
}

void CLineChartCtrl::CutInTp()
{
	CData *pItem;
	POSITION pos;
	if(m_Pt.GetCount()==0)return;
	pos=m_Pt.GetHeadPosition();
	while(pos>0){
		pItem=(CData *)m_Pt.GetAt(pos);
		if(pItem->m_Num)delete pItem->m_Data;
		delete pItem;
		m_Pt.GetNext(pos);
	}
	m_Pt.RemoveAll();
}

void CLineChartCtrl::InitChart(int Index,
						 LPCTSTR TableName,  //图表名称
						 LPCTSTR x_Unit,     //图表X轴单位
						 LPCTSTR y_Unit)     //图表y轴单位
{
	if(Index<0||Index>=m_Pt.GetCount())return;
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos<=0)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	if(pItem->m_Num)delete pItem->m_Data;
	pItem->m_Num   = 0;
	m_Other_Color = FALSE;
	if(TableName)strcpy(pItem->TableName,TableName);
	if(x_Unit)strcpy(pItem->x_Unit,x_Unit);
	if(y_Unit)strcpy(pItem->y_Unit,y_Unit);
	DrawChart(Index);
}


void CLineChartCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	InitGraph();
	if(m_MemDC.GetSafeHdc())dc.BitBlt(0, 0, rcClient.Width()-1, rcClient.Height()-1, &m_MemDC, 0, 0, SRCCOPY);
}

POSITION CLineChartCtrl::GetPosition(int Index)
{
	CData *pItem;
	POSITION pos;
	if(Index<0||Index+1>m_Pt.GetCount())return NULL;
	pos=m_Pt.GetHeadPosition();
	while(pos>0){
		pItem=(CData *)m_Pt.GetAt(pos);
		if(pItem->Index == Index)return pos;
		m_Pt.GetNext(pos);
	}
	return NULL;
}


void CLineChartCtrl::SetData(float x,float y,int Index)
{
	CData *pItem;
	int i;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
			
	if (x > pItem->x_Max) pItem->x_Max = x;
	if (x < pItem->x_Min) pItem->x_Min = x;

	if (y > pItem->y_Max) pItem->y_Max = y;
	if (y < pItem->y_Min) pItem->y_Min = y;

	DATA *Dat;
	if(pItem->m_Num)
	{
		Dat=new DATA[pItem->m_Num+1];
		for(i=0;i<pItem->m_Num;i++)Dat[i]=pItem->m_Data[i];
		Dat[i].x = x;Dat[i].y = y;
		delete pItem->m_Data;
		pItem->m_Num++;
		pItem->m_Data=new DATA[pItem->m_Num];
		for(i=0;i<pItem->m_Num;i++)pItem->m_Data[i]=Dat[i];
		delete Dat;
	}
	else
	{
		pItem->m_Num++;
		pItem->m_Data=new DATA[pItem->m_Num];
		pItem->m_Data[pItem->m_Num-1].x = x;
		pItem->m_Data[pItem->m_Num-1].y = y;
		pItem->x_Max = pItem->x_Min = x;
		pItem->y_Max = pItem->y_Min = y;
	}
	if(pItem->m_Point&&(pItem->m_Point<pItem->m_Num))pItem->m_Point = pItem->m_Num;
	if(pItem->Y_MaxMin)
	{
		if (pItem->y_Max > pItem->Y_Max) pItem->Y_Max = pItem->y_Max;
		if (pItem->y_Min < pItem->Y_Min) pItem->Y_Min = pItem->y_Min;
	}
}



void CLineChartCtrl::SetTitle(LPCTSTR TableName,int Index)
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	strcpy(pItem->TableName,TableName);
}

void CLineChartCtrl::SetXUnit(LPCTSTR x_Unit,int Index)
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	strcpy(pItem->x_Unit,x_Unit);
}

void CLineChartCtrl::SetYUnit(LPCTSTR y_Unit,int Index)
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	strcpy(pItem->y_Unit,y_Unit);
}

void CLineChartCtrl::SetXDecimal(short Decimal,int Index)
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	pItem->x_Decimal = Decimal;
}

void CLineChartCtrl::SetYDecimal(short Decimal,int Index)
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	pItem->y_Decimal = Decimal;
}


void CLineChartCtrl::DrawChart(int Index)
{
	ActiveChart=Index;
	DrawWindow(Index);		//画窗口
	if(!m_SD)DrawLine(Index);		//画数据线
	if(m_VP)DrawVP(Index);			//画平均值线
	if(m_Regress)DrawRegress(Index);		//画斜率线
	if(m_SD)DrawSD(Index);
	InvalidateRect(rcClient, FALSE);
}

void CLineChartCtrl::SetSurveyPoint(short Num,int Index)
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	pItem->m_Point = Num;
}

void CLineChartCtrl::SetYMaxMin(float YMax,float YMin,int Index) //为Y轴赋最大值勤和最小值
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	if(YMax<=YMin)pItem->Y_MaxMin = FALSE;
	else
	{
		pItem->Y_Max	= YMax;
		pItem->Y_Min	= YMin;
		pItem->Y_MaxMin = TRUE;
	}
}


void CLineChartCtrl::DrawWindow(int Index)
{
	CRect Rf;
	char	tt[20],t1[20];
	char	y_char[15][12];
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
//背景填色彩
	if (m_MemDC.GetSafeHdc() == NULL)
	{
		InitGraph();
		if (m_MemDC.GetSafeHdc() == NULL)
		{
			return;
		}
	}
	strcpy(x_Format,"%.");sprintf(tt,"%d",pItem->x_Decimal);strcat(x_Format,tt);strcat(x_Format,"f"); //X方向数据格式
	strcpy(y_Format,"%.");sprintf(tt,"%d",pItem->y_Decimal);strcat(y_Format,tt);strcat(y_Format,"f"); //Y方向数据格式
	m_MemDC.SelectStockObject(NULL_PEN );
	CBrush Brush;  
	CBrush *OldBrush;
	Brush.CreateSolidBrush(RGB_BK);
	OldBrush=m_MemDC.SelectObject(&Brush);
	m_MemDC.Rectangle(rcClient);
	m_MemDC.SelectObject(OldBrush);
	DeleteObject(Brush);

//标出注句
	int len;
	rGraph      = rcClient;
	m_MemDC.SetBkMode(TRANSPARENT);
	if(strlen(pItem->TableName))
	{
		CFont testFont;
		SetFont(-12,0,0,0,700,0,0,0,0,3,2,1,34);
        testFont.CreateFontIndirect(&m_Logfont);
		pOldFont=m_MemDC.SelectObject(&testFont);
        bm = m_MemDC.GetTextExtent(_T(""),1).cx/2;
		m_MemDC.GetTextMetrics(&tm);
		bh = tm.tmHeight+bm;
		bw = tm.tmAveCharWidth;
		Rf = rcClient;
		Rf.top+=4;
		Rf.bottom = bh;
		m_MemDC.SetTextColor(RGB_TITLE);
		m_MemDC.DrawText(pItem->TableName,Rf,DT_VCENTER|DT_CENTER);
		m_MemDC.SelectObject(pOldFont);
		testFont.DeleteObject();
		rGraph = Rf;
		rGraph.top     = Rf.bottom-bh/2;
		rGraph.bottom  = rcClient.bottom;
	}
	else rGraph.top+=4;
	CFont testFont1;
	SetFont(-11,0,0,0,400,0,0,0,0,3,2,1,34);
    testFont1.CreateFontIndirect(&m_Logfont);
	pOldFont=(CFont *)m_MemDC.SelectObject(&testFont1);
	m_MemDC.GetTextMetrics(&tm);
    bm = m_MemDC.GetTextExtent(_T(""),1).cx/3;
	bh = tm.tmHeight+bm;
	bw = tm.tmAveCharWidth+bm-1;
	if(pItem->Y_MaxMin)
	{
		sprintf(tt,y_Format,pItem->Y_Max);
		sprintf(t1,y_Format,pItem->Y_Min);;
	}
	else
	{
		sprintf(tt,y_Format,pItem->y_Max);
		sprintf(t1,y_Format,pItem->y_Min);;
	}
	if(strlen(tt)<strlen(t1))strcpy(tt,t1);
	if(strlen(pItem->y_Unit))
	{
		rGraph.top+=bh;
		len = strlen(pItem->y_Unit)/2*bw;
		if((int)strlen(tt)*bw<len)rGraph.left = rcClient.left+4+len;
		else rGraph.left = rcClient.left+4+strlen(tt)*bw;
		m_MemDC.SetTextColor(RGB_UNIT);
		m_MemDC.TextOut(rGraph.left-len,rGraph.top-bh,pItem->y_Unit);
	}
	else rGraph.left+=strlen(tt)*bw+4;
	rGraph.bottom-=4;
	rGraph.bottom-=bh;
	rGraph.right-=6;
	if(strlen(pItem->x_Unit))
	{
		rGraph.right-=strlen(pItem->x_Unit)*bw;
		m_MemDC.SetTextColor(RGB_UNIT);
		m_MemDC.TextOut(rGraph.right+4,rGraph.bottom-7,pItem->x_Unit);
	}
	else rGraph.right-=bw;
//得到绘图范围，并在坐标轴上标出短线，写上数字============================================	
	rDraw = rGraph;
	rDraw.InflateRect(-4,-4,-4,-4);
	rDraw.right-=6;
	CPen pen_Line;
	CPen* pOldPen_Line;
	pen_Line.CreatePen(PS_SOLID, 1, RGB_COOR);
	pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
	m_MemDC.MoveTo(rGraph.left,rGraph.top);
	m_MemDC.LineTo(rGraph.left,rGraph.bottom);
	m_MemDC.LineTo(rGraph.right,rGraph.bottom);
	
//---------纵轴处理------------------------------------------
	int i;

	if(!m_SD)
	{
		if(pItem->m_Num==1&&pItem->m_Point==0)
		{
			m_x = rDraw.left+rDraw.Width()/2;
			m_y = rDraw.bottom-rDraw.Height()/2;
			m_MemDC.MoveTo(rGraph.left,m_y);
			m_MemDC.LineTo(rGraph.left+4,m_y);
			m_MemDC.MoveTo(m_x,rGraph.bottom);
			m_MemDC.LineTo(m_x,rGraph.bottom-2);
			sprintf(tt,y_Format,pItem->m_Data[0].y);
			m_MemDC.TextOut(rGraph.left-int(strlen(tt))*bw-4,m_y-bh/2+1,tt);
			sprintf(tt,x_Format,pItem->m_Data[0].x);
			m_MemDC.TextOut(m_x-int(strlen(tt)*bw/2.0),rGraph.bottom+2,tt);
			m_MemDC.SelectObject(pOldPen_Line);
			DeleteObject(pen_Line);
			m_MemDC.SelectObject(pOldFont);
			DeleteObject(testFont1);
			return;
		}
		if(pItem->Y_MaxMin)y_Scale = (double)((pItem->Y_Max-pItem->Y_Min))/10.0;  //纵轴方向的比例
		else y_Scale = (double)((pItem->y_Max-pItem->y_Min))/10.0;  //纵轴方向的比例
		m_MemDC.SetTextColor(RGB_FONT);
		for(i=0;i<11;i++)
		{
			if(pItem->Y_MaxMin)sprintf(tt,y_Format,pItem->Y_Min+y_Scale*i);
			else sprintf(tt,y_Format,pItem->y_Min+y_Scale*i);
			if(i==0)strcpy(y_char[i],tt);
			else if(i==10)strcpy(y_char[i],tt);
			else
			{
				if(i-i/2*2==0)
				{
					if(strcmp(y_char[i-2],tt)&&strlen(y_char[i-2]))strcpy(y_char[i],tt);
					else strcpy(y_char[i],"");
				}
				else strcpy(y_char[i],"");
			}
		}
		strcpy(tt,y_char[10]);
		for(i=9;i>0;i--)
		{
			if(strlen(y_char[i])==0)continue;
			if(strcmp(tt,y_char[i])==0)strcpy(y_char[i],"");
			else strcpy(tt,y_char[i]);
		}
		strcpy(tt,y_char[0]);
		for(i=0;i<11;i++)
		{
			if(strlen(y_char[i])==0)continue;
			if(strlen(y_char[i])>strlen(tt))strcpy(tt,y_char[i]);
		}
		for(i=0;i<11;i++)
		{
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height()*(y_Scale*i))/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height()*(y_Scale*i))/(pItem->y_Max-pItem->y_Min));
			m_MemDC.MoveTo(rGraph.left,m_y);
			m_MemDC.LineTo(rGraph.left+4,m_y);
			if((i==0||i==10)&&strlen(y_char[i]))m_MemDC.TextOut(rGraph.left-int(strlen(y_char[i]))*bw-2,m_y-bh/2+1,y_char[i]);
			else
			{
				if(i-i/2*2==0&&strlen(y_char[i]))m_MemDC.TextOut(rGraph.left-int(strlen(y_char[i]))*bw-2,m_y-bh/2+1,y_char[i]);
			}
		}
//----横轴处理---------------------------------------
		int count;
		if(pItem->m_Point)
		{
			x_Scale = (double)(rDraw.Width())/(pItem->m_Point-1);
			count = pItem->m_Point;
		}
		else 
		{
			x_Scale = (double)(rDraw.Width())/(double)(pItem->x_Max-pItem->x_Min);
			count = pItem->m_Num;
		}
		int Step = 1;
		Step = count/10;
		if(Step==0)Step = 1;
		for(i=0;i<count;i+=Step)
		{
			if(pItem->m_Point==0)
			{
				m_x=rDraw.left+int(x_Scale*(pItem->m_Data[i].x-pItem->x_Min));
				sprintf(tt,x_Format,pItem->m_Data[i].x);
			}
			else
			{
				m_x=rDraw.left+int(x_Scale*i);
				sprintf(tt,"%d",i+1);
			}
			m_MemDC.MoveTo(m_x,rGraph.bottom);
			m_MemDC.LineTo(m_x,rGraph.bottom-4);
			if(!m_SD)m_MemDC.TextOut(m_x-int(strlen(tt)*bw/2.0),rGraph.bottom+2,tt);
		}
		if(pItem->m_Point)
		{
			m_x=rDraw.left+int(x_Scale*i);
			m_MemDC.MoveTo(m_x,rGraph.bottom);
			m_MemDC.LineTo(m_x,rGraph.bottom-4);
			if(!m_SD)
			{
				m_x=rDraw.left+int(x_Scale*count);
				sprintf(tt,"%d",count);
				m_MemDC.TextOut(m_x-int(strlen(tt)*bw/2.0),rGraph.bottom+2,tt);
			}
		}
	}
	m_MemDC.SelectObject(pOldPen_Line);
	DeleteObject(pen_Line);
	m_MemDC.SelectObject(pOldFont);
	testFont1.DeleteObject();
}


void CLineChartCtrl::DrawLine(int Index)
{
	int i;
	CPen* pOldPen_Line;
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	if(pItem->m_Num==1&&pItem->m_Point==0)
	{
		m_x = rDraw.left+rDraw.Width()/2;
		m_y = rDraw.bottom-rDraw.Height()/2;
		m_MemDC.Ellipse(m_x-2,m_y-2,m_x+2,m_y+2);
	}
	else
	{
		m_Other_Color_Num = 0;
		for(i=0;i<pItem->m_Num;i++)
		{
			CPen pen_Line;
			if(pItem->m_Point==0)m_x=rDraw.left+int(x_Scale*(pItem->m_Data[i].x-pItem->x_Min));
			else m_x=rDraw.left+int(x_Scale*i);
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height())*(pItem->m_Data[i].y-pItem->Y_Min)/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height())*(pItem->m_Data[i].y-pItem->y_Min)/(pItem->y_Max-pItem->y_Min));
			if(m_Other_Color)
			{
				if(i>=m_Begin_Point&&m_Other_Color_Num<m_gg)
				{
					m_Other_Color_Point[m_Other_Color_Num++] = pItem->m_Data[i];
					if(m_Other_Color_Num>=695)m_Other_Color_Num--;
					if(i>m_Begin_Point&&m_Other_Color_Num<=m_gg)
					{
						pen_Line.CreatePen(PS_SOLID, 1, RGB(255,255,0));
						pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
					}
					else 
					{
						pen_Line.CreatePen(PS_SOLID, 1, RGB_LINE);
						pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
					}
				}
				else 
				{
					pen_Line.CreatePen(PS_SOLID, 1, RGB_LINE);
					pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
				}
			}
			else 
			{
				pen_Line.CreatePen(PS_SOLID, 1, RGB_LINE);
				pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
			}
			if(i==0)m_MemDC.MoveTo(m_x,m_y);
			else m_MemDC.LineTo(m_x,m_y);
			m_MemDC.Ellipse(m_x-2,m_y-2,m_x+2,m_y+2);
			//m_MemDC.Ellipse(m_x-3,m_y-3,m_x+3,m_y+3);
			m_MemDC.SelectObject(pOldPen_Line);
			DeleteObject(pen_Line);
		}
	}
}



void CLineChartCtrl::DrawVP(int Index)
{
	CPen pen_Line;
	CPen* pOldPen_Line;
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return ;
	pItem=(CData *)m_Pt.GetAt(pos);
	pen_Line.CreatePen(PS_SOLID, 1, RGB_VP);
	pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
	GetVP(Index);
	if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height())*(VP-pItem->Y_Min)/(pItem->Y_Max-pItem->Y_Min));
	else m_y = rDraw.bottom-(int)((rDraw.Height())*(VP-pItem->y_Min)/(pItem->y_Max-pItem->y_Min));
	m_x = rDraw.left;
	m_px = rDraw.right;
	m_MemDC.MoveTo(m_x,m_y);
	m_MemDC.LineTo(m_px,m_y);
	m_MemDC.SelectObject(pOldPen_Line);
	DeleteObject(pen_Line);
	CFont testFont2;
	SetFont(-12,0,0,0,400,0,0,0,0,3,2,1,34);
    testFont2.CreateFontIndirect(&m_Logfont);
	pOldFont=(CFont *)m_MemDC.SelectObject(&testFont2);
	m_MemDC.GetTextMetrics(&tm);
    bm = m_MemDC.GetTextExtent(_T(""),1).cx/3;
	bh = tm.tmHeight+bm;
	bw = tm.tmAveCharWidth+bm-1;
	char tt[12];
	sprintf(tt,y_Format,VP);
	m_MemDC.SetTextColor(RGB_VP);
	m_MemDC.TextOut(m_x+(m_px-m_x)/2-strlen(tt)*bw/2,m_y-bh,tt);

	m_MemDC.SelectObject(pOldFont);
	DeleteObject(testFont2);
}

double CLineChartCtrl::GetVP(int Index)
{
	int i,no1,no2;
	float a1[3],b,Max=-100000000,Min=10000000;
	CData *pItem;
	if(!m_Other_Color)
	{
		POSITION pos = GetPosition(Index);
		if(pos==NULL)return 0;
		pItem=(CData *)m_Pt.GetAt(pos);
		if(pItem->m_Num==0)VP=0;
		else if(pItem->m_Num<=2)
		{
			VP = 0;
			for(i=0;i<pItem->m_Num;i++)VP =VP+pItem->m_Data[i].y;
			VP = VP/float(pItem->m_Num);
		}
		else if(pItem->m_Num==3)
		{
			a1[0]=(float)fabs(pItem->m_Data[0].y-pItem->m_Data[1].y);
			a1[1]=(float)fabs(pItem->m_Data[1].y-pItem->m_Data[2].y);
			a1[2]=(float)fabs(pItem->m_Data[0].y-pItem->m_Data[2].y);
			b=a1[0];no1=0;
			for(i=1;i<pItem->m_Num;i++)
			{
				if(b>a1[i])
				{
					b=a1[i];
					no1=i;
				}
			}
			if(no1==0)VP=(float)((pItem->m_Data[0].y+pItem->m_Data[1].y)/2.0);
			if(no1==1)VP=(float)((pItem->m_Data[2].y+pItem->m_Data[1].y)/2.0);
			if(no1==2)VP=(float)((pItem->m_Data[2].y+pItem->m_Data[0].y)/2.0);
		}
		else
		{
			VP = 0;
			for(i=0;i<pItem->m_Num;i++)
			{
				VP+=pItem->m_Data[i].y;
				if(Max<pItem->m_Data[i].y){Max=pItem->m_Data[i].y;no1 = i;}
				if(Min>pItem->m_Data[i].y){Min=pItem->m_Data[i].y;no2 = i;}
			}
			VP = VP-pItem->m_Data[no1].y-pItem->m_Data[no2].y;
			VP = VP/float(pItem->m_Num-2);
		}
	}
	else
	{
		if(m_Other_Color_Num==0)VP=m_Other_Color_Point[0].y;
		else if(m_Other_Color_Num<=2)
		{
			VP = 0;
			for(i=0;i<m_Other_Color_Num;i++)VP =VP+m_Other_Color_Point[i].y;
			VP = VP/(float)(m_Other_Color_Num);
		}
		else if(m_Other_Color_Num==3)
		{
			a1[0]=(float)fabs(m_Other_Color_Point[0].y-m_Other_Color_Point[1].y);
			a1[1]=(float)fabs(m_Other_Color_Point[1].y-m_Other_Color_Point[2].y);
			a1[2]=(float)fabs(m_Other_Color_Point[0].y-m_Other_Color_Point[2].y);
			b=a1[0];no1=0;
			for(i=1;i<m_Other_Color_Num;i++)
			{
				if(b>a1[i])
				{
					b=a1[i];
					no1=i;
				}
			}
			if(no1==0)VP=(m_Other_Color_Point[0].y+m_Other_Color_Point[1].y)/(float)2.0;
			if(no1==1)VP=(m_Other_Color_Point[2].y+m_Other_Color_Point[1].y)/(float)2.0;
			if(no1==2)VP=(m_Other_Color_Point[2].y+m_Other_Color_Point[0].y)/(float)2.0;
		}
		else
		{
			VP = 0;
			for(i=0;i<m_Other_Color_Num;i++)
			{
				VP+=m_Other_Color_Point[i].y;
				if(Max<m_Other_Color_Point[i].y){Max=m_Other_Color_Point[i].y;no1 = i;}
				if(Min>m_Other_Color_Point[i].y){Min=m_Other_Color_Point[i].y;no2 = i;}
			}
			VP = VP-m_Other_Color_Point[no1].y-m_Other_Color_Point[no2].y;
			VP = VP/(float)(m_Other_Color_Num-2);
		}
	}
	return VP;
}

double CLineChartCtrl::GetY_Max(int Index)		//得到Y轴的数据最大值
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return 0;
	pItem=(CData *)m_Pt.GetAt(pos);
	return pItem->y_Max;
}
double CLineChartCtrl::GetY_Min(int Index)		//得到Y轴的数据最小值
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return 0;
	pItem=(CData *)m_Pt.GetAt(pos);
	return pItem->y_Min;
}
double CLineChartCtrl::GetX_Max(int Index)		//得到X轴的数据最大值
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return 0;
	pItem=(CData *)m_Pt.GetAt(pos);
	return pItem->x_Max;
}

double CLineChartCtrl::GetX_Min(int Index)		//得到X轴的数据最小值
{
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return 0;
	pItem=(CData *)m_Pt.GetAt(pos);
	return pItem->x_Min;
}

void CLineChartCtrl::DrawRegress(int Index)
{
	CPen pen_Line;
	CPen* pOldPen_Line;
	CData *pItem;
	char tt[40];
	double bb1;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return ;
	pItem=(CData *)m_Pt.GetAt(pos);
	if(pItem->m_Num<2)return;
	Regress(Index);
	if(!m_Other_Color||(m_Other_Color&&m_Other_Color_Num<2))
	{	
		if(pItem->m_Num>=2)
		{
			pen_Line.CreatePen(PS_SOLID, 1, RGB_SCALE);
			pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
			bb1 = Qj+DltA*pItem->m_Data[0].x;
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->Y_Min))/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->y_Min))/(pItem->y_Max-pItem->y_Min));
			m_MemDC.SetTextColor(RGB_SCALE);
			m_MemDC.MoveTo(rDraw.left,m_y);
			bb1 = Qj+DltA*pItem->m_Data[pItem->m_Num-1].x;
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->Y_Min))/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->y_Min))/(pItem->y_Max-pItem->y_Min));
			m_MemDC.LineTo(rDraw.right,m_y);
			m_MemDC.SelectObject(pOldPen_Line);
			DeleteObject(pen_Line);
			m_x = rDraw.left;
		}
	}
	else
	{
		if(m_Other_Color_Num>=2)
		{
			pen_Line.CreatePen(PS_SOLID, 1, RGB_SCALE);
			pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
			bb1 = Qj+DltA*pItem->m_Data[0].x;
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->Y_Min))/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->y_Min))/(pItem->y_Max-pItem->y_Min));
			m_MemDC.SetTextColor(RGB_SCALE);
			m_MemDC.MoveTo(rDraw.left,m_y);

			bb1 = Qj+DltA*pItem->m_Data[pItem->m_Num-1].x;
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->Y_Min))/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->y_Min))/(pItem->y_Max-pItem->y_Min));
			m_MemDC.LineTo(rDraw.right,m_y);
			m_MemDC.SelectObject(pOldPen_Line);
			DeleteObject(pen_Line);
			m_x = rDraw.left;
		}
	}
	CFont testFont3;
	SetFont(-12,0,0,0,400,0,0,0,0,3,2,1,34);
    testFont3.CreateFontIndirect(&m_Logfont);
	pOldFont=(CFont *)m_MemDC.SelectObject(&testFont3);
	m_MemDC.GetTextMetrics(&tm);
    bm = m_MemDC.GetTextExtent(_T(""),1).cx/3;
	bh = tm.tmHeight+bm;
	bw = tm.tmAveCharWidth+bm-1;
	if(!m_Other_Color||(m_Other_Color&&m_Other_Color_Num<2))
	{	
		if(pItem->m_Num>=2)
		{
			bb1 = Qj+DltA*(pItem->m_Data[0].x+(pItem->m_Data[pItem->m_Num-1].x-pItem->m_Data[0].x)/2);
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->Y_Min))/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->y_Min))/(pItem->y_Max-pItem->y_Min));
			sprintf(tt,"Cor = %.1f%",Cor);
			m_MemDC.TextOut(m_x+rDraw.Width()/2,m_y-bh,tt);
		}
		else m_y = rDraw.bottom-(rDraw.Height()/2);
	}
	else
	{
		if(m_Other_Color_Num>=2)
		{
			bb1 = Qj+DltA*(m_Other_Color_Point[0].x+(m_Other_Color_Point[m_Other_Color_Num-1].x-m_Other_Color_Point[0].x)/2);
			if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->Y_Min))/(pItem->Y_Max-pItem->Y_Min));
			else m_y = rDraw.bottom-(int)((rDraw.Height()*(bb1-pItem->y_Min))/(pItem->y_Max-pItem->y_Min));
			sprintf(tt,"Cor = %.1f%",Cor);
			m_MemDC.TextOut(m_x+rDraw.Width()/2,m_y-bh,tt);
		}
	}
	sprintf(tt,"Rat=%.4f",Kor);
	m_MemDC.TextOut(m_x+rDraw.Width()/2,m_y-bh-bh,tt);
	m_MemDC.SelectObject(pOldFont);
	DeleteObject(testFont3);
}

void CLineChartCtrl::SetFont(int A1,int A2,int A3,int A4,int A5,int A6,int A7,int A8,int A9,int A10,int A11,int A12,int A13)
{
	m_Logfont.lfHeight =A1;
	m_Logfont.lfWidth = A2;
	m_Logfont.lfEscapement = A3;
	m_Logfont.lfOrientation = A4;
	m_Logfont.lfWeight = A5;
	m_Logfont.lfItalic = A6;
	m_Logfont.lfUnderline = A7;
	m_Logfont.lfStrikeOut = A8;
	m_Logfont.lfCharSet = A9;
	m_Logfont.lfOutPrecision =A10;
	m_Logfont.lfClipPrecision = A11;
	m_Logfont.lfQuality = A12;
	m_Logfont.lfPitchAndFamily = A13;
}

double CLineChartCtrl::GetRegress(int Index)
{
	Regress(Index);
	return Kor;
}


void CLineChartCtrl::Regress(int Index)
{
	CData *pItem;
	DATA S_Data[1000];
	int i,Num ,m_Po,Mathod=0,Total;
	double tempxy,temp;
	char tt[20];
	if(!m_Other_Color||(m_Other_Color&&m_Other_Color_Num<2))
	{
		POSITION pos = GetPosition(Index);
		if(pos==NULL)
		{
			DltA	= 0;
			Cor		= 0;
			Qj		= 0;
			return;
		}
		pItem=(CData *)m_Pt.GetAt(pos);
		if(pItem->m_Num<2)
		{
			DltA	= 0;
			Cor		= 0;
			Qj		= 0;
			return;
		}
		for(i=0;i<pItem->m_Num;i++)S_Data[i] = pItem->m_Data[i];
		Total = pItem->m_Num;
	}
	else
	{
		if(m_Other_Color_Num<2)
		{
			DltA	= 0;
			Cor		= 0;
			Qj		= 0;
			return;
		}
		for(i=0;i<m_Other_Color_Num;i++)S_Data[i]	= m_Other_Color_Point[i];
		Total = m_Other_Color_Num;
	}
//=============先判定该反应的总体斜率特性=======================
	Regress(S_Data,Total);
	while(Cor<90.0)
	{
		tempxy = Qj+DltA*S_Data[0].x;
		temp   = fabs(tempxy - S_Data[0].y);
		m_Po = 0;
		for(i=1;i<Total;i++)
		{
			tempxy = Qj+DltA*S_Data[i].x;
			if(temp<fabs(tempxy - S_Data[i].y))
			{
				temp   = fabs(tempxy - S_Data[i].y);
				m_Po = i;
			}
		}
		Num = 0;
		for(i=0;i<Total;i++)if(i!=m_Po)S_Data[Num++] = S_Data[i];
		Regress(S_Data,Num);
		Total = Num;
	}
	Kor=DltA*60;	// 每分钟的变化率
	sprintf(tt,"%.4f",Kor);
	if(!m_Other_Color)if(pItem->m_Num==2)Cor=100;
	else if(m_Other_Color_Num==2)Cor=100;
}


void CLineChartCtrl::Regress(DATA *m_Reg,int Num)
{
	float x_avg,y_avg,tempxy,tempx,tempy;
	int i;
//=============常用的计算方法（均是每秒钟读一个点）========================
	x_avg = y_avg = 0;
	for(i=0;i<Num;i++)
	{
		x_avg+=m_Reg[i].x;
		y_avg+=m_Reg[i].y;
	}
	y_avg/=(float)Num;
	x_avg/=(float)Num;
	tempxy = tempx = tempy = 0;
	for(i=0;i<Num;i++)
	{
		tempxy+=(m_Reg[i].y-y_avg)*(m_Reg[i].x-x_avg);
		tempx+=(m_Reg[i].x-x_avg)*(m_Reg[i].x-x_avg);
		tempy+=(m_Reg[i].y-y_avg)*(m_Reg[i].y-y_avg);
	}
	DltA = tempxy/tempx;
	Qj = y_avg-(DltA*x_avg);
	tempxy = tempxy/Num;
	tempx = (float)sqrt(tempx/Num);
	tempy = (float)sqrt(tempy/Num);
	if(tempxy==0)Cor = 100;
	else Cor = (float)fabs(tempxy/(tempx*tempy)*100);
}




void CLineChartCtrl::DrawSD(int Index)
{
	char tt[40],t1[20];
	double x_Scale;
	CRect Rs;
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return ;
	pItem=(CData *)m_Pt.GetAt(pos);
	if(pItem->m_Num<2)return;
	
	int count;
	if(!m_SameDay)count = 31;
	else count = pItem->m_Point;
	x_Scale = (double)(rDraw.Width())/(count-1);
	CFont testFont4;
	SetFont(-12,0,0,0,400,0,0,0,0,3,2,1,34);
    testFont4.CreateFontIndirect(&m_Logfont);
	pOldFont=(CFont *)m_MemDC.SelectObject(&testFont4);
	m_MemDC.GetTextMetrics(&tm);
    bm = m_MemDC.GetTextExtent(_T(""),1).cx/3;
	bh = tm.tmHeight+bm;
	bw = tm.tmAveCharWidth+bm-1;
	m_MemDC.SetTextColor(RGB_FONT);
	CPen pen_Line;
	CPen* pOldPen_Line;
	pen_Line.CreatePen(PS_SOLID, 1, RGB_COOR);
	pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
	Rs = rDraw;
	Rs.top+=15;
	Rs.bottom-=15;
	int Step = 1,i;
	for(i=0;i<count;i+=Step)
	{
		m_x=Rs.left+int(x_Scale*i);
		m_x=rDraw.left+int(x_Scale*i);
		m_MemDC.MoveTo(m_x,rGraph.bottom);
		m_MemDC.LineTo(m_x,rGraph.bottom-4);
		sprintf(tt,"%d",i+1);
		m_MemDC.TextOut(m_x-int(strlen(tt)*bw/2.0),rGraph.bottom+2,tt);
	}

	CalculateSD(Index);
	int m,n;
	double Min,Max,Mia;
	m=1;
	while(Mean+SD*m<pItem->y_Max||Mean-SD*m>pItem->y_Min)m++;
	if(m<4)m=4;
	if(m>8)m = 8;
	Min = Mean-SD*m;
	Max = Mean+SD*m;
	y_Scale = (double)((Max-Min))/10.0;  //纵轴方向的比例
	for(i=0;i<11;i++)
	{
		m_y = Rs.bottom-(int)((Rs.Height()*(y_Scale*i))/(Max-Min));
		m_MemDC.MoveTo(rGraph.left,m_y);
		m_MemDC.LineTo(rGraph.left+4,m_y);
		sprintf(tt,y_Format,Min+y_Scale*i);
		if(i==10)m_MemDC.TextOut(rGraph.left-int(strlen(tt))*bw,m_y-bh/2+1,tt);
		if(i==0)
		{
			strcpy(t1,tt);
			m_MemDC.TextOut(rGraph.left-int(strlen(tt))*bw,m_y-bh/2+1,tt);
		}
		else
		{
			if(i-i/2*2==0){
				if(strcmp(t1,tt)==0)continue;
				else
				{
					strcpy(t1,tt);
					m_MemDC.TextOut(rGraph.left-int(strlen(tt))*bw,m_y-bh/2+1,tt);
				}
			}
		}
	}
	m_MemDC.SelectObject(pOldPen_Line);
	DeleteObject(pen_Line);
	pen_Line.CreatePen(PS_SOLID, 1, RGB_VP);
	pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
	sprintf(tt,y_Format,Mean);
	m_y = Rs.bottom-(int)((Rs.Height()*(Mean-Min))/(Max-Min));
	m_MemDC.MoveTo(Rs.left,m_y);
	m_MemDC.LineTo(Rs.right,m_y);
	m_MemDC.SelectObject(pOldPen_Line);
	DeleteObject(pen_Line);
	m_MemDC.SetTextColor(RGB_VP);
	m_MemDC.TextOut(rGraph.right+2,m_y-bh/2+1,tt);
	pen_Line.CreatePen(PS_DOT, 1, RGB_SCALE);
	pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
	m_MemDC.SetTextColor(RGB_SCALE);
	for(i=0;i<m;i++)
	{
		Mia = Mean+SD*(i+1);
		m_y = Rs.bottom-(int)((Rs.Height()*(Mia-Min))/(Max-Min));	
		m_MemDC.MoveTo(Rs.left,m_y);
		m_MemDC.LineTo(Rs.right,m_y);
		sprintf(tt,"%dSD",i+1);
		m_MemDC.TextOut(rGraph.right+2,m_y-bh/2+1,tt);
		Mia = Mean-SD*(i+1);
		m_y = Rs.bottom-(int)((Rs.Height()*(Mia-Min))/(Max-Min));	
		m_MemDC.MoveTo(Rs.left,m_y);
		m_MemDC.LineTo(Rs.right,m_y);
		sprintf(tt,"-%dSD",i+1);
		m_MemDC.TextOut(rGraph.right+2,m_y-bh/2+1,tt);
	}
	m_MemDC.SelectObject(pOldPen_Line);
	DeleteObject(pen_Line);
	pen_Line.CreatePen(PS_SOLID, 1, RGB_LINE);
	pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
	for(i=0;i<pItem->m_Num;i++)
	{
		m_x=Rs.left+int(x_Scale*(pItem->m_Data[i].x-1));
		m_y = Rs.bottom-(int)((Rs.Height()*(pItem->m_Data[i].y-Min))/(Max-Min));
		if(i==0)m_MemDC.MoveTo(m_x,m_y);
		else m_MemDC.LineTo(m_x,m_y);
	}
	for(i=0;i<pItem->m_Num;i++)
	{
		if(!m_SameDay)m_x=Rs.left+int(x_Scale*(pItem->m_Data[i].x-1));
		else m_x=Rs.left+int(x_Scale*i);
		m_y = Rs.bottom-(int)((Rs.Height()*(pItem->m_Data[i].y-Min))/(Max-Min));
		if(pItem->m_Data[i].y<=Mean+SD&&pItem->m_Data[i].y>=Mean-SD)n = 1;
		else if((pItem->m_Data[i].y<=Mean+2.0*SD&&pItem->m_Data[i].y>Mean+SD)||(pItem->m_Data[i].y<Mean-SD&&pItem->m_Data[i].y>=Mean-2.0*SD))n = 2;
		else if((pItem->m_Data[i].y<=Mean+3.0*SD&&pItem->m_Data[i].y>Mean+2.0*SD)||(pItem->m_Data[i].y<Mean-2.0*SD&&pItem->m_Data[i].y>=Mean-3.0*SD))n = 3;
		else n=4;
		if(n==1)m_MemDC.Ellipse(m_x-2,m_y-2,m_x+2,m_y+2);
		else if(n==2)m_MemDC.Rectangle(m_x-2,m_y-2,m_x+2,m_y+2);
		else if(n==3)m_MemDC.Ellipse(m_x-4,m_y-4,m_x+4,m_y+4);
		else m_MemDC.Rectangle(m_x-4,m_y-4,m_x+4,m_y+4);
	}
	m_MemDC.SelectObject(pOldPen_Line);
	DeleteObject(pen_Line);
	sprintf(tt,"Mean = %.2f ; SD = %.3f ; CV = %.1f",Mean,SD,CV);
	strcat(tt,"%");
	m_MemDC.SetTextColor(RGB_UNIT);
	m_x = Rs.left+Rs.Width()/2-strlen(tt)/2*bw;
	m_y = Rs.top-bh;
	m_MemDC.TextOut(m_x,m_y,tt);
	m_MemDC.SelectObject(pOldFont);
	DeleteObject(testFont4);
}


void CLineChartCtrl::CalculateSD(int Index)
{
	int i;
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return ;
	pItem=(CData *)m_Pt.GetAt(pos);
	if(m_SDCal)
	{
		Mean = 0;
		for(i=0;i<pItem->m_Num;i++)Mean+=pItem->m_Data[i].y;
		Mean=Mean/(float)(pItem->m_Num);  //计算均值
		SD=0;
		for(i=0;i<pItem->m_Num;i++)SD+=(pItem->m_Data[i].y-Mean)*(pItem->m_Data[i].y-Mean);
		SD=SD/(float)(pItem->m_Num-1);
		SD=(float)sqrt(SD);		//样本标准差
	}
	CV = SD/Mean*100;   //样本变异系数
}

void CLineChartCtrl::DrawMouse(int Index,int mouse_x)//Index默认0， mouse_x鼠标相对控件框的坐标
{
	int i;
	char tt[40];
	CPen pen_Line;
	CPen* pOldPen_Line;
	CData *pItem;
	POSITION pos = GetPosition(Index);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);
	if (pItem->m_Num==0)
		return;
	if(pItem->m_Num==1&&pItem->m_Point==0)
	{
		m_x = rDraw.left+rDraw.Width()/2;
		m_y = rDraw.bottom-rDraw.Height()/2;
		//m_MemDC.Ellipse(m_x-2,m_y-2,m_x+2,m_y+2);
		m_MemDC.Ellipse(m_x-4,m_y-4,m_x+4,m_y+4);
	}
	else
	{
		float Datatemp;
		Datatemp = float((mouse_x-rDraw.left)/x_Scale+pItem->x_Min);
		for(i=0;i<pItem->m_Num;i++)
		{
			if(pItem->m_Data[i].x>Datatemp)
				break;
		}
		if ((pItem->m_Data[i].x - Datatemp)>(Datatemp-pItem->m_Data[i-1].x))
		{
			i=i-1;
		}
		if(pItem->m_Point==0)m_x=rDraw.left+int(x_Scale*(pItem->m_Data[i].x-pItem->x_Min));
		else m_x=rDraw.left+int(x_Scale*i);
		if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height())*(pItem->m_Data[i].y-pItem->Y_Min)/(pItem->Y_Max-pItem->Y_Min));
		else m_y = rDraw.bottom-(int)((rDraw.Height())*(pItem->m_Data[i].y-pItem->y_Min)/(pItem->y_Max-pItem->y_Min));
		pen_Line.CreatePen(PS_SOLID, 1, RGB_LINE);
		pOldPen_Line = m_MemDC.SelectObject(&pen_Line);
		//m_MemDC.Ellipse(m_x-2,m_y-2,m_x+2,m_y+2);
		m_MemDC.Ellipse(m_x-4,m_y-4,m_x+4,m_y+4);
		sprintf(tt,"%.4f ",pItem->m_Data[i].y);
		m_MemDC.SetTextColor(RGB(255, 255, 0));

		m_MemDC.TextOut(m_x,m_y,tt);
		m_MemDC.SelectObject(pOldPen_Line);
		DeleteObject(pen_Line);
	}
}



void CLineChartCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		int x = point.x;
		int y = point.y;
		if (x>rDraw.left&&x<rDraw.right
			&&y>(rDraw.top-5)&&y<(rDraw.bottom+5))
		{
			DrawChart(0);
			DrawMouse(0,x);
		}
	CWnd::OnLButtonDown(nFlags, point);
}

void CLineChartCtrl::DrawRect(double data_x,double data_y,int sel)
{

	CPen* pOldPen_Line;
	CData *pItem;
	POSITION pos = GetPosition(0);
	if(pos==NULL)return;
	pItem=(CData *)m_Pt.GetAt(pos);

	if(pItem->m_Point==0)m_x=rDraw.left+int(x_Scale*(data_x-pItem->x_Min));
	else m_x=rDraw.left+int(x_Scale);
	if(pItem->Y_MaxMin)m_y = rDraw.bottom-(int)((rDraw.Height())*(data_y-pItem->Y_Min)/(pItem->Y_Max-pItem->Y_Min));
	else m_y = rDraw.bottom-(int)((rDraw.Height())*(data_y-pItem->y_Min)/(pItem->y_Max-pItem->y_Min));

	CBrush   brush(RGB(255,0,0));    //红色 
	CBrush   brush1(RGB(255,255,255));    //白色 
	CBrush     *old;
	if (sel==0)
	{
		old   =   m_MemDC.SelectObject(&brush); 
	}
	else if (sel == 1)
	{
		old   =   m_MemDC.SelectObject(&brush1); 
	}
	m_MemDC.Ellipse(m_x-5,m_y-5,m_x+5,m_y+5); 
	m_MemDC.SelectObject(old); 	
}
