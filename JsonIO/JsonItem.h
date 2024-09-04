#pragma once
#include "IJsonValue.h"
#include "JsonIO.h"

namespace json
{
	class JsonItem : public IJsonValue
	{
	public:
		virtual ~JsonItem() {}
		bool operator==(const IJsonValue& other) { return this->getString() == other.getString(); }
		explicit operator bool() const { return getBool(); }
		explicit operator short() const { return getShort(); }
		explicit operator int() const { return getInt(); }
		explicit operator float() const { return getFloat(); }
		explicit operator double() const { return getDouble(); }
		operator std::string() const { return getString(); }
		operator JObject& () { return getObject(); }
		operator JArray& () { return getArray(); }
	    operator const JObject& () const { return getObject(); }
		operator const JArray& () const { return getArray(); }
		bool isError() const { return getType() == E_JsonType::Error; }
		bool isObject() const { return getType() == E_JsonType::Object; }
		bool isArray() const { return getType() == E_JsonType::Array; }
		bool isUndefined() const { return getType() == E_JsonType::Undefined; }
		bool isNull() const { return getType() == E_JsonType::Null; }
		bool isPrimitive() const {
			switch (getType())
			{
			case E_JsonType::Error:
			case E_JsonType::Object:
			case E_JsonType::Array:
			case E_JsonType::Undefined:
				return false;
			default: return true;
			}
		}
		bool isBool() const { return getType() == E_JsonType::Bool; }
		bool isString() const { return getType() == E_JsonType::String; }
		JSONIO_API JsonValue& find(const std::string& path);
		JSONIO_API const JsonValue& find(const std::string& path) const;
		JSONIO_API bool isNumber() const;

	};

}

