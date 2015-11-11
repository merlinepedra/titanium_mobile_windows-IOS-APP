/**
* Titanium.UI.TextField for Windows
*
* Copyright (c) 2014-2015 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License.
* Please see the LICENSE included with this distribution for details.
*/

#include "TitaniumWindows/UI/TextField.hpp"
#include "TitaniumWindows/UI/WindowsViewLayoutDelegate.hpp"
#include "TitaniumWindows/Utility.hpp"
#include "Titanium/detail/TiImpl.hpp"

namespace TitaniumWindows
{
	namespace UI
	{
		TextField::TextField(const JSContext& js_context) TITANIUM_NOEXCEPT
			  : Titanium::UI::TextField(js_context)
		{
		}

		void TextField::postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments)
		{
			Titanium::UI::TextField::postCallAsConstructor(js_context, arguments);	
			
			Titanium::UI::TextField::setLayoutDelegate<WindowsViewLayoutDelegate>();

			// Parent of the text box.
			// TextField have to deal with multiple controls. (TextBox & PasswordBox)
			parent__ = ref new Windows::UI::Xaml::Controls::Grid();

			getViewLayoutDelegate<WindowsViewLayoutDelegate>()->setComponent(parent__);

			text_box__ = ref new Windows::UI::Xaml::Controls::TextBox();
			text_box__->AcceptsReturn = false;
			text_box__->IsSpellCheckEnabled = true;

			parent__->Children->Append(text_box__);
			parent__->SetColumn(text_box__, 0);
			parent__->SetRow(text_box__, 0);

			initTextComponent();
		}

		void TextField::initTextComponent()
		{
			// TIMOB-19143: reset MinWidth to fix size issues
			if (text_box__) {
				text_box__->MinWidth = 0;
			} else if (password_box__) {
				password_box__->MinWidth = 0;
			}

		}

		void TextField::JSExportInitialize()
		{
			JSExport<TextField>::SetClassVersion(1);
			JSExport<TextField>::SetParent(JSExport<Titanium::UI::TextField>::Class());

			JSExport<TextField>::AddValueProperty("width", std::mem_fn(&js_get_width), std::mem_fn(&js_set_width_min));
		}

		void TextField::set_passwordMask(const bool& passwordMask) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_passwordMask(passwordMask);

			// password mask is little bit tricky here, because Xaml uses different component for it.
			// Now we need to dispose TextBox and inherit properties from it.
			// Don't forget to update layout delegate too...
			if (passwordMask && text_box__) {
				password_box__ = ref new Windows::UI::Xaml::Controls::PasswordBox();
				//password_box__->Foreground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Black);

				// Remove existing text box
				if (parent__->Children->Size > 0) {
					parent__->Children->RemoveAt(0);
				}

				// Add password box
				parent__->Children->Append(password_box__);
				parent__->SetColumn(password_box__, 0);
				parent__->SetRow(password_box__, 0);

				// copy properties which we interested in
				password_box__->PlaceholderText = text_box__->PlaceholderText;
				password_box__->Password = text_box__->Text;
				password_box__->MaxLength = text_box__->MaxLength;
				password_box__->Width = text_box__->Width;
				password_box__->Height = text_box__->Height;

				password_box__->IsEnabled = !text_box__->IsReadOnly;
				password_box__->VerticalAlignment = text_box__->VerticalAlignment;

				// Update color based on property string
				set_color(get_color());

				text_box__ = nullptr;
			} else if (!passwordMask && password_box__) {
				text_box__ = ref new Windows::UI::Xaml::Controls::TextBox();
				text_box__->AcceptsReturn = false;
				text_box__->IsSpellCheckEnabled = true;

				// Remove existing text box
				if (parent__->Children->Size > 0) {
					parent__->Children->RemoveAt(0);
				}

				// Add text box
				parent__->Children->Append(text_box__);
				parent__->SetColumn(text_box__, 0);
				parent__->SetRow(text_box__, 0);

				// copy properties which we interested in
				text_box__->PlaceholderText = password_box__->PlaceholderText;
				text_box__->Text = password_box__->Password;
				text_box__->MaxLength = password_box__->MaxLength;
				text_box__->Width = password_box__->Width;
				text_box__->Height = password_box__->Height;
				text_box__->IsReadOnly = !password_box__->IsEnabled;
				text_box__->VerticalAlignment = password_box__->VerticalAlignment;

				// Update color based on property string
				set_color(get_color());

				password_box__ = nullptr;
			}

