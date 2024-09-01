#include "pch.h"
#include "JsonItem.h"
#include "JsonIOHelper.h"

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
}

