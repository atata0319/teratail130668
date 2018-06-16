
// GnuplotTestDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "GnuplotTest.h"
#include "GnuplotTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGnuplotTestDlg �_�C�A���O



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


// CGnuplotTestDlg ���b�Z�[�W �n���h���[

BOOL CGnuplotTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CGnuplotTestDlg::OnPaint()
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

	// �W�����͂̃��_�C���N�g����
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	// �R���\�[����ʂȂ��� gnuplot.exe �����s
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hread;
	if (!CreateProcess(L"C:\\gnuplot\\bin\\gnuplot.exe", NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return;
	}
	CloseHandle(hread);

	// �R�}���h�𑗐M
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

	// �o���オ�����C���[�W��ǂݍ���
	CImage image;
	image.Load(L"test.png");

	// �s�N�`���[�R���g���[���ɕ\��
	CStatic* picture = reinterpret_cast<CStatic*>(GetDlgItem(IDC_PICTURE));
	picture->SetBitmap(image);

	// �s�v�t�@�C�����폜
	DeleteFile(L"test.png");
}