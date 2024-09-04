#include "pch.h"
#include "IJsonSerializable.h"
#include "JsonValue.h"

namespace json
{

	std::string JsonSerializable::serialize(bool indent)
	{
		std::ostringstream s;
		serialize(s, indent);
		return s.str();
	}

	bool JsonSerializable::serialize(std::ostream& stream, bool indent)
	{
		if (stream.good())
		{
			JObject j;
			toObject(j);
			j.write(stream, indent);
		}
		return false;
	}

	bool JsonSerializable::deserialize(std::istream& stream)
	{
		JObject o;
		bool result = false;
		if (stream.good())
		{
			result = o.read(stream);
			if (result)
			{
				fromObject(o);
			}
		}
		return result;
	}

	bool JsonSerializable::deserialize(std::string& json)
	{
		std::istringstream s{ json };
		return deserialize(s);
	}
}


