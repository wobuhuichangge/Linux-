#include<iostream>
#include<json/json.h>
#include<memory>
#include"mongoose.h"

using namespace std;
class Util
{
    public:
        string mgStrToString(struct mg_str *ms)
        {
            string ret = "";
            int len = ms->len;
            for(auto i = 0;i<len;i++)
            {
                ret.push_back(ms->p[i]);
            }

            return ret;
        }
};
