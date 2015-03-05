/**
 * Titanium.UI.TableViewRow
 *
 * Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_TABLEVIEWROW_HPP_
#define _TITANIUM_TABLEVIEWROW_HPP_

#include "Titanium/UI/View.hpp"
#include "Font.hpp"

namespace Titanium
{
	namespace UI
	{
		using namespace HAL;

		/*!
		  @class
		  @discussion This is the Titanium TableViewRow Module.
		  See http://docs.appcelerator.com/titanium/latest/#!/api/Titanium.UI.TableViewRow
		*/
		class TITANIUMKIT_EXPORT TableViewRow : public View, public JSExport<TableViewRow>
		{

		public:

			/*!
			  @property
			  @abstract color
			  @discussion Default text color of the row when not selected, as a color name or hex triplet.
			*/
			virtual std::string color() const TITANIUM_NOEXCEPT;
			virtual void setColor(std::string color) TITANIUM_NOEXCEPT;

			/*!
			  @property
			  @abstract font
			  @discussion Font to use for the row title.
			*/
			virtual Font font() const TITANIUM_NOEXCEPT;
			virtual void setFont(Font font) TITANIUM_NOEXCEPT;

			/*!
			  @property
			  @abstract hasCheck
			  @discussion Determines whether a system-provided checkmark is displayed on the right-hand side of the row.
			*/
			virtual bool hasCheck() const TITANIUM_NOEXCEPT;
			virtual void setHasCheck(bool hasCheck) TITANIUM_NOEXCEPT;

			/*!
			  @property
			  @abstract hasChild
			  @discussion Determines whether a system-provided arrow is displayed on the right-hand side of the row.
			*/
			virtual bool hasChild() const TITANIUM_NOEXCEPT;
			virtual void setHasChild(bool hasChild) TITANIUM_NOEXCEPT;

			/*!
			  @property
			  @abstract title
			  @discussion Text to display on the row.
			*/
			virtual std::string title() const TITANIUM_NOEXCEPT;
			virtual void setTitle(std::string title) TITANIUM_NOEXCEPT;

			TableViewRow(const JSContext&) TITANIUM_NOEXCEPT;
			virtual ~TableViewRow() = default;
			TableViewRow(const TableViewRow&) = default;
			TableViewRow& operator=(const TableViewRow&) = default;
#ifdef TITANIUM_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
			TableViewRow(TableViewRow&&)                 = default;
			TableViewRow& operator=(TableViewRow&&)      = default;
#endif

			static void JSExportInitialize();

			virtual JSValue TableViewRow::_TableViewRow() const TITANIUM_NOEXCEPT final;

			virtual JSValue js_color() const TITANIUM_NOEXCEPT final;
			virtual JSValue js_font() const TITANIUM_NOEXCEPT final;
			virtual JSValue js_hasCheck() const TITANIUM_NOEXCEPT final;
			virtual JSValue js_hasChild() const TITANIUM_NOEXCEPT final;
			virtual JSValue js_title() const TITANIUM_NOEXCEPT final;

			virtual bool js_set_color(const JSValue& argument) TITANIUM_NOEXCEPT;
			virtual bool js_set_font(const JSValue& argument) TITANIUM_NOEXCEPT;
			virtual bool js_set_hasCheck(const JSValue& argument) TITANIUM_NOEXCEPT;
			virtual bool js_set_hasChild(const JSValue& argument) TITANIUM_NOEXCEPT;
			virtual bool js_set_title(const JSValue& argument) TITANIUM_NOEXCEPT;

			virtual JSValue js_getColor(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_setColor(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_getFont(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_setFont(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_getHasCheck(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_setHasCheck(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_getHasChild(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_setHasChild(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_getTitle(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;
			virtual JSValue js_setTitle(const std::vector<JSValue>&, JSObject&) TITANIUM_NOEXCEPT final;

			protected:
#pragma warning(push)
#pragma warning(disable : 4251)
				std::string color__;
				Font font__;
				bool hasCheck__;
				bool hasChild__;
				std::string title__;
#pragma warning(pop)
		};
	} // namespace UI
} // namespace Titanium
#endif // _TITANIUM_TABLEVIEWROW_HPP_