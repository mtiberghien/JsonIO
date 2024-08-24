#pragma once
#include "pch.h"


namespace json
{
	const std::map<std::string, char> g_escapes_read = { {"\\r", '\r'} , {"\\n", '\n'}, {"\\t", '\t' }, { "\\f", '\f' }, { "\\v", '\v' } };
	enum E_IndentMode { Increment, Decrement, None };

	static bool ichar_equals(char a, char b)
	{
		return std::tolower(static_cast<unsigned char>(a)) ==
			std::tolower(static_cast<unsigned char>(b));
	}

	static bool iequals(const std::string& a, const std::string& b)
	{
		return a.size() == b.size() &&
			std::equal(a.begin(), a.end(), b.begin(), ichar_equals);
	}

	static void handleIndent(std::ostream& stream, bool indent, int& indentLevel, E_IndentMode mode = E_IndentMode::None)
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

	static std::ostream& doIndent(std::ostream& stream, bool indent, int& indentLevel)
	{
		stream << std::string(indent * indentLevel * 4, ' ');
		return stream;
	}

	static bool findFirstNonSpaceCharacter(std::istream& stream)
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

	static E_JsonValueType readValueType(std::istream& stream)
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


	static bool readNextCharacter(std::istream& stream, char expectedChar)
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

	static bool readStringValue(std::istream& stream, std::string& value)
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

	static bool tryGetNumber(const std::string& value, double& result, E_JsonType& type)
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

	static bool  isNumber(const std::string& value)
	{
		double d;
		E_JsonType t;
		return tryGetNumber(value, d, t);
	}

	static bool setNonStringPrimitiveValue(std::string& primitive, double& result, E_JsonType& type)
	{
		if (json::iequals(primitive, "null"))
		{
			type = E_JsonType::Null;
			return true;
		}
		if (json::iequals(primitive , "true") || json::iequals( primitive , "false"))
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

	static bool readNonStringPrimitive(std::istream& stream, double& result, E_JsonType& type)
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
}


