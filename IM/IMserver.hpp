#pragma once

#include<iostream>
#include<string>
#include"mongoose.h"
using namespace std;
struct mg_serve_http_opts http_opts;
class IMServer
{
    private:
        string port;
        struct mg_mgr mgr;
        struct mg_connection *nc;
        volatile bool quit;
    public:

        static void Broadcast(struct mg_connection *nc,string msg)
        {
            struct mg_connection *c;
            for(c = mg_next(nc->mgr,NULL);\
                    c!=NULL;c=mg_next(nc->mgr,c))
            {
                //if(c ==nc)
                //  continue;
                mg_send_websocket_frame(c,WEBSOCKET_OP_TEXT,msg.c_str(),msg.size());
            }
        }

        static void LoginHandler()
        {

        }
        
        static void RegisterHandler()
        {

        }

        static void EventHandler(mg_connection *nc,int ev,void *data)
        {
            switch(ev)
            {


                case MG_EV_HTTP_REQUEST://正常的http请求
                    {//尽量不要在case 语句中定义变量 否则要用{}。
                    }
                        struct http_message *hm = (struct http_message*)data;
                        mg_serve_http(nc,hm,http_opts);
                        cout<<"hello*****************"<<endl;
                    }
                    break;
                case MG_EV_WEBSOCKET_HANDSHAKE_DONE://websocket升级事件完成
                    {
                        Broadcast(nc,"some body join...");
                    }
                    break;

                case MG_EV_WEBSOCKET_FRAME://正常的websocket数据请求
                    {
                        struct websocket_message *wm = (struct websocket_message*)data;
                        struct mg_str ms = {(const char*)wm->data,wm->size};
                        string msg = Util::mgStrToString(&ms);
                        Broadcast(nc,msg);
                    }
                    break;


                case MG_EV_CLOSE:
                    std::cout<<"close link"<<
                    break;


                default:
                    std::cout<<"other ev:"<<ev<<std::endl;
                    break;
       
            }
        }
    public:
        IMServer(string _port="8080"):port(_port),quit(false)
        {
            //http_opts.document_root = "web";
            //http_opts.enable_directory_listing = "yes";
        }
        void InitServer()
        {
            mg_mgr_init(&mgr,NULL);
            nc = mg_bind(&mgr,port.c_str(),EventHandler);
           
            mg_register_http_endpoint(nc,"/login",LoginHandler);
            mg_register_http_endpoint(nc,"/register",RegisterHandler);

            mg_set_protocol_http_websocket(nc);//设置listen socket
            http_opts.document_root = "web";
        }

        void Start()
        {
            int timeout = 20000;
            cout<<"IM Server Start Port:"<<port<<"... Done"<<endl;
            while(!quit)
            {
                mg_mgr_poll(&mgr,timeout);//进入事件监听
            }
        }
        ~IMServer()
        {
            mg_mgr_free(&mgr);
        }

};

