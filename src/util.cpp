#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;
string _Trim(string str)
{
    if(!str.empty()){ 
        str.erase(0, str.find_first_not_of(" \t"));
        if(!str.empty())
            str.erase(str.find_last_not_of(" \t")+1);
    }
    return str;
}
        
string _ToLower(string str)
{
    if(!str.empty())
        transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

bool _EntryValid(string str)
{
    vector<bool> bv(str.length());
    transform(str.begin(), str.end(), bv.begin(), ::isalnum);
    for(vector<bool>::const_iterator it = bv.begin(); it != bv.end(); ++it)
        if(!*it)return false;
    return true;
}

void vermsg()
{
    cout << "\n"
    "LDMM -- Li Ding Molecular Modelling Package (Ver 0.4 Alpha)\n"
    "By\n"
    "Copyright (C) 2011, Li Ding <dingmaotu@126.com>\n"
    "Institute of Geology and Geophysics, CAS\n"
    << endl;
}

void usage()
{
    cout << "\n"
    "Usage:\n"
    "\tldmm [CONFIGFILE]\n"
    "\tldmm will use CONFIGFILE to run a MD simulation.\n"
    << endl;
}


// Not Portable!
#include <unistd.h>
#include <libgen.h>
string exe_dir()
{
    char * buf = new char[256];
    if(readlink("/proc/self/exe", buf, 256) < 256)
        return (string(dirname(buf))+"/");
    else
        return string();
}
