#include "pch.h"
#include "JsonItem.h"
#include "JsonIOHelper.h"
#include "JUndefined.h"
#include "JsonValue.h"

namespace json
{
	std::string toString(E_JsonType type)
	{
		switch (type)
		{
		case E_JsonType::Object: return "Object";
		case E_JsonType::Array: return "Array";
		case E_JsonType::Bool: return "Bool";
		case E_JsonType::Short: return "Short";
		case E_JsonType::Int: return "Int";
		case E_JsonType::Float: return "Float";
		case E_JsonType::Double: return "Double";
		case E_JsonType::String: return "String";
		case E_JsonType::Null: return "Null";
		case E_JsonType::Error: return "Error";
		default: return "Undefined";
		}
	}

	bool JsonItem::isNumber() const
	{
		if (isPrimitive() && !isNull())
		{
			if (!isString())
			{
				return true;
			}
			else
			{
				double d;
				E_JsonType t;
				return JsonIOHelper::tryGetNumber(getString(), d, t);
			}
		}
		return false;
	}

	bool findFieldPart(std::istream& stream, std::string& fieldPart)
	{
		std::ostringstream s;
		int sizeRead = 0;
		while (!stream.eof())
		{
			char c = stream.get();
			bool terminationChar = c == '[' || c == ']' || c == '.';
			if (terminationChar && sizeRead > 0)
			{
				break;
			}
			if (!terminationChar && !isspace(c) && c!=EOF)
			{
				s << c;
				sizeRead++;
			}
		}
		fieldPart = s.str();
		return sizeRead > 0;
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

	JsonValue& getValueRecursive(JsonValue& result, std::istream& stream)
	{
		if (!result.isError())
		{
			std::string fieldPart;
			if (findFieldPart(stream, fieldPart))
			{
				return getValueRecursive(result[fieldPart], stream);
			}
		}

		return result;
	}

	const JsonValue& getValueRecursive(const JsonValue& result, std::istream& stream)
	{
		if (!result.isError())
		{
			std::string fieldPart;
			if (findFieldPart(stream, fieldPart))
			{
				return getValueRecursive(result[fieldPart], stream);
			}
		}

		return result;
	}


	JsonValue& JsonItem::find(const std::string& path)
	{
		std::string fieldPart;
		std::istringstream ps{ path };
		if (findFieldPart(ps, fieldPart))
		{
			return getValueRecursive(this->operator[](fieldPart), ps);
		}

		return JVoidProvider::getError();
	}

	const JsonValue& JsonItem::find(const std::string& path) const
	{
		std::string fieldPart;
		std::istringstream ps{ path };
		if (findFieldPart(ps, fieldPart))
		{
			return getValueRecursive(this->operator[](fieldPart), ps);
		}

		return JVoidProvider::getError();
	}
}

