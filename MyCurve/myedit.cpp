// myedit.cpp : ʵ���ļ�
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



// myedit ��Ϣ�������



void myedit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nChar== VK_RETURN) //���뿪ʼ���� 
	{ 
		CDialog* Pwnd=(CDialog*)GetParent() ;//ȡ�öԻ���ָ�� 
		Pwnd->NextDlgCtrl( ); //�л�����һ�����뽹�� 
	} 
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
