#pragma once
#include<iostream>
#include<cstdio>
#include<fstream>
#include<string>
#include<map>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unordered_map>
#include"speech.h"
#include"base/http.h"

#define ASR_PATH "temp_file/asr.wav"
#define TTS_PATH "temp_file/play.mp3"
#define CMD_ETC "command.etc"


//工具类
class Util
{
    public:
        static bool Exec(std::string command,bool is_print)
        {
            if(!is_print)
            {
                command +=">/dev/null 2>&1";
            }
            FILE *fp = popen(command.c_str(),"r");
            if(nullptr == fp)
            {
                std::cerr<<"popen exec \'"<<command<<"\'Error"<<std::endl;
                return false;
            }
            
            if(is_print){

            

                char ch;
                 while(fread(&ch ,1,1,fp)>0)
                {
                  fwrite(&ch,1,1,stdout);
                }
                
            }
           pclose(fp);
           return true;
        }
        
};

//语音识别
class SpeechRec
{
private:
    std::string app_id;
    std::string api_key;
    std::string secret_key;
    aip::Speech *client;

public:
    SpeechRec()
    {
        app_id = "16886831";
        api_key = "0gnhXdywegbDFrjRiso8HZxM";
        secret_key ="2hbfy7RUgChH1xSwLozF3plDVAnB5Kgd";
        client = new aip::Speech(app_id,api_key,secret_key);
    }

    bool ASR(std::string path,std::string &out)
    {
        
        std::map<std::string,std::string> options;
        
        options["dev_pid"]= "1536";
        options["lan"]= "ZH";
        std::string file_content;
        aip::get_file_content(ASR_PATH,&file_content);
        Json::Value result = client->recognize(file_content,"wav",16000,options);
        
        int err_code = result["err_no"].asInt();//statu code
        if(0 == err_code)
        {
            out = result["result"][0].asString();
        }
        else
        {
            std::cerr<<"recognize error!"<<std::endl;
        }
        
        return true;

    }

    bool TTS(std::string message)
    {
        bool ret;
        std::ofstream ofile;
        std::string file_ret;
        std::map<std::string,std::string> options;
       // options[]=



        ofile.open(TTS_PATH,std::ios::out|std::ios::binary);
        Json::Value result = client->text2audio(message,options,file_ret);
        if(!file_ret.empty())
        {
            ofile<<file_ret;
            ret =true;
        }
        else
        {
            std::cerr<<"text2audio error"<<std::endl;
            ret = false;
        }

        ofile.close();
        return ret ;
    }
};

//图灵机器人
class TLRobot
{
private:
    std::string url;// "http://openapi.tuling123.com/openapi/api/v2";
    std::string api_key;// "4dbc471597324e7b943bf0cf52fefb8a";
    std::string user_id; 
    aip::HttpClient client;
private:
    bool IsCodeLegal(int code)
    {
        bool result = false;
        switch(code)
        {
            
            case 5000:
                break;
            case 10004:
                result = true;
                break;
            default:
                result = true;
                break;
        }
        return result;
    }
    std::string MessageToJson(std::string message)
    {   
        Json::Value root;
        Json::StreamWriterBuilder wb;
        std::ostringstream os;
        Json::Value item1;
        Json::Value item2;

        root["reqType"]= 0;
        item1["text"]= message;
        item2["inputText"]= item1;
        root["perception"]= item2;
        
        item2.clear();
        item2["apiKey"]= api_key;
        item2["userId"]= user_id;
        root["userInfo"] = item2;

        std::unique_ptr<Json::StreamWriter> sw(wb.newStreamWriter());
        sw->write(root,&os);
        std::string json_string =os.str();
        std::cout<<"debug:"<<json_string<<std::endl;


        return json_string;

    }

    std::string RequestTL(std::string &request_json)
    {
    
        std::string response;
        int code = client.post(url,nullptr,request_json,nullptr,&response);
        if(code!=CURLcode::CURLE_OK)
        {
            std::cerr<<"post error"<<std::endl;
            return "";
        }
        
        return response;
        
    }
    
