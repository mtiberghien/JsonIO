#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Json.h"

using namespace json;

namespace JsonIOtests
{
	TEST_CLASS(JsonValuetests)
	{
		TEST_METHOD(TestEquality)
		{
			JsonValue v = 1;
			JsonValue v2 = 1;
			Assert::AreEqual(1, (int)v);
			Assert::IsTrue(v == v2);
		}
	};
}