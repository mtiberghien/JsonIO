#pragma once
#include "JsonIO.h"

namespace json
{
	class JSONIO_API IJsonContainer
	{
	public:
		virtual ~IJsonContainer() {}
		virtual std::size_t size() const = 0;
		virtual void clear() = 0;
		virtual bool isEmpty() const = 0;
	};
}


