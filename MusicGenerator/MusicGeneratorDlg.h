/***********************************************************************
Author:		John Pinkerton
File:		MusicGeneratorDlg.h
Purpose:	The main windows form that contains the UI controls.
***********************************************************************/

#pragma once
#include "afxwin.h"
#include "GA.h"
#include "afxcmn.h"
#include "DrumFitnessDlg.h"
#include "BassFitnessDlg.h"
#include "KitInstrument.h"
#include "BassInstrument.h"


// CMusicGeneratorDlg dialog
class CMusicGeneratorDlg : public CDialog
{
// Construction
public:
	CMusicGeneratorDlg(CWnd* pParent = NULL);	// standard constructor
	// kill events for text boxes
	afx_msg void OnEnKillfocusPopsize();
	afx_msg void OnEnKillfocusCrossrate();
	afx_msg void OnEnKillfocusMutrate();
	afx_msg void OnEnKillfocusResolution();
	// fitness dialogs
	DrumFitnessDlg drumFitDlg;
	BassFitnessDlg bassFitDlg;
	// button click and tab events
	afx_msg void OnBnClickedBrestart();
	afx_msg void OnBnClickedBrun();
	afx_msg void OnBnClickedBexit();
	afx_msg void OnBnClickedBexport();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);


// Dialog Data
	enum { IDD = IDD_MUSICGENERATOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// windows controls
	CEdit mPopSizeEdit;
	CEdit mCrossoverEdit;
	CEdit mMutationEdit;
	CEdit mResolutionText;
	CButton mStartButton;
	CEdit mLogWin;
	CTabCtrl mTabCtrl;
	// the ga
	GA *ga;
	// run the ga
	void RunGA();
	// is the ga running?
	bool isRunning;
	// build a kit instrument
	KitInstrument* BuildKit(void);
	// update the instrument parameters from their dialogs
	void UpdateInstrumentParams(void);
	// display the specified fitness dialog
	void ShowFitnessDlg(int dlg);
	// Build a bass instrument
	BassInstrument* BuildBass(void);
	// build an instrument defined by the instrument tab control
	InstrumentType* BuildInstrument(void);
	// update the parameter values for the ga
	void UpdateGAParameters(void);
	bool includeDifference ;
public:
	CButton m_difference;
};
