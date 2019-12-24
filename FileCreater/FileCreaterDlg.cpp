
// FileCreaterDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "FileCreater.h"
#include "FileCreaterDlg.h"
#include "afxdialogex.h"
#include "CmnFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileCreaterDlg �_�C�A���O




CFileCreaterDlg::CFileCreaterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileCreaterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileCreaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_EditPath);
	DDX_Control(pDX, IDC_EDIT1, m_EditSize);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_EditName);
}

BEGIN_MESSAGE_MAP(CFileCreaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEARCH, &CFileCreaterDlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileCreaterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_INC_1GB, &CFileCreaterDlg::OnBnClickedInc1gb)
	ON_BN_CLICKED(IDC_DEC_1GB, &CFileCreaterDlg::OnBnClickedDec1gb)
END_MESSAGE_MAP()


// CFileCreaterDlg ���b�Z�[�W �n���h���[

BOOL CFileCreaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	CString strSpecialPath;
	strSpecialPath = CCmnFile::GetSpecialFolderPath(CSIDL_DESKTOP);
	m_EditPath.SetWindowTextW(strSpecialPath);
	m_EditName.SetWindowTextW(_T("TestFile.dat"));
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CFileCreaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CFileCreaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CFileCreaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileCreaterDlg::OnBnClickedSearch()
{
	CString strBuff;
	CCmnFile::SelectFolder(this,strBuff,CMN_FILE_TITLE_DIR);
	if(strBuff != _T(""))
	{
		m_EditPath.SetWindowTextW(strBuff);
	}
}


