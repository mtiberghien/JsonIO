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
		virtual void writeObject(JObject& object) = 0;
		virtual void readObject(const JObject& object) = 0;

	};

}


