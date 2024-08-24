#include "pch.h"
#include "JVoid.h"
#include "JsonValue.h"
#include "JObject.h"

namespace json
{
	JsonValue& JVoid::operator[](const std::string& key)
	{
		return JVoidProvider::getError();
	}

	const JsonValue& JVoid::operator[](const std::string& key) const
	{
		return JVoidProvider::getError();
	}

	JsonValue& JVoid::operator[](int index)
	{
		return JVoidProvider::getError();
	}

	const JsonValue& JVoid::operator[](int index) const
	{
		return JVoidProvider::getError();
	}

	JObject& JVoid::getObject()
	{
		return JObjectProvider::getObjectError();
	}

	JArray& JVoid::getArray()
	{
		return JArrayProvider::getArrayError();
	}

	const JObject& JVoid::getObject() const
	{
		return JObjectProvider::getObjectError();
	}

	const JArray& JVoid::getArray() const
	{
		return JArrayProvider::getArrayError();
	}

	JsonValue JVoidProvider::m_void{ E_JsonType::Error };

	JsonValue& JVoidProvider::getError()
	{
		return m_void;
	}
}
