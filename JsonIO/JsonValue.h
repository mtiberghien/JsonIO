#pragma once
#include "IJsonValue.h"
#include "JPrimitive.h"
#include "JsonIO.h"

namespace json
{
	class JObject;
	class JVoid;
	class JArray;

	class JSONIO_API JsonValue : public IJsonValue
	{
	public:
		JsonValue();
		JsonValue(E_JsonType a_Type);
		template<class T>
		JsonValue(T value) :m_ptr(std::make_shared<JPrimitive<T>>(value)) {}
		JsonValue(const JObject& ref);
		JsonValue(const JArray& ref);
		JsonValue(const JsonValue& ref) :m_ptr(ref.m_ptr) {}
		E_JsonType getType() const override { return m_ptr->getType(); }
		bool getBool(bool defaultValue = false) const override { return m_ptr->getBool(defaultValue); }
		short getShort(short defaultValue = 0) const override { return m_ptr->getShort(defaultValue); }
		int getInt(int defaultValue = 0) const override { return m_ptr->getInt(defaultValue); }
		float getFloat(float defaultValue = 0) const override { return m_ptr->getFloat(defaultValue); }
		double getDouble(double defaultValue = 0) const override { return m_ptr->getDouble(defaultValue); }
		std::string getString(const std::string& defaultValue = "") const override { return m_ptr->getString(defaultValue); }
		JObject& getObject() override { return m_ptr->getObject(); }
		JArray& getArray() override { return m_ptr->getArray(); }
		const JObject& getObject() const override { return m_ptr->getObject(); }
		const JArray& getArray() const override { return m_ptr->getArray(); }
		const JsonValue& operator[](const std::string& key) const override { return m_ptr->operator[](key); }
		const JsonValue& operator[](int index) const override { return m_ptr->operator[](index); }
		JsonValue& operator[](const std::string& key) override { return m_ptr->operator[](key); }
		JsonValue& operator[](int index) override { return m_ptr->operator[](index); }
		explicit operator bool() const { return getBool(); }
		explicit operator short() const { return getShort(); }
		explicit operator int() const { return getInt(); }
		explicit operator float() const { return getFloat(); }
		explicit operator double() const { return getDouble(); }
		operator std::string() const { return getString(); }
	    operator JObject&() { return getObject(); }
		operator JArray& () { return getArray(); }
		explicit operator const JObject& () const { return getObject(); }
		explicit operator const JArray& () const { return getArray(); }
		bool operator==(const JsonValue& other)
		{
			return getString() == other.getString();
		}
		JsonValue& operator=(const JsonValue& value) 
		{
			if (getType() != E_JsonType::Error)
			{
				this->m_ptr = value.m_ptr;
			}
			return *this;
		}
		void write(std::ostream& stream, bool indent, int& indentLevel) const override { m_ptr->write(stream, indent, indentLevel); }
		static bool read(std::istream&, JsonValue& value, bool& hasNext);
	private:
		std::shared_ptr<IJsonValue> m_ptr;

	};
}