void CFileCreaterDlg::OnBnClickedButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B	
	CString strDirPath;
	CString strFilePath;
	CString strPath;
	CString strSize;
	CString strCommandText;
	CString strCommand;
	m_EditPath.GetWindowTextW(strDirPath);
	m_EditName.GetWindowTextW(strFilePath);
	strPath.Format(strDirPath + _T("\\") + strFilePath);
	m_EditSize.GetWindowTextW(strSize);
	strCommandText.Format(_T("fsutil file createnew "));
	strCommand = _T("\"") + strCommandText + strPath + _T(" ") +strSize + _T("\"");
	
	if(strSize == _T(""))
	{
		MessageBox(_T("�t�@�C���T�C�Y����͂��Ă��������B"),_T("�x��"),MB_OK | MB_ICONWARNING);
		return;
	}

	if(CCmnFile::IsFileExist(strPath))
	{
		if(IDOK != MessageBox(_T("�t�@�C�������Ƀt�@�C�������݂��Ă��܂��B\r\n�㏑�����܂����H"),_T("�x��"),MB_OKCANCEL | MB_ICONWARNING))
		{
			return;
		}
		else
		{
			if(!DeleteFile(strPath))
			{
				MessageBox(_T("�t�@�C���̏㏑���Ɏ��s���܂����B"),_T("�x��"),MB_OK | MB_ICONWARNING);
				return;
			}
		}
	}

	CStringA strACommand(strCommand);
	system(strACommand);
	//RunCommand(_T("cmd.exe /c"),strCommand);

}
//�e���v���[�g�p����1
//�G�X�P�[�v�L�[��RETURN�L�[�����̏���
//Alt + F4 �ɂ�����I���𖳎��i�T�u�X���b�h����ƒ��ɗ��Ƃ���Ȃ����߁j
//�_�C�A���O��������ɋL�q���悤
BOOL CFileCreaterDlg::PreTranslateMessage(MSG* pMsg)
{

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		CWnd* pwndCtrl = GetFocus();
		int ctrl_ID = pwndCtrl->GetDlgCtrlID();
		CEdit* pEdit;
		switch (ctrl_ID)
		{
			case IDC_BUTTON1:
			case IDC_EDIT1:
				OnBnClickedButton1();
				break;
			case IDC_SEARCH:
				OnBnClickedSearch();
				break;
			case IDC_EDIT_FILENAME:
				pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
				pEdit->SetFocus();
				break;
			case IDC_EDIT_PATH:
				//pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
				pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FILENAME);
				pEdit->SetFocus();
				break;
			default:
			break;
		}
		//
	}
	if(pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{ 
		case VK_RETURN:
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}
	if(pMsg->message ==  WM_SYSCOMMAND)
	{
		//switch (pMsg->wParam)
		//{ 
		////case SC_CLOSE: 
		////	return TRUE;
		//default:
		//	break;
		//}
	}		
	return CDialog::PreTranslateMessage(pMsg); 
}
/////////////////////////////////////////////////////////////////////////////
//
//	(RUN)�R�}���h����
//
//	BOOL RunCommand(CString Path)
//
//	���́F	�p�X
//
//	�o�́F	�����FTURE�A���s�FFALSE
//
//	�@�\�F	�w�肳�ꂽ���s�t�@�C�������s����
//
/////////////////////////////////////////////////////////////////////////////
BOOL CFileCreaterDlg::RunCommand(CString Path,CString Para)
{
	wchar_t lpPath[MAX_PATH];
	BOOL bError = FALSE;
	StrCpyW(lpPath,Path);

	if(!bError)
	{

		SHELLEXECUTEINFO	sei = { 0 };
		DWORD				ret = ERROR_SUCCESS;
		//�\���̂̃T�C�Y
		sei.cbSize = sizeof(SHELLEXECUTEINFO);						
		//�N�����̃E�C���h�E�n���h��
		sei.hwnd = NULL;//prei->hWnd;									
		//�N����̕\�����
		sei.nShow = SW_SHOWNORMAL;									
		//���̃p�����[�^���d�v�ŁA�Z�b�g���Ȃ���SHELLEXECUTEINFO�\���̂�hProcess�����o���Z�b�g����Ȃ��B
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		//�R�}���h���C���p�����[�^
		sei.lpParameters = Para;
		//�N���v���O����
		sei.lpFile = lpPath;
		try{
			//�v���Z�X�N��
			if(!ShellExecuteEx(&sei)){	
				throw 1;
			}
			//NULL�`�F�b�N
			if(sei.hProcess == NULL) {
				throw 1;
			}
			//�I����҂�
			WaitForSingleObject(sei.hProcess, INFINITE);
			//�I���R�[�h�擾
			if(!GetExitCodeProcess(sei.hProcess, &ret))
			{
				throw 1;
			}
		}
		catch(...)
		{
			bError = TRUE;
			//�J��
			if(sei.hProcess != NULL)
			{
				CloseHandle(sei.hProcess);
			}
			return FALSE;
		}
		//�J��
		if(sei.hProcess != NULL)
		{
			CloseHandle(sei.hProcess);
		}		
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

CString CFileCreaterDlg::HFileSelect(CString filter)
{
	//�t�B���^�[�̐ݒ�̎d��
	//�����ꂾ��cpp��.h�����[�U�[�ɐ؂�ւ��Ă��炤���ƂɂȂ�
    //filter = _T("�\�[�X�t�@�C��(*.cpp;*.c)|*.cpp;*.c|�w�b�_�t�@�C��(*.hpp;*.h)|*.hpp;*.h||");
	//                       ��������           �����e�@�@�@�@�@�@��������        �����e�@��"||"�ŏI��
	//filter = _T("������|*.cpp;*.c;*.hpp;*.h||");
	//                                          ���啶������������ʂ��Ă��Ȃ�
	//filter = _T("�t�@�C��(*.wav;*.mp3)|*.wav;*.mp3||");
	CFileDialog	selDlg(TRUE, NULL, NULL, NULL, filter);
	CString str;
    if (selDlg.DoModal() == IDOK)
    {
        str = selDlg.GetPathName();
	}
	else
	{
		str.Format(_T(""));
	}
    return str;
}

void CFileCreaterDlg::OnBnClickedInc1gb()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CString strSize;
	__int64 LSize = 0;
	m_EditSize.GetWindowTextW(strSize);
	LSize = _ttoi64(strSize);
	LSize += 1024*1024*1024;
	strSize.Format(_T("%llu"),LSize);
	m_EditSize.SetWindowTextW(strSize);
}


void CFileCreaterDlg::OnBnClickedDec1gb()
{
	CString strSize;
	__int64 LSize = 0;
	m_EditSize.GetWindowTextW(strSize);
	LSize = _ttoi64(strSize);
	LSize -= 1024*1024*1024;
	if(LSize < 0)
	{
		LSize = 0;
	}
	strSize.Format(_T("%llu"),LSize);
	m_EditSize.SetWindowTextW(strSize);
}
