#include<iostream>
#include<sstream>
#include<memory>
#include<string>
#include<json/json.h>

using namespace std;

int main()
{
    Json::Value root;
    Json::StreamWriterBuilder wb;
    std::ostringstream os;
    
    root["name"] = "张三";
    root["high"] = 1.87;
    root["age"]= 18;

    std::unique_ptr<Json::StreamWriter> jsonWriter(wb.newStreamWriter());
    jsonWriter->write(root,&os);
    std::string s = os.str();

    std::cout<<s<<std::endl;
    return 0;
}
