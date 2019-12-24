#include "StdAfx.h"
#include "CmnFile.h"
#include <locale.h>

/////////////////////////////////////////////////////////////////////////////
//	変数定義
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//	デフォルトフォルダ	SelectFolder関数用
TCHAR *				CCmnFile::m_DefaultFolder	= NULL;
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//
//	コンストラクタ
//
//	CCmnFile()
//
//	入力：	なし
//
//	出力：	なし
//
//	機能：	初期化を行う。
//
/////////////////////////////////////////////////////////////////////////////
CCmnFile::CCmnFile(void)
{
}


/////////////////////////////////////////////////////////////////////////////
//
//	デストラクタ
//
//	~CCmnFile()
//
//	入力：	なし
//
//	出力：	なし
//
//	機能：	後処理を行う。
//
/////////////////////////////////////////////////////////////////////////////
CCmnFile::~CCmnFile(void)
{
}


BOOL CCmnFile::FileNameToPathName(CString &dir, LPCTSTR fname)
{
//	ASSERT(fname != NULL);

	TCHAR					path[MAX_PATH + 2];
	BOOL					bRet;

#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
	::_tcscpy(path, fname);
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
	::_tcscpy_s(path, MAX_PATH, fname);
#endif
	//	パス名を抽出
	bRet	= ::PathRemoveFileSpec(path);
	dir	= path;

	return (bRet == FALSE) ? FALSE : TRUE;
}
	/////////////////////////////////////////////////////////////////////////////
	//	アプリケーションディレクトリ取得
	//	入力：	HINSTANCE		hInstance;		アプリケーションのインスタンスハンドル
	//			CString			path;			アプリケーションディレクトリの格納先
	//	出力：	TRUE			正常終了
	//			FALSE			異常終了
	/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::GetApplPath(HINSTANCE hInstance, CString &path)
{
	TCHAR					appl_path[MAX_PATH + 2];
	BOOL					bRet;

	//	モジュール名称を取得
	::GetModuleFileName(hInstance, appl_path, MAX_PATH);

	//	パス名を抽出
	bRet	= CCmnFile::FileNameToPathName(path, appl_path);

	return bRet;
}
////////////////////////////////////////////////////////////////////////////
//
//	1行読み込み(MFC)
//
//	BOOL ReadString(CFile *file, CString &str)
//
//	入力：	CFile			*file;			ファイルクラスへのポインタ
//			CString			&str;			読み込んだ文字列の格納先
//
//	出力：	TRUE			正常終了
//			FALSE			読み込みを行わなかった。
//
//	機能：	fileで指定されたファイルから、1行分の文字列を読み込みstrに格納する。
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::ReadString(CFile *file, CString &str)
{
//	ASSERT(file != NULL);
//	ASSERT(file->m_hFile != CFile::hFileNull);

	TCHAR					data;
	UINT					rbytes;
#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
	LONG					size;
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
	LONGLONG				size;
#endif
	BOOL					bRet	= TRUE;
	LONG					loop;

	//	読み込み先をクリアする。
	str.Empty();

	for(loop = 0; ; loop++) {
		rbytes	= file->Read(&data, sizeof(TCHAR));
		if(rbytes != sizeof(TCHAR)) {
			if(loop == 0) {
				//	1バイトも読み込めずにエラーが発生した場合は、エラーとする。
				bRet	= FALSE;
			}
			break;
		}
		if(data == 0x0D) {				//	CR or CR/LF
			rbytes	= file->Read(&data, sizeof(TCHAR));
			if(data != 0x0A) {
				//	LFではないので１文字分戻る
				size	= sizeof(TCHAR);
				file->Seek(size * -1, CFile::current);
				break;
			}
			break;

		} else if(data == 0x0A) {		//	LF
			break;

		} else if(data == 0x1A) {		//	EOF
			bRet	= FALSE;
			break;
		}

		str	+= data;
	}

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////
//
//	ディレクトリの有無診断
//
//	BOOL IsDirExist(LPCTSTR dirname)
//
//	入力：	LPCTSTR			dirname;		ディレクトリ名称
//
//	出力：	TRUE			存在する。
//			FALSE			存在しない。
//
//	機能：	dirnameで指定されたディレクトリが存在するか診断する。
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::IsDirExist(LPCTSTR dirname)
{
//	ASSERT(dirname != NULL);

	DWORD					status;
	BOOL					bRet	= FALSE;

	status	= ::GetFileAttributes(dirname);
	if(status != 0xFFFFFFFF) {
		if((status & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			bRet	= TRUE;
		}
	}

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////
//
//	ディレクトリ作成
//
//	BOOL MakeDirectory(LPCTSTR dirname)
//
//	入力：	LPCTSTR			dirname;		作成するディレクトリ名称
//
//	出力：	FALSE			異常終了
//			TRUE			正常終了
//
//	機能：	dirnameで指定されたディレクトリを作成する。
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::MakeDirectory(LPCTSTR dirname)
{
//	ASSERT(dirname != NULL);

	BOOL					bRet;
	CString					dir;

	//	親ディレクトリの存在チェック
	bRet	= CCmnFile::FileNameToPathName(dir, dirname);
	if(bRet != FALSE) {
		if(IsDirExist(dir) == FALSE) {
			//	親ディレクトリが存在しない
			bRet	= CCmnFile::MakeDirectory(dir);
		}
		if(bRet != FALSE) {
			bRet	= ::CreateDirectory(dirname, NULL);
		}
	}

	return (bRet == FALSE) ? FALSE : TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
//	ディレクトリ削除
//
//	BOOL DeleteDirectory(LPCTSTR dirname)
//
//	入力：	LPCTSTR			dirname;		削除するディレクトリ名称
//
//	出力：	FALSE			異常終了
//			TRUE			正常終了
//
//	機能：	dirnameで指定されたディレクトリを削除する。
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::DeleteDirectory(LPCTSTR dirname)
{
//	ASSERT(dirname != NULL);

	BOOL					bRet;

	bRet	= ::RemoveDirectory(dirname);
	return (bRet == FALSE) ? FALSE : TRUE;
}


/////////////////////////////////////////////////////////////////////////////
//
//	フルパスファイル名称の作成
//
//	BOOL	MakeFileName(CString &pathname, LPCTSTR dirname, LPCTSTR fname)
//
//	入力：	CString			&pathname;		ファイル名称(フルパス)の格納先
//			LPCTSTR			dirname;		ディレクトリ名称
//			LPCTSTR			fname;			ファイル名称
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	dirname、fnameからファイル名称(フルパス)を作成し、pathnameに格納する。
//
//	備考：	dirname、fnameの内容により以下のように動作する。
//				dirname = C:\QWE 	fname = qwe.txt		→	pathname = C:\QWE\qwe.txt
//				dirname = C:\QWE	fname = ..\qwe.txt	→	pathname = C:\qwe.txt
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::MakeFileName(CString &pathname, LPCTSTR dirname, LPCTSTR fname)
{
//	ASSERT(dirname != NULL);
//	ASSERT(fname != NULL);

	TCHAR					path[MAX_PATH + 2];
	LPTSTR					ptr;

	ptr	= ::PathCombine(path, dirname, fname);
	if(ptr != NULL) {
		pathname	= path;
	}

	return (ptr == NULL) ? FALSE : TRUE;
}


/////////////////////////////////////////////////////////////////////////////
//
//	ファイルの有無診断
//
//	BOOL IsFileExist(LPCTSTR fname)
//
//	入力：	LPCTSTR			fname;			ファイル名称
//
//	出力：	TRUE			存在する。
//			FALSE			存在しない。
//
//	機能：	fnameで指定されたファイルが存在するか診断する。
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::IsFileExist(LPCTSTR fname)
{
//	ASSERT(fname != NULL);

	DWORD					status;
	BOOL					bRet	= FALSE;

	status	= ::GetFileAttributes(fname);
	if(status != 0xFFFFFFFF) {
		if((status & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			bRet	= TRUE;
		}
	}

	return bRet;
}


////////////////////////////////////////////////////////////////////////////
//
//	ファイルオープン(バイナリ読み書きモード)
//
//	BOOL OpenBinaryRW(CFile *file, LPCTSTR fname)
//
//	入力：	CFile			*file;			CFileクラスへのポインタ
//			LPCTSTR			fname;			ファイル名称
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	fnameで指定されたファイルを、バイナリ読み書きモードでオープンする。
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::OpenBinaryRW(CFile *file, LPCTSTR fname)
{
//	ASSERT(file != NULL);
//	ASSERT(fname != NULL);

	BOOL				bRet	= FALSE;

	if(file->m_hFile == CFile::hFileNull) {
		bRet	= file->Open(fname, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeBinary, NULL);
	}
	return (bRet == FALSE) ? FALSE : TRUE;
}
////////////////////////////////////////////////////////////////////////////
//
//	ファイルオープン(テキスト書き出しモード)(MFC)
//
//	BOOL OpenTextW(CFile *file, LPCTSTR fname)
//
//	入力：	CFile			*file;			CFileクラスへのポインタ
//			LPCTSTR			fname;			ファイル名称
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	fnameで指定されたファイルを、テキスト書き出しモードでオープンする。
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::OpenTextW(CFile *file, LPCTSTR fname)
{
//	ASSERT(file != NULL);
//	ASSERT(fname != NULL);

	BOOL				bRet	= FALSE;

	if(file->m_hFile == CFile::hFileNull) {
		bRet	= file->Open(fname, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText, NULL);
	}
	return (bRet == FALSE) ? FALSE : TRUE;
}
////////////////////////////////////////////////////////////////////////////
//
//	ファイルオープン(バイナリ書き出しモード)(MFC)
//
//	BOOL OpenBinaryW(CFile *file, LPCTSTR fname)
//
//	入力：	CFile			*file;			CFileクラスへのポインタ
//			LPCTSTR			fname;			ファイル名称
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	fnameで指定されたファイルを、バイナリ書き出しモードでオープンする。
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::OpenBinaryW(CFile *file, LPCTSTR fname)
{
//	ASSERT(file != NULL);
//	ASSERT(fname != NULL);

	BOOL				bRet	= FALSE;

	if(file->m_hFile == CFile::hFileNull) {
		bRet	= file->Open(fname, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeBinary, NULL);
	}
	return (bRet == FALSE) ? FALSE : TRUE;
}

////////////////////////////////////////////////////////////////////////////
//
//	ファイルシーク(MFC)
//
//	BOOL Seek(CFile *file, LONGLONG bytes, DWORD offset)
//
//	入力：	CFile			*file;			ファイルハンドル
//			LONGLONG		bytes;			移動するバイト数
//			DWORD			offset;			開始位置(CMN_FILE_SEEK_xxx)
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	fileで指定されたファイルのファイルポインタを
//			bytes、offsetで指定された位置に設定する。
//
////////////////////////////////////////////////////////////////////////////
#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
BOOL CCmnFile::Seek(CFile *file, LONG bytes, DWORD offset)
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
BOOL CCmnFile::Seek(CFile *file, LONGLONG bytes, DWORD offset)
#endif
{
//	ASSERT(file != NULL);
//	ASSERT(file->m_hFile != CFile::hFileNull);
//	ASSERT((offset == CMN_FILE_SEEK_BEGIN) || (offset == CMN_FILE_SEEK_CURRENT) || (offset == CMN_FILE_SEEK_END));

	BOOL					bRet	= TRUE;
	UINT					uiOffset;

	try {
		uiOffset	= (offset == CMN_FILE_SEEK_BEGIN) ? CFile::begin
					: (offset == CMN_FILE_SEEK_END) ?  CFile::end
					: CFile::current;
		file->Seek(bytes, uiOffset);
	} catch(CFileException *e) {
		bRet	= FALSE;
		e->Delete();
	}

	return bRet;
}
////////////////////////////////////////////////////////////////////////////
//
//	ファイルクローズ(MFC)
//
//	void CloseFile(CFile *file)
//
//	入力：	CFile			*file;			ファイルクラスへのポインタ
//
//	出力：	なし
//
//	機能：	fileで指定されたファイルのクローズを行う。
//
////////////////////////////////////////////////////////////////////////////
void CCmnFile::CloseFile(CFile *file)
{
//	ASSERT(file != NULL);

	if(file->m_hFile != CFile::hFileNull) {
		file->Close();
	}
	return;
}

////////////////////////////////////////////////////////////////////////////
//
//	書式付き文字列書き込み(MFC)
//
//	BOOL WriteFormatString(CFile *file, LPCTSTR format, ...)
//
//	入力：	CFile			*file;			ファイルクラスへのポインタ
//			LPCTSTR			format;			出力フォーマット
//
//	出力：	TRUE			正常終了
//			FALSE			読み込みを行わなかった。
//
//	機能：	fileで指定されたファイルに、formatで指定された書式に従い、
//			文字列出力を行う。
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::WriteFormatString(CFile *file, LPCTSTR format, ...)
{
//	ASSERT(file != NULL);
//	ASSERT(file->m_hFile != CFile::hFileNull);
//	ASSERT(format != NULL);
	setlocale(LC_ALL, "japanese");
	CString					str;
	va_list					list;
	BOOL					bRet	= FALSE;

	va_start(list, format);
	str.FormatV(format, list);
	va_end(list);

	if(str.GetLength() > 0) {
		file->Write(static_cast<LPCTSTR>(str), str.GetLength());
		bRet	= TRUE;
	}

	return bRet;
}

////////////////////////////////////////////////////////////////////////////
//
//	ファイルフラッシュ(MFC)
//
//	void FlushFile(CFile *file)
//
//	入力：	CFile			*file;			ファイルクラスへのポインタ
//
//	出力：	なし
//
//	機能：	fileで指定されたファイルのバッファ内のデータを書き出し、
//			バッファをクリアする。
//
////////////////////////////////////////////////////////////////////////////
void CCmnFile::FlushFile(CFile *file)
{
//	ASSERT(file != NULL);
//	ASSERT(file->m_hFile != CFile::hFileNull);

	file->Flush();
	return;
}
////////////////////////////////////////////////////////////////////////////
//
//	ファイルオープン(バイナリ書き出しモード)(SDK)
//
//	HANDLE OpenBinaryRW(LPCTSTR fname)
//
//	入力：	LPCTSTR			fname;			ファイル名称
//
//	出力：	INVALID_HANDLE_VALUE以外	正常終了
//			INVALID_HANDLE_VALUE		異常終了
//
//	機能：	fnameで指定されたファイルを、バイナリ読み書きモードでオープンする。
//
////////////////////////////////////////////////////////////////////////////
HANDLE CCmnFile::OpenBinaryRW(LPCTSTR fname)
{
//	ASSERT(fname != NULL);

	HANDLE				hFile;

	hFile	= ::CreateFile(
						fname,
						GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL								);
	return hFile;
}
BOOL CCmnFile::Seek(HANDLE hFile, LONGLONG bytes, DWORD offset)
{
//	ASSERT(hFile != INVALID_HANDLE_VALUE);
//	ASSERT((offset == CMN_FILE_SEEK_BEGIN) || (offset == CMN_FILE_SEEK_CURRENT) || (offset == CMN_FILE_SEEK_END));

	LARGE_INTEGER			point;
	BOOL					bRet	= TRUE;

	::SetLastError(ERROR_SUCCESS);
	point.QuadPart	= bytes;
	point.LowPart	= ::SetFilePointer(hFile, point.LowPart, &point.HighPart, offset);
	if(point.LowPart == INVALID_SET_FILE_POINTER) {
		if(::GetLastError() != ERROR_SUCCESS) {
			bRet	= FALSE;
		}
	}
	return bRet;
}

////////////////////////////////////////////////////////////////////////////
//
//	ファイルクローズ(SDK)
//
//	void CloseFile(HANDLE hFile)
//
//	入力：	HANDLE			hFile;			ファイルハンドル
//
//	出力：	なし
//
//	機能：	hFileで指定されたファイルのクローズを行う。
//
////////////////////////////////////////////////////////////////////////////
void CCmnFile::CloseFile(HANDLE hFile)
{
	if(hFile != INVALID_HANDLE_VALUE) {
		::CloseHandle(hFile);
	}
	return;
}

////////////////////////////////////////////////////////////////////////////
//
//	書式付き文字列書き込み(SDK)
//
//	BOOL WriteFormatString(HANDLE hFile, LPCTSTR format, ...)
//
//	入力：	HANDLE			hFile;			ファイルハンドル
//			LPCTSTR			format;			出力フォーマット
//
//	出力：	TRUE			正常終了
//			FALSE			読み込みを行わなかった。
//
//	機能：	fileで指定されたファイルに、formatで指定された書式に従い、
//			文字列出力を行う。
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::WriteFormatString(HANDLE hFile, LPCTSTR format, ...)
{
//	ASSERT(hFile != INVALID_HANDLE_VALUE);
//	ASSERT(format != NULL);

#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
	va_list					list;
	BOOL					bRet	= FALSE;
	INT						len;
	TCHAR					*str;
	DWORD					wbytes;
	INT						iRet;

	va_start(list, format);

	len	= 256;
	while(1) {
		str	= new TCHAR[len];
		memset(str, 0, (sizeof(TCHAR) * (len + 1)));
		iRet	= _vsntprintf(str, len, format, list);
		if(iRet != -1) {
			len		= ::_tcslen(str);
			bRet	= ::WriteFile(hFile, str, len * sizeof(TCHAR), &wbytes, NULL);
			delete [] str;
			break;
		}
		delete [] str;
		len	<<= 1;
	}
	va_end(list);

	return (bRet == FALSE) ? FALSE : TRUE;
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
	va_list					list;
	BOOL					bRet	= FALSE;
	INT						len;
	TCHAR					*str;
	DWORD					wbytes;

	va_start(list, format);
	len	= ::_vsctprintf(format, list);
	if(len > 0) {
		str	= new TCHAR[len + 2];
		::_vstprintf_s(str, len + 1, format, list);
		bRet	= ::WriteFile(hFile, str, len * sizeof(TCHAR), &wbytes, NULL);
		delete [] str;
	}
	va_end(list);

	return (bRet == FALSE) ? FALSE : TRUE;
#endif
}

////////////////////////////////////////////////////////////////////////////
//
//	ファイルフラッシュ(SDK)
//
//	BOOL FlushFile(HANDLE hFile)
//
//	入力：	HANDLE			hFile;			ファイルハンドル
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	hFileで指定されたファイルのバッファ内のデータを書き出し、
//			バッファをクリアする。
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::FlushFile(HANDLE hFile)
{
//	ASSERT(hFile != INVALID_HANDLE_VALUE);

	BOOL					bRet;

	bRet	= ::FlushFileBuffers(hFile);
	return (bRet == FALSE) ? FALSE : TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
//	ログ出力関数
//
//	void LogWrite(LPCTSTR str)
//
//	入力：	LPCTSTR			str;			出力する文字列
//
//	出力：	なし
//
//	機能：	strで指定された文字列をログファイルに出力する。
//
/////////////////////////////////////////////////////////////////////////////
void CCmnFile::LogWrite(LPCTSTR str,HANDLE m_hFile)
{
//	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
//	ASSERT(str != NULL);

	SYSTEMTIME				time;
	TCHAR					buffer[64];
	DWORD					len;

	//	日付の作成
	::GetLocalTime(&time);
#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
	::_stprintf(
			buffer,
			_T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond,
			time.wMilliseconds							);
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
	::_stprintf_s(
			buffer,
			sizeof(buffer) / sizeof(TCHAR),
			_T("%04d/%02d/%02d,%02d:%02d:%02d.%03d"),
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond,
			time.wMilliseconds							);
#endif

	//	ログ出力
	len	= static_cast<DWORD>(::_tcslen(str));
	if((str[len - 1] != 0x0D) && (str[len - 1] != 0x0A)) {
		//	改行コードを付加して出力する。
		CCmnFile::WriteFormatString(m_hFile, _T("%s,%s\r\n"), buffer, str);
	} else {
		CCmnFile::WriteFormatString(m_hFile, _T("%s,%s"), buffer, str);
	}

	//	ファイルフラッシュ
	CCmnFile::FlushFile(m_hFile);

	return;
}
/////////////////////////////////////////////////////////////////////////////
//
//	パスの拡張子を変更
//
//	BOOL	RenameExtension(CString &fname, LPCTSTR extension)
//
//	入力：	CString			&fname;			ファイル名称
//			LPCTSTR			extension;		変更後の拡張子(ex. ".txt")
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	fnameで指定されたファイル名の拡張子を、extensionで指定された拡張子に変更する。
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::RenameExtension(CString &fname, LPCTSTR extension)
{
//	ASSERT(extension != NULL);

	TCHAR					path[MAX_PATH + 2];
	BOOL					bRet;

#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
	::_tcscpy(path, fname);
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
	::_tcscpy_s(path, MAX_PATH, fname);
#endif
	bRet	= ::PathRenameExtension(path, extension);
	if(bRet != FALSE) {
		fname	= path;
	}

	return (bRet == FALSE) ? FALSE : TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
//	テンポラリディレクトリ取得
//
//	BOOL	GetTempPath(CString &path)
//
//	入力：	CString			path;			テンポラリディレクトリ名の格納先
//
//	出力：	TRUE			正常終了
//			FALSE			異常終了
//
//	機能：	テンポラリディレクトリを取得し、pathに格納する。
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::GetTempPath(CString &path)
{
	BOOL					bRet	= FALSE;
	DWORD					nLen;
	TCHAR					*mp;

	//	テンポラリディレクトリ名を取得
	nLen	= ::GetTempPath(0, NULL);
	if(nLen > 0) {
		mp	= new TCHAR[nLen + 2];
		memset(mp, 0, sizeof(TCHAR) * (nLen + 1));
		nLen	= ::GetTempPath(nLen + 1, mp);
		path	= mp;
		delete [] mp;
		bRet	= TRUE;
	}

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////
//
//	ディレクトリクリア(※取扱注意)
//
//	BOOL ClearDirectory(LPCTSTR dirname)
//
//	入力：	LPCTSTR			dirname;		クリアディレクトリ名称
//
//	出力：	FALSE			異常終了
//			TRUE			正常終了
//
//	機能：	dirnameで指定されたディレクトリ以下のディレクトリ、ファイルを削除する。
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::ClearDirectory(LPCTSTR dirname)
{
//	ASSERT(dirname != NULL);

	CString					name;
	CString					str;
	HANDLE					hFile;
	WIN32_FIND_DATA			wfd;
	DWORD					status;
	BOOL					bRet = FALSE;;
	BOOL					bStatus	= FALSE;
	LONG					pos;

	CCmnFile::MakeFileName(name, dirname, _T("*.*"));
	hFile	= ::FindFirstFile(name, &wfd);	// 最初のファイルを検索
	if(hFile != INVALID_HANDLE_VALUE) {
		bStatus	= TRUE;
		name	= dirname;
		pos	= name.GetLength();
		if((pos > 0) && (name.GetAt(pos - 1) == '\\')) {
			name.Delete(pos - 1);
		}

		while(1) {
			str.Format(_T("%s\\%s"), (LPCTSTR)name, wfd.cFileName);
			status	= ::GetFileAttributes(str);

			if(status & FILE_ATTRIBUTE_DIRECTORY) {
				if(		(::_tcscmp(wfd.cFileName, _T(".")) != 0)
					&&	(::_tcscmp(wfd.cFileName, _T("..")) != 0)	) {
					//	サブディレクトリ内のファイルを削除
					bRet	= CCmnFile::ClearDirectory(str);
					if(bRet == FALSE) {
						bStatus	= FALSE;
						break;
					}

					//	ディレクトリ削除
					::SetFileAttributes(str, FILE_ATTRIBUTE_NORMAL);
					bRet	= CCmnFile::DeleteDirectory(str);
					if(bRet == FALSE) {
						bStatus	= FALSE;
						break;
					}
				}
			} else {
				//	ファイルの削除
				::SetFileAttributes(str, FILE_ATTRIBUTE_NORMAL);
				bRet	= ::DeleteFile(str);
				if(bRet == FALSE) {
					bStatus	= FALSE;
					break;
				}
			}

			bRet	= ::FindNextFile(hFile, &wfd);	// 次を検索
			if(bRet == FALSE) {
				break;
			}
		}

		bRet	= ::FindClose(hFile);
	}
	if(bRet)
	{
		if(!RemoveDirectory(dirname))
		{
			bStatus = FALSE;
		}
	}
	return bStatus;
}
BOOL CCmnFile::FolderCopy( CString from, CString to )
{  
	int iRet;
	SHFILEOPSTRUCT tSHFile;
	tSHFile.hwnd = ::GetDesktopWindow();  
	tSHFile.wFunc = FO_COPY; 
	tSHFile.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION |FOF_NOERRORUI;  
	tSHFile.fAnyOperationsAborted = TRUE;  
	tSHFile.hNameMappings = NULL;  
	tSHFile.lpszProgressTitle = _T("");  
  
	// パス名のCStringの末尾に\0をつけて設定  
	from += "0";  
	to += "0";  
	from.SetAt( from.GetLength()-1, NULL );  
	to.SetAt( to.GetLength()-1, NULL );  
	tSHFile.pFrom = from;  
	tSHFile.pTo = to;  
  
	
	iRet = SHFileOperation( &tSHFile ); // 実行
	if(iRet != 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}
////////////////////////////////////////////////////////////////////////////
//
//	フォルダ選択ダイアログの表示
//
//	入力：	CWnd			*parent;		親ウィンドウクラスへのポインタ
//			CString			&dirname;		フォルダ名(フルパス)の格納先
//			LPCTSTR			title;			タイトル文字列
//			LPCTSTR			basedir;		デフォルトフォルダのパス名(省略可)
//
//	出力：	FALSE			キャンセルされた。
//			TRUE			正常終了
//
//	機能：	フォルダ選択ダイアログを表示する。
//
//	備考：	本関数呼び出し前に、COMの初期化が必要となる。
//			(CoInitialize関数を参照)
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::SelectFolder(CWnd *parent, CString &dirname, LPCTSTR title, LPCTSTR basedir)
{
//	ASSERT(title != NULL);

	BROWSEINFO				binfo;
	TCHAR					dir[MAX_PATH + 2];
	LPITEMIDLIST			itemListPtr;
	BOOL					bRet	= FALSE;
			
	//	フォルダ選択ダイアログ表示
//		MODIFY_2013_10_AISTAFF
//	binfo.hwndOwner			= parent->m_hWnd;
	if(parent != 0){
		binfo.hwndOwner			= parent->m_hWnd;
	} else {
		binfo.hwndOwner			= 0;
	}
//		MODIFY_2013_10_AISTAFF
	binfo.pidlRoot			= 0;
	binfo.pszDisplayName	= 0;
	binfo.lpszTitle			= title;
//	binfo.ulFlags			= BIF_RETURNONLYFSDIRS;
	binfo.ulFlags			= BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;	//	(新規フォルダ作成ボタンを追加)
	binfo.lpfn				= BrowseCallbackProc;

	//	デフォルトフォルダの設定
	m_DefaultFolder	= new TCHAR[MAX_PATH + 2];
	if(basedir != NULL) {
#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
		::_tcscpy(m_DefaultFolder, basedir);
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
		::_tcscpy_s(m_DefaultFolder, MAX_PATH, basedir);
#endif
	} else {
		::GetCurrentDirectory(MAX_PATH, m_DefaultFolder);
	}

	itemListPtr	= ::SHBrowseForFolder(&binfo);
	if(itemListPtr) {
		//	アイテムリストからパス文字列に変換
		bRet	= ::SHGetPathFromIDList(itemListPtr, dir);
		if(bRet != FALSE) {
			dirname	= dir;
			bRet	= TRUE;
		}

		//	アイテムリストの解放
		::CoTaskMemFree(itemListPtr);
	}

	delete [] m_DefaultFolder;
	m_DefaultFolder	= NULL;

	return bRet;
}
////////////////////////////////////////////////////////////////////////////
//
//	SHBrowseForFolder関数のコールバックルーチン
//
//	入力：	HWND			hWnd;			ウィンドウハンドル
//			UINT			msg;			メッセージＩＤ
//			LPARAM			lparam;			メッセージパラメータ
//			LPARAM			ldata;			アプリケーション定義値
//
//	出力：	０				正常終了
//
//	機能：	デフォルトパスを設定する。
//
////////////////////////////////////////////////////////////////////////////
INT	CALLBACK CCmnFile::BrowseCallbackProc(HWND hWnd, UINT msg, LPARAM /* lparam */, LPARAM /* ldata */)
{
	switch(msg) {
		case BFFM_INITIALIZED:
			//	フォルダ選択のデフォルトパスを設定
			::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)m_DefaultFolder);
			break;

		default:
			break;
	}

	return 0;
}

CString CCmnFile::GetSpecialFolderPath(int nID)
{
	TCHAR SpecialPath[MAX_PATH];
	CString strRet;
	strRet.Format(_T(""));
	if(SHGetSpecialFolderPath(NULL,SpecialPath,nID,FALSE))
	{
		strRet = SpecialPath;
	}
	return strRet;
}