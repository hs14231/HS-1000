#include "StdAfx.h"
#include "CmnFile.h"
#include <locale.h>

/////////////////////////////////////////////////////////////////////////////
//	�ϐ���`
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//	�f�t�H���g�t�H���_	SelectFolder�֐��p
TCHAR *				CCmnFile::m_DefaultFolder	= NULL;
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//
//	�R���X�g���N�^
//
//	CCmnFile()
//
//	���́F	�Ȃ�
//
//	�o�́F	�Ȃ�
//
//	�@�\�F	���������s���B
//
/////////////////////////////////////////////////////////////////////////////
CCmnFile::CCmnFile(void)
{
}


/////////////////////////////////////////////////////////////////////////////
//
//	�f�X�g���N�^
//
//	~CCmnFile()
//
//	���́F	�Ȃ�
//
//	�o�́F	�Ȃ�
//
//	�@�\�F	�㏈�����s���B
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
	//	�p�X���𒊏o
	bRet	= ::PathRemoveFileSpec(path);
	dir	= path;

	return (bRet == FALSE) ? FALSE : TRUE;
}
	/////////////////////////////////////////////////////////////////////////////
	//	�A�v���P�[�V�����f�B���N�g���擾
	//	���́F	HINSTANCE		hInstance;		�A�v���P�[�V�����̃C���X�^���X�n���h��
	//			CString			path;			�A�v���P�[�V�����f�B���N�g���̊i�[��
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::GetApplPath(HINSTANCE hInstance, CString &path)
{
	TCHAR					appl_path[MAX_PATH + 2];
	BOOL					bRet;

	//	���W���[�����̂��擾
	::GetModuleFileName(hInstance, appl_path, MAX_PATH);

	//	�p�X���𒊏o
	bRet	= CCmnFile::FileNameToPathName(path, appl_path);

	return bRet;
}
////////////////////////////////////////////////////////////////////////////
//
//	1�s�ǂݍ���(MFC)
//
//	BOOL ReadString(CFile *file, CString &str)
//
//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
//			CString			&str;			�ǂݍ��񂾕�����̊i�[��
//
//	�o�́F	TRUE			����I��
//			FALSE			�ǂݍ��݂��s��Ȃ������B
//
//	�@�\�F	file�Ŏw�肳�ꂽ�t�@�C������A1�s���̕������ǂݍ���str�Ɋi�[����B
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

	//	�ǂݍ��ݐ���N���A����B
	str.Empty();

	for(loop = 0; ; loop++) {
		rbytes	= file->Read(&data, sizeof(TCHAR));
		if(rbytes != sizeof(TCHAR)) {
			if(loop == 0) {
				//	1�o�C�g���ǂݍ��߂��ɃG���[�����������ꍇ�́A�G���[�Ƃ���B
				bRet	= FALSE;
			}
			break;
		}
		if(data == 0x0D) {				//	CR or CR/LF
			rbytes	= file->Read(&data, sizeof(TCHAR));
			if(data != 0x0A) {
				//	LF�ł͂Ȃ��̂łP�������߂�
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
//	�f�B���N�g���̗L���f�f
//
//	BOOL IsDirExist(LPCTSTR dirname)
//
//	���́F	LPCTSTR			dirname;		�f�B���N�g������
//
//	�o�́F	TRUE			���݂���B
//			FALSE			���݂��Ȃ��B
//
//	�@�\�F	dirname�Ŏw�肳�ꂽ�f�B���N�g�������݂��邩�f�f����B
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
//	�f�B���N�g���쐬
//
//	BOOL MakeDirectory(LPCTSTR dirname)
//
//	���́F	LPCTSTR			dirname;		�쐬����f�B���N�g������
//
//	�o�́F	FALSE			�ُ�I��
//			TRUE			����I��
//
//	�@�\�F	dirname�Ŏw�肳�ꂽ�f�B���N�g�����쐬����B
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::MakeDirectory(LPCTSTR dirname)
{
//	ASSERT(dirname != NULL);

	BOOL					bRet;
	CString					dir;

	//	�e�f�B���N�g���̑��݃`�F�b�N
	bRet	= CCmnFile::FileNameToPathName(dir, dirname);
	if(bRet != FALSE) {
		if(IsDirExist(dir) == FALSE) {
			//	�e�f�B���N�g�������݂��Ȃ�
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
//	�f�B���N�g���폜
//
//	BOOL DeleteDirectory(LPCTSTR dirname)
//
//	���́F	LPCTSTR			dirname;		�폜����f�B���N�g������
//
//	�o�́F	FALSE			�ُ�I��
//			TRUE			����I��
//
//	�@�\�F	dirname�Ŏw�肳�ꂽ�f�B���N�g�����폜����B
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
//	�t���p�X�t�@�C�����̂̍쐬
//
//	BOOL	MakeFileName(CString &pathname, LPCTSTR dirname, LPCTSTR fname)
//
//	���́F	CString			&pathname;		�t�@�C������(�t���p�X)�̊i�[��
//			LPCTSTR			dirname;		�f�B���N�g������
//			LPCTSTR			fname;			�t�@�C������
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	dirname�Afname����t�@�C������(�t���p�X)���쐬���Apathname�Ɋi�[����B
//
//	���l�F	dirname�Afname�̓��e�ɂ��ȉ��̂悤�ɓ��삷��B
//				dirname = C:\QWE 	fname = qwe.txt		��	pathname = C:\QWE\qwe.txt
//				dirname = C:\QWE	fname = ..\qwe.txt	��	pathname = C:\qwe.txt
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
//	�t�@�C���̗L���f�f
//
//	BOOL IsFileExist(LPCTSTR fname)
//
//	���́F	LPCTSTR			fname;			�t�@�C������
//
//	�o�́F	TRUE			���݂���B
//			FALSE			���݂��Ȃ��B
//
//	�@�\�F	fname�Ŏw�肳�ꂽ�t�@�C�������݂��邩�f�f����B
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
//	�t�@�C���I�[�v��(�o�C�i���ǂݏ������[�h)
//
//	BOOL OpenBinaryRW(CFile *file, LPCTSTR fname)
//
//	���́F	CFile			*file;			CFile�N���X�ւ̃|�C���^
//			LPCTSTR			fname;			�t�@�C������
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	fname�Ŏw�肳�ꂽ�t�@�C�����A�o�C�i���ǂݏ������[�h�ŃI�[�v������B
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
//	�t�@�C���I�[�v��(�e�L�X�g�����o�����[�h)(MFC)
//
//	BOOL OpenTextW(CFile *file, LPCTSTR fname)
//
//	���́F	CFile			*file;			CFile�N���X�ւ̃|�C���^
//			LPCTSTR			fname;			�t�@�C������
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	fname�Ŏw�肳�ꂽ�t�@�C�����A�e�L�X�g�����o�����[�h�ŃI�[�v������B
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
//	�t�@�C���I�[�v��(�o�C�i�������o�����[�h)(MFC)
//
//	BOOL OpenBinaryW(CFile *file, LPCTSTR fname)
//
//	���́F	CFile			*file;			CFile�N���X�ւ̃|�C���^
//			LPCTSTR			fname;			�t�@�C������
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	fname�Ŏw�肳�ꂽ�t�@�C�����A�o�C�i�������o�����[�h�ŃI�[�v������B
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
//	�t�@�C���V�[�N(MFC)
//
//	BOOL Seek(CFile *file, LONGLONG bytes, DWORD offset)
//
//	���́F	CFile			*file;			�t�@�C���n���h��
//			LONGLONG		bytes;			�ړ�����o�C�g��
//			DWORD			offset;			�J�n�ʒu(CMN_FILE_SEEK_xxx)
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	file�Ŏw�肳�ꂽ�t�@�C���̃t�@�C���|�C���^��
//			bytes�Aoffset�Ŏw�肳�ꂽ�ʒu�ɐݒ肷��B
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
//	�t�@�C���N���[�Y(MFC)
//
//	void CloseFile(CFile *file)
//
//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
//
//	�o�́F	�Ȃ�
//
//	�@�\�F	file�Ŏw�肳�ꂽ�t�@�C���̃N���[�Y���s���B
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
//	�����t�������񏑂�����(MFC)
//
//	BOOL WriteFormatString(CFile *file, LPCTSTR format, ...)
//
//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
//			LPCTSTR			format;			�o�̓t�H�[�}�b�g
//
//	�o�́F	TRUE			����I��
//			FALSE			�ǂݍ��݂��s��Ȃ������B
//
//	�@�\�F	file�Ŏw�肳�ꂽ�t�@�C���ɁAformat�Ŏw�肳�ꂽ�����ɏ]���A
//			������o�͂��s���B
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
//	�t�@�C���t���b�V��(MFC)
//
//	void FlushFile(CFile *file)
//
//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
//
//	�o�́F	�Ȃ�
//
//	�@�\�F	file�Ŏw�肳�ꂽ�t�@�C���̃o�b�t�@���̃f�[�^�������o���A
//			�o�b�t�@���N���A����B
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
//	�t�@�C���I�[�v��(�o�C�i�������o�����[�h)(SDK)
//
//	HANDLE OpenBinaryRW(LPCTSTR fname)
//
//	���́F	LPCTSTR			fname;			�t�@�C������
//
//	�o�́F	INVALID_HANDLE_VALUE�ȊO	����I��
//			INVALID_HANDLE_VALUE		�ُ�I��
//
//	�@�\�F	fname�Ŏw�肳�ꂽ�t�@�C�����A�o�C�i���ǂݏ������[�h�ŃI�[�v������B
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
//	�t�@�C���N���[�Y(SDK)
//
//	void CloseFile(HANDLE hFile)
//
//	���́F	HANDLE			hFile;			�t�@�C���n���h��
//
//	�o�́F	�Ȃ�
//
//	�@�\�F	hFile�Ŏw�肳�ꂽ�t�@�C���̃N���[�Y���s���B
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
//	�����t�������񏑂�����(SDK)
//
//	BOOL WriteFormatString(HANDLE hFile, LPCTSTR format, ...)
//
//	���́F	HANDLE			hFile;			�t�@�C���n���h��
//			LPCTSTR			format;			�o�̓t�H�[�}�b�g
//
//	�o�́F	TRUE			����I��
//			FALSE			�ǂݍ��݂��s��Ȃ������B
//
//	�@�\�F	file�Ŏw�肳�ꂽ�t�@�C���ɁAformat�Ŏw�肳�ꂽ�����ɏ]���A
//			������o�͂��s���B
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
//	�t�@�C���t���b�V��(SDK)
//
//	BOOL FlushFile(HANDLE hFile)
//
//	���́F	HANDLE			hFile;			�t�@�C���n���h��
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	hFile�Ŏw�肳�ꂽ�t�@�C���̃o�b�t�@���̃f�[�^�������o���A
//			�o�b�t�@���N���A����B
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
//	���O�o�͊֐�
//
//	void LogWrite(LPCTSTR str)
//
//	���́F	LPCTSTR			str;			�o�͂��镶����
//
//	�o�́F	�Ȃ�
//
//	�@�\�F	str�Ŏw�肳�ꂽ����������O�t�@�C���ɏo�͂���B
//
/////////////////////////////////////////////////////////////////////////////
void CCmnFile::LogWrite(LPCTSTR str,HANDLE m_hFile)
{
//	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
//	ASSERT(str != NULL);

	SYSTEMTIME				time;
	TCHAR					buffer[64];
	DWORD					len;

	//	���t�̍쐬
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

	//	���O�o��
	len	= static_cast<DWORD>(::_tcslen(str));
	if((str[len - 1] != 0x0D) && (str[len - 1] != 0x0A)) {
		//	���s�R�[�h��t�����ďo�͂���B
		CCmnFile::WriteFormatString(m_hFile, _T("%s,%s\r\n"), buffer, str);
	} else {
		CCmnFile::WriteFormatString(m_hFile, _T("%s,%s"), buffer, str);
	}

	//	�t�@�C���t���b�V��
	CCmnFile::FlushFile(m_hFile);

	return;
}
/////////////////////////////////////////////////////////////////////////////
//
//	�p�X�̊g���q��ύX
//
//	BOOL	RenameExtension(CString &fname, LPCTSTR extension)
//
//	���́F	CString			&fname;			�t�@�C������
//			LPCTSTR			extension;		�ύX��̊g���q(ex. ".txt")
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	fname�Ŏw�肳�ꂽ�t�@�C�����̊g���q���Aextension�Ŏw�肳�ꂽ�g���q�ɕύX����B
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
//	�e���|�����f�B���N�g���擾
//
//	BOOL	GetTempPath(CString &path)
//
//	���́F	CString			path;			�e���|�����f�B���N�g�����̊i�[��
//
//	�o�́F	TRUE			����I��
//			FALSE			�ُ�I��
//
//	�@�\�F	�e���|�����f�B���N�g�����擾���Apath�Ɋi�[����B
//
/////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::GetTempPath(CString &path)
{
	BOOL					bRet	= FALSE;
	DWORD					nLen;
	TCHAR					*mp;

	//	�e���|�����f�B���N�g�������擾
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
//	�f�B���N�g���N���A(���戵����)
//
//	BOOL ClearDirectory(LPCTSTR dirname)
//
//	���́F	LPCTSTR			dirname;		�N���A�f�B���N�g������
//
//	�o�́F	FALSE			�ُ�I��
//			TRUE			����I��
//
//	�@�\�F	dirname�Ŏw�肳�ꂽ�f�B���N�g���ȉ��̃f�B���N�g���A�t�@�C�����폜����B
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
	hFile	= ::FindFirstFile(name, &wfd);	// �ŏ��̃t�@�C��������
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
					//	�T�u�f�B���N�g�����̃t�@�C�����폜
					bRet	= CCmnFile::ClearDirectory(str);
					if(bRet == FALSE) {
						bStatus	= FALSE;
						break;
					}

					//	�f�B���N�g���폜
					::SetFileAttributes(str, FILE_ATTRIBUTE_NORMAL);
					bRet	= CCmnFile::DeleteDirectory(str);
					if(bRet == FALSE) {
						bStatus	= FALSE;
						break;
					}
				}
			} else {
				//	�t�@�C���̍폜
				::SetFileAttributes(str, FILE_ATTRIBUTE_NORMAL);
				bRet	= ::DeleteFile(str);
				if(bRet == FALSE) {
					bStatus	= FALSE;
					break;
				}
			}

			bRet	= ::FindNextFile(hFile, &wfd);	// ��������
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
  
	// �p�X����CString�̖�����\0�����Đݒ�  
	from += "0";  
	to += "0";  
	from.SetAt( from.GetLength()-1, NULL );  
	to.SetAt( to.GetLength()-1, NULL );  
	tSHFile.pFrom = from;  
	tSHFile.pTo = to;  
  
	
	iRet = SHFileOperation( &tSHFile ); // ���s
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
//	�t�H���_�I���_�C�A���O�̕\��
//
//	���́F	CWnd			*parent;		�e�E�B���h�E�N���X�ւ̃|�C���^
//			CString			&dirname;		�t�H���_��(�t���p�X)�̊i�[��
//			LPCTSTR			title;			�^�C�g��������
//			LPCTSTR			basedir;		�f�t�H���g�t�H���_�̃p�X��(�ȗ���)
//
//	�o�́F	FALSE			�L�����Z�����ꂽ�B
//			TRUE			����I��
//
//	�@�\�F	�t�H���_�I���_�C�A���O��\������B
//
//	���l�F	�{�֐��Ăяo���O�ɁACOM�̏��������K�v�ƂȂ�B
//			(CoInitialize�֐����Q��)
//
////////////////////////////////////////////////////////////////////////////
BOOL CCmnFile::SelectFolder(CWnd *parent, CString &dirname, LPCTSTR title, LPCTSTR basedir)
{
//	ASSERT(title != NULL);

	BROWSEINFO				binfo;
	TCHAR					dir[MAX_PATH + 2];
	LPITEMIDLIST			itemListPtr;
	BOOL					bRet	= FALSE;
			
	//	�t�H���_�I���_�C�A���O�\��
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
	binfo.ulFlags			= BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;	//	(�V�K�t�H���_�쐬�{�^����ǉ�)
	binfo.lpfn				= BrowseCallbackProc;

	//	�f�t�H���g�t�H���_�̐ݒ�
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
		//	�A�C�e�����X�g����p�X������ɕϊ�
		bRet	= ::SHGetPathFromIDList(itemListPtr, dir);
		if(bRet != FALSE) {
			dirname	= dir;
			bRet	= TRUE;
		}

		//	�A�C�e�����X�g�̉��
		::CoTaskMemFree(itemListPtr);
	}

	delete [] m_DefaultFolder;
	m_DefaultFolder	= NULL;

	return bRet;
}
////////////////////////////////////////////////////////////////////////////
//
//	SHBrowseForFolder�֐��̃R�[���o�b�N���[�`��
//
//	���́F	HWND			hWnd;			�E�B���h�E�n���h��
//			UINT			msg;			���b�Z�[�W�h�c
//			LPARAM			lparam;			���b�Z�[�W�p�����[�^
//			LPARAM			ldata;			�A�v���P�[�V������`�l
//
//	�o�́F	�O				����I��
//
//	�@�\�F	�f�t�H���g�p�X��ݒ肷��B
//
////////////////////////////////////////////////////////////////////////////
INT	CALLBACK CCmnFile::BrowseCallbackProc(HWND hWnd, UINT msg, LPARAM /* lparam */, LPARAM /* ldata */)
{
	switch(msg) {
		case BFFM_INITIALIZED:
			//	�t�H���_�I���̃f�t�H���g�p�X��ݒ�
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