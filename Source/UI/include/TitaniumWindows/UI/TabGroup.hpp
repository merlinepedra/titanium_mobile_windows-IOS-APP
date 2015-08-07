/**
 * Titanium.UI.TabGroup for Windows
 *
 * Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUMWINDOWS_TABGROUP_HPP_
#define _TITANIUMWINDOWS_TABGROUP_HPP_

#include "TitaniumWindows_UI_EXPORT.h"
#include "Titanium/UI/TabGroup.hpp"
#include <collection.h>

namespace TitaniumWindows
{
	namespace UI
	{
		using namespace HAL;

		class Window;

		/*!
		  @class

		  @discussion This is the Titanium.UI.TabGroup implementation for Windows.
		*/
		class TITANIUMWINDOWS_UI_EXPORT TabGroup final : public Titanium::UI::TabGroup, public JSExport<TabGroup>
		{

		public:
			TabGroup(const JSContext&) TITANIUM_NOEXCEPT;
			virtual ~TabGroup()                  = default;
			TabGroup(const TabGroup&)            = default;
			TabGroup& operator=(const TabGroup&) = default;
#ifdef TITANIUM_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
			TabGroup(TabGroup&&)                 = default;
			TabGroup& operator=(TabGroup&&)      = default;
#endif

			static void JSExportInitialize();

			virtual void postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) override;
			virtual void addTab(const std::shared_ptr<Titanium::UI::Tab>& tab) TITANIUM_NOEXCEPT override;
			virtual void open() TITANIUM_NOEXCEPT override;
			virtual void close() TITANIUM_NOEXCEPT override;

			virtual void set_tabs(const std::vector<std::shared_ptr<Titanium::UI::Tab>>& tabs) TITANIUM_NOEXCEPT override;

			virtual void set_activeTab(const std::shared_ptr<Titanium::UI::Tab>& activeTab) TITANIUM_NOEXCEPT override;
			virtual void set_activeTab(const std::shared_ptr<Titanium::UI::Tab>& activeTab, const bool updateUI) TITANIUM_NOEXCEPT;

		private:
			void appendWindowsTabItem(const std::shared_ptr<Titanium::UI::Tab>& tab) TITANIUM_NOEXCEPT;

#pragma warning(push)
#pragma warning(disable : 4251)
			std::shared_ptr<TitaniumWindows::UI::Window> window__;
			Windows::UI::Xaml::Controls::Grid^  grid__;

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
			Windows::UI::Xaml::Controls::Pivot^ pivot__;
#else
			//
			// We're going to use ListView for tab navigation because there's no Pivot for Windows Store app.
			// On Windows 10 we might want to use SplitView instead.
			// http://igrali.com/2015/04/12/getting-started-with-splitview-control-in-universal-apps/
			//
			Windows::UI::Xaml::Controls::ListView^ sectionView__;
			Windows::UI::Xaml::Data::CollectionViewSource^ sectionViewSource__;
			Windows::Foundation::Collections::IObservableVector<::Platform::String^>^ sectionViewItems__;
#endif

#pragma warning(pop)

		};
	}  // namespace UI
}  // namespace TitaniumWindows
#endif // _TITANIUMWINDOWS_TABGROUP_HPP_