#include "pch.h"
#include "IJsonSerializable.h"
#include "JsonValue.h"

namespace json
{

	std::string IJsonSerializable::serialize(bool indent)
	{
		std::ostringstream s;
		serialize(s, indent);
		return s.str();
	}

	bool IJsonSerializable::serialize(std::ostream& stream, bool indent)
	{
		if (stream.good())
		{
			JObject j;
			writeObject(j);
			j.write(stream, indent);
		}
		return false;
	}

	bool IJsonSerializable::deserialize(std::istream& stream)
	{
		JObject o;
		bool result = false;
		if (stream.good())
		{
			result = o.read(stream);
			if (result)
			{
				readObject(o);
			}
		}
		return result;
	}

	bool IJsonSerializable::deserialize(std::string& json)
	{
		std::istringstream s{ json };
		return deserialize(s);
	}
}


