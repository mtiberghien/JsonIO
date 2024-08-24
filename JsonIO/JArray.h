#pragma once
#include "IJsonValue.h"
#include "JsonIO.h"


namespace json
{
	class JsonValue;

	class JSONIO_API JArray : public IJsonValue
	{
	public:
		JArray();
		JArray(const JArray& copy)
		{
			read(copy.getString());
		}
		JArray(std::initializer_list<JsonValue> items) : m_items(items) {}
		~JArray() { m_items.clear(); }
		bool isEmpty() const { return m_items.empty(); }
		E_JsonType getType() const override { return E_JsonType::Array; }
		bool getBool(bool defaultValue = false) const override { return !isEmpty(); }
		short getShort(short defaultValue = 0) const override { return (short)m_items.size(); }
		int getInt(int defaultValue = 0) const override { return (int)m_items.size(); }
		float getFloat(float defaultValue = 0) const override { return (float)m_items.size(); }
		double getDouble(double defaultValue = 0) const override { return (double)m_items.size(); }
		JObject& getObject() override;
		JArray& getArray() override { return *this; }
		const JObject& getObject() const override;
		const JArray& getArray() const override { return *this; }
		const JsonValue& operator[](const std::string& key) const override;
		const JsonValue& operator[](int index) const override;
		std::string getString(const std::string& defaultValue = "") const override;
		JsonValue& operator[](const std::string& key) override;
		JsonValue& operator[](int index) override;
		void write(std::ostream& stream, bool indent, int& indentLevel) const override;
		virtual bool read(std::istream& stream);
		virtual bool read(const std::string& json);
		virtual void push_back(JsonValue&& item) { m_items.push_back(item); }
		virtual void push_back(const JsonValue& item) { m_items.push_back(item); }
		virtual void operator<<(const JsonValue& item) { m_items.push_back(item); }
		void clear() { m_items.clear(); }
		std::size_t size() const { return m_items.size(); }
	private:
		std::vector<JsonValue> m_items;
	};

	class JSONIO_API JArrayError : public JArray
	{
		E_JsonType getType() const override { return E_JsonType::Error; }
		bool read(std::istream& stream) override { return false; }
		bool read(const std::string& json) override { return false; }
		void push_back(JsonValue&& item) override {}
		void push_back(const JsonValue& item) override {}
		void operator<<(const JsonValue& item) override {}
	};

	class JSONIO_API JArrayProvider
	{
	public:
		static JArray& getArrayError();
	private:
		static JArrayError m_error;
	};
}

