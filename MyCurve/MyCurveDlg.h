// MyCurveDlg.h : 头文件
//
#include "LineChartCtrl.h"
#pragma once#include "afxwin.h"
#include "myedit.h"




// CMyCurveDlg 对话框
class CMyCurveDlg : public CDialog
{
// 构造
public:
	CMyCurveDlg(CWnd* pParent = NULL);	// 标准构造函数
	CLineChartCtrl m_wndLineChart;   //   控件变量名 
// 对话框数据
	enum { IDD = IDD_MYCURVE_DIALOG };
	double corrcoef(double *d1, double *d2, int dataL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持



// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	double Spline_x[6],Spline_y[6],Spline_a[6],Spline_b[6],Spline_c[6];
	int Spline_n;

	CString m_sEditshow;
	CString m_dEdit_X1;
	CString m_dEdit_X2;
	CString m_dEdit_X3;
	CString m_dEdit_X4;
	CString m_dEdit_Y1;
	CString m_dEdit_Y2;
	CString m_dEdit_Y3;
	CString m_dEdit_Y4;
	CString m_sEdit_ini1;
	CString m_sEdit_ini2;
	CString m_sEdit_ini3;
	CString m_sEdit_ini4;
	//CString m_sEdit_ini5;
	afx_msg void OnBnClickedButton2();
	CString m_dEdit_X5;
	CString m_dEdit_X6;
	CString m_dEdit_X7;
	CString m_dEdit_X8;
	CString m_dEdit_X9;
	CString m_dEdit_X10;
	CString m_dEdit_Y5;
	CString m_dEdit_Y6;
	CString m_dEdit_Y7;
	CString m_dEdit_Y8;
	CString m_dEdit_Y9;
	CString m_dEdit_Y10;
	int m_iEdit_Num;
	int InputNum;
	double Parameters_a;
	double Parameters_b;
	double Parameters_c;
	double Parameters_d;
	double Parameters_e;
	double StartingPoint_y;
	CString m_sEdit_input_od;
	CString m_sEdit_Result;
	int m_iSelFunction;
	BOOL bMonotonicity;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnKillfocusEdit2();
	CComboBox m_cCombo_SelFunction;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboSelfunc();

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	float GetRealValue(int sel_log5p_exp5p,float R0, float K, float a, float b, float c, float meas_abs, float max_standard_value,float min_standard_value,int Monotonicity);
	myedit m_mMyedit_X_V1;
	myedit m_mMyedit_X_V2;
	myedit m_mMyedit_X_V3;
	myedit m_mMyedit_X_V4;
	myedit m_mMyedit_X_V5;
	myedit m_mMyedit_X_V6;
	myedit m_mMyedit_X_V7;
	myedit m_mMyedit_X_V8;
	myedit m_mMyedit_X_V9;
	myedit m_mMyedit_X_V10;

	myedit m_mMyedit_Y_V1;
	myedit m_mMyedit_Y_V2;
	myedit m_mMyedit_Y_V3;
	myedit m_mMyedit_Y_V4;
	myedit m_mMyedit_Y_V5;
	myedit m_mMyedit_Y_V6;
	myedit m_mMyedit_Y_V7;
	myedit m_mMyedit_Y_V8;
	myedit m_mMyedit_Y_V9;
	myedit m_mMyedit_Y_V10;
	myedit m_mMyedit_InputNum;
	myedit m_mMyedit_Ini1;
	myedit m_mMyedit_Ini2;
	myedit m_mMyedit_Ini3;
	myedit m_mMyedit_Ini4;
	//myedit m_mMyedit_Ini5;
	CString m_dEdit_StarPoint;
};
