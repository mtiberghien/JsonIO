// JsonModele.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Json.h"
#include <iostream>

using namespace json;


class Item : public JsonSerializable
{
public:
    Item() {}
    Item(const std::string& name, int id) : m_name(name), m_id(id) {}
    void toObject(JObject& object) override
    {
        object["name"] = m_name;
        object["id"] = m_id;
    }
    void fromObject(const JObject& object) override
    {
        m_name = object["name"].getString("default");
        m_id = object["id"].getInt(-1);
    }
    std::string getName() const { return m_name; }
    int getId() const { return m_id; }
private:
    std::string m_name;
    int m_id{ 0 };
};

class Items : public JsonSerializable
{
public:
    Items()
    {
    }
    void toObject(JObject& object) override
    {
        JArray l_devices;
        for (auto& d : m_items)
        {
            JObject o;
            d.toObject(o);
            l_devices.push_back(o);
        }
        object["devices"] = l_devices;
    }

    void fromObject(const JObject& object) override
    {
        m_items.clear();
        const JArray& items = object["devices"].getArray();
        for (const auto& v : items)
        {
            const JObject& o = v.getObject();
            Item it;
            it.fromObject(o);
            add(it);
        }
    }

    void add(const Item& d)
    {
        m_items.push_back(d);
    }
    void add(Item&& d)
    {
        m_items.push_back(d);
    }
    Item& operator[](int index)
    {
        return m_items[index];
    }
    std::string to_string()
    {
        return serialize(false);
    }
private:
    std::vector<Item> m_items;
};

std::ostream& operator<<(std::ostream& stream, E_JsonType type)
{
    stream << toString(type);
    return stream;
}



void TestValue(const JsonValue& v, E_JsonType type=E_JsonType::String)
{
    std::cout << v.getType() << ": ";
    switch (type)
    {
    case E_JsonType::Bool: std::cout << v.getBool(); break;
    case E_JsonType::Short: std::cout << v.getShort(); break;
    case E_JsonType::Int: std::cout << v.getInt(); break;
    case E_JsonType::Float: std::cout << v.getFloat(); break;
    case E_JsonType::Double: std::cout << std::setprecision(12) << v.getDouble(); break;
    case E_JsonType::String: std::cout << v.getString(); break;
    }
    std::cout << std::endl;
}

void ChangeValue(JsonValue& v)
{
    v = "test";
}

#include <fstream>

void TestJObject()
{
    JObject o;
    o["isOk"] = true;
    o["value"] = 3500;
    o["item"] = JObject{ R"({"id": 1, "name": "item"})" };
    o["price"] = 35.78;
    JArray a;
    JArray a2{ 10, "20", o["item"], nullptr };
    for (int i = 1; i < 4; i++)
    {
        a << JObject{ {"id", i} };
    }
    o["ids"] = a;
    o["values"] = a2;
    std::cout << o.getString() << std::endl;
    o.write(std::cout, true);
    std::cout << "ids[0]: " << o["ids"][0].getString() << std::endl;
    std::cout << "o[0][\"0\"]: " << (std::string)o[0]["0"] << std::endl;
    std::cout << "isOk: " << o["isOk"].getBool() << std::endl;
    std::cout << "values: " << o["values"].getString() << std::endl;
    std::cout << "values[1]: " << (int)o["values"][1] << std::endl;
    std::cout << "ids[2].id: " << o.find("ids[2].id").getInt() << std::endl;
    std::cout << "price: " << o["price"].getDouble() << std::endl;
    std::cout << "test default: " << o.find("path.does.not.exist").getString("not found") << std::endl;

    // Expected results
    /*

        ids[0]: {"id", 1}
        o[0]["0"]: {"id", 1}
        isOk: true
        values: [10, "20", {"id": 1, "name": "item"}, null]
        values[1]: 20
        ids[2].id: 3
        price: 35.78
        test default: not found
    */
}



void TestSerialization()
{
    Items d;
    d.add({ "device_1", 1 });
    d.add({ "device_2" , 2 });
    d.serialize(std::cout, true);
    Items d2;
    std::string djson = d.serialize(false);
    d2.deserialize(djson);
    d2.add({ "device_3", 3 });
    std::cout << d2.to_string() << std::endl;
    std::cout << "items[1]: " << d2[1].getName() << std::endl;

    // Expected results
    /*
    {
        "devices":
        [
            {
               "id": 1,
               "name": "device_1"
            },
            {
                "id": 2,
                "name": "device_2"
            }
        ]
    }
    {"devices": [{"id": 1, "name": "device_1"}, {"id": 2, "name": "device_2"}, {"id": 3, "name": "device_3"}]}
    items[1]: device_2
    */
}




