// DrumFitnessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MusicGenerator.h"
#include "DrumFitnessDlg.h"


// DrumFitnessDlg dialog

IMPLEMENT_DYNAMIC(DrumFitnessDlg, CDialog)
DrumFitnessDlg::DrumFitnessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DrumFitnessDlg::IDD, pParent)
{
}

DrumFitnessDlg::~DrumFitnessDlg()
{
}

void DrumFitnessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, kickOnBeatSlider);
	DDX_Control(pDX, IDC_SLIDER2, snareOnBeatSlider);
	DDX_Control(pDX, IDC_SLIDER3, numCymbNoteSlider);
	DDX_Control(pDX, IDC_SLIDER4, numPercNoteSlider);
	DDX_Control(pDX, IDC_SLIDER5, accSnareCnt);
	DDX_Control(pDX, IDC_SLIDER6, accentKickCnt);
	DDX_Control(pDX, IDC_SLIDER7, accentClosedHHCnt);
}


BEGIN_MESSAGE_MAP(DrumFitnessDlg, CDialog)

END_MESSAGE_MAP()


// DrumFitnessDlg message handlers

BOOL DrumFitnessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->numCymbNoteSlider.SetRange(0, 99);
	this->numPercNoteSlider.SetRange(0, 99);
	this->kickOnBeatSlider.SetRange(0, 99);
	this->snareOnBeatSlider.SetRange(0, 99);
	this->accSnareCnt.SetRange(0, 99);
	this->accentKickCnt.SetRange(0,99);
	this->accentClosedHHCnt.SetRange(0,99);
	return TRUE;
}