			initTextComponent();
		}

		void TextField::set_color(const std::string& colorName) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_color(colorName);
			const auto color_obj = WindowsViewLayoutDelegate::ColorForName(colorName);
			const auto color_brush = ref new Windows::UI::Xaml::Media::SolidColorBrush(color_obj);
			if (text_box__) {
				text_box__->Foreground = color_brush;
			} else if (password_box__) {
				password_box__->Foreground = color_brush;
			}
		}

		void TextField::set_editable(const bool& editable) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_editable(editable);
			if (text_box__) {
				text_box__->IsReadOnly = !editable;
			} else if (password_box__) {
				// there's no IsReadOnly property in PasswordBox.
				password_box__->IsEnabled = !editable;
			}
		}

		void TextField::set_hintText(const std::string& hintText) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_hintText(hintText);
			if (text_box__) {
				text_box__->PlaceholderText = TitaniumWindows::Utility::ConvertUTF8String(hintText);
			} else if (password_box__) {
				password_box__->PlaceholderText = TitaniumWindows::Utility::ConvertUTF8String(hintText);
			}
		}

		void TextField::set_keyboardType(const Titanium::UI::KEYBOARD& keyboardType) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_keyboardType(keyboardType);
			if (text_box__) {
				auto scope_name = ref new Windows::UI::Xaml::Input::InputScopeName();
				if (keyboardType == Titanium::UI::KEYBOARD::ASCII) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::Default;
				} else if (keyboardType == Titanium::UI::KEYBOARD::DECIMAL_PAD) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::Number;
				} else if (keyboardType == Titanium::UI::KEYBOARD::DEFAULT) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::Default;
				} else if (keyboardType == Titanium::UI::KEYBOARD::EMAIL) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::EmailSmtpAddress;
				} else if (keyboardType == Titanium::UI::KEYBOARD::NAMEPHONE_PAD) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::NameOrPhoneNumber;
				} else if (keyboardType == Titanium::UI::KEYBOARD::NUMBERS_PUNCTUATION) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::CurrencyAmountAndSymbol;
				} else if (keyboardType == Titanium::UI::KEYBOARD::NUMBER_PAD) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::Number;
				} else if (keyboardType == Titanium::UI::KEYBOARD::PHONE_PAD) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::TelephoneNumber;
				} else if (keyboardType == Titanium::UI::KEYBOARD::URL) {
					scope_name->NameValue = Windows::UI::Xaml::Input::InputScopeNameValue::Url;
				} else {
					return;
				}
				auto scope = text_box__->InputScope;
				if (!scope) {
					scope = ref new Windows::UI::Xaml::Input::InputScope();
					text_box__->InputScope = scope;
				}
				text_box__->InputScope->Names->Clear();
				text_box__->InputScope->Names->Append(scope_name);
			} else {
				TITANIUM_LOG_WARN("TextField.keyboardType can not be used with passwordMask");
			}
		}

		void TextField::set_maxLength(const int32_t& maxLength) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_maxLength(maxLength);
			if (text_box__) {
				text_box__->MaxLength = maxLength;
			} else if (password_box__) {
				password_box__->MaxLength = maxLength;
			}
		}

		void TextField::set_textAlign(const Titanium::UI::TEXT_ALIGNMENT& textAlign) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_textAlign(textAlign);
			if (text_box__) {
				if (textAlign == Titanium::UI::TEXT_ALIGNMENT::CENTER) {
					text_box__->TextAlignment = Windows::UI::Xaml::TextAlignment::Center;
				} else if (textAlign == Titanium::UI::TEXT_ALIGNMENT::LEFT) {
					text_box__->TextAlignment = Windows::UI::Xaml::TextAlignment::Left;
				} else if (textAlign == Titanium::UI::TEXT_ALIGNMENT::RIGHT) {
					text_box__->TextAlignment = Windows::UI::Xaml::TextAlignment::Right;
				}
				// TODO Windows supports JUSTIFY!
			} else {
				TITANIUM_LOG_WARN("TexeField.textAlign can not be used with passwordMask");
			}
		}

		std::string TextField::get_value() const TITANIUM_NOEXCEPT
		{
			if (text_box__) {
				return TitaniumWindows::Utility::ConvertUTF8String(text_box__->Text);
			} else if (password_box__) {
				return TitaniumWindows::Utility::ConvertUTF8String(password_box__->Password);
			}
			return "";
		}

		void TextField::set_value(const std::string& value) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_value(value);
			if (text_box__) {
				text_box__->Text = TitaniumWindows::Utility::ConvertUTF8String(value);
			} else if (password_box__) {
				password_box__->Password = TitaniumWindows::Utility::ConvertUTF8String(value);
			}
		}

		void TextField::set_verticalAlign(const Titanium::UI::TEXT_VERTICAL_ALIGNMENT& verticalAlign) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::set_verticalAlign(verticalAlign);
			auto align = Windows::UI::Xaml::VerticalAlignment::Center;
			if (verticalAlign == Titanium::UI::TEXT_VERTICAL_ALIGNMENT::BOTTOM) {
				align = Windows::UI::Xaml::VerticalAlignment::Bottom;
			} else if (verticalAlign == Titanium::UI::TEXT_VERTICAL_ALIGNMENT::CENTER) {
				align = Windows::UI::Xaml::VerticalAlignment::Center;
			} else if (verticalAlign == Titanium::UI::TEXT_VERTICAL_ALIGNMENT::TOP) {
				align = Windows::UI::Xaml::VerticalAlignment::Top;
			}
			// TODO Windows supports stretch!

			if (text_box__) {
				text_box__->VerticalAlignment = align;
			} else if (password_box__) {
				password_box__->VerticalAlignment = align;
			}
		}

		void TextField::blur() TITANIUM_NOEXCEPT
		{
			// TODO Windows doesn't allow forcibly losing focus, need to set focus on another control to achieve it! Can we set focus on the window or parent or something?
			TITANIUM_LOG_WARN("TextField.blur() is not supported yet");
		}

		void TextField::focus() TITANIUM_NOEXCEPT
		{
			if (text_box__) {
				text_box__->Focus(Windows::UI::Xaml::FocusState::Programmatic);
			} else if (password_box__) {
				password_box__->Focus(Windows::UI::Xaml::FocusState::Programmatic);
			}
		}

		bool TextField::hasText() TITANIUM_NOEXCEPT
		{
			if (text_box__) {
				return !text_box__->Text->IsEmpty();
			} else if (password_box__) {
				return !password_box__->Password->IsEmpty();
			}
			return false;
		}

		void TextField::enableEvent(const std::string& event_name) TITANIUM_NOEXCEPT
		{
			Titanium::UI::TextField::enableEvent(event_name);

			const JSContext ctx = this->get_context();

			using namespace Windows::UI::Xaml::Input;
			using namespace Windows::UI::Xaml;

			if (event_name == "blur") {
				const auto lostfocus = ref new RoutedEventHandler([this, ctx](Platform::Object^ sender, RoutedEventArgs^ e) {
					JSObject eventArgs = ctx.CreateObject();
					eventArgs.SetProperty("value", ctx.CreateString(this->get_value()));

					this->fireEvent("blur", eventArgs);
				});
				if (text_box__) {
					blur_event_ = text_box__->LostFocus += lostfocus;
				} else if (password_box__) {
					blur_event_ = password_box__->LostFocus += lostfocus;
				}
			} else if (event_name == "change") {
				if (text_box__) {
					change_event_ = text_box__->TextChanged += ref new Controls::TextChangedEventHandler([this, ctx](Platform::Object^ sender, Controls::TextChangedEventArgs^ e) {
						JSObject eventArgs = ctx.CreateObject();
						eventArgs.SetProperty("value", ctx.CreateString(this->get_value()));

						this->fireEvent("change", eventArgs);
					});
				} else if (password_box__) {
					change_event_ = password_box__->PasswordChanged += ref new RoutedEventHandler([this, ctx](Platform::Object^ sender, RoutedEventArgs^ e){
						JSObject eventArgs = ctx.CreateObject();
						eventArgs.SetProperty("value", ctx.CreateString(this->get_value()));

						this->fireEvent("change", eventArgs);
					});
				}
			} else if (event_name == "focus") {
				const auto gotfocus = ref new RoutedEventHandler([this, ctx](Platform::Object^ sender, RoutedEventArgs^ e) {
					JSObject eventArgs = ctx.CreateObject();
					eventArgs.SetProperty("value", ctx.CreateString(this->get_value()));

					this->fireEvent("focus", eventArgs);
				});
				if (text_box__) {
					focus_event_ = text_box__->GotFocus += gotfocus;
				} else if (password_box__) {
					focus_event_ = password_box__->GotFocus += gotfocus;
				}
			} else if (event_name == "return") {
				const auto keydown = ref new KeyEventHandler([this, ctx](Platform::Object^ sender, KeyRoutedEventArgs^ e) {
					if (e->Key == Windows::System::VirtualKey::Enter) {
						JSObject eventArgs = ctx.CreateObject();
						eventArgs.SetProperty("value", ctx.CreateString(this->get_value()));

						this->fireEvent("return", eventArgs);
					}
				});
				if (text_box__) {
					return_event_ = text_box__->KeyDown += keydown;
				} else if (password_box__) {
					return_event_ = password_box__->KeyDown += keydown;
				}
			}
		}

		bool TextField::js_set_width_min(const JSValue& argument) TITANIUM_NOEXCEPT
		{
			js_set_width(argument);
			TITANIUM_ASSERT(argument.IsString() || argument.IsNumber());
			layoutDelegate__->set_minWidth(static_cast<std::string>(argument));
			return true;
		}

	} // namespace UI
} // namespace TitaniumWindows
