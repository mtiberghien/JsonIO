#pragma once
#include "JsonItem.h"
#include "JsonIO.h"
#include <vector>
#include "IJsonContainer.h"

namespace json
{
	class JsonValue;

	class JSONIO_API JObject : public JsonItem, public IJsonContainer
	{
	public:
		JObject() {}
		JObject(std::initializer_list < std::pair<const std::string, JsonValue>> properties);
		JObject(const std::string& json) { read(json); }
		virtual ~JObject() { m_properties.clear(); }
		bool isEmpty() const override { return m_properties.empty(); }
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
		void clear() override { m_properties.clear(); }
		void write(std::ostream& stream, bool indent = false) { int indentLevel = 0; write(stream, indent, indentLevel); }
		virtual bool read(std::istream& stream);
		virtual bool read(const std::string& json);
		JsonValue& find(const std::string& path);
		bool exists(const std::string& key) const;
		size_t size() const override { return (int)m_properties.size(); }
		virtual JObject& operator=(const JObject& object);
		std::map<std::string, JsonValue>::const_iterator begin() const { return m_properties.begin(); }
		std::map<std::string, JsonValue>::const_iterator end() const { return m_properties.end(); }
	private:
		std::map<std::string, JsonValue> m_properties;
	};

	class JObjectError : public JObject
	{
	public:
		JObjectError() {}
		JObjectError(const JObject&& object) {}
		E_JsonType getType() const override { return E_JsonType::Error; }
		JsonValue& operator[](const std::string& key) override;
		JsonValue& operator[](int index) override;
		bool read(std::istream& stream) override;
		bool read(const std::string& json) override;
		JObject& operator=(const JObject& object) override { return *this; }
	};

	class JObjectProvider
	{
	public:
		static JSONIO_API JObject& getObjectError();
	private:
		static JObjectError m_error;
	};
}


