/**
 * TitaniumKit Titanium.UI.ListSection
 *
 * Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "Titanium/UI/ListSection.hpp"
#include "Titanium/detail/TiImpl.hpp"

namespace Titanium
{
	namespace UI
	{

		ListDataItem js_to_ListDataItem(const JSObject& object)
		{
			ListDataItem item;

			auto properties = object.GetProperty("properties");
			if (properties.IsObject()) {
				item.properties = static_cast<JSObject>(properties).GetProperties();
			} else {
				TITANIUM_LOG_WARN("Failed to process ListDataItem.properties: Not a JSObject");
			}
			item.templateId = static_cast<std::string>(object.GetProperty("template"));

			// collect all binding properties
			for (const auto& name : static_cast<std::vector<JSString>>(object.GetPropertyNames())) {
				if (name == "properties" || name == "templates") {
					continue;
				}
				item.bindings.emplace(name, object.GetProperty(name));
			}
			return item;
		}

		JSObject ListDataItem_to_js(const JSContext& js_context, const ListDataItem& item)
		{
			auto object = js_context.CreateObject();
			object.SetProperty("template", js_context.CreateString(item.templateId));
			object.SetProperty("properties", js_context.CreateObject(item.properties));
			for (auto kv : item.bindings) {
				object.SetProperty(kv.first, kv.second);
			}
			return object;
		}

		static std::vector<ListDataItem> js_to_ListDataItem_array(const JSArray& js_array)
		{
			std::vector<ListDataItem> items;
			const auto js_items = static_cast<std::vector<JSValue>>(js_array);
			for (const auto js_item : js_items) {
				if (js_item.IsObject()) {
					items.push_back(js_to_ListDataItem(static_cast<JSObject>(js_item)));
				}
			}
			return items;
		}

		ListSection::ListSection(const JSContext& js_context) TITANIUM_NOEXCEPT
			: Module(js_context),
			listviewAnimationProperties_ctor__(js_context.CreateObject(JSExport<Titanium::UI::ListViewAnimationProperties>::Class())),
			footerTitle__(""),
			headerTitle__("")
		{
			TITANIUM_LOG_DEBUG("ListSection:: ctor ", this);
		}

		void ListSection::postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) {
			HAL_LOG_DEBUG("ListSection:: postCallAsConstructor ", this);
		}

		TITANIUM_PROPERTY_READWRITE(ListSection, std::string, footerTitle)
		
		TITANIUM_PROPERTY_READWRITE(ListSection, std::string, headerTitle)

		TITANIUM_PROPERTY_READWRITE(ListSection, View_shared_ptr_t, footerView)

		TITANIUM_PROPERTY_READWRITE(ListSection, View_shared_ptr_t, headerView)

		std::vector<ListDataItem> ListSection::get_items() const TITANIUM_NOEXCEPT
		{
			return items__;
		}

		void ListSection::set_items(const std::vector<ListDataItem>& value) TITANIUM_NOEXCEPT
		{
			items__ = value;
			items_set_notify(0, items__.size());
		}

		void ListSection::setItems(const std::vector<ListDataItem>& dataItems, const std::shared_ptr<ListViewAnimationProperties>& animation) TITANIUM_NOEXCEPT
		{
			set_items(dataItems);
		}

		void ListSection::appendItems(const std::vector<ListDataItem>& dataItems, const std::shared_ptr<ListViewAnimationProperties>& animation) TITANIUM_NOEXCEPT
		{
			items__.insert(items__.end(), dataItems.begin(), dataItems.end());
			items_set_notify(items__.size(), dataItems.size());
		}

		void ListSection::insertItemsAt(uint32_t index, const std::vector<ListDataItem>& dataItems, const std::shared_ptr<ListViewAnimationProperties>& animation) TITANIUM_NOEXCEPT
		{
			items__.insert(items__.begin() + index, dataItems.begin(), dataItems.end());
			items_set_notify(index, dataItems.size());
		}

		void ListSection::replaceItemsAt(uint32_t index, uint32_t count, const std::vector<ListDataItem>& dataItems, const std::shared_ptr<ListViewAnimationProperties>& animation) TITANIUM_NOEXCEPT
		{
			items__.erase (items__.begin() + index, items__.begin() + index + count);
			items__.insert(items__.begin() + index, dataItems.begin(), dataItems.end());
			items_set_notify(index, dataItems.size());
		}

		void ListSection::deleteItemsAt(uint32_t index, uint32_t count, const std::shared_ptr<ListViewAnimationProperties>& animation) TITANIUM_NOEXCEPT
		{
			items__.erase (items__.begin() + index, items__.begin() + index + count);
			items_set_notify(index, count);
		}

		ListDataItem ListSection::getItemAt(uint32_t index) TITANIUM_NOEXCEPT
		{
			return items__.at(index);
		}

		void ListSection::updateItemAt(uint32_t index, const ListDataItem& dataItem, const std::shared_ptr<ListViewAnimationProperties>& animation) TITANIUM_NOEXCEPT
		{
			items__.at(index) = dataItem;
			items_set_notify(index, 1);
		}

		void ListSection::items_set_notify(size_t index, size_t count)
		{
			TITANIUM_LOG_WARN("ListSection::items_set_notify: Unimplemented");
		}
		
		void ListSection::JSExportInitialize() {
			JSExport<ListSection>::SetClassVersion(1);
			JSExport<ListSection>::SetParent(JSExport<Module>::Class());

			TITANIUM_ADD_PROPERTY(ListSection, footerTitle);
			TITANIUM_ADD_PROPERTY(ListSection, headerTitle);
			TITANIUM_ADD_PROPERTY(ListSection, footerView);
			TITANIUM_ADD_PROPERTY(ListSection, headerView);
			TITANIUM_ADD_PROPERTY(ListSection, items);

			TITANIUM_ADD_FUNCTION(ListSection, appendItems);
			TITANIUM_ADD_FUNCTION(ListSection, insertItemsAt);
			TITANIUM_ADD_FUNCTION(ListSection, replaceItemsAt);
			TITANIUM_ADD_FUNCTION(ListSection, deleteItemsAt);
			TITANIUM_ADD_FUNCTION(ListSection, getItemAt);
			TITANIUM_ADD_FUNCTION(ListSection, updateItemAt);
			TITANIUM_ADD_FUNCTION(ListSection, getFooterTitle);
			TITANIUM_ADD_FUNCTION(ListSection, setFooterTitle);
			TITANIUM_ADD_FUNCTION(ListSection, getFooterView);
			TITANIUM_ADD_FUNCTION(ListSection, setFooterView);
			TITANIUM_ADD_FUNCTION(ListSection, getHeaderTitle);
			TITANIUM_ADD_FUNCTION(ListSection, setHeaderTitle);
			TITANIUM_ADD_FUNCTION(ListSection, getHeaderView);
			TITANIUM_ADD_FUNCTION(ListSection, setHeaderView);
			TITANIUM_ADD_FUNCTION(ListSection, getItems);
			TITANIUM_ADD_FUNCTION(ListSection, setItems);
		}

		TITANIUM_PROPERTY_GETTER(ListSection, footerTitle)
		{
			return get_context().CreateString(get_footerTitle());
		}

		TITANIUM_PROPERTY_SETTER(ListSection, footerTitle)
		{
			TITANIUM_ASSERT(argument.IsString());
			set_footerTitle(static_cast<std::string>(argument));
			return true;
		}

		TITANIUM_PROPERTY_GETTER(ListSection, headerTitle)
		{
			return get_context().CreateString(get_headerTitle());
		}

		TITANIUM_PROPERTY_SETTER(ListSection, headerTitle)
		{
			TITANIUM_ASSERT(argument.IsString());
			set_headerTitle(static_cast<std::string>(argument));
			return true;
		}

		TITANIUM_PROPERTY_GETTER(ListSection, footerView)
		{
			auto view = get_footerView();
			if (view) {
				return view->get_object();
			}  else {
				return get_context().CreateNull();	
			}
		}

		TITANIUM_PROPERTY_SETTER(ListSection, footerView)
		{
			TITANIUM_ASSERT(argument.IsObject());
			set_footerView(static_cast<JSObject>(argument).GetPrivate<View>());
			return true;
		}

		TITANIUM_PROPERTY_GETTER(ListSection, headerView)
		{
			auto view = get_headerView();
			if (view) {
				return view->get_object();
			}  else {
				return get_context().CreateNull();	
			}
		}

		TITANIUM_PROPERTY_SETTER(ListSection, headerView)
		{
			TITANIUM_ASSERT(argument.IsObject());
			set_headerView(static_cast<JSObject>(argument).GetPrivate<View>());
			return true;
		}

		TITANIUM_PROPERTY_GETTER(ListSection, items)
		{
			std::vector<JSValue> js_items;
			for (auto & item : get_items()) {
				js_items.push_back(ListDataItem_to_js(get_context(), item));
			}
			return get_context().CreateArray(js_items);
		}

		TITANIUM_PROPERTY_SETTER(ListSection, items)
		{
			if (!argument.IsObject()) {
				return false;
			}
			const auto items = static_cast<JSObject>(argument);
			if (!items.IsArray()) {
				return false;
			}
			set_items(js_to_ListDataItem_array(items));
			return true;
		}

		TITANIUM_FUNCTION(ListSection, setItems)
		{
			const auto js_context = this_object.get_context();
			if (arguments.size() >= 1) {
				JSObject animation = js_context.CreateObject();
				const auto _0 = arguments.at(0);
				TITANIUM_ASSERT(_0.IsObject());

				const auto dataItems = js_to_ListDataItem_array(static_cast<JSObject>(_0));

				if (arguments.size() >= 2) {
					const auto _1 = arguments.at(1);
					if (_1.IsObject()) {
						animation = listviewAnimationProperties_ctor__.CallAsConstructor({ _1 });
					}
				}
				setItems(dataItems, animation.GetPrivate<ListViewAnimationProperties>());
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, appendItems)
		{
			const auto js_context = this_object.get_context();
			if (arguments.size() >= 1) {
				JSObject animation = js_context.CreateObject();
				const auto _0 = arguments.at(0);
				TITANIUM_ASSERT(_0.IsObject());
				const auto dataItems = js_to_ListDataItem_array(static_cast<JSObject>(_0));

				if (arguments.size() >= 2) {
					const auto _1 = arguments.at(1);
					if (_1.IsObject()) {
						animation = listviewAnimationProperties_ctor__.CallAsConstructor({ _1 });
					}
				}
				appendItems(dataItems, animation.GetPrivate<ListViewAnimationProperties>());
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, insertItemsAt)
		{
			const auto js_context = this_object.get_context();
			if (arguments.size() >= 2) {
				const auto _0 = arguments.at(0);
				TITANIUM_ASSERT(_0.IsNumber());
				const auto _1 = arguments.at(1);
				TITANIUM_ASSERT(_1.IsObject());

				JSObject animation = js_context.CreateObject();
				const auto itemIndex = static_cast<uint32_t>(_0);
				const auto dataItems = js_to_ListDataItem_array(static_cast<JSObject>(_1));

				if (arguments.size() >= 3) {
					const auto _2 = arguments.at(2);
					if (_2.IsObject()) {
						animation = listviewAnimationProperties_ctor__.CallAsConstructor({ _2 });
					}
				}
				insertItemsAt(itemIndex, dataItems, animation.GetPrivate<ListViewAnimationProperties>());
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, replaceItemsAt)
		{
			const auto js_context = this_object.get_context();
			if (arguments.size() >= 3) {
				const auto _0 = arguments.at(0);
				TITANIUM_ASSERT(_0.IsNumber());
				const auto _1 = arguments.at(1);
				TITANIUM_ASSERT(_1.IsNumber());
				const auto _2 = arguments.at(2);
				TITANIUM_ASSERT(_2.IsObject());

				JSObject animation = js_context.CreateObject();
				const auto index = static_cast<uint32_t>(_0);
				const auto count = static_cast<uint32_t>(_1);
				const auto dataItems = js_to_ListDataItem_array(static_cast<JSObject>(_2));

				if (arguments.size() >= 4) {
					const auto _3 = arguments.at(3);
					if (_3.IsObject()) {
						animation = listviewAnimationProperties_ctor__.CallAsConstructor({ _3 });
					}
				}

				replaceItemsAt(index, count, dataItems, animation.GetPrivate<ListViewAnimationProperties>());
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, deleteItemsAt)
		{
			const auto js_context = this_object.get_context();
			if (arguments.size() >= 2) {
				const auto _0 = arguments.at(0);
				TITANIUM_ASSERT(_0.IsNumber());
				const auto _1 = arguments.at(1);
				TITANIUM_ASSERT(_1.IsNumber());

				JSObject animation = js_context.CreateObject();
				const auto itemIndex = static_cast<uint32_t>(_0);
				const auto count = static_cast<uint32_t>(_1);

				if (arguments.size() >= 3) {
					const auto _2 = arguments.at(2);
					if (_2.IsObject()) {
						animation = listviewAnimationProperties_ctor__.CallAsConstructor({ _2 });
					}
				}
				deleteItemsAt(itemIndex, count, animation.GetPrivate<ListViewAnimationProperties>());
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, getItemAt)
		{
			if (arguments.size() >= 1) {
				const auto _0 = arguments.at(0);
				TITANIUM_ASSERT(_0.IsNumber());
				const auto itemIndex = static_cast<uint32_t>(_0);
				return ListDataItem_to_js(get_context(), getItemAt(itemIndex));
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, updateItemAt)
		{
			const auto js_context = this_object.get_context();
			if (arguments.size() >= 2) {
				const auto _0 = arguments.at(0);
				TITANIUM_ASSERT(_0.IsNumber());
				const auto _1 = arguments.at(1);
				TITANIUM_ASSERT(_1.IsObject());

				JSObject animation = js_context.CreateObject();
				const auto index    = static_cast<uint32_t>(_0);
				const auto dataItem = js_to_ListDataItem(static_cast<JSObject>(_1));

				if (arguments.size() >= 3) {
					const auto _2 = arguments.at(2);
					if (_2.IsObject()) {
						animation = listviewAnimationProperties_ctor__.CallAsConstructor({ _2 });
					}
				}

				updateItemAt(index, dataItem, animation.GetPrivate<ListViewAnimationProperties>());
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, getFooterTitle)
		{
			return js_get_footerTitle();
		}

		TITANIUM_FUNCTION(ListSection, setFooterTitle)
		{
			if (arguments.size() >= 1) {
				js_set_footerTitle(arguments.at(0));
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, getFooterView)
		{
			return this_object.GetProperty("footerView");
		}

		TITANIUM_FUNCTION(ListSection, setFooterView)
		{
			if (arguments.size() >= 1) {
				this_object.SetProperty("footerView", arguments.at(0));
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, getHeaderTitle)
		{
			return js_get_headerTitle();
		}

		TITANIUM_FUNCTION(ListSection, setHeaderTitle)
		{
			if (arguments.size() >= 1) {
				js_set_headerTitle(arguments.at(0));
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, getHeaderView)
		{
			return this_object.GetProperty("headerView");
		}

		TITANIUM_FUNCTION(ListSection, setHeaderView)
		{
			if (arguments.size() >= 1) {
				this_object.SetProperty("headerView", arguments.at(0));
			}
			return get_context().CreateUndefined();
		}

		TITANIUM_FUNCTION(ListSection, getItems)
		{
			return js_get_items();
		}

	} // namespace UI
} // namespace Titanium