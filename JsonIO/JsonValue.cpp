#include "pch.h"
#include "JsonValue.h"
#include "JObject.h"
#include "JUndefined.h"
#include "JArray.h"
#include "JsonIOHelper.h"

namespace json
{
	JsonValue::JsonValue(const JObject& ref) : m_ptr(new JObject(ref))
	{
	}
	JsonValue::JsonValue() :m_ptr(new JUndefined()) {}
	JsonValue::JsonValue(const JArray& ref) : m_ptr(new JArray(ref))
	{
	}

	JsonValue::JsonValue(const JsonValue& ref)
	{
		switch (ref.getType())
		{
		case E_JsonType::Error:
			m_ptr = std::make_unique<JUndefined>();
			break;
		case E_JsonType::Object:
			m_ptr = std::make_unique<JObject>(ref.getObject());
			break;
		case E_JsonType::Array:
			m_ptr = std::make_unique <JArray>(ref.getArray());
			break;
		case E_JsonType::Bool:
		{
			m_ptr = std::make_unique<JPrimitive<bool>>(ref.getBool());
			break;
		}
		case E_JsonType::Short:
		{
			m_ptr = std::make_unique<JPrimitive<short>>(ref.getShort());
			break;
		}
		case E_JsonType::Int:
		{
			m_ptr = std::make_unique<JPrimitive<int>>(ref.getInt());
			break;
		}
		case E_JsonType::Float:
		{
			m_ptr = std::make_unique<JPrimitive<float>>(ref.getFloat());
			break;
		}
		case E_JsonType::Double:
		{
			m_ptr = std::make_unique<JPrimitive<double>>(ref.getDouble());
			break;
		}
		case E_JsonType::String:
		{
			m_ptr = std::make_unique<JPrimitive<std::string>>(ref.getString());
			break;
		}
		case E_JsonType::Null:
		{
			m_ptr = std::make_unique<JPrimitive<std::nullptr_t>>(nullptr);
			break;
		}
		case E_JsonType::Undefined:
		{
			m_ptr = std::make_unique<JUndefined>();
			break;
		}
		default:
			m_ptr = std::make_unique<JUndefinedError>();
			break;
		}
	}

	JsonValue::JsonValue(E_JsonType a_Type)
	{
		switch (a_Type)
		{
		case E_JsonType::Bool:
		{
			m_ptr = std::make_unique<JPrimitive<bool>>(false);
			break;
		}
		case E_JsonType::Short:
		{
			m_ptr = std::make_unique<JPrimitive<short>>((short)0);
			break;
		}
		case E_JsonType::Int:
		{
			m_ptr = std::make_unique<JPrimitive<int>>(0);
			break;
		}
		case E_JsonType::Float:
		{
			m_ptr = std::make_unique<JPrimitive<float>>((float)0);
			break;
		}
		case E_JsonType::Double:
		{
			m_ptr = std::make_unique<JPrimitive<double>>((double)0);
			break;
		}
		case E_JsonType::String:
		{
			m_ptr = std::make_unique<JPrimitive<std::string>>("");
			break;
		}
		case E_JsonType::Object:
		{
			m_ptr = std::make_unique<JObject>();
			break;
		}
		case E_JsonType::Array:
			m_ptr = std::make_unique<JArray>();
			break;
		case E_JsonType::Null:
		{
			m_ptr = std::make_unique<JPrimitive<std::nullptr_t>>(nullptr);
			break;
		}
		case E_JsonType::Undefined:
		{
			m_ptr = std::make_unique<JUndefined>();
			break;
		}
		default:
			m_ptr = std::make_unique<JUndefinedError>();
			break;
		}
	}
}


