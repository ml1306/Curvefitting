// myedit.cpp : 实现文件
//

#include "stdafx.h"
#include "MyCurve.h"
#include "myedit.h"


// myedit

IMPLEMENT_DYNAMIC(myedit, CEdit)

myedit::myedit()
{

}

myedit::~myedit()
{
}


BEGIN_MESSAGE_MAP(myedit, CEdit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// myedit 消息处理程序



void myedit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar== VK_RETURN) //插入开始部分 
	{ 
		CDialog* Pwnd=(CDialog*)GetParent() ;//取得对话框指针 
		Pwnd->NextDlgCtrl( ); //切换到下一个输入焦点 
	} 
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
