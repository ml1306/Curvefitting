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

//��ֵ�ͷ�������Զ�����
class CData : public CObject
{
public:
	int			Index;
	char		TableName[200];   //ͼ������
	char		x_Unit[40];       //x�ᵥλ
	char		y_Unit[40];       //y�ᵥλ
	BOOL        Y_MaxMin;		  //y�����û��������ֵ����Сֵ
	float		Y_Max;			  //y�����û��������ֵ
	float		Y_Min;			  //y�����û�������Сֵ
	float		x_Min;		 	  //x����Сֵ
	float		x_Max;			  //x�����ֵ
	float		y_Min;			  //y����Сֵ
	float		y_Max;			  //y�����ֵ
	short		x_Decimal;        //X��С��λ
	short		y_Decimal;		  //Y��С��λ
	short       m_Point;          //�ƻ�X������
	short		m_Num;            //����
	DATA		*m_Data;	      //x�����ֵ��y�����ֵ
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
	int     m_x,m_y,m_Index,m_px;   //��ǰ����ͼ��
	double  x_Scale,y_Scale;
	CFont	testFont;
	CFont	*pOldFont;
	TEXTMETRIC tm;
	int      bm,bw,bh;   //�ֿ���ָ�
	float   VP;		 //ƽ��ֵ
	float   Cor,Kor,Qj,DltA;	 //���ϵ����б�� �ؾ�
	float   SD,Mean,CV;
	float   Inter_Time;

//	CRect	m_rectL;

	void	 GetColor();
	void DrawMouse(int Index,int mouse_x);
	


	void	SetBkColor(COLORREF RGB){RGB_BK = RGB;}			//����ɫ
	void	SetFontColor(COLORREF RGB){RGB_FONT = RGB;}		//���ݱ�עɫ
	void	SetTitleColor(COLORREF RGB){RGB_TITLE = RGB;}	//ȷ��ͼ��������ɫ
	void	SetUnitColor(COLORREF RGB){RGB_UNIT = RGB;}		//ȷ��X��Y�����ɫ
	void	SetCoColor(COLORREF RGB){RGB_COOR = RGB;}		//ȷ���������ɫ
	void	SetLineColor(COLORREF RGB){RGB_LINE = RGB;}		//ȷ���ɼ������ߵ���ɫ
	void	SetVpColor(COLORREF RGB){RGB_VP = RGB;}			//ȷ��ƽ��ֵ����ɫ
	void	SetScaleColor(COLORREF RGB){RGB_SCALE = RGB;}	//ȷ��б����ɫ
	void	SetSdColor(COLORREF RGB){RGB_SD = RGB;}			//ȷ��SD����ɫ
	void	SetFont(int A1,int A2,int A3,int A4,int A5,int A6,int A7,int A8,int A9,int A10,int A11,int A12,int A13);

	void	SetXDecimal(short Decimal,int Index = 0);	//ȷ��X���С��λ
	void	SetYDecimal(short Decimal,int Index = 0);	//ȷ��Y���С��λ
	void    SetTitle(LPCTSTR TableName,int Index = 0);	//ȷ��ͼ�������
	void    SetXUnit(LPCTSTR x_Unit,int Index = 0);		//ȷ��X��ĵ�λ
	void    SetYUnit(LPCTSTR y_Unit,int Index = 0);		//ȷ��Y��ĵ�λ
	void    SetYMaxMin(float YMax,float YMin,int Index = 0); //ΪY�ḳ���ֵ�ں���Сֵ

	BOOL	AddChart(int Index=0,LPCTSTR TableName = NULL,LPCTSTR x_Unit = NULL,LPCTSTR y_Unit = NULL);	//����ͼ��
	void	InitGraph();			//ͼ������
	void	CutInTp();				//ɾ��ͼ��
	int	    GetIndex(){return m_Index;}


	void    InitChart(int Index = 0,LPCTSTR TableName = NULL,LPCTSTR x_Unit = NULL,LPCTSTR y_Unit = NULL);	//ͼ���ʼ��
	POSITION GetPosition(int Index = 0);					//ȡ��ͼ��λ��
	void	SetData(float x , float y ,int Index = 0);	//��ͼ����������
	void    SetSurveyPoint(short Num,int Index=0);				//ȷ���ⶨ����
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
	void	DrawWindow(int Index=0);						//�������ڳ�ʼ����
	void    DrawLine(int Index=0);
	void    DrawVP(int Index=0);
	void	DrawRegress(int Index=0);
	void    DrawSD(int Index = 0);
	void	CalculateSD(int Index = 0);
	
	
	double	GetVP(int Index=0);			//ƽ��ֵ����
	double  GetRegress(int Index=0);	//�õ�б�ʼ���
	double  GetY_Max(int Index=0);		//�õ�Y����������ֵ
	double  GetY_Min(int Index=0);		//�õ�Y���������Сֵ
	double  GetX_Max(int Index=0);		//�õ�X����������ֵ
	double  GetX_Min(int Index=0);		//�õ�X���������Сֵ
	void	Regress(int nIndex=0);	//���ϵ���ͱ仯�ʼ��㣨��ת����1���ӷ�Χ��
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

