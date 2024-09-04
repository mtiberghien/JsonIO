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
		virtual void toObject(JObject& object) = 0;
		virtual void fromObject(const JObject& object) = 0;

	};

	class JSONIO_API JsonSerializable : public IJsonSerializable
	{
	public:
		virtual ~JsonSerializable() {}
		std::string serialize(bool indent = false);
		bool serialize(std::ostream& stream, bool indent = false);
		bool deserialize(std::istream& stream);
		bool deserialize(std::string& string);
	};

}


