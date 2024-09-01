#include "pch.h"
#include "JsonIOHelper.h"
#include "JsonValue.h"

namespace json
{
	const std::map<std::string, char> g_escapes_read = { {"\\r", '\r'} , {"\\n", '\n'}, {"\\t", '\t' }, { "\\f", '\f' }, { "\\v", '\v' } };

	bool JsonIOHelper::tryGetNumber(const std::string& value, double& result, E_JsonType& type)
	{
		char* end = nullptr;
		type = E_JsonType::Undefined;
		double val = strtod(value.c_str(), &end);
		if (end != value.c_str() && *end == '\0' && val != HUGE_VAL)
		{
			result = val;
			short s = (short)val;
			if (s == val)
			{
				type = E_JsonType::Short;
			}
			else
			{
				int i = (int)val;
				if (i == val)
				{
					type = E_JsonType::Int;
				}
				else
				{
					type = E_JsonType::Double;
				}
			}

			return true;
		};
		return false;
	}

	bool JsonIOHelper::ichar_equals(char a, char b)
	{
		return std::tolower(static_cast<unsigned char>(a)) ==
			std::tolower(static_cast<unsigned char>(b));
	}

	bool JsonIOHelper::iequals(const std::string& a, const std::string& b)
	{
		return a.size() == b.size() &&
			std::equal(a.begin(), a.end(), b.begin(), ichar_equals);
	}

	void JsonIOHelper::handleIndent(std::ostream& stream, bool indent, int& indentLevel, E_IndentMode mode)
	{
		switch (mode)
		{
		case E_IndentMode::Increment: indentLevel++; break;
		case E_IndentMode::Decrement: indentLevel--; break;
		default:break;
		}
		if (indent)
		{
			stream << std::endl;
		}
	}

	std::ostream& JsonIOHelper::doIndent(std::ostream& stream, bool indent, int& indentLevel)
	{
		stream << std::string(indent * indentLevel * 4, ' ');
		return stream;
	}

	bool JsonIOHelper::findFirstNonSpaceCharacter(std::istream& stream)
	{
		while (!stream.eof())
		{
			char c = stream.peek();
			if (std::isspace(c))
			{
				stream.get();
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	E_JsonValueType JsonIOHelper::readValueType(std::istream& stream)
	{
		if (findFirstNonSpaceCharacter(stream))
		{
			switch (stream.peek())
			{
			case '{': return E_JsonValueType::Object;
			case '[': return E_JsonValueType::Array;
			case '"': return E_JsonValueType::StringPrimitive;
			default: return E_JsonValueType::NonStringPrimitive;
			}
		}
		return E_JsonValueType::Error;
	}


	bool JsonIOHelper::readNextCharacter(std::istream& stream, char expectedChar)
	{
		while (!stream.eof() && stream.peek() != expectedChar)
		{
			stream.get();
		}
		if (stream.peek() == expectedChar)
		{
			stream.get();
			return true;
		}
		return false;
	}

	bool JsonIOHelper::readStringValue(std::istream& stream, std::string& value)
	{
		std::ostringstream s;
		if (readNextCharacter(stream, '"'))
		{
			while (!stream.eof() && stream.peek() != '"')
			{
				char c = (char)stream.get();
				if (c == '\\')
				{
					std::ostringstream skey;
					skey << c << (char)stream.peek();
					std::string key = skey.str();
					if (g_escapes_read.find(key) != g_escapes_read.end())
					{
						stream.get();
						c = g_escapes_read.at(key);
					}
				}
				s << c;
			}
			if (readNextCharacter(stream, '"'))
			{
				value = s.str();
				return true;
			}
		}
		return false;
	}

	bool JsonIOHelper::isNumber(const std::string& value)
	{
		double d;
		E_JsonType t;
		return tryGetNumber(value, d, t);
	}

	bool JsonIOHelper::setNonStringPrimitiveValue(std::string& primitive, double& result, E_JsonType& type)
	{
		if (iequals(primitive, "null"))
		{
			type = E_JsonType::Null;
			return true;
		}
		if (iequals(primitive, "true") || iequals(primitive, "false"))
		{
			result = (double)(iequals(primitive, "true"));
			type = E_JsonType::Bool;
			return true;
		}
		else
		{
			return tryGetNumber(primitive, result, type);
		}

	}

	bool JsonIOHelper::readNonStringPrimitive(std::istream& stream, double& result, E_JsonType& type)
	{
		std::ostringstream s;
		char c;
		while (!stream.eof())
		{
			c = stream.peek();
			if (!std::isspace(c) && c != '}' && c != ']' && c != ',')
			{
				s << (char)stream.get();
			}
			else
			{
				break;
			}
		}
		std::string value = s.str();
		return !value.empty() && setNonStringPrimitiveValue(value, result, type);
	}

	bool JsonIOHelper::read(std::istream& stream, JsonValue& value, bool& hasNext)
	{
		bool isOk = false;
		E_JsonValueType type = JsonIOHelper::readValueType(stream);
		switch (type)
		{
		case E_JsonValueType::Object:
		{
			JObject o;
			if (o.read(stream))
			{
				value = o;
				isOk = true;
			}
			break;

		}
		case E_JsonValueType::StringPrimitive:
		{
			std::string s;
			if (JsonIOHelper::readStringValue(stream, s))
			{
				value = s;
				isOk = true;
			}
			break;
		}
		case E_JsonValueType::Array:
		{
			JArray a;
			if (a.read(stream))
			{
				value = a;
				isOk = true;
			}
			break;
		}
		case E_JsonValueType::NonStringPrimitive:
		{
			double d;
			E_JsonType t;
			if (JsonIOHelper::readNonStringPrimitive(stream, d, t))
			{
				switch (t)
				{
				case E_JsonType::Bool: value = (bool)d; break;
				case E_JsonType::Short: value = (short)d; break;
				case E_JsonType::Int: value = (int)d; break;
				case E_JsonType::Float: value = (float)d; break;
				case E_JsonType::Null: value = nullptr; break;
				default: value = d; break;
				}
				isOk = true;
			}
			break;
		}
		default:
			break;
		}
		if (!isOk)
		{
			return false;
		}
		hasNext = JsonIOHelper::findFirstNonSpaceCharacter(stream) && stream.peek() == ',';
		if (hasNext)
		{
			stream.get();
		}
		else
		{
			isOk = stream.peek() == '}' || stream.peek() == ']';
		}
		return isOk;
	}

}

