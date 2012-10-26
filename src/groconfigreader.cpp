#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

#include "util.h"
#include "groconfigreader.h"

void GroConfigReader::Process(string line)
{
    switch(_state)
    {
        case GROTITLE:
            {
                size_t p = line.find("t=");
                if(p == string::npos)
                    _desc = line;
                else{
                    _desc = line.substr(0, p);
                    istringstream ss(line.substr(p+2));
                    ss >> _time;
                }

                _state = GRONUM;
            }
            break;
        case GRONUM:
            {
                stringstream ss(line);
                ss >> _np;
                if(_np > 0){
                    _psp = new PSP(_np);
                    _atomNames = new string[_np];
                    _state = GROCOORD;
                }else{
                    _state = GROTITLE;
                }
            }
            break;
        case GROCOORD:
            {
                ParseAtom(_ToLower(line));
            }
            break;

        case GROPBC:
            {
                stringstream ss(line);
                int i = 0;
                while(!ss.fail())
                    ss >> _box[i++];
                _state = GROEND;
            }
            break;

        case GROEND:
            return ;
    }
}

void GroConfigReader::InitTypes(FFLJ12_6 *ff)
{
    if(_psp)
        for(int i=1; i <= _psp->GetNP(); ++i)
            (*_psp)[i].type = ff->GetTypeIndex(_atomNames[i-1]);
}

void GroConfigReader::ParseAtom(const string entry)
{
    string residueName = _Trim(entry.substr(5, 5));
    string atomName = _Trim(entry.substr(10, 5));
    int residueNum;
    int atomNum;
    stringstream ss(entry.substr(0,5));
    ss >> residueNum;
    ss.str(entry.substr(15, 5));
    ss.clear();
    ss >> atomNum;

    ss.str(entry.substr(20));
    ss.clear();
    for(int i=0; i < DIM; ++i)
        ss >> (*_psp)[atomNum].x[i];

    if(_initV)
    {
        for(int i=0; i < DIM; ++i)
            if(ss.good()){
                ss >> (*_psp)[atomNum].v[i];
            }else{
                _initV = false;
                break;
            }
    }

    _atomNames[atomNum-1] = atomName;
    
    if(atomNum == _np){
        _state = GROPBC;
        return ;
    }
}
