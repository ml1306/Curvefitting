// LineChartCtrl.h : header file
//
// Written by Yuheng Zhao (yuheng@ministars.com) 
// http://www.ministars.com
// The original idea and part of the code from Ken C. Len's CHistogramCtrl
// http://www.codeguru.com/controls/histogram_control.shtml
//
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.
//

#ifndef __LINECHARTCTRL_H__
#define __LINECHARTCTRL_H__

#include <afxtempl.h>

typedef struct{
	float x;
	float y;
}DATA;

//均值和方差软件自动计算
class CData : public CObject
{
public:
	int			Index;
	char		TableName[200];   //图表名称
	char		x_Unit[40];       //x轴单位
	char		y_Unit[40];       //y轴单位
	BOOL        Y_MaxMin;		  //y轴由用户设置最大值和最小值
	float		Y_Max;			  //y轴由用户设置最大值
	float		Y_Min;			  //y轴由用户设置最小值
	float		x_Min;		 	  //x轴最小值
	float		x_Max;			  //x轴最大值
	float		y_Min;			  //y轴最小值
	float		y_Max;			  //y轴最大值
	short		x_Decimal;        //X轴小数位
	short		y_Decimal;		  //Y轴小数位
	short       m_Point;          //计划X轴总数
	short		m_Num;            //点数
	DATA		*m_Data;	      //x方向的值和y方向的值
};


/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrl window

class CLineChartCtrl : public CWnd
{
// Construction
public:
	static BOOL RegisterWndClass(HINSTANCE hInstance);
	CLineChartCtrl();
	COLORREF RGB_BK,RGB_FONT,RGB_LINE,RGB_VP,RGB_SCALE,RGB_SD; 
	COLORREF RGB_TITLE,RGB_UNIT,RGB_COOR;
	BOOL     m_VP,m_Regress,m_SD,m_SDCal,m_SameDay;
	BOOL     m_Other_Color;
	short	 x_Decimal,y_Decimal,m_Begin_Point,m_gg;
	int		 ActiveChart;
// Attributes
public:
    LOGFONT     m_Logfont;
    CFont       m_PrinterFont,  // for the printer
                m_Font;         // for the grid
	DATA	m_Other_Color_Point[700];
	short   m_Other_Color_Num;
	FILE	*F_Color;
	CObList m_Pt;
	CDC     m_MemDC;
	CBitmap m_Bitmap;
	CRect	rcClient;
	CRect   rGraph,rDraw;
	char    x_Format[10];
	char    y_Format[10];
	int     m_x,m_y,m_Index,m_px;   //当前操作图面
	double  x_Scale,y_Scale;
	CFont	testFont;
	CFont	*pOldFont;
	TEXTMETRIC tm;
	int      bm,bw,bh;   //字宽和字高
	float   VP;		 //平均值
	float   Cor,Kor,Qj,DltA;	 //相关系数和斜率 截距
	float   SD,Mean,CV;
	float   Inter_Time;

//	CRect	m_rectL;

	void	 GetColor();
	void DrawMouse(int Index,int mouse_x);
	


	void	SetBkColor(COLORREF RGB){RGB_BK = RGB;}			//背景色
	void	SetFontColor(COLORREF RGB){RGB_FONT = RGB;}		//数据标注色
	void	SetTitleColor(COLORREF RGB){RGB_TITLE = RGB;}	//确定图表标题的颜色
	void	SetUnitColor(COLORREF RGB){RGB_UNIT = RGB;}		//确定X、Y轴的颜色
	void	SetCoColor(COLORREF RGB){RGB_COOR = RGB;}		//确定坐标的颜色
	void	SetLineColor(COLORREF RGB){RGB_LINE = RGB;}		//确定采集数据线的颜色
	void	SetVpColor(COLORREF RGB){RGB_VP = RGB;}			//确定平均值的颜色
	void	SetScaleColor(COLORREF RGB){RGB_SCALE = RGB;}	//确定斜率颜色
	void	SetSdColor(COLORREF RGB){RGB_SD = RGB;}			//确定SD的颜色
	void	SetFont(int A1,int A2,int A3,int A4,int A5,int A6,int A7,int A8,int A9,int A10,int A11,int A12,int A13);

	void	SetXDecimal(short Decimal,int Index = 0);	//确定X轴的小数位
	void	SetYDecimal(short Decimal,int Index = 0);	//确定Y轴的小数位
	void    SetTitle(LPCTSTR TableName,int Index = 0);	//确定图表的名称
	void    SetXUnit(LPCTSTR x_Unit,int Index = 0);		//确定X轴的单位
	void    SetYUnit(LPCTSTR y_Unit,int Index = 0);		//确定Y轴的单位
	void    SetYMaxMin(float YMax,float YMin,int Index = 0); //为Y轴赋最大值勤和最小值

	BOOL	AddChart(int Index=0,LPCTSTR TableName = NULL,LPCTSTR x_Unit = NULL,LPCTSTR y_Unit = NULL);	//增加图表
	void	InitGraph();			//图表驱动
	void	CutInTp();				//删除图表
	int	    GetIndex(){return m_Index;}


	void    InitChart(int Index = 0,LPCTSTR TableName = NULL,LPCTSTR x_Unit = NULL,LPCTSTR y_Unit = NULL);	//图表初始化
	POSITION GetPosition(int Index = 0);					//取得图表位置
	void	SetData(float x , float y ,int Index = 0);	//向图中输入数据
	void    SetSurveyPoint(short Num,int Index=0);				//确定测定点数
	void    SetVPDraw(BOOL VPDraw){m_VP = VPDraw;}
	void    SetRegressDraw(BOOL RegressDraw){m_Regress = RegressDraw;}
	void    SetSDDraw(BOOL SdDraw){m_SD = SdDraw;}
	void    SetOther(BOOL SdDraw){m_Other_Color = SdDraw;}
	void    SetOtherPoint(short m_SdDraw,short m_n){m_Begin_Point = m_SdDraw;m_gg = m_n;}

	void    SetSDCalculate(BOOL SdDraw){m_SDCal = SdDraw;}
	void    SetSameDay(BOOL SdDraw){m_SameDay = SdDraw;}
	void    SetMeanValue(float MMM){Mean=MMM;}
	void    SetSDValue(float MSD){SD=MSD;}
	
	void    DrawChart(int Index=0);							//
	void	DrawWindow(int Index=0);						//画出窗口初始界面
	void    DrawLine(int Index=0);
	void    DrawVP(int Index=0);
	void	DrawRegress(int Index=0);
	void    DrawSD(int Index = 0);
	void	CalculateSD(int Index = 0);
	
	
	double	GetVP(int Index=0);			//平均值计算
	double  GetRegress(int Index=0);	//得到斜率计算
	double  GetY_Max(int Index=0);		//得到Y轴的数据最大值
	double  GetY_Min(int Index=0);		//得到Y轴的数据最小值
	double  GetX_Max(int Index=0);		//得到X轴的数据最大值
	double  GetX_Min(int Index=0);		//得到X轴的数据最小值
	void	Regress(int nIndex=0);	//相关系数和变化率计算（无转化成1分钟范围）
	void	Regress(DATA *m_Reg,int Num);
	void DrawRect(double data_x,double data_y,int sel);

	// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineChartCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLineChartCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineChartCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
#endif

