#pragma once


/////////////////////////////////////////////////////////////////////////////
//	�萔��`
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//	�t�@�C���I�[�v��(OpenDialog)�^�ۑ��_�C�A���O�̕\��(SaveDialog)�p
//	�t�B���^�������`
#define	CMN_FILE_FILTER_TXT	_T("txt�t�@�C��|*.txt|�S�Ẵt�@�C��|*.*||")	//	�e�L�X�g�t�@�C��
#define	CMN_FILE_FILTER_DAT	_T("dat�t�@�C��|*.dat|�S�Ẵt�@�C��|*.*||")	//	dat�t�@�C��

//	�g���q�������`
#define	CMN_FILE_EXT_TXT	_T("txt")		//	�e�L�X�g�t�@�C��
#define	CMN_FILE_EXT_DAT	_T("dat")		//	dat�t�@�C��
//---------------------------------------------------------------------------
//	�t�H���_�I���_�C�A���O�̕\��(SelectFolder)�p
//	�^�C�g��������
#define	CMN_FILE_TITLE_DIR	_T("�t�H���_��I�����Ă��������B")
//---------------------------------------------------------------------------
//	�t�@�C������(FindFile)�p
//	�^�C�g��������
#define	CMN_FILE_FIND_MODE_FILE	(0)			//	�t�@�C���̂݌���
#define	CMN_FILE_FIND_MODE_DIR	(1)			//	�f�B���N�g���̂݌���
#define	CMN_FILE_FIND_MODE_BOTH	(2)			//	�t�@�C���A�f�B���N�g������������
//---------------------------------------------------------------------------
//	�t�@�C���V�[�N(Seek)�p
//	�J�n�_
#define	CMN_FILE_SEEK_BEGIN		(FILE_BEGIN)	//	�t�@�C���̐擪
#define	CMN_FILE_SEEK_CURRENT	(FILE_CURRENT)	//	���݂̈ʒu
#define	CMN_FILE_SEEK_END		(FILE_END)		//	�t�@�C���̏I�[
//---------------------------------------------------------------------------


class CCmnFile
{
public:
	CCmnFile(void);
	virtual ~CCmnFile(void);
	/////////////////////////////////////////////////////////////////////////////
	//	�t�@�C����(�t���p�X)����p�X���擾
	//	���́F	CString			&dir;			�f�B���N�g�����̊i�[��
	//			LPCTSTR			fname;			�t�@�C������
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL FileNameToPathName(CString &dir, LPCTSTR fname);

