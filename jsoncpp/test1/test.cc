#include<iostream>
#include<sstream>
#include<memory>
#include<string>
#include<json/json.h>

void ParseJson(std::string &s)
{
    JSONCPP_STRING errs;
    Json::Value root;
    Json::CharReaderBuilder rb;
    std::unique_ptr<Json::CharReader> const jsonReader(rb.newCharReader());
    bool res = jsonReader->parse(s.data(),s.data()+s.size(),&root,&errs);
    if(!res||!errs.empty())
    {
        std::cerr<<"json parse error"<<errs<<std::endl;
        return;
    }

    std::cout<<"name: "<<root["name"].asString()<<std::endl;
    std::cout<<"high: "<<root["high"].asFloat()<<std::endl;
    std::cout<<"age: "<<root["age"].asInt()<<std::endl;

    
}

int main()
{
    //模拟json串
    std::string s = "{ \"age\" : 26,\"high\" :  181.2, \"name\" : \"zhangsan\"}";
    std::cout << s << std::endl;
    ParseJson(s);
    return 0;

}
