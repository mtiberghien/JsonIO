#pragma once
#include "JsonItem.h"
#include "JVoid.h"
#include "ioHelper.h"
#include "JsonIO.h"
#include "JObject.h"
#include "JArray.h"

namespace json
{
	class JVoid;
	class JsonValue;

	const std::map<char, std::string> g_escapes = { {'\r', "\\r"}, {'\n', "\\n"},{'\t', "\\t"},{'\f',"\\f"},{'\v', "\\v"} };


	template<class T>
	class JPrimitive : public JsonItem
	{
	public:
		JPrimitive(T value) : m_value(value)
		{
		}
		JPrimitive(T&& value) : m_value(std::move(value)) {}
		~JPrimitive()
		{
		}
		E_JsonType getType() const override { return getType<T>(); }
		bool getBool(bool defaultValue = false) const override
		{
			return getBool<T>(defaultValue);
		}
		short getShort(short defaultValue = 0) const override 
		{ 
			return getShort<T>(defaultValue);
		}
		int getInt(int defaultValue = 0) const override 
		{ 
			return getInt<T>(defaultValue); 
		}
		float getFloat(float defaultValue = 0) const override
		{
			return getFloat<T>(defaultValue);
		}
		double getDouble(double defaultValue = 0) const override
		{
			return getDouble<T>(defaultValue);
		}

		std::string getString(const std::string& defaultValue = "") const override
		{
			return getString<T>(defaultValue);
		}

		JsonValue& operator[](const std::string& key) override
		{
			return JVoidProvider::getError();
		}

		const JsonValue& operator[](const std::string& key) const override
		{
			return JVoidProvider::getError();
		}

		JsonValue& operator[](int index) override
		{
			return JVoidProvider::getError();
		}

		const JsonValue& operator[](int index) const override
		{
			return JVoidProvider::getError();
		}

		void write(std::ostream& stream, bool indent, int& indentLevel) const override
		{
			write<T>(stream, indent, indentLevel);
		}

		JObject& getObject() override { return JObjectProvider::getObjectError(); }
		JArray& getArray() override { return JArrayProvider::getArrayError(); }
		const JObject& getObject() const override { return JObjectProvider::getObjectError(); }
		const JArray& getArray() const override { return JArrayProvider::getArrayError(); }

	private:
		template<class T>
		E_JsonType getType() const
		{
			switch (sizeof(T))
			{
				case sizeof(bool) : return E_JsonType::Bool;
				case sizeof(short) : return E_JsonType::Short;
				case sizeof(int) : return E_JsonType::Int;
				case sizeof(double) : return E_JsonType::Double;
				default: return E_JsonType::Undefined;
			}
		}
		template<>
		E_JsonType getType<float>() const { return E_JsonType::Float; }
		template<>
		E_JsonType getType<std::string>() const { return E_JsonType::String; }
		template<>
		E_JsonType getType<const char*>() const { return E_JsonType::String; }
		template<>
		E_JsonType getType<std::nullptr_t>() const { return E_JsonType::Null; }
		template<class T>
		std::string getString(const std::string& defaultValue = "") const
		{
			std::ostringstream s;
			s << m_value;
			return s.str();
		}
		template<>
		std::string getString<bool>(const std::string& defaultValue) const
		{
			return getBool() ? "true" : "false";
		}
		template<>
		std::string getString<std::nullptr_t>(const std::string& defaultValue) const
		{
			return "null";
		}
		template<>
		std::string getString<double>(const std::string& defaultValue) const
		{
			std::ostringstream s;
			s << std::setprecision(12);
			s << m_value;
			return s.str();
		}
		template<class T>
		float getFloat(float defaultValue) const
		{
			return (float)m_value;

		}
		template<>
		float getFloat<const char*>(float defaultValue) const
		{
			double d;
			E_JsonType t;
			if (tryGetNumber(getString(), d, t))
			{
				return (float)d;
			}
			return defaultValue;
		}
		template<>
		float getFloat<std::string>(float defaultValue) const
		{
			return getFloat<const char*>(defaultValue);
		}
		template<>
		float getFloat<std::nullptr_t>(float defaultValue) const
		{
			return getFloat<const char*>(defaultValue);
		}
		
		template<class T>
		double getDouble(double defaultValue) const
		{
			return (double)m_value;
		}
		template<>
		double getDouble<const char*>(double defaultValue) const
		{
			double d;
			E_JsonType t;
			if (tryGetNumber(getString(), d, t))
			{
				return d;
			}
			return defaultValue;
		}
		template<>
		double getDouble<std::string>(double defaultValue) const
		{
			return getDouble<const char*>(defaultValue);
		}
		template<>
		double getDouble<std::nullptr_t>(double defaultValue) const
		{
			return getDouble<const char*>(defaultValue);
		}
		template<class T>
		int getInt(int defaultValue) const
		{
			return (int)m_value;
		}
		template<>
		int getInt<const char*>(int defaultValue) const
		{
			double d;
			E_JsonType t;
			if (tryGetNumber(getString(), d, t))
			{
				return (int)d;
			}
			return defaultValue;
		}
		template<>
		int getInt<std::string>(int defaultValue) const
		{
			return getInt<const char*>(defaultValue);
		}
		template<>
		int getInt<std::nullptr_t>(int defaultValue) const
		{
			return getInt<const char*>(defaultValue);
		}
		template<class T>
		short getShort(short defaultValue) const
		{
			return (short)m_value;
		}
		template<>
		short getShort<const char*>(short defaultValue) const
		{
			double d;
			E_JsonType t;
			if (tryGetNumber(getString(), d, t))
			{
				return (short)d;
			}
			return defaultValue;
		}
		template<>
		short getShort<std::string>(short defaultValue) const
		{
			return getShort<const char*>(defaultValue);
		}
		template<>
		short getShort<std::nullptr_t>(short defaultValue) const
		{
			return getShort<const char*>(defaultValue);
		}
		template<class T>
		bool getBool(bool defaultValue) const
		{
			return (bool)m_value;
		}
		template<>
		bool getBool<const char*>(bool defaultValue) const
		{
			return !getString().empty();
		}
		template<>
		bool getBool<std::string>(bool defaultValue) const
		{
			return getBool<const char*>(defaultValue);
		}
		template<class T>
		void write(std::ostream& stream, bool indent, int& indentLevel) const
		{
			stream << getString<T>();
		}
		template<>
		void write<const char*>(std::ostream& stream, bool indent, int& indentLevel) const
		{
			stream << '"';
			for (char c : getString<std::string>())
			{
				if (g_escapes.find(c) != g_escapes.end())
				{
					stream << g_escapes.at(c);
				}
				else
				{
					stream << c;
				}
			}
			stream << '"';
		}
		template<>
		void write<std::string>(std::ostream& stream, bool indent, int& indentLevel) const
		{
			write<const char*>(stream, indent, indentLevel);
		}
		T m_value;
	};

}

