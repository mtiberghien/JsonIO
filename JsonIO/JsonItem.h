#pragma once
#include "IJsonValue.h"

namespace json
{
	class JsonItem : public IJsonValue
	{
	public:
		virtual ~JsonItem() {}
		bool operator==(const IJsonValue& other) { return this->getString() == other.getString(); }
		explicit operator bool() const { return getBool(); }
		explicit operator short() const { return getShort(); }
		explicit operator int() const { return getInt(); }
		explicit operator float() const { return getFloat(); }
		explicit operator double() const { return getDouble(); }
		operator std::string() const { return getString(); }
		operator JObject& () { return getObject(); }
		operator JArray& () { return getArray(); }
	    operator const JObject& () const { return getObject(); }
		operator const JArray& () const { return getArray(); }

	};

}

