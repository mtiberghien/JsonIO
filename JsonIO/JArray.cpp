#include "pch.h"
#include "JArray.h"
#include "JsonValue.h"
#include "JsonIOHelper.h"
#include "JObject.h"

namespace json
{
	JArray::JArray() {};
	JsonValue& JArray::operator[](const std::string& key) {
		double d;
		E_JsonType t;
		if (JsonIOHelper::tryGetNumber(key, d, t))
		{
			return m_items[(int)d];
		}
		return JVoidProvider::getError();
	}

	const JsonValue& JArray::operator[](const std::string& key) const {
		double d;
		E_JsonType t;
		if (JsonIOHelper::tryGetNumber(key, d, t))
		{
			return m_items.at((int)d);
		}
		return JVoidProvider::getError();
	}

	JsonValue& JArray::operator[](int index)
	{
		if (index >= 0 && (unsigned int)index < m_items.size())
		{
			return m_items[index];
		}
		return JVoidProvider::getError();
	}

	const JsonValue& JArray::operator[](int index) const
	{
		if (index >= 0 && (unsigned int)index < m_items.size())
		{
			return m_items.at(index);
		}
		return JVoidProvider::getError();
	}

	JArray& JArray::operator=(const JArray& array)
	{
		read(array.getString());
		return *this;
	}

	void JArray::write(std::ostream& stream, bool indent, int& indentLevel) const
	{
		if (!isEmpty())
		{
			JsonIOHelper::doIndent(stream, indent, indentLevel);
		}
		stream << "[";
		if (!isEmpty())
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Increment);
			for (unsigned int i = 0; i < m_items.size(); i++)
			{
				JsonIOHelper::doIndent(stream, indent, indentLevel);
				m_items[i].write(stream, indent, indentLevel);
				if (i < m_items.size() - 1)
				{
					stream << ", ";
					JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
				}

			}
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Decrement);
			JsonIOHelper::doIndent(stream, indent, indentLevel);
		}
		stream << "]";
		if (indentLevel == 0)
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
		}
	}

	std::string JArray::getString(const std::string& defaultValue) const
	{
		std::ostringstream s;
		int indentLevel = 0;
		write(s, false, indentLevel);
		return s.str();
	}

	bool JArray::read(std::istream& stream)
	{
		clear();
		if (JsonIOHelper::readNextCharacter(stream, '['))
		{
			bool hasNext = false;
			bool isOk = false;
			do
			{
				m_items.push_back({});
				isOk = JsonIOHelper::read(stream, m_items.back(), hasNext);
			} while (isOk && hasNext);
			if (!isOk)
			{
				return false;
			}
			if (JsonIOHelper::readNextCharacter(stream, ']'))
			{
				return true;
			}
		}
		return false;
	}

	bool JArray::read(const std::string& json)
	{
		std::istringstream s{ json };
		return read(s);
	}


	JObject& JArray::getObject()
	{
		return JObjectProvider::getObjectError();
	}

	const JObject& JArray::getObject() const
	{
		return JObjectProvider::getObjectError();
	}

	JArrayError JArrayProvider::m_error;

	JArray& JArrayProvider::getArrayError()
	{
		return m_error;
	}
}

