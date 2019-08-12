// BassFitnessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MusicGenerator.h"
#include "BassFitnessDlg.h"


// BassFitnessDlg dialog

IMPLEMENT_DYNAMIC(BassFitnessDlg, CDialog)
BassFitnessDlg::BassFitnessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(BassFitnessDlg::IDD, pParent)
	, isMajor(FALSE)
{
}

BassFitnessDlg::~BassFitnessDlg()
{
}

void BassFitnessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NUMNOTESSLIDER, numNotesSlider);
	DDX_Control(pDX, IDC_CONSSLIDER, consonanceSlider);
	DDX_Control(pDX, IDC_DISSLIDER, disonanceSlider);
	DDX_Check(pDX, IDC_MAJORCHECK, isMajor);
	DDX_Control(pDX, IDC_EVENACCENTSLIDER, evenAccentSlider);
	DDX_Control(pDX, IDC_ODDACCENTSLIDER, oddAccentSlider);
}


BEGIN_MESSAGE_MAP(BassFitnessDlg, CDialog)
END_MESSAGE_MAP()


// BassFitnessDlg message handlers

BOOL BassFitnessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->numNotesSlider.SetRange(0, 99);
	this->consonanceSlider.SetRange(0, 99);
	this->disonanceSlider.SetRange(0, 99);
	this->evenAccentSlider.SetRange(0, 99);
	this->oddAccentSlider.SetRange(0, 99);
	return TRUE;
}