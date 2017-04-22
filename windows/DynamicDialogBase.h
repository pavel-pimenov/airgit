#pragma once
#pragma once
/*
* Copyright (C) 2011-2017 AirDC++ Project
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef DYNAMIC_DIALOG_BASE_H
#define DYNAMIC_DIALOG_BASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlcrack.h>
#include "stdafx.h"

#include "DynamicTabPage.h"
#include <airdcpp/Util.h>


class DynamicDialogBase : public CDialogImpl<DynamicDialogBase> {
public:

	enum { IDD = IDD_DYNAMIC_DIALOG };

	DynamicDialogBase(const string& aName) : name(aName) {
		//Create a dummy page to fill in dialog items
		page = make_shared<DynamicTabPage>(DynamicTabPage());
	}
	~DynamicDialogBase() {

	}

	BEGIN_MSG_MAP_EX(DynamicDialogBase)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_VSCROLL, onScroll)
		MESSAGE_HANDLER(WM_SETFOCUS, onSetFocus)
		MESSAGE_HANDLER(WM_SIZE, onSize)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {

		page->Create(m_hWnd);

		// save the original size
		GetClientRect(&rcOriginalRect);

		updateLayout();

		CenterWindow(GetParent());
		SetWindowText(Text::toT(name).c_str());

		return TRUE;
	}

	LRESULT onScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
		int nDelta = 0;
		int nPos = HIWORD(wParam);

		switch (LOWORD(wParam)) {
		case SB_THUMBTRACK:
			nDelta = nPos - curScrollPos;
			break;
		case SB_THUMBPOSITION:
			nDelta = nPos - curScrollPos;
			break;

		default:
			return 0;
		}

		curScrollPos += nDelta;
		SetScrollPos(SB_VERT, curScrollPos, FALSE);
		ScrollWindowEx(0, -nDelta, &scrollRect, &clipRect, NULL, NULL, SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
		UpdateWindow();
		bHandled = TRUE;
		return 0;
	}

	LRESULT onSetFocus(UINT /* uMsg */, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/) {
		return 0;
	}

	LRESULT onSize(UINT /* uMsg */, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/) {
		updateLayout();
		return 0;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (wID == IDOK) {
			page->write();
		}
		EndDialog(wID);
		return 0;
	}

	void updateLayout() {
		CRect OKbtnRect;
		GetDlgItem(IDOK).GetClientRect(&OKbtnRect);

		CRect baseRect;
		GetClientRect(&baseRect); // current base dialog rect

		CRect pageRect; 
		page->GetClientRect(&pageRect);
		pageRect.top += pageSpacing;
		pageRect.right = baseRect.right - (OKbtnRect.Width() + 30); //OK button left
		pageRect.left = baseRect.left + pageSpacing;
		page->MoveWindow(pageRect);

		page->updateLayout();
		page->GetClientRect(&pageRect); //page might have resized, get the rect again...

		//Set the scrolling area, exclude OK button area by button left edge
		scrollRect = pageRect;
		scrollRect.top = -(pageRect.bottom);

		clipRect.left = pageRect.left;
		clipRect.top = 0;
		clipRect.right = pageRect.right;
		clipRect.bottom = pageRect.bottom;

		//Set scrollbar dimensions.
		SCROLLINFO si = { 0 };
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(SB_VERT, &si);
		si.nMax = scrollRect.bottom;
		si.nPage = baseRect.bottom;
		SetScrollInfo(SB_VERT, &si);

	}

	shared_ptr<DynamicTabPage> getPage() { return page; }

private:

	// dialog size in the resource view (original size)
	CRect rcOriginalRect;

	// actual scroll position
	int curScrollPos = 0;

	int pageSpacing = 10;

	CEdit ctrlEdit;

	string name;
	bool loading;
	CRect scrollRect;
	CRect clipRect;

	shared_ptr<DynamicTabPage> page;

};
#endif