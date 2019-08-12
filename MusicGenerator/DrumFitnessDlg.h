/***********************************************************************
Author:		John Pinkerton
File:		DrumFitnessDlg.h
Purpose:	Header file for the Windows Dialg that sets parameters for
			fitness function of the DrumKitIntrument.
***********************************************************************/

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DrumFitnessDlg dialog

class DrumFitnessDlg : public CDialog
{
	DECLARE_DYNAMIC(DrumFitnessDlg)

public:
	DrumFitnessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DrumFitnessDlg();
	CSliderCtrl kickOnBeatSlider;
	CSliderCtrl snareOnBeatSlider;
	CSliderCtrl numCymbNoteSlider;
	CSliderCtrl numPercNoteSlider;
	CSliderCtrl accSnareCnt;
	CSliderCtrl accentKickCnt;
	CSliderCtrl accentClosedHHCnt;

// Dialog Data
	enum { IDD = IDD_DRUMFITNESSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
