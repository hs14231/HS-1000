
// FileCreaterDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"


// CFileCreaterDlg �_�C�A���O
class CFileCreaterDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CFileCreaterDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FILECREATER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSearch();
	CEdit m_EditPath;
	afx_msg void OnBnClickedButton1();
	BOOL PreTranslateMessage(MSG* pMsg);//�e���v���[�g�p����1;
	CEdit m_EditSize;
private:
	BOOL RunCommand(CString Path,CString Para);
	CString HFileSelect(CString filter);//���Ԃ�P���I��
public:
	CEdit m_EditName;
	afx_msg void OnBnClickedInc1gb();
	afx_msg void OnBnClickedDec1gb();
};
