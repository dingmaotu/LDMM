#include <fstream>
#include <string>
#include <cstdio>
#include "configreader.h"

using namespace std;

void ConfigReader::TrimComment(char *str, char comm)
{
    for(int i=0; str[i] != '\0'; ++i)
        if(str[i] == comm){
            str[i] = '\0';
            return;
        }
}

void ConfigReader::LoadConfig()
{
    FILE *in = fopen(_file, "r");
    if(!in) return;

    int c;
    while(!feof(in) && !ferror(in)){
        c = fgetc(in);
        int i = 0;
        ungetc(c, in);
        while(!feof(in) && ((c = fgetc(in)) != '\n'))
            _buf[i++] = c;
        _buf[i] = '\0';
        TrimComment(_buf, _comment);
        string s(_buf);
        Process(s);
    }

    fclose(in);
}
