// MyCurveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyCurve.h"
#include "MyCurveDlg.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyCurveDlg 对话框

double ava[10]={0,0,0,0,0,0,0,0,0,0};
double bva[10]={0,0,0,0,0,0,0,0,0,0};
double SplineCi = 0;


CMyCurveDlg::CMyCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCurveDlg::IDD, pParent)
	, m_sEditshow(_T(""))
	, m_dEdit_X1(_T("1"))
	, m_dEdit_X2(_T("2"))
	, m_dEdit_X3(_T("3"))
	, m_dEdit_X4(_T("4"))
	, m_dEdit_Y1(_T("1"))
	, m_dEdit_Y2(_T("2"))
	, m_dEdit_Y3(_T("6"))
	, m_dEdit_Y4(_T("7"))
	, m_sEdit_ini1(_T("7"))
	, m_sEdit_ini2(_T("6"))
	, m_sEdit_ini3(_T("0"))
	, m_sEdit_ini4(_T("1"))
	//, m_sEdit_ini5(_T("1"))
	, m_dEdit_X5(_T(""))
	, m_dEdit_X6(_T(""))
	, m_dEdit_X7(_T(""))
	, m_dEdit_X8(_T(""))
	, m_dEdit_X9(_T(""))
	, m_dEdit_X10(_T(""))
	, m_dEdit_Y5(_T(""))
	, m_dEdit_Y6(_T(""))
	, m_dEdit_Y7(_T(""))
	, m_dEdit_Y8(_T(""))
	, m_dEdit_Y9(_T(""))
	, m_dEdit_Y10(_T(""))
	, m_iEdit_Num(4)
	, m_sEdit_input_od(_T(""))
	, m_sEdit_Result(_T(""))
	, m_dEdit_StarPoint(_T("0"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sEditshow);
	DDV_MaxChars(pDX, m_sEditshow, 500);
	DDX_Text(pDX, IDC_EDIT_X_V1, m_dEdit_X1);
	DDX_Text(pDX, IDC_EDIT_X_V2, m_dEdit_X2);
	DDX_Text(pDX, IDC_EDIT_X_V3, m_dEdit_X3);
	DDX_Text(pDX, IDC_EDIT_X_V4, m_dEdit_X4);
	DDX_Text(pDX, IDC_EDIT_Y_V1, m_dEdit_Y1);
	DDX_Text(pDX, IDC_EDIT_Y_V2, m_dEdit_Y2);
	DDX_Text(pDX, IDC_EDIT_Y_V3, m_dEdit_Y3);
	DDX_Text(pDX, IDC_EDIT_Y_V4, m_dEdit_Y4);
	DDX_Text(pDX, IDC_EDIT_INI_V1, m_sEdit_ini1);
	DDX_Text(pDX, IDC_EDIT_INI_V2, m_sEdit_ini2);
	DDX_Text(pDX, IDC_EDIT_INI_V3, m_sEdit_ini3);
	DDX_Text(pDX, IDC_EDIT_INI_V4, m_sEdit_ini4);
	//DDX_Text(pDX, IDC_EDIT_INI_V5, m_sEdit_ini5);
	DDX_Text(pDX, IDC_EDIT_X_V5, m_dEdit_X5);
	DDX_Text(pDX, IDC_EDIT_X_V6, m_dEdit_X6);
	DDX_Text(pDX, IDC_EDIT_X_V7, m_dEdit_X7);
	DDX_Text(pDX, IDC_EDIT_X_V8, m_dEdit_X8);
	DDX_Text(pDX, IDC_EDIT_X_V9, m_dEdit_X9);
	DDX_Text(pDX, IDC_EDIT_X_V10, m_dEdit_X10);
	DDX_Text(pDX, IDC_EDIT_Y_V5, m_dEdit_Y5);
	DDX_Text(pDX, IDC_EDIT_Y_V6, m_dEdit_Y6);
	DDX_Text(pDX, IDC_EDIT_Y_V7, m_dEdit_Y7);
	DDX_Text(pDX, IDC_EDIT_Y_V8, m_dEdit_Y8);
	DDX_Text(pDX, IDC_EDIT_Y_V9, m_dEdit_Y9);
	DDX_Text(pDX, IDC_EDIT_Y_V10, m_dEdit_Y10);
	DDX_Text(pDX, IDC_EDIT2, m_iEdit_Num);
	DDX_Text(pDX, IDC_EDIT_INPUT_OD, m_sEdit_input_od);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_sEdit_Result);
	DDX_Control(pDX, IDC_COMBO_SELFUNC, m_cCombo_SelFunction);

	DDX_Control(pDX, IDC_EDIT_X_V1, m_mMyedit_X_V1);
	DDX_Control(pDX, IDC_EDIT_X_V2, m_mMyedit_X_V2);
	DDX_Control(pDX, IDC_EDIT_X_V3, m_mMyedit_X_V3);
	DDX_Control(pDX, IDC_EDIT_X_V4, m_mMyedit_X_V4);
	DDX_Control(pDX, IDC_EDIT_X_V5, m_mMyedit_X_V5);
	DDX_Control(pDX, IDC_EDIT_X_V6, m_mMyedit_X_V6);
	DDX_Control(pDX, IDC_EDIT_X_V7, m_mMyedit_X_V7);
	DDX_Control(pDX, IDC_EDIT_X_V8, m_mMyedit_X_V8);
	DDX_Control(pDX, IDC_EDIT_X_V9, m_mMyedit_X_V9);
	DDX_Control(pDX, IDC_EDIT_X_V10, m_mMyedit_X_V10);


	DDX_Control(pDX, IDC_EDIT_Y_V1, m_mMyedit_Y_V1);
	DDX_Control(pDX, IDC_EDIT_Y_V2, m_mMyedit_Y_V2);
	DDX_Control(pDX, IDC_EDIT_Y_V3, m_mMyedit_Y_V3);
	DDX_Control(pDX, IDC_EDIT_Y_V4, m_mMyedit_Y_V4);
	DDX_Control(pDX, IDC_EDIT_Y_V5, m_mMyedit_Y_V5);
	DDX_Control(pDX, IDC_EDIT_Y_V6, m_mMyedit_Y_V6);
	DDX_Control(pDX, IDC_EDIT_Y_V7, m_mMyedit_Y_V7);
	DDX_Control(pDX, IDC_EDIT_Y_V8, m_mMyedit_Y_V8);
	DDX_Control(pDX, IDC_EDIT_Y_V9, m_mMyedit_Y_V9);
	DDX_Control(pDX, IDC_EDIT_Y_V10, m_mMyedit_Y_V10);
	DDX_Control(pDX, IDC_EDIT2, m_mMyedit_InputNum);
	DDX_Control(pDX, IDC_EDIT_INI_V1, m_mMyedit_Ini1);
	DDX_Control(pDX, IDC_EDIT_INI_V2, m_mMyedit_Ini2);
	DDX_Control(pDX, IDC_EDIT_INI_V3, m_mMyedit_Ini3);
	DDX_Control(pDX, IDC_EDIT_INI_V4, m_mMyedit_Ini4);
	//DDX_Control(pDX, IDC_EDIT_INI_V5, m_mMyedit_Ini5);
	DDX_Text(pDX, IDC_EDIT_STARPOINT, m_dEdit_StarPoint);
}

