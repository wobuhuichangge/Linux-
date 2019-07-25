#pragma once
#include<iostream>

//工具类
class Util
{

};

//语音识别
class SpeechRec
{

};

//图灵机器人
class TLRobot
{
private:    
    std::string url= "http://openapi.tuling123.com/openapi/api/v2";
    std::string api_key= "";
    std::string user_id = "";
    aip::HttpClient client;
private:
    Json::value MessageToJson(std::string message)
    {   
        Json::value root;
        Json::value StreamWriterBuilder wb;
        std::ostringstream os;
        Json::value item1;
        Json::value item2;

        root["reqType"]= 0;
        item1["text"]= message;
        item2["inputText"]= item1;
        root["perception"]= item2;
        
        std::unique_ptr<Json::StreamWriter> jsonWriter(wb.newStreamWriter());
        isonWriter-> writer(root,&os);
        std::string s = os.str();

        return s;

    }

    Json::value RequestTL(Json::value data)
    {
        std::string response;
        Json::value obj;

    }
    
    void JsonToMessage(Json::value &s)
    {
        JSONCPP_STRING errs;
        Json::value root;
        Json::CharReaderBuilder rb;
        std::unique_ptr<Json::CharReader> const jsonReader(rb.newCharReader());
        bool res= jsonReader->parse(s.data(),s.data()+s.size(),&root,&errs);
        if(!res ||!errs.empty())
        {
            std::cerr<<"json parse error!"<<std::endl;
            return;
        }

        Json::value result = root["results"];
        Json::value values = result[0]["values"];
        std::string ret = values["text"].asString();

        std::cout<<"TLRobot:"<<ret<<std::endl;
        
        return ret;

    }
pubilc:
    TLRobot()
    {

    }
    
    std::string Talk()
    {
        Json::value ms = MessageToJson();
        RequestTL();
        JsonToMessage();
    }

    ~TLRobot()
    {

    }
        
};

//Eva
class Eva
{
private:
    SpeechRec sr;
    TLRobot robot;
    srd::unordered_map<std::string,std::string> command;

public：
    Eva()
    {
        char buff[256];
        std::ifstream in
    }
    
    ~Eva()
    {

    }
    
};




