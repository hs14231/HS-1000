
// FileCreaterDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"


// CFileCreaterDlg ダイアログ
class CFileCreaterDlg : public CDialogEx
{
// コンストラクション
public:
	CFileCreaterDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_FILECREATER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSearch();
	CEdit m_EditPath;
	afx_msg void OnBnClickedButton1();
	BOOL PreTranslateMessage(MSG* pMsg);//テンプレート用その1;
	CEdit m_EditSize;
private:
	BOOL RunCommand(CString Path,CString Para);
	CString HFileSelect(CString filter);//たぶん単数選択
public:
	CEdit m_EditName;
	afx_msg void OnBnClickedInc1gb();
	afx_msg void OnBnClickedDec1gb();
};
