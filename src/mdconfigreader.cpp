#include <fstream>
#include <string>
#include "mdconfigreader.h"
#include "util.h"

using namespace std;

MDConfigReader::MDConfigReader(const char* configfile, char c)
    :ConfigReader(configfile, c)
{
    LoadConfig();
    if(!_options.empty()) _valid = true;
}

void MDConfigReader::Process(string line)
{
    size_t pos;
    if((pos = line.find('=')) == string::npos) return;
    _options[_ToLower(_Trim(line.substr(0, pos)))] = _Trim(line.substr(pos+1));
}

string MDConfigReader::GetOption(const string opt)
{
    return _options[_ToLower(opt)];
}
