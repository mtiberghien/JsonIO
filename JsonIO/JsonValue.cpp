#include "pch.h"
#include "JsonValue.h"
#include "JObject.h"
#include "JVoid.h"
#include "JArray.h"
#include "ioHelper.h"

namespace json
{
	JsonValue::JsonValue(const JObject& ref) :m_ptr(new JObject(ref)) {}
	JsonValue::JsonValue() :m_ptr(new JVoid()) {}
	JsonValue::JsonValue(const JArray& ref) :m_ptr((IJsonValue*)new JArray(ref)) {}

	JsonValue::JsonValue(E_JsonType a_Type)
	{
		switch (a_Type)
		{
		case E_JsonType::Bool:
		{
			bool v{ false };
			m_ptr = std::make_shared<JPrimitive<bool>>(v);
			break;
		}
		case E_JsonType::Short:
		{
			short v{ 0 };
			m_ptr = std::make_shared<JPrimitive<short>>(v);
			break;
		}
		case E_JsonType::Int:
		{
			int v{ 0 };
			m_ptr = std::make_shared<JPrimitive<int>>(v);
			break;
		}
		case E_JsonType::Float:
		{
			float v{ 0 };
			m_ptr = std::make_shared<JPrimitive<float>>(v);
			break;
		}
		case E_JsonType::Double:
		{
			double v{ 0 };
			m_ptr = std::make_shared<JPrimitive<double>>(v);
			break;
		}
		case E_JsonType::String:
		{
			std::string v{ "" };
			m_ptr = std::make_shared<JPrimitive<std::string>>(v);
			break;
		}
		case E_JsonType::Object:
		{
			m_ptr = std::make_shared<JObject>();
			break;
		}
		case E_JsonType::Array:
			m_ptr = std::make_shared<JArray>();
			break;
		default:
			m_ptr = std::make_shared<JVoidError>();
			break;
		}
	}

	bool JsonValue::read(std::istream& stream, JsonValue& value, bool& hasNext)
	{
		bool isOk = false;
		E_JsonValueType type = readValueType(stream);
		switch (type)
		{
		case E_JsonValueType::Object:
		{
			JObject o;
			if (o.read(stream))
			{
				value = o;
				isOk = true;
			}
			break;

		}
		case E_JsonValueType::StringPrimitive:
		{
			std::string s;
			if (readStringValue(stream, s))
			{
				value = s;
				isOk = true;
			}
			break;
		}
		case E_JsonValueType::Array:
		{
			JArray a;
			if (a.read(stream))
			{
				value = a;
				isOk = true;
			}
			break;
		}
		case E_JsonValueType::NonStringPrimitive:
		{
			double d;
			E_JsonType t;
			if (readNonStringPrimitive(stream, d, t))
			{
				switch (t)
				{
				case E_JsonType::Bool: value = (bool)d; break;
				case E_JsonType::Short: value = (short)d; break;
				case E_JsonType::Int: value = (int)d; break;
				case E_JsonType::Float: value = (float)d; break;
				case E_JsonType::Null: value = nullptr; break;
				default: value = d; break;
				}
				isOk = true;
			}
			break;
		}
		default:
			break;
		}
		hasNext = findFirstNonSpaceCharacter(stream) && stream.peek() == ',';
		if (hasNext)
		{
			stream.get();
		}
		return isOk;
	}
}


