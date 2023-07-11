/**
 * @file test_config.cc
 * @brief 配置模块测试
 * @version 0.1
 * @date 2021-06-13
 */

#include "src/obeast.h"

obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

obeast::ConfigVar<int>::ptr g_int = 
    obeast::Config::Lookup("global.int", (int)8080, "global int");

obeast::ConfigVar<float>::ptr g_float = 
    obeast::Config::Lookup("global.float", (float)10.2f, "global float");

// 字符串需显示构造，不能传字符串常量
obeast::ConfigVar<std::string>::ptr g_string =
    obeast::Config::Lookup("global.string", std::string("helloworld"), "global string");

obeast::ConfigVar<std::vector<int>>::ptr g_int_vec = 
    obeast::Config::Lookup("global.int_vec", std::vector<int>{1, 2, 3}, "global int vec");

obeast::ConfigVar<std::list<int>>::ptr g_int_list = 
    obeast::Config::Lookup("global.int_list", std::list<int>{1, 2, 3}, "global int list");

obeast::ConfigVar<std::set<int>>::ptr g_int_set = 
    obeast::Config::Lookup("global.int_set", std::set<int>{1, 2, 3}, "global int set");

obeast::ConfigVar<std::unordered_set<int>>::ptr g_int_unordered_set = 
    obeast::Config::Lookup("global.int_unordered_set", std::unordered_set<int>{1, 2, 3}, "global int unordered_set");

obeast::ConfigVar<std::map<std::string, int>>::ptr g_map_string2int = 
    obeast::Config::Lookup("global.map_string2int", std::map<std::string, int>{{"key1", 1}, {"key2", 2}}, "global map string2int");

obeast::ConfigVar<std::unordered_map<std::string, int>>::ptr g_unordered_map_string2int = 
    obeast::Config::Lookup("global.unordered_map_string2int", std::unordered_map<std::string, int>{{"key1", 1}, {"key2", 2}}, "global unordered_map string2int");

////////////////////////////////////////////////////////////
// 自定义配置
class Person {
public:
    Person() {};
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;
    
    std::string toString() const {
        std::stringstream ss;
        ss << "[Person name=" << m_name
           << " age=" << m_age
           << " sex=" << m_sex
           <<"]";
        return ss.str();
    }

    bool operator==(const Person &oth) const {
        return m_name == oth.m_name && m_age == oth.m_age && m_sex == oth.m_sex;
    }
};

// 实现自定义配置的YAML序列化与反序列化，这部分要放在obeast命名空间中
namespace obeast {

template<>
class LexicalCast<std::string, Person> {
public:
    Person operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_age = node["age"].as<int>();
        p.m_sex = node["sex"].as<bool>();
        return p;
    }
};

template<>
class LexicalCast<Person, std::string> {
public:
    std::string operator()(const Person &p) {
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

} // end namespace obeast

obeast::ConfigVar<Person>::ptr g_person = 
    obeast::Config::Lookup("class.person", Person(), "system person");

obeast::ConfigVar<std::map<std::string, Person>>::ptr g_person_map = 
    obeast::Config::Lookup("class.map", std::map<std::string, Person>(), "system person map");

obeast::ConfigVar<std::map<std::string, std::vector<Person>>>::ptr g_person_vec_map = 
    obeast::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person>>(), "system vec map");

void test_class() {
    static uint64_t id = 0;

    if(!g_person->getListener(id)) {
        id = g_person->addListener([](const Person &old_value, const Person &new_value){
            OBEAST_LOG_INFO(g_logger) << "g_person value change, old value:" << old_value.toString()
                << ", new value:" << new_value.toString();
        });
    }

    OBEAST_LOG_INFO(g_logger) << g_person->getValue().toString();

    for (const auto &i : g_person_map->getValue()) {
        OBEAST_LOG_INFO(g_logger) << i.first << ":" << i.second.toString();
    }

    for(const auto &i : g_person_vec_map->getValue()) {
        OBEAST_LOG_INFO(g_logger) << i.first;
        for(const auto &j : i.second) {
            OBEAST_LOG_INFO(g_logger) << j.toString();
        }
    }
}

////////////////////////////////////////////////////////////

template<class T>
std::string formatArray(const T &v) {
    std::stringstream ss;
    ss << "[";
    for(const auto &i:v) {
        ss << " " << i;
    }
    ss << " ]";
    return ss.str();
}

template<class T>
std::string formatMap(const T &m) {
    std::stringstream ss;
    ss << "{";
    for(const auto &i:m) {
        ss << " {" << i.first << ":" << i.second << "}";
    }
    ss << " }";
    return ss.str();
}

void test_config() {
    OBEAST_LOG_INFO(g_logger) << "g_int value: " << g_int->getValue();
    OBEAST_LOG_INFO(g_logger) << "g_float value: " << g_float->getValue();
    OBEAST_LOG_INFO(g_logger) << "g_string value: " << g_string->getValue();
    OBEAST_LOG_INFO(g_logger) << "g_int_vec value: " << formatArray<std::vector<int>>(g_int_vec->getValue());
    OBEAST_LOG_INFO(g_logger) << "g_int_list value: " << formatArray<std::list<int>>(g_int_list->getValue());
    OBEAST_LOG_INFO(g_logger) << "g_int_set value: " << formatArray<std::set<int>>(g_int_set->getValue());
    OBEAST_LOG_INFO(g_logger) << "g_int_unordered_set value: " << formatArray<std::unordered_set<int>>(g_int_unordered_set->getValue());
    OBEAST_LOG_INFO(g_logger) << "g_int_map value: " << formatMap<std::map<std::string, int>>(g_map_string2int->getValue());
    OBEAST_LOG_INFO(g_logger) << "g_int_unordered_map value: " << formatMap<std::unordered_map<std::string, int>>(g_unordered_map_string2int->getValue());

    // 自定义配置项
    test_class();
}

int main(int argc, char *argv[]) {
    // 设置g_int的配置变更回调函数
    g_int->addListener([](const int &old_value, const int &new_value) {
        OBEAST_LOG_INFO(g_logger) << "g_int value changed, old_value: " << old_value << ", new_value: " << new_value;
    });

    OBEAST_LOG_INFO(g_logger) << "before============================";

    test_config();

    // 从配置文件中加载配置，由于更新了配置，会触发配置项的配置变更回调函数
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir("conf");
    OBEAST_LOG_INFO(g_logger) << "after============================";
    
    test_config();

    // 遍历所有配置
    obeast::Config::Visit([](obeast::ConfigVarBase::ptr var){
        OBEAST_LOG_INFO(g_logger) << "name=" << var->getName()
            << " description=" << var->getDescription()
            << " typename=" << var->getTypeName()
            << " value=" << var->toString();
    });

    return 0;
}
