#ifndef _CONFIGREADER_H
#define _CONFIGREADER_H

#include <string>

class ConfigReader
{
    public:
        ConfigReader(const char * configfile, char c)
            :_valid(false), _comment(c), _file(configfile)
        {
            _buf = new char[256];
        }
        virtual ~ConfigReader()
        {
            if(_buf) delete _buf;
        }
        virtual bool IsValid()const {return _valid;}

        static void TrimComment(char *, char);
        void LoadConfig();

    protected:
        bool _valid;
        char _comment;
        const char* _file;
        char * _buf;
        virtual void Process(std::string line)=0;
};

#endif
