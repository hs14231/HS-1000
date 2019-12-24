
// FileCreater.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CFileCreaterApp:
// このクラスの実装については、FileCreater.cpp を参照してください。
//

class CFileCreaterApp : public CWinApp
{
public:
	CFileCreaterApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CFileCreaterApp theApp;