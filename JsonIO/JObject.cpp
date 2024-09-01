#include "pch.h"
#include "JObject.h"
#include "JsonValue.h"
#include "JArray.h"

namespace json
{
	JObject::JObject(std::initializer_list < std::pair<const std::string, JsonValue>> properties): m_properties(properties)
	{
	}

	JObject& JObject::operator=(const JObject& object)
	{
		read(object.getString());
		return *this;
	}

	JsonValue& JObject::operator[](const std::string& key) {
		if (!exists(key))
		{
			m_properties[key] = JsonValue{};
		}
		return m_properties[key];
	}

	const JsonValue& JObject::operator[](const std::string& key) const
	{
		if (!exists(key))
		{
			return JVoidProvider::getError();
		}
		return m_properties.at(key);
	}

	JsonValue& JObject::operator[](int index)
	{
		if (index >= 0 && (unsigned int)index < m_properties.size())
		{
			auto it = m_properties.begin();
			std::advance(it, index);
			return it->second;
		}
		return JVoidProvider::getError();
	}

	const JsonValue& JObject::operator[](int index) const
	{
		if (index >= 0 && (unsigned int)index < m_properties.size())
		{
			auto it = m_properties.begin();
			std::advance(it, index);
			return m_properties.at(it->first);
		}
		return JVoidProvider::getError();
	}

	std::vector<std::string> JObject::getKeys() const
	{
		std::vector<std::string> result;
		for (auto& p : m_properties)
		{
			result.push_back(p.first);
		}
		return result;
	}

	void writeProperty(std::ostream& stream, const std::string& key, const JsonValue& value, bool indent, int& indentLevel)
	{
		JsonIOHelper::doIndent(stream, indent, indentLevel) << '"' << key << '"' << ": ";
		if ((value.getType() == E_JsonType::Object || value.getType() == E_JsonType::Array) && value.getInt() > 0)
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
			if (value.getType() == E_JsonType::Object)
			{
				JsonIOHelper::doIndent(stream, indent, indentLevel);
			}
		}
		value.write(stream, indent, indentLevel);
	}

	void JObject::write(std::ostream& stream, bool indent, int& indentLevel) const
	{
		stream << "{";
		if (!isEmpty())
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Increment);
			for (auto it = m_properties.begin(); it != m_properties.end(); it++)
			{
				writeProperty(stream, it->first, it->second, indent, indentLevel);
				if (it != std::prev(m_properties.end()))
				{
					stream << ", ";
					JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
				}

			}
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Decrement);
			JsonIOHelper::doIndent(stream, indent, indentLevel);
		}
		stream << "}";
		if (indentLevel == 0)
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
		}
	}

	std::string JObject::getString(const std::string& defaultValue) const
	{
		std::ostringstream s;
		int indentLevel = 0;
		write(s, false, indentLevel);
		return s.str();
	}

	bool readKey(std::istream& stream, std::string& key)
	{
		if (JsonIOHelper::readStringValue(stream, key))
		{
			return JsonIOHelper::readNextCharacter(stream, ':');
		}
		return false;
	}

	bool readProperty(JObject& object, std::istream& stream, bool& hasNext)
	{
		std::string key;
		bool isOk = false;
		if (readKey(stream, key))
		{
			isOk = JsonIOHelper::read(stream, object[key], hasNext);
		}
		return isOk;
	}


	bool JObject::read(std::istream& stream)
	{
		clear();
		if (JsonIOHelper::readNextCharacter(stream, '{'))
		{
			bool hasNext = false;
			bool isOk = false;
			do
			{
				isOk = readProperty(*this, stream, hasNext);
			} while (isOk && hasNext);
			if (!isOk)
			{
				return false;
			}
			if (JsonIOHelper::readNextCharacter(stream, '}'))
			{
				return true;
			}
		}
		return false;
	}

	bool JObject::read(const std::string& json)
	{
		std::istringstream s{ json };
		return read(s);
	}

	JsonValue& getObjectValue(JObject& o, const std::string& fieldPath)
	{
		std::size_t pos = fieldPath.find('[');
		if (pos != std::string::npos)
		{
			std::size_t pos2 = fieldPath.find(']', pos);
			if (pos2 != std::string::npos)
			{
				std::string arrayIndex = fieldPath.substr(pos + 1, pos2 - pos - 1);
				std::string fieldId = fieldPath.substr(0, pos);
				if (o.exists(fieldId))
				{
					return o[fieldId][arrayIndex];
				}
			}
		}
		else if (o.exists(fieldPath))
		{
			return o[fieldPath];
		}

		return JVoidProvider::getError();
	}

	const JsonValue& getObjectValue(const JObject& o, const std::string& fieldPath)
	{
		std::size_t pos = fieldPath.find('[');
		if (pos != std::string::npos)
		{
			std::size_t pos2 = fieldPath.find(']', pos);
			if (pos2 != std::string::npos)
			{
				std::string arrayIndex = fieldPath.substr(pos + 1, pos2 - pos - 1);
				std::string fieldId = fieldPath.substr(0, pos);
				if (o.exists(fieldId))
				{
					return o[fieldId][arrayIndex];
				}
			}
		}
		else if (o.exists(fieldPath))
		{
			return o[fieldPath];
		}

		return JVoidProvider::getError();
	}

	JsonValue& getValueRecursive(JsonValue& result, std::istream& stream)
	{
		std::string field;
		if (std::getline(stream, field, '.'))
		{
			if (result.getType() == E_JsonType::Object)
			{
				return getValueRecursive(getObjectValue(result.getObject(), field), stream);
			}
			else
			{
				return JVoidProvider::getError();
			}
		}

		return result;
	}

	const JsonValue& getValueRecursive(const JsonValue& result, std::istream& stream)
	{
		std::string field;
		if (std::getline(stream, field, '.'))
		{
			if (result.getType() == E_JsonType::Object)
			{
				return getValueRecursive(getObjectValue(result.getObject(), field), stream);
			}
			else
			{
				return JVoidProvider::getError();
			}
		}

		return result;
	}


	JsonValue& JObject::find(const std::string& path)
	{
		std::string field;
		std::istringstream ps{ path };
		if (std::getline(ps, field, '.'))
		{
			return getValueRecursive(getObjectValue(*this, field), ps);
		}
		
		return JVoidProvider::getError();
	}

	const JsonValue& JObject::find(const std::string& path) const
	{
		std::string field;
		std::istringstream ps{ path };
		if (std::getline(ps, field, '.'))
		{
			return getValueRecursive(getObjectValue(*this, field), ps);
		}

		return JVoidProvider::getError();
	}

	bool JObject::exists(const std::string& key) const
	{
		return m_properties.find(key) != m_properties.end();
	}

	JArray& JObject::getArray()
	{
		return JArrayProvider::getArrayError();
	}

	const JArray& JObject::getArray() const
	{
		return JArrayProvider::getArrayError();
	}

	JsonValue& JObjectError::operator[](const std::string& key) 
	{
		return JVoidProvider::getError();
	}

	JsonValue& JObjectError::operator[](int index)
	{
		return JVoidProvider::getError();		
	}

	bool JObjectError::read(std::istream& stream)
	{
		return false;
	}

	bool JObjectError::read(const std::string& json)
	{
		return false;
	}

	JObjectError JObjectProvider::m_error;

	JObject& JObjectProvider::getObjectError()
	{
		return m_error;
	}
}


