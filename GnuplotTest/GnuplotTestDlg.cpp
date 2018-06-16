
// GnuplotTestDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "GnuplotTest.h"
#include "GnuplotTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGnuplotTestDlg ダイアログ



CGnuplotTestDlg::CGnuplotTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GNUPLOTTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGnuplotTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGnuplotTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGnuplotTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGnuplotTestDlg メッセージ ハンドラー

BOOL CGnuplotTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CGnuplotTestDlg::OnPaint()
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
HCURSOR CGnuplotTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGnuplotTestDlg::OnBnClickedButton1()
{
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
	HANDLE hreadtmp, hread;
	CFile stdinput;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	CStringA command;

	// 標準入力のリダイレクト準備
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	// コンソール画面なしで gnuplot.exe を実行
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hread;
	if (!CreateProcess(L"C:\\gnuplot\\bin\\gnuplot.exe", NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return;
	}
	CloseHandle(hread);

	// コマンドを送信
	command = "set terminal pngcairo\n";
	stdinput.Write(command, command.GetLength());
	command = "plot sin(x)\n";
	stdinput.Write(command, command.GetLength());
	command = "replot cos(x)\n";
	stdinput.Write(command, command.GetLength());
	command = "set out 'test.png'\n";
	stdinput.Write(command, command.GetLength());
	command = "replot\n";
	stdinput.Write(command, command.GetLength());
	command = "exit\n";
	stdinput.Write(command, command.GetLength());

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	// 出来上がったイメージを読み込む
	CImage image;
	image.Load(L"test.png");

	// ピクチャーコントロールに表示
	CStatic* picture = reinterpret_cast<CStatic*>(GetDlgItem(IDC_PICTURE));
	picture->SetBitmap(image);

	// 不要ファイルを削除
	DeleteFile(L"test.png");
}