BEGIN_MESSAGE_MAP(CMyCurveDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMyCurveDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyCurveDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyCurveDlg::OnBnClickedButton3)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CMyCurveDlg::OnEnKillfocusEdit2)

	ON_CBN_SELCHANGE(IDC_COMBO_SELFUNC, &CMyCurveDlg::OnCbnSelchangeComboSelfunc)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMyCurveDlg 消息处理程序

BOOL CMyCurveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_wndLineChart.SubclassDlgItem(IDC_LINE_CHART_CTRL, this);
	// TODO: 在此添加额外的初始化代码
	m_wndLineChart.SetYDecimal(4);
	m_wndLineChart.SetXDecimal(2);
	m_wndLineChart.SetYMaxMin(0.0,0.0);
	m_wndLineChart.AddChart(0,NULL,"C","OD");
	m_wndLineChart.DrawChart(0);
	InputNum = 4;
	Parameters_a = 1;
	Parameters_b = 1;
	Parameters_c = 1;
	Parameters_d = 1;
	m_iSelFunction = 0;
	m_cCombo_SelFunction.SetCurSel(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyCurveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyCurveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void Exponential5P(double *p, double *x, int m, int n, void *data)
{
	int i;
	//for(i=0;i<5;i++)
	//	bva[i]=1+1*exp(1*log(ava[i])+1*log(1.0)*log(1.0)+1*log(ava[i])*log(ava[i])*log(ava[i]));
	//for(i=0; i<m; ++i) printf("%.7g ", bva[i]);printf("\n");
	for(i = 0; i < n; i++)
	{
		x[i] = bva[i]-p[0]*exp(p[1]*log(ava[i])+p[2]*log(ava[i])*log(ava[i])+p[3]*log(ava[i])*log(ava[i])*log(ava[i]));
	}
}
void LogisticLog4P(double *p, double *x, int m, int n, void *data)
{
	int i;
	for(i = 0; i < n; i++)
	{
		x[i] = bva[i] - (p[0]/(1+exp(-(p[1]+p[2]*log(ava[i])))));
	}
}

void LogisticLog5P(double *p, double *x, int m, int n, void *data)
{
	int i;
	for(i = 0; i < n; i++)
	{
		x[i] = bva[i] - (p[0]/(1+exp(-(p[1]+p[2]*log(ava[i])+p[3]*ava[i]))));
	}
}

void Weibull(double *p, double *x, int m, int n, void *data)
{
	int i;
	for(i = 0; i < n; i++)
	{
		double u = pow(ava[i],p[3]);
		x[i] = bva[i]-(p[0]-p[1]*exp(-p[2]*u));
	}
}
void Logistic(double *p, double *x, int m, int n, void *data)
{
	int i;
	for(i = 0; i < n; i++)
	{
		x[i] = bva[i]-p[0]/(1+p[1]*exp(-p[2]*ava[i]));
	}
}

void Spline(double *p, double *x, int m, int n, void *data)
{
	int i;
	for(i = 0; i < n; i++)
	{
		x[i] = p[0]+p[1]*(ava[i]-SplineCi)+p[2]*(ava[i]-SplineCi)*(ava[i]-SplineCi)+p[3]*(ava[i]-SplineCi)*(ava[i]-SplineCi)*(ava[i]-SplineCi)-bva[i]-(ava[i]-SplineCi);
	}
}
//x[] =输入的X坐标 y[]=输入的y坐标 a[] b[] c[] 每段函数的因子 n输入点的个数
void   NCS(double   x[],double   y[],double   a[],double   b[],double   c[],int   n) 
{ 
	double   h[5],alpha[5],l[6],z[6],u[6]; 
	int   i; 
	for   (i=0;i <n-1;i++) 
	{ 
		h[i]=x[i+1]-x[i]; 
	} 
	for(i=1;i <n-1;i++) 
	{ 
		alpha[i]=3*(y[i+1]-y[i])/h[i]-3*(y[i]-y[i-1])/h[i-1]; 
	} 
	//Now   solve   the   linear   system,using   Court   Factorazation 
	l[0]=u[0]=z[0]=0; 
	for(i=1;i <n-1;i++) 
	{ 
		l[i]=2*(x[i+1]-x[i-1])-h[i-1]*u[i-1]; 
		u[i]=h[i]/l[i]; 
		z[i]=(alpha[i]-h[i-1]*z[i-1])/l[i]; 
	} 
	l[n-1]=1; 
	z[n-1]=b[n-1]=0; 
	for(i=n-2;i> -1;i--) 
	{ 
		b[i]=z[i]-u[i]*b[i+1]; 
		a[i]=(y[i+1]-y[i])/h[i]-h[i]*(b[i+1]+2*b[i])/3; 
		c[i]=(b[i+1]-b[i])/(3*h[i]); 
	} 
} 

double CMyCurveDlg::corrcoef(double *d1, double *d2, int dataL)
{
	int i;
	double xy=0, x=0, y=0, xsum=0, ysum=0;
	double corrc;
	for (i=0; i<dataL; i++)
	{
		xsum += d1[i];
		ysum += d2[i];
	}
	xsum /= dataL;
	ysum /= dataL;
	for (i=0; i<dataL; i++)
	{
		d1[i] -= xsum;
		d2[i] -= ysum;
		x += d1[i] * d1[i];
		y += d2[i] * d2[i];
		xy += d1[i] * d2[i];
	}
	corrc = abs(xy) / (sqrt(x) * sqrt(y));
	return corrc;
}
void CMyCurveDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	register int i;
	int  ret;
	CString tempbuff;
	double p[5], // 5 is max(2, 3, 5)
		x[16]; // 16 is max(2, 3, 5, 6, 16)
	double pvp[6][4];
	double cva[10];

	int m, n;
	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]= LM_DIFF_DELTA;
	UpdateData(TRUE);

	StartingPoint_y = atof(m_dEdit_StarPoint);
	ava[0] = atof(m_dEdit_X1);
	ava[1] = atof(m_dEdit_X2);
	ava[2] = atof(m_dEdit_X3);
	ava[3] = atof(m_dEdit_X4);
	ava[4] = atof(m_dEdit_X5);
	ava[5] = atof(m_dEdit_X6);
	ava[6] = atof(m_dEdit_X7);
	ava[7] = atof(m_dEdit_X8);
	ava[8] = atof(m_dEdit_X9);
	ava[9] = atof(m_dEdit_X10);

	bva[0] = atof(m_dEdit_Y1);
	bva[1] = atof(m_dEdit_Y2);
	bva[2] = atof(m_dEdit_Y3);
	bva[3] = atof(m_dEdit_Y4);
	bva[4] = atof(m_dEdit_Y5);
	bva[5] = atof(m_dEdit_Y6);
	bva[6] = atof(m_dEdit_Y7);
	bva[7] = atof(m_dEdit_Y8);
	bva[8] = atof(m_dEdit_Y9);
	bva[9] = atof(m_dEdit_Y10);
	InputNum=m_iEdit_Num;
	if (InputNum<0||InputNum>10)
	{
		MessageBox("输入点数错误");
		return;
	}
	if (bva[0]<=bva[1])
	{
		bMonotonicity = TRUE;
	}
	else if(bva[0]>bva[1])
	{
		bMonotonicity = FALSE;
	}
	for (i=0;i<InputNum-1;i++)
	{
		if (bMonotonicity)
		{
			if (bva[i]>=bva[i+1])
			{
				MessageBox("非单调性");
				return;
			}
		}
		else
		{
			if (bva[i]<=bva[i+1])
			{
				MessageBox("非单调性");
				return;
			}
		}
		if (ava[i]>=ava[i+1])
		{
			MessageBox("非单调性");
			return;
		}
	}
	n=InputNum;
	p[0] = atof(m_sEdit_ini1);
	p[1] = atof(m_sEdit_ini2);
	p[2] = atof(m_sEdit_ini3);
	p[3] = atof(m_sEdit_ini4);
	//p[4] = atof(m_sEdit_ini5);
	/*if (ava[0]<0||ava[1]<0||ava[2]<0||ava[3]<0||bva[0]<0||bva[1]<0||bva[2]<0||bva[3]<0)
	{
		m_sEditshow = "取值错误";
		UpdateData(FALSE);
		return;
	}*/
	for(i=0; i<n; i++) x[i]=0.0;
	switch(m_iSelFunction)
	{
		case 0:
			{
				m=4;
				ret=dlevmar_dif(Weibull,p, x, m, n, 10000, opts, info, NULL, NULL, NULL);
				for(i=0;i<n;i++)
					cva[i]=p[0]-p[1]*exp(-p[2]*pow(ava[i],p[3]));
				m_sEditshow = "";
				m_sEditshow+="Weibull模型: y=a-b*exp(-c*x^d)\r\n";
			}break;
		case 1:
			{
				m=3;
				ret=dlevmar_dif(Logistic,p, x, m, n, 10000, opts, info, NULL, NULL, NULL);
				for(i=0;i<n;i++)
					cva[i]=p[0]/(1+p[1]*exp(-p[2]*ava[i]));
				m_sEditshow = "";
				m_sEditshow+="Logistic模型: y=a/(1+b*e^(-c*x))\r\n";
			}
			break;
		case 2:
			{
				for (i=0;i<InputNum;i++)
				{
					bva[i]-=StartingPoint_y;
				}
				m=3;
				ret=dlevmar_dif(LogisticLog4P,p, x, m, n, 10000, opts, info, NULL, NULL, NULL);
				for(i=0;i<n;i++)
					cva[i]=p[0]/(1+exp(-(p[1]+p[2]*log(ava[i]))))+StartingPoint_y;
				m_sEditshow = "";
				m_sEditshow+="Logistic-Log 4P 模型:   y=a+b/(1+e^(-(c+d*lnC)))\r\n";
			}
			break;
		case 3:
			{
				for (i=0;i<InputNum;i++)
				{
					bva[i]-=StartingPoint_y;
				}
				m=4;
				ret=dlevmar_dif(LogisticLog5P,p, x, m, n, 10000, opts, info, NULL, NULL, NULL);
				for(i=0;i<n;i++)
					cva[i]=p[0]/(1+exp(-(p[1]+p[2]*log(ava[i])+p[3]*ava[i])))+StartingPoint_y;
				m_sEditshow = "";
				m_sEditshow+="Logistic-Log 5P 模型:   y=a+b/(1+e^(-(c+d*lnC+eC)))\r\n";
			}
			break;
		case 4:
			{
				for (i=0;i<InputNum;i++)
				{
					bva[i]-=StartingPoint_y;
				}
				m=4;
				ret=dlevmar_dif(Exponential5P,p, x, m, n, 10000, opts, info, NULL, NULL, NULL);
				for(i=0;i<n;i++)
					cva[i]=p[0]*exp(p[1]*log(ava[i])+p[2]*log(ava[i])*log(ava[i])+p[3]*log(ava[i])*log(ava[i])*log(ava[i]))+StartingPoint_y;
				m_sEditshow = "";
				m_sEditshow+="Exponential5P 模型:   y=a+b*exp(c*ln(x)+d*(ln(x))^2+e*(ln(x))^3);\r\n";
			}
			break;
		case 5:
			{
					for (int num = 0;num<n;num++)
					{
						Spline_y[num]=bva[num];
						Spline_x[num]=ava[num];
						cva[num]=bva[num];
					}
					//double Spline_x[6],Spline_y[6],Spline_a[6],Spline_b[6],Spline_c[6];
					//int Spline_n;
					Spline_n = n;
					m=4;
					NCS(Spline_x,Spline_y,Spline_a,Spline_b,Spline_c,Spline_n); 
				m_sEditshow = "";
				m_sEditshow+="Spline 模型:   C-Ci=R0i+ai(C-Ci)+bi(C-Ci)^2+ci(C-Ci)^3-R;\r\n";
			}
			break;
		default:break;
	}
	Parameters_a = p[0];
	Parameters_b = p[1];
	Parameters_c = p[2];
	Parameters_d = p[3];
	//Parameters_e = p[4];
	m_sEditshow +="\r\n函数参数  ";
	tempbuff.Format("%f",StartingPoint_y);
	m_sEditshow += tempbuff;
	m_sEditshow += "  ";
	for(i=0; i<m; ++i) 
	{
		CString buff;
		buff.Format("%f",p[i]);
		m_sEditshow += buff;
		m_sEditshow += "  ";
	}
	m_sEditshow += "\r\n";
	m_sEditshow +="\r\n给出数据  ";
	for(i=0; i<n; ++i)
	{
		CString buff;
		switch(m_iSelFunction)
		{
			case 0:
			case 1:
			case 5:
				{
					buff.Format("%f",bva[i]);
				}
				break;
			case 2:
			case 3:
			case 4:
				{
					buff.Format("%f",bva[i]+StartingPoint_y);
				}
				break;
			default:break;
		}
		m_sEditshow += buff;
		m_sEditshow += "  ";
	}
	m_sEditshow += "\r\n";
	m_sEditshow +="\r\n计算结果  ";
	
	//for(i=0; i<n; ++i)
	//{
	//	CString buff;
	//	buff.Format("%f",cva[i]);
	//	m_sEditshow += buff;
	//	m_sEditshow += "  ";
	//}

	m_sEditshow += "\r\n";
	m_sEditshow +="\r\n拟合率     ";
	double sva=0;
	sva = corrcoef(cva,bva,n);

	CString svabuff;
	svabuff.Format("%f",sva);
	m_sEditshow += svabuff;
	m_sEditshow += "  ";
	UpdateData(FALSE);

	m_wndLineChart.InitChart(0,NULL,"C","OD");
	float absmax,absmin,abs_value;
	absmax=-1;
	absmin=9;
	float AbsValue[100];
	float AbsToC[100];
	float AddXValue;
	float StarXValue;
	/*if(ava[0]==0)
		AddXValue =((ava[InputNum-1] - ava[0])*1.1)/101;
	else if(ava[0]>0)
		AddXValue =(ava[InputNum-1] - ava[0])*6/500;

	float StarXValue = ava[InputNum-1] + (ava[InputNum-1] - ava[0])/10;
	for(i=99;i>=0;i--)
	{
		AbsToC[i] = StarXValue;
		StarXValue -= AddXValue;	
	}*/
	switch(m_iSelFunction)
	{
		case 0:
			{
				if(ava[0]==0)
					AddXValue =((ava[InputNum-1] - ava[0])*1.1)/101;
				else if(ava[0]>0)
					AddXValue =(ava[InputNum-1] - ava[0])*6/500;
				StarXValue = ava[InputNum-1] + (ava[InputNum-1] - ava[0])/10;
				for(i=99;i>=0;i--)
				{
					AbsToC[i] = StarXValue;
					StarXValue -= AddXValue;	
				}
				for(i=99;i>=0;i--)
				{
					AbsValue[i]=p[0]-p[1]*exp(-p[2]*pow((double)AbsToC[i],p[3]));
				}
			}
			break;
		case 1:
			{
				if(ava[0]==0)
					AddXValue =((ava[InputNum-1] - ava[0])*1.1)/101;
				else if(ava[0]>0)
					AddXValue =(ava[InputNum-1] - ava[0])*6/500;
				StarXValue = ava[InputNum-1] + (ava[InputNum-1] - ava[0])/10;
				for(i=99;i>=0;i--)
				{
					AbsToC[i] = StarXValue;
					StarXValue -= AddXValue;	
				}
				for(i=99;i>=0;i--)
				{
					AbsValue[i]=p[0]/(1+p[1]*exp(-p[2]*(double)AbsToC[i]));
				}
			}
			break;
		case 2:
			{
				AddXValue =(ava[InputNum-1]*1.1)/101;
				StarXValue = ava[InputNum-1];
				for(i=99;i>=0;i--)
				{
					AbsToC[i] = StarXValue;
					StarXValue -= AddXValue;	
				}

				for(i=99;i>=0;i--)
				{
					AbsValue[i]=p[0]/(1+exp(-(p[1]+p[2]*log(AbsToC[i]))))+StartingPoint_y;
				}
			}
			break;
		case 3:
			{
				AddXValue =(ava[InputNum-1]*1.1)/101;
				StarXValue = ava[InputNum-1];
				for(i=99;i>=0;i--)
				{
					AbsToC[i] = StarXValue;
					StarXValue -= AddXValue;	
				}
				for(i=99;i>=0;i--)
				{
					AbsValue[i]=p[0]/(1+exp(-(p[1]+p[2]*log(AbsToC[i])+p[3]*AbsToC[i])))+StartingPoint_y;
				}
				for(i=99;i>=1;i--)
				{
					if (AbsValue[i]<AbsValue[i-1])
					{
						m_sEditshow += "\r\n\r\n函数非单调性";
						UpdateData(FALSE);
						break;
					}
				}
			}
			break;
		case 4:
			{
				AddXValue =(ava[InputNum-1]*1.1)/101;
				StarXValue = ava[InputNum-1];
				for(i=99;i>=0;i--)
				{
					AbsToC[i] = StarXValue;
					StarXValue -= AddXValue;	
				}
				for(i=99;i>=0;i--)
				{
					AbsValue[i]=p[0]*exp(p[1]*log(AbsToC[i])+p[2]*log(AbsToC[i])*log(AbsToC[i])+p[3]*log(AbsToC[i])*log(AbsToC[i])*log(AbsToC[i]))+StartingPoint_y;
				}
				for(i=99;i>=1;i--)
				{
					if (AbsValue[i]<AbsValue[i-1])
					{
						m_sEditshow += "\r\n\r\n函数非单调性";
						UpdateData(FALSE);
						break;
					}
				}
			}
			break;
		case 5:
			{
				//AddXValue =(ava[InputNum-1]-ava[0])/100;
				AddXValue =(ava[InputNum-1]*1.5-ava[0])/100;
				float StarXValue = ava[InputNum-1]*1.5;
				for(i=99;i>=0;i--)
				{
					AbsToC[i] = StarXValue;
					StarXValue -= AddXValue;	
				}
				for(i=98;i>=0;i--)
				{
					int malinum;
					for(malinum=0;malinum <n-1;malinum++) 
					{ 
						if   (AbsToC[i]> Spline_x[malinum]&&AbsToC[i] <=Spline_x[malinum+1]) 
							break; 
					} 
					if (AbsToC[i]> Spline_x[n-1])
					{
						malinum=n-2;
					}
					AbsValue[i]=Spline_y[malinum]+Spline_a[malinum]*(AbsToC[i]-Spline_x[malinum])+Spline_b[malinum]*(AbsToC[i]-Spline_x[malinum])*(AbsToC[i]-Spline_x[malinum])+Spline_c[malinum]*(AbsToC[i]-Spline_x[malinum])*(AbsToC[i]-Spline_x[malinum])*(AbsToC[i]-Spline_x[malinum]); 
				}
				for(i=98;i>=1;i--)
				{
					if (AbsValue[i]<AbsValue[i-1])
					{
						m_sEditshow += "\r\n\r\n函数非单调性";
						UpdateData(FALSE);
						break;
					}
				}
			}
			break;
		default:break;
	}
	for(i=98;i>=0;i--)
	{
		abs_value=AbsValue[i];
		if(absmin > abs_value)
			absmin=abs_value;
		if(absmax<abs_value)
			absmax=abs_value;
		m_wndLineChart.SetData(AbsToC[i],abs_value,0);
	}
	UpdateData(TRUE);
	bva[0] = atof(m_dEdit_Y1);
	bva[1] = atof(m_dEdit_Y2);
	bva[2] = atof(m_dEdit_Y3);
	bva[3] = atof(m_dEdit_Y4);
	bva[4] = atof(m_dEdit_Y5);
	bva[5] = atof(m_dEdit_Y6);
	bva[6] = atof(m_dEdit_Y7);
	bva[7] = atof(m_dEdit_Y8);
	bva[8] = atof(m_dEdit_Y9);
	bva[9] = atof(m_dEdit_Y10);
	m_wndLineChart.SetYMaxMin(absmax,absmin);	
	m_wndLineChart.DrawChart(0);
	for (i=0;i<n;i++)
	{
		m_wndLineChart.DrawRect( ava[i], bva[i],0);
	}
}

void CMyCurveDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	bva[0] = atof(m_dEdit_Y1);
	bva[1] = atof(m_dEdit_Y2);
	bva[2] = atof(m_dEdit_Y3);
	bva[3] = atof(m_dEdit_Y4);
	bva[4] = atof(m_dEdit_Y5);
	bva[5] = atof(m_dEdit_Y6);
	bva[6] = atof(m_dEdit_Y7);
	bva[7] = atof(m_dEdit_Y8);
	bva[8] = atof(m_dEdit_Y9);
	bva[9] = atof(m_dEdit_Y10);
	switch(m_iSelFunction)
	{
		case 0:
			{
				m_sEdit_ini1.Format("%f",bva[InputNum-1]*1.1);
				m_sEdit_ini2.Format("%f",bva[InputNum-1]*1.1-bva[0]*0.8);
			}
			break;
		case 1:
			{
				m_sEdit_ini1.Format("%f",bva[InputNum-1]*1.1);
				m_sEdit_ini2.Format("%f",bva[InputNum-1]*1.1-bva[0]*0.8);
			}
			break;
		case 2:
			{
				m_sEdit_ini2="0";
				m_sEdit_ini1.Format("%f",bva[InputNum-1]*1.1);
			}
			break;
		default:break;
	}
	m_sEdit_ini3 = "0";
	m_sEdit_ini4 = "1";
	//m_sEdit_ini5 = "1";
	UpdateData(FALSE);

}

void CMyCurveDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	bva[0] = atof(m_dEdit_Y1);
	bva[1] = atof(m_dEdit_Y2);
	bva[2] = atof(m_dEdit_Y3);
	bva[3] = atof(m_dEdit_Y4);
	bva[4] = atof(m_dEdit_Y5);
	bva[5] = atof(m_dEdit_Y6);
	bva[6] = atof(m_dEdit_Y7);
	bva[7] = atof(m_dEdit_Y8);
	bva[8] = atof(m_dEdit_Y9);
	bva[9] = atof(m_dEdit_Y10);
	float  m_input_od = atof(m_sEdit_input_od);
	CString tempo;
	//if (m_input_od>bva[InputNum-1]*1.5)
	//{
	//	MessageBox("超出最大值1.5倍");
	//	return;
	//}
	double A, x, y, B, C;
	switch(m_iSelFunction)
	{
		case 0:
			{
				y=Parameters_d;
				A=-((log((Parameters_a - m_input_od)/Parameters_b))/Parameters_c);
				B=1;
				B=B/y;
				x=pow(A, B);
			}
			break;
		case  1:
			{
				x=-1*(log(((Parameters_a/m_input_od)-1)/Parameters_b)/Parameters_c);
			}
			break;
		case  2:
			{
				x=exp((-log(Parameters_a/(m_input_od-StartingPoint_y)-1)-Parameters_b)/Parameters_c);
			}
			break;
		case  3:
			{
				int num =0;
				A=0;B=ava[InputNum-1]*1.5;
				y=m_input_od;
				BOOL bDir=FALSE;
				while (1)
				{
					num++;
					x=(A+B)/2;
					C=Parameters_a/(1+exp(-(Parameters_b+Parameters_c*log(x)+Parameters_d*x)))+StartingPoint_y;
					if (bMonotonicity)//注意搜索方向
					{
						if (C<y)
						{
							bDir = TRUE;
						}
						else
						{
							bDir = FALSE;
						}
					}
					else
					{
						if (C>y)
						{
							bDir = TRUE;
						}
						else
						{
							bDir = FALSE;
						}
					}
					if (bDir)
					{
						if ((y-C)<1E-5)
						{
							break;
						}
						else
						{
							A=x;
						}
					}
					else
					{
						if ((C-y)<1E-5)
						{
							break;
						}
						else
						{
							B=x;
						}
					}
					if (num>100000)
					{
						m_sEdit_Result = "错误";
						UpdateData(FALSE);
						return;
					}
				}
				//tempo.Format("%f",GetRealValue(0,StartingPoint_y,Parameters_a,Parameters_b,Parameters_c,Parameters_d,y,ava[InputNum-1],ava[0],1));
				//tempo.Format("%f",GetRealValue(0,0,0.2846,-2.9,0.4493,0.0282,0.0464,96,8.5,1));
				//MessageBox(tempo);
			}
			break;
		case  4:
			{
				int num =0;
				A=0;B=ava[InputNum-1]*1.5;
				y=m_input_od;
				int Monotonicity;
				BOOL bDir=FALSE;
				while (1)
				{
					num++;
					x=(A+B)/2;
					C=Parameters_a*exp(Parameters_b*log(x)+Parameters_c*log(x)*log(x)+Parameters_d*log(x)*log(x)*log(x))+StartingPoint_y;
					if (bMonotonicity)//注意搜索方向
					{
						if (C<y)
						{
							bDir = TRUE;
						}
						else
						{
							bDir = FALSE;
						}
					}
					else
					{
						if (C>y)
						{
							bDir = TRUE;
						}
						else
						{
							bDir = FALSE;
						}
					}
					if (bDir)
					{
						if ((y-C)<1E-5)
						{
							break;
						}
						else
						{
							A=x;
						}
					}
					else
					{
						if ((C-y)<1E-5)
						{
							break;
						}
						else
						{
							B=x;
						}
					}
					if (num>100000)
					{
						m_sEdit_Result = "错误";
						UpdateData(FALSE);
						return;
					}
				}
				tempo.Format("%f",GetRealValue(1,StartingPoint_y,Parameters_a,Parameters_b,Parameters_c,Parameters_d,y,ava[InputNum-1],ava[0],1));
				MessageBox(tempo);
			}
			break;
		case 5:
			{
				int i;
				for(i=0;i <Spline_n-1;i++) //根据吸光度选择曲线区间
				{ 
					if (bMonotonicity)//bMonotonicity 判断单调方向 TRUE 为正
					{
						if   (m_input_od>= Spline_y[i]&&m_input_od <=Spline_y[i+1]) 
							break; 
					}
					else
					{
						if   (m_input_od<= Spline_y[i]&&m_input_od >=Spline_y[i+1]) 
							break; 
					}
				} 
				int num =0;
				A=Spline_x[i];B=Spline_x[i+1];
				y=m_input_od;
				BOOL bDir=TRUE;
				while (1)
				{
					num++;
					x=(A+B)/2;
					C=Spline_y[i]+Spline_a[i]*(x-Spline_x[i])+Spline_b[i]*(x-Spline_x[i])*(x-Spline_x[i])+Spline_c[i]*(x-Spline_x[i])*(x-Spline_x[i])*(x-Spline_x[i]); 
					if (bMonotonicity)//注意搜索方向
					{
						if (C<y)
						{
							bDir = TRUE;
						}
						else
						{
							bDir = FALSE;
						}
					}
					else
					{
						if (C>y)
						{
							bDir = TRUE;
						}
						else
						{
							bDir = FALSE;
						}
					}
					if (bDir)
					{
						if (abs(y-C)<1E-5)
						{
							break;
						}
						else
						{
							A=x;
						}
					}
					else
					{
						if (abs(C-y)<1E-5)
						{
							break;
						}
						else
						{
							B=x;
						}
					}
					if (num>100000)
					{
						m_sEdit_Result = "错误";
						UpdateData(FALSE);
						return;
					}
				}
			}
			break;
		default:break;
	}
	m_sEdit_Result.Format("%f",x);
	UpdateData(FALSE);
	m_wndLineChart.DrawRect(x ,m_input_od,1);
	Invalidate(1);
}