    std::string JsonToMessage(std::string &str)
    {
        // std::cout<<"JsonToEchoMessage:"<<str<<std::endl;
        
        JSONCPP_STRING errs;
        Json::Value root;
        Json::CharReaderBuilder rb;
        std::unique_ptr<Json::CharReader> const jsonReader(rb.newCharReader());
        bool res= jsonReader->parse(str.data(),str.data()+str.size(),&root,&errs);
        if(!res ||!errs.empty())
        {
            std::cerr<<"json parse error!"<<std::endl;
            return "";
        }

        int code = root["intent"]["code"].asInt();
        if(!IsCodeLegal(code))
        {
            std::cerr<<"response error"<<std::endl;
            return "";
        }
        Json::Value item = root["result"][0];
        std::string msg = item["values"]["text"].asString();

       // std::cout<<"TLRobot:"<<ret<<std::endl;
        
        return msg;

    }

public:

    TLRobot(std::string id = "1")
    {
        url ="http://openapi.tuling123.com/openapi/api/v2";
        api_key="4dbc471597324e7b943bf0cf52fefb8a";
        user_id= id;
    }   
    
    std::string Talk(std::string message)
    {
        std::string ms = MessageToJson(message);
        std::string response = RequestTL(ms);
        std::string echo_message = JsonToMessage(response);
        //Json::Value _result = ret["results"];
        //Json::Value values = _result[0]["values"];
        //std::cout<<"TLRobot:"<<values["text"].asString()<<std::endl;
        return echo_message;
        
        
        
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
    TLRobot r;
    std::unordered_map<std::string,std::string> commands;
private:
    bool Record()
    {
        std::cout<<"debug: Record..."<<std::endl;
        std::string command = "arecord -t wav -c 1 -r 16000 -d 5 -f S16_LE ";
        command += ASR_PATH;
        bool ret = Util::Exec(command,false);
        std::cout<<"debug:Record end"<<std::endl;
        return ret;
    }

    bool Play()
    {
        std::string command = "cvlc --play-and-exit";
        command +=TTS_PATH;
        return Util::Exec(command,false);

    }

//public：
    
    bool LoadEtc()
    {
        std::ifstream in(CMD_ETC);
        if(!in.is_open())
        {
            std::cerr<<"open error"<<std::endl;
            return false;
        }

        std::string sep = "：";
        char line[256];
        while(in.getline(line,sizeof(line)))
        {
            std::string str = line;
            std::size_t pos = str.find(sep);
            if(std::string::npos == pos)
            {
                std::cerr<<"load etc error"<<std::endl;
                exit(2);
            }
            std::string k = str.substr(0,pos);
            std::string v = str.substr(pos+sep.size());
            k +="。";//此语句只为了匹配语音识别环境（每句话结尾后都为。）
            commands.insert(std::make_pair(k,v));
        }
        std::cout<<"load command etc...done"<<std::endl;
        in.close();
    }
public:
    Eva()
    {

    }
    
    bool IsCommand(std::string message,std::string &cmd)
    {
        auto iter = commands.find(message);
        if(iter == commands.end())
        {
            return false;
        }
        
        cmd = iter->second;
    }
     void Run()
     {
        volatile bool quit = false;
        while(!quit)
        {
            if(Record())
            {
                std::string message;
                if(sr.ASR(ASR_PATH,message))
                {
                    //1.command
                    std::string cmd = "";
                    if(IsCommand(message,cmd))
                    {
                        std::cout<<"Eva@localhose $"<<cmd<<std::endl;
                        Util::Exec(cmd,true);
                        continue;
                    }
                    std::cout<<"me"<<message<<std::endl;
                    if(message == "nizouba。")
                    {
                        std::string quit_message = "nawojiuzoule。";
                        std::cout<<"eva"<<quit_message<<std::endl;
                        if(sr.TTS(message))
                        {
                            this->Play();
                        }
                        exit(0);
                    }
                    //2.Tuling
                    //std::cout<<"me:#"<<message<<std::endl;
                    std::string echo = r.Talk(message);
                    std::cout<<"tuling"<<echo<<std::endl;
                    if(sr.TTS(echo))
                    {
                        this->Play();
                    }
                }
                
            }

            else
            {
                std::cerr<<"Record error"<<std::endl;
            }

            sleep(2);
        }
      
     }   
    ~Eva()
    {

    }
    
};