int main()
{
   E_JsonType types[] = { E_JsonType::Bool, E_JsonType::Short, E_JsonType::Int, E_JsonType::Float, E_JsonType::Double, E_JsonType::String };
    JObject o;
    JObject so;
    JArray ta;
    ta.push_back(1);
    ta.push_back("test");
    so["hello"] = "hello\nworld";
    o["b"] = true;
    o["s"] = 2;
    o["i"] = 400000;
    o["f"] = 3.14;
    o["d"] = 3.141592653;
    o["st"] = "Hello";
    o["o"] = so;
    o["t"] = ta;
    o["n"] = nullptr;
    for (auto t : types)
    {
        std::cout << "\tGet" << t << std::endl;
        unsigned short s = 7;
        TestValue(true, t);
        TestValue(false, t);
        TestValue(s, t);
        TestValue(2024, t);
        TestValue(-2024, t);
        TestValue((float)3.14, t);
        TestValue(3.141592653, t);
        TestValue("Hello", t);
        TestValue("123.456", t);
        TestValue("", t);
        TestValue(o, t);
        TestValue(ta, t);
        TestValue(nullptr, t);
    }

    JsonValue v(1);
    TestValue(v);
    ChangeValue(v);
    TestValue(v);

    TestValue(o["o"]["hello"]);
    TestValue(o[1]);
    JObject& oRef = o["o"].getObject();
    JArray& tRef = o["t"].getArray();
    std::string ojson = o.getString();
    JObject o2;
    o2.read(ojson);
    TestValue(o2["o"]["hello"]);
    oRef.clear();
    tRef.clear();
    for (int i = 0; i < 3; i++)
    {
        tRef.push_back(i);
    }
    oRef["tab"] = ta;
    oRef["o2"] = JObject{ R"({"a": 1, "b": "Test", "c": [1, "deux", {"d":true}]})" };
    o[10] = "test";
    TestValue(o[11]);
    TestValue(ta["1"]);
    TestValue(ta[0]);
    TestValue(ta[3]);
    o.write(std::cout, true);
    TestValue(o.find("o.o2.c[2].d"));
    std::cout << o["t"].getString() << std::endl;
    std::cout << ta.getString() << std::endl;
    TestValue(o.find("o.tab[1]"));
    o.clear();
    o.write(std::cout, true);
    
    TestSerialization();
    TestJObject();

    JObject obj;
    JsonValue& v1 = obj.find("test.a");
    v1 = 3;
    JsonValue& v2 = obj.find("test.b");
    TestValue(v1);
    TestValue(v2);

    o["n"] = nullptr;

    JArray aTest{ 1, "2", JObject{R"({"item": 3})"} };
    aTest << nullptr << JArray{ 1,2,3 };
    TestValue(aTest);
    aTest[3] = 4;
    TestValue(aTest);
    JObject oTest{ {"id", 1}, {"deux" , 2.0}, {"n", nullptr}, {"a", JArray{1,2,3}}, {"d", E_JsonType::Double} };
    TestValue(oTest);
    std::map<std::string, JsonValue> list{ {"id", 1}, {"2", "deux"} };
    for (std::pair<const std::string&, const JsonValue&> p : list)
    {
        std::cout << p.first << ": " << p.second.getString() << std::endl;
    }

    JObject& oError = JObjectProvider::getObjectError();
    TestValue(oError);
    oError = JObject{ {"test", "c'est un test"} };
    TestValue(oError);
    TestValue(JObjectProvider::getObjectError());

    JArray& aError = JArrayProvider::getArrayError();
    TestValue(aError);
    aError = JArray{ 1,2,3 };
    TestValue(aError);
    TestValue(JArrayProvider::getArrayError());

    JsonValue vFind = oTest.find("id");
    TestValue(vFind);
    vFind = oTest.find("notFound");
    TestValue(vFind);
    TestValue(oTest["id"]);

    std::getchar();
}
