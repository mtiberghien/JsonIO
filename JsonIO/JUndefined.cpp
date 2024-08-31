#include "pch.h"
#include "JUndefined.h"
#include "JsonValue.h"
#include "JObject.h"

namespace json
{
	JsonValue& JUndefined::operator[](const std::string& key)
	{
		return JVoidProvider::getError();
	}

	const JsonValue& JUndefined::operator[](const std::string& key) const
	{
		return JVoidProvider::getError();
	}

	JsonValue& JUndefined::operator[](int index)
	{
		return JVoidProvider::getError();
	}

	const JsonValue& JUndefined::operator[](int index) const
	{
		return JVoidProvider::getError();
	}

	JObject& JUndefined::getObject()
	{
		return JObjectProvider::getObjectError();
	}

	JArray& JUndefined::getArray()
	{
		return JArrayProvider::getArrayError();
	}

	const JObject& JUndefined::getObject() const
	{
		return JObjectProvider::getObjectError();
	}

	const JArray& JUndefined::getArray() const
	{
		return JArrayProvider::getArrayError();
	}

	JsonValue JVoidProvider::m_void{ E_JsonType::Error };

	JsonValue& JVoidProvider::getError()
	{
		return m_void;
	}
}