void CMyCurveDlg::OnEnKillfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	InputNum=m_iEdit_Num;
	if (InputNum<0||InputNum>10)
	{
		MessageBox("输入点数错误");
		return;
	}
}



BOOL CMyCurveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if(pMsg->message==WM_KEYDOWN &&(pMsg->wParam==VK_ESCAPE/*||pMsg->wParam==VK_RETURN*/))
	//	return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CMyCurveDlg::OnCbnSelchangeComboSelfunc()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iSelFunction=m_cCombo_SelFunction.GetCurSel();
}

void CMyCurveDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

void CMyCurveDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

/*float CMyCurveDlg::GetRealValue(int sel_log5p_exp5p,float R0, float K, float a, float b, float c, float meas_abs, float max_standard_value,float min_standard_value,int Monotonicity)
//{
//	int num =0;
//	double A, B,x, y;				
//	A=0;
//	B=max_standard_value*1.5;
//	y=meas_abs;
//
//	float meas_result;
//	while (1)
//	{
//		num++;
//		x=(A+B)/2;
//		if(sel_log5p_exp5p == 0)					// 公式选择处 [10/7/2010]
//			meas_result=K/(1+exp(-(a+b*log(x)+c*x)))+R0;
//		else
//			meas_result=K*exp(a*log(x)+b*log(x)*log(x)+c*log(x)*log(x)*log(x))+R0;
//		if (meas_result<y)
//		{
//			if ((y-meas_result)<1E-3)				//已经无限接近真解,可以认为 C = y ;
//				break;
//			else
//				A=x;
//		}
//		else
//		{
//			if ((meas_result-y)<1E-3)
//				break;
//			else									// 进一步缩小范围.
//				B=x;
//		}
//		if(num>1000)								//迭代次数超过限定值,说明求不出所需的值
//		{
//			x=max_standard_value*1.5;
//			break;
//		}
//	}	
//
//	return x;
//}
{
	int num =0;
	double A, B,x, y,meas_result;                                     
	A=min_standard_value;
	B=max_standard_value;
	y=meas_abs;
	while (1)
	{
		num++;
		meas_result=(A+B)/2;
		if(sel_log5p_exp5p == 0)                                                        // 公式选择处 [10/7/2010]
			x=K/(1+exp(-(a+b*log(meas_result)+c*meas_result)))+R0;
		else
			x=K*exp(a*log(meas_result)+b*log(meas_result)*log(meas_result)+c*log(meas_result)*log(meas_result)*log(meas_result))+R0;;
		if (x<y)
		{
			if ((y-x)<1E-3)                                                //已经无限接近真解,可以认为 C = y ;
				break;
			else
				A=meas_result;
		}
		else
		{
			if ((x-y)<1E-3)
				break;
			else                                                                                                      // 进一步缩小范围.
				B=meas_result;
		}
		if(num>1000)                                                                                      //迭代次数超过限定值,说明求不出所需的值
		{
			meas_result=max_standard_value*1.1;
			break;
		}
	}          
	return (float)meas_result;
}*/

