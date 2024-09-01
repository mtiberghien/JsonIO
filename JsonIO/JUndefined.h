#pragma once
#include "IJsonValue.h"
#include "JsonIO.h"

namespace json
{
	class JSONIO_API  JUndefined : public IJsonValue
	{
	public:
		JUndefined() {}
		~JUndefined() {}
		E_JsonType getType() const override { return E_JsonType::Undefined; }
		bool getBool(bool defaultValue = false) const override { return defaultValue; }
		short getShort(short defaultValue = 0) const override { return defaultValue; }
		int getInt(int defaultValue = 0) const override { return defaultValue; }
		float getFloat(float defaultValue = 0) const override { return defaultValue; }
		double getDouble(double defaultValue = 0) const override { return defaultValue; }
		std::string getString(const std::string& defaultValue = "") const override { return defaultValue; }
		JObject& getObject() override;
		JArray& getArray() override;
		const JObject& getObject() const override;
		const JArray& getArray() const override;
		JsonValue& operator[](const std::string& key) override;
		const JsonValue& operator[](const std::string& key) const override;
		JsonValue& operator[](int index) override;
		const JsonValue& operator[](int index) const override;
		void write(std::ostream& stream, bool indent, int& indentLevel) const override {};
	};

	class JUndefinedError : public JUndefined
	{
	public :
		JUndefinedError() {}
		E_JsonType getType() const override { return E_JsonType::Error; }
	};

	class JSONIO_API JVoidProvider
	{
	public:
		static JsonValue& getError();
	private:
		static JsonValue m_void;
	};
}


