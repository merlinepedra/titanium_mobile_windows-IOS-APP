/**
 * Windows Native Wrapper for Windows.UI.Xaml.PropertyMetadata
 *
 * Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _WINDOWS_UI_XAML_PROPERTYMETADATA_HPP_
#define _WINDOWS_UI_XAML_PROPERTYMETADATA_HPP_

#include "TitaniumWindows_Native_EXPORT.h"
#include "Titanium/detail/TiBase.hpp"
#include "TitaniumWindows/Utility.hpp"
#include "Platform.Object.hpp"

namespace Titanium
{
	namespace Windows
	{
		namespace UI
		{
			namespace Xaml
			{


		using namespace HAL;

		class TITANIUMWINDOWS_NATIVE_EXPORT PropertyMetadata : public ::Titanium::Platform::Object, public JSExport<PropertyMetadata>
		{

		public:
			TITANIUM_PROPERTY_READONLY_DEF(CreateDefaultValueCallback);
			TITANIUM_PROPERTY_READONLY_DEF(DefaultValue);

			TITANIUM_FUNCTION_DEF(Create);

			PropertyMetadata(const JSContext&) TITANIUM_NOEXCEPT;

			virtual ~PropertyMetadata() = default;
			PropertyMetadata(const PropertyMetadata&) = default;
			PropertyMetadata& operator=(const PropertyMetadata&) = default;
#ifdef TITANIUM_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
			PropertyMetadata(PropertyMetadata&&)                 = default;
			PropertyMetadata& operator=(PropertyMetadata&&)      = default;
#endif

			static void JSExportInitialize();

			virtual void postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) override;

			::Windows::UI::Xaml::PropertyMetadata^ unwrapWindows_UI_Xaml_PropertyMetadata() const;
			void wrap(::Windows::UI::Xaml::PropertyMetadata^ object);

		private:
			::Windows::UI::Xaml::PropertyMetadata^ unwrap() const;

		};

			} // namespace Xaml
		} // namespace UI
	} // namespace Windows
} // namespace Titanium
#endif // _WINDOWS_UI_XAML_PROPERTYMETADATA_HPP_