	/////////////////////////////////////////////////////////////////////////////
	//	�t���p�X�t�@�C�����̂̍쐬
	//	���́F	CString			&pathname;		�t�@�C������(�t���p�X)�̊i�[��
	//			LPCTSTR			dirname;		�f�B���N�g������
	//			LPCTSTR			fname;			�t�@�C������
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL MakeFileName(CString &pathname, LPCTSTR dirname, LPCTSTR fname);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���̗L���f�f
	//	���́F	LPCTSTR			fname;		�t�@�C������
	//	�o�́F	TRUE			���݂���B
	//			FALSE			���݂��Ȃ��B
	////////////////////////////////////////////////////////////////////////////
	static	BOOL IsFileExist(LPCTSTR fname);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���I�[�v��(�o�C�i���ǂݏ������[�h)(MFC)
	//	���́F	CFile		*file;			CFile�N���X�ւ̃|�C���^
	//			LPCTSTR		fname;			�t�@�C������
	//	�o�́F	FALSE		�ُ�I��
	//			TRUE		����I��
	////////////////////////////////////////////////////////////////////////////
	static	BOOL OpenBinaryRW(CFile *file, LPCTSTR fname);

	/////////////////////////////////////////////////////////////////////////////
	//	�A�v���P�[�V�����f�B���N�g���擾
	//	���́F	HINSTANCE		hInstance;		�A�v���P�[�V�����̃C���X�^���X�n���h��
	//			CString			path;			�A�v���P�[�V�����f�B���N�g���̊i�[��
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL GetApplPath(HINSTANCE hInstance, CString &path);

	////////////////////////////////////////////////////////////////////////////
	//	1�s�ǂݍ���(MFC)
	//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
	//			CString			&str;			�ǂݍ��񂾕�����̊i�[��
	//	�o�́F	TRUE			����I��
	//			FALSE			�ǂݍ��݂��s��Ȃ������B
	////////////////////////////////////////////////////////////////////////////
	static	BOOL ReadString(CFile *file, CString &str);

	////////////////////////////////////////////////////////////////////////////
	//	�f�B���N�g���̗L���f�f
	//	���́F	LPCTSTR			dirname;	�f�B���N�g������
	//	�o�́F	TRUE			���݂���B
	//			FALSE			���݂��Ȃ��B
	////////////////////////////////////////////////////////////////////////////
	static	BOOL IsDirExist(LPCTSTR dirname);

	/////////////////////////////////////////////////////////////////////////////
	//	�f�B���N�g���쐬
	//	���́F	LPCTSTR			dirname;		�쐬����f�B���N�g������
	//	�o�́F	FALSE			�ُ�I��
	//			TRUE			����I��
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL MakeDirectory(LPCTSTR dirname);

	/////////////////////////////////////////////////////////////////////////////
	//	�f�B���N�g���폜
	//	���́F	LPCTSTR			dirname;		�폜����f�B���N�g������
	//	�o�́F	FALSE			�ُ�I��
	//			TRUE			����I��
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL DeleteDirectory(LPCTSTR dirname);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���I�[�v��(�e�L�X�g�����o�����[�h)(MFC)
	//	���́F	CFile		*file;			CFile�N���X�ւ̃|�C���^
	//			LPCTSTR		fname;			�t�@�C������
	//	�o�́F	FALSE		�ُ�I��
	//			TRUE		����I��
	////////////////////////////////////////////////////////////////////////////
	static	BOOL OpenTextW(CFile *file, LPCTSTR fname);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���I�[�v��(�o�C�i�������o�����[�h)(MFC)
	//	���́F	CFile		*file;			CFile�N���X�ւ̃|�C���^
	//			LPCTSTR		fname;			�t�@�C������
	//	�o�́F	FALSE		�ُ�I��
	//			TRUE		����I��
	////////////////////////////////////////////////////////////////////////////
	static	BOOL OpenBinaryW(CFile *file, LPCTSTR fname);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���V�[�N(MFC)
	//	���́F	CFile			*file;			�t�@�C���n���h��
	//			LONGLONG		bytes;			�ړ�����o�C�g��
	//			DWORD			offset;			�J�n�ʒu(CMN_FILE_SEEK_xxx)
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	////////////////////////////////////////////////////////////////////////////
#if	!defined(_MSC_VER) || (_MSC_VER < 1400)		//	for VC6		(VC6	_MSC_VER = 1200)
	static	BOOL Seek(CFile *file, LONG bytes, DWORD offset);
#else											//	for VC2005	(VC2005	_MSC_VER = 1400)
	static	BOOL Seek(CFile *file, LONGLONG bytes, DWORD offset);
#endif

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���N���[�Y(MFC)
	//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
	//	�o�́F	�Ȃ�
	////////////////////////////////////////////////////////////////////////////
	static	void CloseFile(CFile *file);

	////////////////////////////////////////////////////////////////////////////
	//	�����t�������񏑂�����(MFC)
	//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
	//			LPCTSTR			format;			�o�̓t�H�[�}�b�g
	//	�o�́F	TRUE			����I��
	//			FALSE			�ǂݍ��݂��s��Ȃ������B
	////////////////////////////////////////////////////////////////////////////
	static	BOOL WriteFormatString(CFile *file, LPCTSTR format, ...);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���t���b�V��(MFC)
	//	���́F	CFile			*file;			�t�@�C���N���X�ւ̃|�C���^
	//	�o�́F	�Ȃ�
	////////////////////////////////////////////////////////////////////////////
	static	void FlushFile(CFile *file);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���I�[�v��(�o�C�i�������o�����[�h)(SDK)
	//	���́F	LPCTSTR		fname;			�t�@�C������
	//	�o�́F	INVALID_HANDLE_VALUE�ȊO	����I��
	//			INVALID_HANDLE_VALUE		�ُ�I��
	////////////////////////////////////////////////////////////////////////////
	static	HANDLE OpenBinaryRW(LPCTSTR fname);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���V�[�N(SDK)
	//	���́F	HANDLE			hFile;			�t�@�C���n���h��
	//			LONGLONG		bytes;			�ړ�����o�C�g��
	//			DWORD			offset;			�J�n�ʒu(CMN_FILE_SEEK_xxx)
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	////////////////////////////////////////////////////////////////////////////
	static	BOOL Seek(HANDLE hFile, LONGLONG bytes, DWORD offset);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���N���[�Y(SDK)
	//	���́F	HANDLE			hFile;			�t�@�C���n���h��
	//	�o�́F	�Ȃ�
	////////////////////////////////////////////////////////////////////////////
	static	void CloseFile(HANDLE hFile);

	////////////////////////////////////////////////////////////////////////////
	//	�����t�������񏑂�����(SDK)
	//	���́F	HANDLE			hFile;			�t�@�C���n���h��
	//			LPCTSTR			format;			�o�̓t�H�[�}�b�g
	//	�o�́F	TRUE			����I��
	//			FALSE			�ǂݍ��݂��s��Ȃ������B
	////////////////////////////////////////////////////////////////////////////
	static	BOOL WriteFormatString(HANDLE hFile, LPCTSTR format, ...);

	////////////////////////////////////////////////////////////////////////////
	//	�t�@�C���t���b�V��(SDK)
	//	���́F	HANDLE			hFile;			�t�@�C���n���h��
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	////////////////////////////////////////////////////////////////////////////
	static	BOOL FlushFile(HANDLE hFile);

	/////////////////////////////////////////////////////////////////////////////
	//	���O�o�͊֐�
	//	���́F	LPCTSTR			str;			�o�͂��镶����
	//	�o�́F	�Ȃ�
	/////////////////////////////////////////////////////////////////////////////
	static void LogWrite(LPCTSTR str,HANDLE m_hFile);

	/////////////////////////////////////////////////////////////////////////////
	//	�p�X�̊g���q��ύX
	//	���́F	CString			&fname;			�t�@�C������
	//			LPCTSTR			extension;		�ύX��̊g���q
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL RenameExtension(CString &fname, LPCTSTR extension);

	/////////////////////////////////////////////////////////////////////////////
	//	�e���|�����f�B���N�g���擾
	//	���́F	CString			path;			�e���|�����f�B���N�g�����̊i�[��
	//	�o�́F	TRUE			����I��
	//			FALSE			�ُ�I��
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL GetTempPath(CString &path);

	/////////////////////////////////////////////////////////////////////////////
	//	�f�B���N�g���N���A(���戵����)
	//	���́F	LPCTSTR			dirname;		�N���A�f�B���N�g������
	//	�o�́F	FALSE			�ُ�I��
	//			TRUE			����I��
	//	���l�F	�w��f�B���N�g���ȉ��̃t�@�C���A�f�B���N�g����S�č폜���邽�߁A
	//			���ӂ��Ďg�p���邱�ƁI�I
	/////////////////////////////////////////////////////////////////////////////
	static	BOOL ClearDirectory(LPCTSTR dirname);

	/////////////////////////////////////////////////////////////////////////////
	//	�f�B���N�g���R�s�[(���戵����)
	//	���́F	CString			from;		�R�s�[���t�H���_Path
	//	���́F	CString			to  ;		�R�s�[��t�H���_Path
	//	�o�́F	FALSE			�ُ�I��
	//			TRUE			����I��
	//	���l�F	
	//			
	/////////////////////////////////////////////////////////////////////////////
	static BOOL FolderCopy( CString from, CString to );

	////////////////////////////////////////////////////////////////////////////
	//	�t�H���_�I���_�C�A���O�̕\��
	//	���́F	CWnd		*parent;		�e�E�B���h�E�N���X�ւ̃|�C���^
	//			CString		&dirname;		�t�H���_��(�t���p�X)�̊i�[��
	//			LPCTSTR		title;			�^�C�g��������
	//			LPCTSTR		basedir;		�f�t�H���g�t�H���_�̃p�X��(�ȗ���)
	//	�o�́F	FALSE		�L�����Z�����ꂽ�B
	//			TRUE		����I��
	////////////////////////////////////////////////////////////////////////////
	static	BOOL SelectFolder(CWnd *parent,  CString &dirname, LPCTSTR title, LPCTSTR basedir = NULL);

	////////////////////////////////////////////////////////////////////////////
	//	����t�H���_��PATH���擾����
	//	�o�́F	�f�X�N�g�b�v��PATH			����
	//			_T("")						���s
	////////////////////////////////////////////////////////////////////////////
	static CString GetSpecialFolderPath(int nID);
private:
	//	SHBrowseForFolder�֐��̃R�[���o�b�N���[�`��
	static	INT	CALLBACK BrowseCallbackProc(HWND hWnd, UINT msg, LPARAM lparam, LPARAM ldata);

private:
	//	�f�t�H���g�t�H���_	SelectFolder�֐��p
	static	TCHAR	*m_DefaultFolder;
};