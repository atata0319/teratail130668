
// GnuplotTest.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CGnuplotTestApp:
// このクラスの実装については、GnuplotTest.cpp を参照してください。
//

class CGnuplotTestApp : public CWinApp
{
public:
	CGnuplotTestApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CGnuplotTestApp theApp;
