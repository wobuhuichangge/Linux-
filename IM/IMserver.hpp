#pragma once
#include<string>
#include"mongoose.h"
#include<iostream>
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
        static void EventHandler(mg_connection *nc,int ev,void *data)
        {
            switch(ev)
            {


                case MG_EV_HTTP_REQUEST:
                    {
                        struct http_message *hm = (struct http_message*)data;
                        mg_serve_http(nc,hm,http_opts);
                        cout<<"hello*****************"<<endl;
                    }
                    break;
                case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
                    {
                        Broadcast(nc,"some body join...");
                    }
                    break;

                case MG_EV_WEBSOCKET_FRAME:

                    break;


                case MG_EV_CLOSE:

                    break;


                default:
                    std::cout<<"other ev:"<<ev<<std::endl;
                    break;
       
            }
        }
    public:
        IMServer(string _port="8080"):port(_port),quit(false)
        {
            http_opts.document_root = "web";
            http_opts.enable_directory_listing = "yes";
        }
        void InitServer()
        {
            mg_mgr_init(&mgr,NULL);
            nc = mg_bind(&mgr,port.c_str(),EventHandler);
            mg_set_protocol_http_websocket(nc);
            //http_opts.document_root = "web";
        }

        void Start()
        {
            int timeout = 20000;
            cout<<"IM Server Start Port:"<<port<<"... Done"<<endl;
            while(!quit)
            {
                mg_mgr_poll(&mgr,timeout);
            }
        }
        ~IMServer()
        {
            mg_mgr_free(&mgr);
        }

};

