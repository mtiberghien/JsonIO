#pragma once
#include "JsonIO.h"
#include "JObject.h"

namespace json
{
	class JSONIO_API IJsonSerializable
	{
	public:
		IJsonSerializable() {};
		virtual ~IJsonSerializable() {};
		std::string serialize(bool indent = false);
		bool serialize(std::ostream& stream, bool indent = false);
		bool deserialize(std::istream& stream);
		bool deserialize(std::string& string);
		virtual void toObject(JObject& object) = 0;
		virtual void fromObject(const JObject& object) = 0;

	};

}


