#pragma once
#include "JsonIO.h"
#include <string>

namespace json
{
	enum class E_JsonType { Object, Array, Bool, Short, Int, Float, Double, String, Null, Undefined, Error };
	enum class E_JsonValueType { Object, Array, StringPrimitive, NonStringPrimitive, Error};

	JSONIO_API extern std::string toString(E_JsonType type);

	class JsonValue;
	class JObject;
	class JArray;

	class JSONIO_API IJsonValue
	{
	public:
		IJsonValue() {}
		virtual ~IJsonValue() {}
		virtual E_JsonType getType() const = 0;
		virtual bool getBool(bool defaultValue = false) const = 0;
		virtual short getShort(short defaultValue = 0) const = 0;
		virtual int getInt(int defaultValue = 0) const = 0;
		virtual float getFloat(float defaultValue = 0) const = 0;
		virtual double getDouble(double defaultValue = 0) const = 0;
		virtual std::string getString(const std::string& defaultValue = "") const = 0;
		virtual JObject& getObject() = 0;
		virtual JArray& getArray() = 0;
		virtual JsonValue& operator[](const std::string& key) = 0;
		virtual JsonValue& operator[](int index) = 0;
		virtual const JObject& getObject() const = 0;
		virtual const JArray& getArray() const = 0;
		virtual const JsonValue& operator[](const std::string& key) const = 0;
		virtual const JsonValue& operator[](int index) const = 0;
		virtual void write(std::ostream& stream, bool indent, int& indentLevel) const = 0;

	};
}


