
// FileCreaterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "FileCreater.h"
#include "FileCreaterDlg.h"
#include "afxdialogex.h"
#include "CmnFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CFileCreaterDlg ダイアログ




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


// CFileCreaterDlg メッセージ ハンドラー

BOOL CFileCreaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	CString strSpecialPath;
	strSpecialPath = CCmnFile::GetSpecialFolderPath(CSIDL_DESKTOP);
	m_EditPath.SetWindowTextW(strSpecialPath);
	m_EditName.SetWindowTextW(_T("TestFile.dat"));
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CFileCreaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。	
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
		MessageBox(_T("ファイルサイズを入力してください。"),_T("警告"),MB_OK | MB_ICONWARNING);
		return;
	}

	if(CCmnFile::IsFileExist(strPath))
	{
		if(IDOK != MessageBox(_T("ファイルが既にファイルが存在しています。\r\n上書きしますか？"),_T("警告"),MB_OKCANCEL | MB_ICONWARNING))
		{
			return;
		}
		else
		{
			if(!DeleteFile(strPath))
			{
				MessageBox(_T("ファイルの上書きに失敗しました。"),_T("警告"),MB_OK | MB_ICONWARNING);
				return;
			}
		}
	}

	CStringA strACommand(strCommand);
	system(strACommand);
	//RunCommand(_T("cmd.exe /c"),strCommand);

}
//テンプレート用その1
//エスケープキーとRETURNキー無視の処理
//Alt + F4 における終了を無視（サブスレッドが作業中に落とされないため）
//ダイアログ作った時に記述しよう
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
//	(RUN)コマンド処理
//
//	BOOL RunCommand(CString Path)
//
//	入力：	パス
//
//	出力：	成功：TURE、失敗：FALSE
//
//	機能：	指定された実行ファイルを実行する
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
		//構造体のサイズ
		sei.cbSize = sizeof(SHELLEXECUTEINFO);						
		//起動側のウインドウハンドル
		sei.hwnd = NULL;//prei->hWnd;									
		//起動後の表示状態
		sei.nShow = SW_SHOWNORMAL;									
		//このパラメータが重要で、セットしないとSHELLEXECUTEINFO構造体のhProcessメンバがセットされない。
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		//コマンドラインパラメータ
		sei.lpParameters = Para;
		//起動プログラム
		sei.lpFile = lpPath;
		try{
			//プロセス起動
			if(!ShellExecuteEx(&sei)){	
				throw 1;
			}
			//NULLチェック
			if(sei.hProcess == NULL) {
				throw 1;
			}
			//終了を待つ
			WaitForSingleObject(sei.hProcess, INFINITE);
			//終了コード取得
			if(!GetExitCodeProcess(sei.hProcess, &ret))
			{
				throw 1;
			}
		}
		catch(...)
		{
			bError = TRUE;
			//開放
			if(sei.hProcess != NULL)
			{
				CloseHandle(sei.hProcess);
			}
			return FALSE;
		}
		//開放
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
	//フィルターの設定の仕方
	//↓これだとcppと.hをユーザーに切り替えてもらうことになる
    //filter = _T("ソースファイル(*.cpp;*.c)|*.cpp;*.c|ヘッダファイル(*.hpp;*.h)|*.hpp;*.h||");
	//                       ↑説明文           ↑内容　　　　　　↑説明文        ↑内容　↑"||"で終了
	//filter = _T("説明文|*.cpp;*.c;*.hpp;*.h||");
	//                                          ↓大文字小文字を区別していない
	//filter = _T("ファイル(*.wav;*.mp3)|*.wav;*.mp3||");
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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
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
