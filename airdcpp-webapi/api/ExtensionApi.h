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

#ifndef DCPLUSPLUS_DCPP_EXTENSION_API_H
#define DCPLUSPLUS_DCPP_EXTENSION_API_H

#include <web-server/stdinc.h>
#include <web-server/ExtensionManagerListener.h>

#include <api/base/HierarchicalApiModule.h>
#include <api/ExtensionInfo.h>

#include <airdcpp/typedefs.h>

namespace webserver {
	class ExtensionManager;
	class ExtensionApi : public ParentApiModule<string, ExtensionInfo>, private ExtensionManagerListener {
	public:
		ExtensionApi(Session* aSession);
		~ExtensionApi();

		static StringList subscriptionList;
	private:
		void addExtension(const ExtensionPtr& aExtension) noexcept;

		static json serializeExtension(const ExtensionPtr& aExtension) noexcept;

		api_return handleDownloadExtension(ApiRequest& aRequest);
		api_return handlePostExtension(ApiRequest& aRequest);
		api_return handleRemoveExtension(ApiRequest& aRequest);

		void on(ExtensionManagerListener::ExtensionAdded, const ExtensionPtr& aExtension) noexcept override;
		void on(ExtensionManagerListener::ExtensionRemoved, const ExtensionPtr& aExtension) noexcept override;
		void on(ExtensionManagerListener::ExtensionUpdated, const ExtensionPtr& aExtension) noexcept override;

		ExtensionManager& em;

		ExtensionPtr getExtension(ApiRequest& aRequest);
	};
}

#endif