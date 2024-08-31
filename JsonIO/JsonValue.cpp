#include "pch.h"
#include "JsonValue.h"
#include "JObject.h"
#include "JVoid.h"
#include "JArray.h"
#include "ioHelper.h"

namespace json
{
	JsonValue::JsonValue(const JObject& ref) : m_ptr(new JObject(ref))
	{
	}
	JsonValue::JsonValue() :m_ptr(new JVoid()) {}
	JsonValue::JsonValue(const JArray& ref) : m_ptr(new JArray(ref))
	{
	}

	JsonValue::JsonValue(const JsonValue& ref)
	{
		switch (ref.getType())
		{
		case E_JsonType::Error:
			m_ptr = std::make_unique<JVoid>();
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
			m_ptr = std::make_unique<JVoid>();
			break;
		}
		default:
			m_ptr = std::make_unique<JVoidError>();
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
			m_ptr = std::make_unique<JVoid>();
			break;
		}
		default:
			m_ptr = std::make_unique<JVoidError>();
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
		if (!isOk)
		{
			return false;
		}
		hasNext = findFirstNonSpaceCharacter(stream) && stream.peek() == ',';
		if (hasNext)
		{
			stream.get();
		}
		else
		{
			isOk = stream.peek() == '}' || stream.peek() == ']';
		}
		return isOk;
	}
}


