/**
* Titanium.UI.TextField for Windows
*
* Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License.
* Please see the LICENSE included with this distribution for details.
*/

#ifndef _TITANIUMWINDOWS_UI_TEXTFIELD_HPP_
#define _TITANIUMWINDOWS_UI_TEXTFIELD_HPP_

#include "TitaniumWindows_UI_EXPORT.h"
#include "Titanium/UI/TextField.hpp"

namespace TitaniumWindows
{
	namespace UI
	{
		using namespace HAL;

		/*!
		  @class

		  @discussion This is the Titanium.UI.TextField implementation for
		  Windows.
		*/
		class TITANIUMWINDOWS_UI_EXPORT TextField final : public Titanium::UI::TextField, public JSExport<TextField>
		{
		public:
			TextField(const JSContext&) TITANIUM_NOEXCEPT;

			virtual ~TextField() = default;
			TextField(const TextField&) = default;
			TextField& operator=(const TextField&) = default;
#ifdef TITANIUM_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
			TextField(TextField&&) = default;
			TextField& operator=(TextField&&) = default;
#endif

			static void JSExportInitialize();

			virtual void postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) override;

//			virtual void set_autocapitalization(const Titanium::UI::TEXT_AUTOCAPITALIZATION& autoCapitalization) TITANIUM_NOEXCEPT override final;
//			virtual void set_borderStyle(const Titanium::UI::INPUT_BORDERSTYLE& borderStyle) TITANIUM_NOEXCEPT override final;
//			virtual void set_clearButtonMode(const Titanium::UI::INPUT_BUTTONMODE& clearButtonMode) TITANIUM_NOEXCEPT override final;
			virtual void set_color(const std::string& color) TITANIUM_NOEXCEPT override final;
			virtual void set_editable(const bool& editable) TITANIUM_NOEXCEPT override final;
//			virtual void set_ellipsize(const bool& ellipsize) TITANIUM_NOEXCEPT override final;
//			virtual void set_enableReturnKey(const bool& enableReturnKey) TITANIUM_NOEXCEPT override final;
			virtual void set_hintText(const std::string& hintText) TITANIUM_NOEXCEPT override final;
			virtual void set_keyboardType(const Titanium::UI::KEYBOARD& keyboardType) TITANIUM_NOEXCEPT override final;
//			virtual void set_leftButtonMode(const Titanium::UI::INPUT_BUTTONMODE& leftButtonMode) TITANIUM_NOEXCEPT override final;
			virtual void set_maxLength(const int32_t& maxLength) TITANIUM_NOEXCEPT override final;
//			virtual void set_passwordMask(const bool& passwordMask) TITANIUM_NOEXCEPT override final;
//			virtual void set_returnKeyType(const Titanium::UI::RETURNKEY& returnKeyType) TITANIUM_NOEXCEPT override final;
//			virtual void set_rightButtonMode(const Titanium::UI::INPUT_BUTTONMODE& rightButtonMode) TITANIUM_NOEXCEPT override final;
//			virtual void set_suppressReturn(const bool& suppressReturn) TITANIUM_NOEXCEPT override final;
			virtual void set_textAlign(const Titanium::UI::TEXT_ALIGNMENT& textAlign) TITANIUM_NOEXCEPT override final;
			virtual std::string get_value() const TITANIUM_NOEXCEPT override final;
			virtual void set_value(const std::string& value) TITANIUM_NOEXCEPT override final;
			virtual void set_verticalAlign(const Titanium::UI::TEXT_VERTICAL_ALIGNMENT& verticalAlign) TITANIUM_NOEXCEPT override final;

			virtual void blur() TITANIUM_NOEXCEPT override final;
			virtual void focus() TITANIUM_NOEXCEPT override final;
			virtual bool hasText() TITANIUM_NOEXCEPT override final;

			virtual void enableEvent(const std::string& event_name) TITANIUM_NOEXCEPT override final;

			virtual bool js_set_width_min(const JSValue& argument) TITANIUM_NOEXCEPT final;

		private:
			Windows::UI::Xaml::Controls::TextBox^ text_box__;

			// Event handlers
			Windows::Foundation::EventRegistrationToken blur_event_;
			Windows::Foundation::EventRegistrationToken change_event_;
			Windows::Foundation::EventRegistrationToken focus_event_;
			Windows::Foundation::EventRegistrationToken return_event_;
		};
	} // namespace UI
} // namespace TitaniumWindows

#endif  // _TITANIUMWINDOWS_UI_TEXTFIELD_HPP_
