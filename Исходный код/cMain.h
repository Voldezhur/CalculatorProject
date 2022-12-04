#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	// textbox for calculator output
	wxTextCtrl* m_txt1 = nullptr;

	// grid of numbers
	int nFieldWidth = 4;
	int nFieldHeight = 4;
	wxButton** btn;
	int* nField = nullptr;
	bool bFirstClick = true;

	void onButtonClicked(wxCommandEvent& evt);
	void onOperandButtonClicked(wxCommandEvent& evt);
	void onClearButtonClicked(wxCommandEvent& evt);
	void onEnterButtonClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};