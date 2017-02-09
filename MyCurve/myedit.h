#pragma once


// myedit

class myedit : public CEdit
{
	DECLARE_DYNAMIC(myedit)

public:
	myedit();
	virtual ~myedit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


