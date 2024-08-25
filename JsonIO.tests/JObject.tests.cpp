#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Json.h"

using namespace json;

namespace JsonIOtests
{
	TEST_CLASS(JObjecttests)
	{
	public:
		
		TEST_METHOD(JObjectIsEmpty)
		{
			JObject o;
			Assert::IsTrue(o.isEmpty(), L"o doit etre vide");
		}

		TEST_METHOD(JObjectAddBool)
		{
			JObject o;
			o["b"] = true;
			o["b2"] = false;
			Assert::AreEqual(2, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue( o["b"].getType() == E_JsonType::Bool);
			Assert::IsTrue(o["b"].getBool());
			Assert::IsFalse(o["b2"].getBool());
		}

		TEST_METHOD(JObjectAddShort)
		{
			JObject o;
			o["s"] = (short)10;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["s"].getType() == E_JsonType::Short);
			Assert::AreEqual((short)10, o["s"].getShort());
		}

		TEST_METHOD(JObjectAddInt)
		{
			JObject o;
			o["i"] = 33000;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["i"].getType() == E_JsonType::Int);
			Assert::AreEqual(33000, o["i"].getInt());
		}

		TEST_METHOD(JObjectAddFloat)
		{
			JObject o;
			o["f"] = (float)3.14;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["f"].getType() == E_JsonType::Float);
			Assert::AreEqual((float)3.14, o["f"].getFloat());
		}

		TEST_METHOD(JObjectAddDouble)
		{
			JObject o;
			o["d"] = 3.141592653;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["d"].getType() == E_JsonType::Double);
			Assert::AreEqual(3.141592653, o["d"].getDouble());
		}

		TEST_METHOD(JObjectAddString)
		{
			JObject o;
			o["st"] = "Test";
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["st"].getType() == E_JsonType::String);
			Assert::AreEqual(std::string{"Test"}, o["st"].getString());
		}

		TEST_METHOD(JObjectAddObject)
		{
			JObject o;
			JObject o2;
			o2["s"] = "Test";
			o["o"] = o2;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["o"].getType() == E_JsonType::Object);
			Assert::AreEqual(std::string{ "Test" }, o["o"]["s"].getString());
		}

		TEST_METHOD(JObjectAddArray)
		{
			JObject o;
			JArray a;
			for (int i = 1; i <= 3; i++)
			{
				a.push_back(i);
			}
			o["a"] = a;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["a"].getType() == E_JsonType::Array);
			Assert::AreEqual(3, o["a"].getInt());
			for (int i = 1; i <= 3; i++)
			{
				Assert::AreEqual(i, o["a"][i - 1].getInt());
			}
		}

		TEST_METHOD(JObjectAddNull)
		{
			JObject o;
			o["n"] = nullptr;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["n"].getType() == E_JsonType::Null);
			Assert::AreEqual(std::string{ "null" }, o["n"].getString());
		}

		TEST_METHOD(JObjectReadString)
		{
			std::string json{ R"({"b": true, "s": 1, "i": 33000, "f": 3.14, "d": 3.141592653, "st": "Test", "n": null, "o": {"s": "Test"}, "a": [1, 2, 3]})" };
			JObject o;
			Assert::IsTrue(o.read(json));
			Assert::AreEqual(9, o.getInt());
			Assert::IsTrue(o["b"].getType() == E_JsonType::Bool, L"b doit etre booleen");
			Assert::IsTrue(o["b"].getBool(), L"b doit etre vrai");
			Assert::IsTrue(o["s"].getType() == E_JsonType::Short, L"s doit etre short");
			Assert::AreEqual((short)1, o["s"].getShort());
			Assert::IsTrue(o["i"].getType() == E_JsonType::Int, L"i doit etre int");
			Assert::AreEqual(33000, o["i"].getInt());
			Assert::IsTrue(o["f"].getType() == E_JsonType::Double, L"f doit etre double");
			Assert::AreEqual((float)3.14, o["f"].getFloat());
			Assert::IsTrue(o["d"].getType() == E_JsonType::Double, L"d doit etre double");
			Assert::AreEqual(3.141592653, o["d"].getDouble());
			Assert::IsTrue(o["st"].getType() == E_JsonType::String, L"st doit etre string");
			Assert::AreEqual(std::string{ "Test" }, o["st"].getString());
			Assert::IsTrue(o["n"].getType() == E_JsonType::Null, L"n doit etre null");
			Assert::AreEqual(std::string{ "null" }, o["n"].getString());
			Assert::IsTrue(o["o"].getType() == E_JsonType::Object, L"o doit etre object");
			Assert::AreEqual(1, o["o"].getInt());
			Assert::AreEqual(std::string{ "Test" }, o["o"]["s"].getString());
			Assert::IsTrue(o["a"].getType() == E_JsonType::Array, L"a doit etre array");
			Assert::AreEqual(3, o["a"].getInt());
			for (int i = 1; i <= 3; i++)
			{
				Assert::AreEqual(i, o["a"][i - 1].getInt());
			}
		}

		TEST_METHOD(JObjectGetString)
		{
			std::string json{ R"({"a": true, "b": 1, "c": 33000, "d": 3.14, "e": 3.141592653, "f": "Test", "g": {"h": "Test"}, "i": [1, 2, 3], "j": null})" };
			JObject o;
			Assert::IsTrue(o.read(json));
			Assert::AreEqual(9, o.getInt());
			Assert::AreEqual(json, o.getString());
		}

		TEST_METHOD(JObjectWrite)
		{
			std::string json{ R"({"a": true, "b": 1, "c": 33000, "d": 3.14, "e": 3.141592653, "f": "Test", "g": {"h": "Test"}, "i": [1, 2, 3], "j": null})" };
			std::string json2{ R"({
    "a": true, 
    "b": 1, 
    "c": 33000, 
    "d": 3.14, 
    "e": 3.141592653, 
    "f": "Test", 
    "g": 
    {
        "h": "Test"
    }, 
    "i": 
    [
        1, 
        2, 
        3
    ]
}
)" };
			JObject o;
			JObject o2;
			Assert::IsTrue(o.read(json));
			Assert::IsTrue(o2.read(json2));
			std::ostringstream s;
			std::ostringstream s2;
			o.write(s);
			o2.write(s2, true);
			Assert::AreEqual(json, s.str());
			Assert::AreEqual(json2, s2.str());
		}

		TEST_METHOD(JObjectFind)
		{
			std::string json{ R"({"b": true, "s": 1, "i": 33000, "f": 3.14, "d": 3.141592653, "st": "Test", "o": {"s": "Test"}, "a": [1, {"value": 2}, 3]})" };
			JObject o{ json };
			JsonValue& v = o.find("o.s");
			Assert::IsTrue(v.getType() == E_JsonType::String);
			Assert::AreEqual(std::string{ "Test" }, v.getString());
			v = o.find("a[1].value");
			Assert::IsTrue(v.getType() == E_JsonType::Short);
			Assert::AreEqual(2, v.getInt());
		}

		TEST_METHOD(JObjectInit)
		{
			JObject o{ {"a",1},{"b", false}, {"c", JArray{1,"2", nullptr}} };
			Assert::AreEqual(3, (int)o.size());
			Assert::AreEqual(1, (int)o["a"]);
			Assert::IsFalse((bool)o["b"]);
			Assert::AreEqual(3, (int)((JArray&)o["c"]).size());
			JObject o2{ o };
			Assert::IsTrue(o == o2);
			JsonValue v = o;
			Assert::IsTrue(v == o);
		}
	};
}
