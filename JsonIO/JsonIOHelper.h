#pragma once
#include "IJsonValue.h"
#include <map>
#include <string>


namespace json
{
	enum E_IndentMode { Increment, Decrement, None };

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

	class JsonIOHelper
	{
	public:
		static bool ichar_equals(char a, char b);

		static bool iequals(const std::string& a, const std::string& b);

		static void handleIndent(std::ostream& stream, bool indent, int& indentLevel, E_IndentMode mode = E_IndentMode::None);

		static std::ostream& doIndent(std::ostream& stream, bool indent, int& indentLevel);

		static bool findFirstNonSpaceCharacter(std::istream& stream);


		static E_JsonValueType readValueType(std::istream& stream);


		static bool readNextCharacter(std::istream& stream, char expectedChar);

		static bool readStringValue(std::istream& stream, std::string& value);

		static bool  isNumber(const std::string& value);

		static bool setNonStringPrimitiveValue(std::string& primitive, double& result, E_JsonType& type);

		static bool readNonStringPrimitive(std::istream& stream, double& result, E_JsonType& type);

		static bool read(std::istream&, JsonValue& value, bool& hasNext);

	};

	
}


