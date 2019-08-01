#pragma once
#include<iostream>
#include<string>
#include"mongoose.h"


using namespace std;

struct mg_server_http_opts s_http_server_opts;
class IMServer
{
    private:
        string port;
        struct mg_mgr mgr;
        struct mg_connection *nc;
        static volatile bool quit;
    private:
        static void EventHandler(struct mg_connection *nc,int ev,void *data)
        {
            switch(ev)
            case MG_EV_HTTP_REQUEST:
            {
                struct http_message *hm = (struct http_message*)data;
                mg_serve_http(nc,hm,s_http_server_opts);
                break;
            }
            
            case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
            {
                break;
            }
            
            case ME_EV_WEBSOCKET_FRAME:
            {
                break;
            }
            
            case MG_EV_CLOSE:
            {
                break;
            }
            
            default:
                cout<<"other ev:"<<ev<<endl;
                break;
        }
    public:
        IMServer(string _port="8080"):port(_port)
        {
            s_http_server_opts.document_root = "web";
            s_http_server_opts.enable_directory_listing = "yes";
        }
        void InitServer()
        {
            mg_mgr_init(&mgr,NULL);
            nc = mg_bind(&mgr,port.c_str(),IMServer::EventHandler);
            mg_set_protocol_http_websocket(nc);
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
volatile bool IMServer::quit = false;
