#pragma once
#include "IJsonValue.h"
#include "JsonIO.h"

namespace json
{
	class JsonValue;

	class JSONIO_API JObject : public IJsonValue
	{
	public:
		JObject() {}
		JObject(std::initializer_list < std::pair<const std::string, JsonValue>> properties);
		JObject(const JObject& copy)
		{
			read(copy.getString());
		}
		JObject(const std::string& json) { read(json); }
		~JObject() { m_properties.clear(); }
		bool isEmpty() const { return m_properties.empty(); }
		E_JsonType getType() const override { return E_JsonType::Object; }
		bool getBool(bool defaultValue = false) const override { return !isEmpty(); }
		short getShort(short defaultValue = 0) const override { return (short)m_properties.size(); }
		int getInt(int defaultValue = 0) const override { return (int)m_properties.size(); }
		float getFloat(float defaultValue = 0) const override { return (float)m_properties.size(); }
		double getDouble(double defaultValue = 0) const override { return (double)m_properties.size(); }
		std::string getString(const std::string& defaultValue = "") const override;
		JObject& getObject() override { return *this; }
		JArray& getArray() override;
		const JObject& getObject() const override { return *this; };
		const JArray& getArray() const override; 
		virtual JsonValue& operator[](const std::string& key) override;
		const JsonValue& operator[](const std::string& key) const override;
		const JsonValue& operator[](int index) const override;
		virtual JsonValue& operator[](int index) override;
		void write(std::ostream& stream, bool indent, int& indentLevel) const override;
		std::vector<std::string> getKeys() const;
		void clear() { m_properties.clear(); }
		void write(std::ostream& stream, bool indent = false) { int indentLevel = 0; write(stream, indent, indentLevel); }
		virtual bool read(std::istream& stream);
		virtual bool read(const std::string& json);
		JsonValue& find(const std::string& path);
		bool exists(const std::string& key) const;
		size_t size() const { return (int)m_properties.size(); }
	private:
		std::map<std::string, JsonValue> m_properties;
	};

	class JSONIO_API JObjectError : public JObject
	{
	public:
		E_JsonType getType() const override { return E_JsonType::Error; }
		JsonValue& operator[](const std::string& key) override;
		JsonValue& operator[](int index) override;
		bool read(std::istream& stream) override;
		bool read(const std::string& json) override;
	};

	class JSONIO_API JObjectProvider
	{
	public:
		static JObject& getObjectError();
	private:
		static JObjectError m_error;
	};
}