float CMyCurveDlg::GetRealValue(int sel_log5p_exp5p,float R0, float K, float a, float b, float c, float meas_abs, float max_standard_value,float min_standard_value,int Monotonicity)
{
	int num =0;
	double A, B,x, y,meas_result;                                                    
	A=min_standard_value;
	B=max_standard_value;
	y=meas_abs;
	BOOL bDir=TRUE;                             // 增加单调性的处理 [1/21/2011]
	while (1)
	{
		num++;
		meas_result=(A+B)/2;
		if(sel_log5p_exp5p == 0)                                                                              // 公式选择处 [10/7/2010]
			x=K/(1+exp(-(a+b*log(meas_result)+c*meas_result)))+R0;
		else
			x=K*exp(a*log(meas_result)+b*log(meas_result)*log(meas_result)+c*log(meas_result)*log(meas_result)*log(meas_result))+R0;
		// 修正算法，增加单调性 [1/21/2011]
		if (Monotonicity == 1)                    //递增
		{
			if (x<y)
				bDir = TRUE;
			else
				bDir = FALSE;
		}
		else
		{
			if (x>y)
				bDir = TRUE;
			else
				bDir = FALSE;
		}
		if (bDir)
		{
			if (abs(y-x)<1E-5)
				break;
			else
				A=meas_result;
		}
		else
		{
			if (abs(x-y)<1E-5)
				break;
			else
				B=meas_result;
		}                              
		if(num>100000)      //迭代次数超过限定值,说明求不出所需的值
			break;
	}              
	return (float)meas_result;
}