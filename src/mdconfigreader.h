#ifndef _MDCONFIGREADER_H
#define _MDCONFIGREADER_H
#include <iostream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

#include "configreader.h"

class MDConfigReader: public ConfigReader
{
    public:
        MDConfigReader(const char *, char);

        string GetOption(const string);
        template <typename Number>
        Number GetNumber(const string);

    protected:
        virtual void Process(string line);
        map<string, string> _options;
};
template <typename Number>
Number MDConfigReader::GetNumber(const string opt)
{
    Number n = -1;
    string tmp = GetOption(opt);
    if(!tmp.empty()){
        stringstream tmps(tmp);
        tmps >> n;
    }
    return n;
}

#endif
