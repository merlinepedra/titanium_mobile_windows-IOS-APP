/**
* Titanium.App.Properties for Windows
*
* Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License.
* Please see the LICENSE included with this distribution for details.
*/

#ifndef _TITANIUMWINDOWS_PROPERTIES_HPP_
#define _TITANIUMWINDOWS_PROPERTIES_HPP_

#include "Titanium/App/Properties.hpp"
#include "TitaniumWindows_Ti_EXPORT.h"

using namespace Windows::Storage;
using namespace Windows::Foundation;

namespace TitaniumWindows
{
	namespace App
	{
		using namespace HAL;

		/*!
		  @class

		  @discussion This is the Titanium.App.Properties implementation for Windows.
		*/
		class TITANIUMWINDOWS_TI_EXPORT Properties final : public Titanium::App::Properties, public JSExport<Properties>
		{

		public:
			Properties(const JSContext&) TITANIUM_NOEXCEPT;

			virtual ~Properties() = default;
			Properties(const Properties&) = default;
			Properties& operator=(const Properties&) = default;
#ifdef TITANIUM_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
			Properties(Properties&&) = default;
			Properties& operator=(Properties&&) = default;
#endif

			static void JSExportInitialize();

		private:

			virtual bool getBool(const std::string& property, bool default) TITANIUM_NOEXCEPT override;
			virtual double getDouble(const std::string& property, double default) TITANIUM_NOEXCEPT override;
			virtual double getInt(const std::string& property, double default) TITANIUM_NOEXCEPT override;
			virtual std::string getString(const std::string& property, const std::string& default) TITANIUM_NOEXCEPT override;
			virtual bool hasProperty(const std::string& property) TITANIUM_NOEXCEPT override;
			virtual std::vector<std::string> listProperties() TITANIUM_NOEXCEPT override;
			virtual void removeProperty(const std::string& property) TITANIUM_NOEXCEPT override;
			virtual void setBool(const std::string& property, bool value) TITANIUM_NOEXCEPT override;
			virtual void setDouble(const std::string& property, double value) TITANIUM_NOEXCEPT override;
			virtual void setInt(const std::string& property, int value) TITANIUM_NOEXCEPT override;
			virtual void setString(const std::string& property, const std::string& value) TITANIUM_NOEXCEPT override;

			ApplicationDataContainer^ local_settings_;

		};
	} // namespace App
}  // namespace TitaniumWindows
#endif // _TITANIUMWINDOWS_PROPERTIES_HPP_