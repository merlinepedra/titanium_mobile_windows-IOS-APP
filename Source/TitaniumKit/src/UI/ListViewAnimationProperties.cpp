/**
 * TitaniumKit ListViewAnimationProperties
 *
 * Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "Titanium/UI/ListViewAnimationProperties.hpp"
#include "Titanium/detail/TiImpl.hpp"

namespace Titanium
{
	namespace UI
	{
		using namespace HAL;

		ListViewAnimationProperties::ListViewAnimationProperties(const JSContext& js_context) TITANIUM_NOEXCEPT
			: Module(js_context)
		{
		}

		void ListViewAnimationProperties::postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments)
		{
			TITANIUM_ASSERT(arguments.size() > 0);
			const auto _0 = arguments.at(0);
			TITANIUM_ASSERT(_0.IsObject());
			const auto js_animations = static_cast<JSObject>(_0);			

			// animated property
			if (js_animations.HasProperty("animated")) {
				const auto animated = js_animations.GetProperty("animated");
				TITANIUM_ASSERT(animated.IsBoolean());
				set_animated(static_cast<bool>(animated));
			}
		}

		void ListViewAnimationProperties::JSExportInitialize() {
			JSExport<Module>::SetClassVersion(1);
			JSExport<Module>::SetParent(JSExport<Module>::Class());
		}

		bool ListViewAnimationProperties::get_animated() const TITANIUM_NOEXCEPT
		{
			return animated__;
		}

		void ListViewAnimationProperties::set_animated(const bool& animated) TITANIUM_NOEXCEPT
		{
			animated__ = animated;
		}
	} // namespace UI
} // namespace Titanium
