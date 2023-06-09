/**
 * TitaniumKit
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_DETAIL_TIIMPL_HPP_
#define _TITANIUM_DETAIL_TIIMPL_HPP_

#ifdef NDEBUG
#define TITANIUM_ASSERT(expr)
#else
#define TITANIUM_ASSERT(expr) ((expr) ? (void)0 : \
    HAL::detail::ThrowRuntimeError("Assertion failed", #expr))
#endif

#ifdef NDEBUG
#define TITANIUM_ASSERT_AND_THROW(expr, message) 
#else
#define TITANIUM_ASSERT_AND_THROW(expr, message) ((expr) ? (void)0 : \
    HAL::detail::ThrowRuntimeError("Invalid argument supplied", message))
#endif

#include "Titanium/detail/TiLogger.hpp"

// For implementing the bridge getter function for a function property (cpp)
#define TITANIUM_FUNCTION(MODULE, NAME) \
JSValue MODULE::js_##NAME(const std::vector<JSValue>& arguments, JSObject& this_object)

// For implementing the bridge getter function for a value property (cpp)
#define TITANIUM_PROPERTY_GETTER(MODULE, NAME) \
JSValue MODULE::js_get_##NAME() const

// For implementing the bridge setter function for a value property (cpp)
#define TITANIUM_PROPERTY_SETTER(MODULE, NAME) \
bool MODULE::js_set_##NAME(const JSValue& argument)

// For implementing the native c++ getter function for a value property (cpp)
#define TITANIUM_PROPERTY_READ(MODULE, TYPE, NAME) \
TYPE MODULE::get_##NAME() const TITANIUM_NOEXCEPT \
{ \
	return NAME##__; \
}

// For implementing the native c++ setter function for a value property (cpp)
#define TITANIUM_PROPERTY_WRITE(MODULE, TYPE, NAME) \
void MODULE::set_##NAME(const TYPE& NAME) TITANIUM_NOEXCEPT \
{ \
	NAME##__ = NAME; \
}

// For implementing the native c++ getter and setter functions for a value property (cpp)
#define TITANIUM_PROPERTY_READWRITE(MODULE, TYPE, NAME) \
TITANIUM_PROPERTY_READ(MODULE, TYPE, NAME) \
TITANIUM_PROPERTY_WRITE(MODULE, TYPE, NAME)

// For adding a function as a property on the JS Object this type backs (cpp, in JSExportInitialize)
#define TITANIUM_ADD_FUNCTION(MODULE, NAME) \
JSExport<MODULE>::AddFunctionProperty(#NAME, std::mem_fn(&MODULE::js_##NAME))

// For adding a value as a read-only property on the JS Object this type backs (cpp, in JSExportInitialize)
#define TITANIUM_ADD_PROPERTY_READONLY(MODULE, NAME) \
JSExport<MODULE>::AddValueProperty(#NAME, std::mem_fn(&MODULE::js_get_##NAME))

// For adding a value as a property on the JS Object this type backs (cpp, in JSExportInitialize)
#define TITANIUM_ADD_PROPERTY(MODULE, NAME) \
JSExport<MODULE>::AddValueProperty(#NAME, std::mem_fn(&MODULE::js_get_##NAME), std::mem_fn(&MODULE::js_set_##NAME))

#define JSOBJECT_GETPROPERTY(IN, NAME, TYPE, DEFAULT_VALUE) \
IN.HasProperty(#NAME) ? static_cast<TYPE>(IN.GetProperty(#NAME)) : DEFAULT_VALUE

#define ENSURE_ARGUMENT_BOUNDS(INDEX) TITANIUM_ASSERT_AND_THROW((arguments.size() >= INDEX + 1), ("Index out of bounds: "#INDEX))

#define ENSURE_OPTIONAL_OBJECT_AT_INDEX(OUT,INDEX) \
  auto OUT = this_object.get_context().CreateObject(); \
  if (arguments.size() >= INDEX + 1) { \
    const auto _##INDEX = arguments.at(INDEX); \
    TITANIUM_ASSERT_AND_THROW(_##INDEX.IsObject(), "Expected Object"); \
    OUT = static_cast<JSObject>(_##INDEX);\
  }

#define ENSURE_OPTIONAL_BOOL_AT_INDEX(OUT,INDEX,VALUE) \
  auto OUT = VALUE; \
  if (arguments.size() >= INDEX + 1) { \
    const auto _##INDEX = arguments.at(INDEX); \
    TITANIUM_ASSERT_AND_THROW(_##INDEX.IsBoolean(), "Expected boolean"); \
    OUT = static_cast<bool>(_##INDEX);\
  }

#define ENSURE_OPTIONAL_STRING_AT_INDEX(OUT,INDEX,VALUE) \
  std::string OUT = VALUE; \
  if (arguments.size() >= INDEX + 1) { \
    const auto _##INDEX = arguments.at(INDEX); \
    OUT = static_cast<std::string>(_##INDEX);\
  }

#define ENSURE_OPTIONAL_NUMBER_AT_INDEX(OUT,INDEX,VALUE,TYPE) \
  TYPE OUT = VALUE; \
  if (arguments.size() >= INDEX + 1) { \
    const auto _##INDEX = arguments.at(INDEX); \
    TITANIUM_ASSERT_AND_THROW(_##INDEX.IsNumber(), "Expected Number"); \
    OUT = static_cast<TYPE>(_##INDEX);\
  }

#define ENSURE_OPTIONAL_ARRAY_AT_INDEX(OUT,INDEX) \
  auto OUT = this_object.get_context().CreateArray(); \
  if (arguments.size() >= INDEX + 1) { \
    const auto _##INDEX = arguments.at(INDEX); \
    TITANIUM_ASSERT_AND_THROW(_##INDEX.IsObject(), "Expected Object"); \
    const auto _obj_##INDEX = static_cast<JSObject>(_##INDEX); \
    TITANIUM_ASSERT_AND_THROW(_obj_##INDEX.IsArray(), "Expected Array"); \
    OUT = static_cast<JSArray>(_obj_##INDEX); \
  }

#define ENSURE_OPTIONAL_INT_AT_INDEX(OUT,INDEX,VALUE) \
  ENSURE_OPTIONAL_NUMBER_AT_INDEX(OUT,INDEX,VALUE,int32_t)

#define ENSURE_OPTIONAL_UINT_AT_INDEX(OUT,INDEX,VALUE) \
  ENSURE_OPTIONAL_NUMBER_AT_INDEX(OUT,INDEX,VALUE,uint32_t)

#define ENSURE_OPTIONAL_DOUBLE_AT_INDEX(OUT,INDEX,VALUE) \
  ENSURE_OPTIONAL_NUMBER_AT_INDEX(OUT,INDEX,VALUE,double)

#define ENSURE_NUMBER_AT_INDEX(OUT,INDEX,TYPE) \
  ENSURE_ARGUMENT_BOUNDS(INDEX); \
  const auto _##INDEX = arguments.at(INDEX); \
  TITANIUM_ASSERT_AND_THROW(_##INDEX.IsNumber(), "Expected Number"); \
  auto OUT = static_cast<TYPE>(_##INDEX);

#define ENSURE_BOOL_AT_INDEX(OUT,INDEX) \
  ENSURE_ARGUMENT_BOUNDS(INDEX); \
  const auto _##INDEX = arguments.at(INDEX); \
  TITANIUM_ASSERT_AND_THROW(_##INDEX.IsBoolean(), "Expected boolean"); \
  auto OUT = static_cast<bool>(_##INDEX);

#define ENSURE_STRING_AT_INDEX(OUT,INDEX) \
  ENSURE_ARGUMENT_BOUNDS(INDEX); \
  const auto _##INDEX = arguments.at(INDEX); \
  auto OUT = static_cast<std::string>(_##INDEX);

#define ENSURE_VALUE_AT_INDEX(OUT,INDEX) \
  ENSURE_ARGUMENT_BOUNDS(INDEX); \
  auto OUT = arguments.at(INDEX);

#define ENSURE_OBJECT_AT_INDEX(OUT,INDEX) \
  ENSURE_ARGUMENT_BOUNDS(INDEX); \
  const auto _##INDEX = arguments.at(INDEX); \
  TITANIUM_ASSERT_AND_THROW(_##INDEX.IsObject(), "Expected Object"); \
  auto OUT = static_cast<JSObject>(_##INDEX);

#define ENSURE_ARRAY_AT_INDEX(OUT,INDEX) \
  ENSURE_ARGUMENT_BOUNDS(INDEX); \
  const auto _##INDEX = arguments.at(INDEX); \
  TITANIUM_ASSERT_AND_THROW(_##INDEX.IsObject(), "Expected Object"); \
  const auto _obj_##INDEX = static_cast<JSObject>(_##INDEX); \
  TITANIUM_ASSERT_AND_THROW(_obj_##INDEX.IsArray(), "Expected Array"); \
  auto OUT = static_cast<JSArray>(_obj_##INDEX);

#define ENSURE_ARRAY(IN, OUT) \
  TITANIUM_ASSERT_AND_THROW(IN.IsObject(), "Expected Object"); \
  const auto obj_##IN = static_cast<JSObject>(IN); \
  TITANIUM_ASSERT_AND_THROW(obj_##IN.IsArray(), "Expected Array"); \
  auto OUT = static_cast<JSArray>(obj_##IN);

#define ENSURE_INT_AT_INDEX(OUT,INDEX) \
  ENSURE_NUMBER_AT_INDEX(OUT,INDEX,int32_t)

#define ENSURE_UINT_AT_INDEX(OUT,INDEX) \
  ENSURE_NUMBER_AT_INDEX(OUT,INDEX,uint32_t)

#define ENSURE_DOUBLE_AT_INDEX(OUT,INDEX) \
  ENSURE_NUMBER_AT_INDEX(OUT,INDEX,double)

#define ENSURE_TIME_AT_INDEX(OUT,INDEX) \
  ENSURE_NUMBER_AT_INDEX(OUT##_##INDEX,INDEX,uint32_t); \
  auto OUT = std::chrono::seconds(static_cast<std::chrono::seconds::rep>(OUT##_##INDEX));

// Shorthand for getter accessor function for a value property that has a getter (cpp)
#define TITANIUM_FUNCTION_AS_GETTER(MODULE, NAME, PROPERTY) \
TITANIUM_FUNCTION(MODULE, NAME) \
{ \
  return js_get_##PROPERTY(); \
}

// Shorthand for setter accessor function for a value property that has a setter (cpp)
#define TITANIUM_FUNCTION_AS_SETTER(MODULE, NAME, PROPERTY) \
TITANIUM_FUNCTION(MODULE, NAME) \
{ \
  ENSURE_VALUE_AT_INDEX(argument, 0); \
  js_set_##PROPERTY(argument); \
  return get_context().CreateUndefined(); \
}

#define ENSURE_MODULE_OBJECT(IN,OUT,TYPE) \
std::shared_ptr<TYPE> OUT = nullptr; \
if (IN.IsObject()) { \
  OUT = static_cast<JSObject>(IN).GetPrivate<TYPE>(); \
}

#define ENSURE_TYPE_ARRAY(IN,OUT,TYPE) \
TITANIUM_ASSERT(IN.IsObject()); \
auto js_##IN = static_cast<JSObject>(IN); \
TITANIUM_ASSERT(js_##IN.IsArray()); \
auto js_##OUT = static_cast<std::vector<JSValue>>(static_cast<JSArray>(js_##IN)); \
std::vector<TYPE> OUT; \
for (auto v : js_##OUT) { \
	OUT.push_back(static_cast<TYPE>(v)); \
}

#define ENSURE_STRING_ARRAY(IN,OUT) ENSURE_TYPE_ARRAY(IN,OUT,std::string)

#define ENSURE_OBJECT_ARRAY(IN,OUT,TYPE) \
TITANIUM_ASSERT(IN.IsObject()); \
auto js_##IN = static_cast<JSObject>(IN); \
TITANIUM_ASSERT(js_##IN.IsArray()); \
auto js_##OUT = static_cast<std::vector<JSValue>>(static_cast<JSArray>(js_##IN)); \
std::vector<std::shared_ptr<TYPE>> OUT; \
for (auto v : js_##OUT) { \
	OUT.push_back(static_cast<JSObject>(v).GetPrivate<TYPE>()); \
}

#define TITANIUM_PROPERTY_GETTER_TYPE(MODULE, NAME, TYPE, CHECK) \
TITANIUM_PROPERTY_GETTER(MODULE, NAME) { \
	return get_context().Create##CHECK(static_cast<TYPE>(get_##NAME())); \
}

#define TITANIUM_PROPERTY_SETTER_TYPE(MODULE, NAME, TYPE, CHECK) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
	TITANIUM_ASSERT(argument.Is##CHECK()); \
	set_##NAME(static_cast<TYPE>(argument)); \
	return true; \
}

#define TITANIUM_PROPERTY_GETTER_ENUM(MODULE, NAME) \
  TITANIUM_PROPERTY_GETTER_TYPE(MODULE, NAME, std::uint32_t, Number)

#define TITANIUM_PROPERTY_SETTER_ENUM(MODULE, NAME, ENUM_NAME) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
  TITANIUM_ASSERT(argument.IsNumber()); \
  set_##NAME(static_cast<ENUM_NAME>(static_cast<std::uint32_t>(argument))); \
  return true; \
}

#define TITANIUM_PROPERTY_GETTER_STRUCT(MODULE, NAME, STRUCT_NAME) \
TITANIUM_PROPERTY_GETTER(MODULE, NAME) { \
	return STRUCT_NAME##_to_js(get_context(), get_##NAME()); \
}

#define TITANIUM_PROPERTY_SETTER_STRUCT(MODULE, NAME, STRUCT_NAME) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
	TITANIUM_ASSERT(argument.IsObject()); \
	set_##NAME(js_to_##STRUCT_NAME(static_cast<JSObject>(argument))); \
	return true; \
}

#define TITANIUM_PROPERTY_GETTER_TIME(MODULE, NAME) \
TITANIUM_PROPERTY_GETTER(MODULE, NAME) { \
	return get_context().CreateNumber(static_cast<double>(get_##NAME().count())); \
}

#define TITANIUM_PROPERTY_SETTER_TIME(MODULE, NAME) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
	TITANIUM_ASSERT(argument.IsNumber()); \
	set_##NAME(std::chrono::milliseconds(static_cast<std::chrono::milliseconds::rep>(static_cast<std::uint32_t>(argument)))); \
	return true; \
}

#define TITANIUM_PROPERTY_SETTER_TIME_SECONDS(MODULE, NAME) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
  TITANIUM_ASSERT(argument.IsNumber()); \
  set_##NAME(std::chrono::seconds(static_cast<std::chrono::seconds::rep>(static_cast<std::uint32_t>(argument)))); \
  return true; \
}


#define TITANIUM_PROPERTY_GETTER_STRING(MODULE, NAME) \
	TITANIUM_PROPERTY_GETTER_TYPE(MODULE, NAME, std::string, String)

#define TITANIUM_PROPERTY_SETTER_STRING(MODULE, NAME) \
	TITANIUM_PROPERTY_SETTER_TYPE(MODULE, NAME, std::string, String)

#define TITANIUM_PROPERTY_GETTER_DOUBLE(MODULE, NAME) \
	TITANIUM_PROPERTY_GETTER_TYPE(MODULE, NAME, double, Number)

#define TITANIUM_PROPERTY_SETTER_DOUBLE(MODULE, NAME) \
	TITANIUM_PROPERTY_SETTER_TYPE(MODULE, NAME, double, Number)

#define TITANIUM_PROPERTY_GETTER_INT(MODULE, NAME) \
	TITANIUM_PROPERTY_GETTER_TYPE(MODULE, NAME, std::int32_t, Number)

#define TITANIUM_PROPERTY_SETTER_INT(MODULE, NAME) \
	TITANIUM_PROPERTY_SETTER_TYPE(MODULE, NAME, std::int32_t, Number)

#define TITANIUM_PROPERTY_GETTER_UINT(MODULE, NAME) \
	TITANIUM_PROPERTY_GETTER_TYPE(MODULE, NAME, std::uint32_t, Number)

#define TITANIUM_PROPERTY_SETTER_UINT(MODULE, NAME) \
	TITANIUM_PROPERTY_SETTER_TYPE(MODULE, NAME, std::uint32_t, Number)

#define TITANIUM_PROPERTY_GETTER_BOOL(MODULE, NAME) \
	TITANIUM_PROPERTY_GETTER_TYPE(MODULE, NAME, bool, Boolean)

#define TITANIUM_PROPERTY_SETTER_BOOL(MODULE, NAME) \
	TITANIUM_PROPERTY_SETTER_TYPE(MODULE, NAME, bool, Boolean)

#define TITANIUM_PROPERTY_GETTER_STRING_ARRAY(MODULE, NAME) \
TITANIUM_PROPERTY_GETTER(MODULE, NAME) { \
	std::vector<JSValue> values; \
	for (auto value : get_##NAME()) { \
		values.push_back(get_context().CreateString(value)); \
	} \
	return get_context().CreateArray(values); \
}

#define TITANIUM_PROPERTY_SETTER_STRING_ARRAY(MODULE, NAME) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
	ENSURE_STRING_ARRAY(argument, NAME); \
	set_##NAME(NAME); \
	return true; \
}

#define TITANIUM_PROPERTY_GETTER_OBJECT(MODULE, NAME) \
TITANIUM_PROPERTY_GETTER(MODULE, NAME) { \
	const auto NAME = get_##NAME(); \
	if (NAME != nullptr) { \
		return NAME->get_object(); \
		} \
	return get_context().CreateNull(); \
}

#define TITANIUM_PROPERTY_SETTER_OBJECT(MODULE, NAME, TYPE) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
	TITANIUM_ASSERT(argument.IsObject()); \
	set_##NAME(static_cast<JSObject>(argument).GetPrivate<TYPE>()); \
	return true; \
}

#define TITANIUM_PROPERTY_GETTER_OBJECT_ARRAY(MODULE, NAME) \
TITANIUM_PROPERTY_GETTER(MODULE, NAME) { \
	std::vector<JSValue> values; \
	for (auto value : get_##NAME()) { \
		values.push_back(value->get_object()); \
	} \
	return get_context().CreateArray(values); \
}

#define TITANIUM_PROPERTY_SETTER_OBJECT_ARRAY(MODULE, NAME, TYPE) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
	ENSURE_OBJECT_ARRAY(argument, NAME, TYPE); \
	set_##NAME(NAME); \
	return true; \
}

#define TITANIUM_PROPERTY_GETTER_UNIMPLEMENTED(MODULE, NAME) \
TITANIUM_PROPERTY_GETTER(MODULE, NAME) { \
	TITANIUM_LOG_WARN(#MODULE "::" #NAME ": getter unimplemented"); \
	return get_context().CreateUndefined(); \
}

#define TITANIUM_PROPERTY_SETTER_UNIMPLEMENTED(MODULE, NAME) \
TITANIUM_PROPERTY_SETTER(MODULE, NAME) { \
	TITANIUM_LOG_WARN(#MODULE "::" #NAME ": setter unimplemented"); \
	return false; \
}

#endif  // _TITANIUM_DETAIL_TIIMPL_HPP_
