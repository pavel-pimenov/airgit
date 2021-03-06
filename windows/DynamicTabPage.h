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

#ifndef DYNAMIC_TAB_PAGE_H
#define DYNAMIC_TAB_PAGE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlcrack.h>
#include "stdafx.h"

#include "ConfigUtil.h"

class DynamicTabPage : public CDialogImpl<DynamicTabPage> {
public:

	enum { IDD = IDD_TABPAGE };

	DynamicTabPage();
	~DynamicTabPage();

	void resizePage(CRect & windowRect);
	void updateLayout(CRect & windowRect);

	BEGIN_MSG_MAP_EX(DynamicTabPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, onCtlColor)
		//MESSAGE_HANDLER(WM_CTLCOLORDLG, onCtlColor)
		MESSAGE_HANDLER(WM_SETFOCUS, onSetFocus)
		MESSAGE_HANDLER(WM_ERASEBKGND, onEraseBackground)
		MESSAGE_HANDLER(WM_PARENTNOTIFY, OnClick)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT onEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL & /*bHandled*/);
	LRESULT onCtlColor(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);

	LRESULT onSetFocus(UINT /* uMsg */, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/) {
		return 0;
	}

	LRESULT OnClick(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL & bHandled) {

		if (LOWORD(wParam) == WM_LBUTTONDOWN) {
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			HWND hWndCtrl = ChildWindowFromPoint(pt);
			if (hWndCtrl != NULL && hWndCtrl != m_hWnd && hWndCtrl != GetParent().m_hWnd) {
				for (auto cfg : configs) {
					if (cfg->handleClick(hWndCtrl))
						break;
				}
			}
		}
		bHandled = FALSE;
		return 0;
	}


	void addConfigItem(webserver::ExtensionSettingItem& aSetting) {
		auto item = ConfigUtil::getConfigItem(aSetting);
		if(item)
			configs.emplace_back(item);
	}

	bool write() {
	
		try {
			for (auto cfg : configs) {
				cfg->write();
			}
		} catch (const ArgumentException& e) {
			string error = e.getErrorJson().at("field") + " : " + e.getErrorJson().at("message");
			MessageBox(Text::toT(error).c_str());
			return false;
		}
		return true;
	}

	vector<shared_ptr<ConfigUtil::ConfigIem>>& getConfigs() { return configs; }

private:

	bool loading;
	vector<shared_ptr<ConfigUtil::ConfigIem>> configs;

	int m_prevConfigBottomMargin = 0;
	int m_configSpacing = 20;


};


#endif
