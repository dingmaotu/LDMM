#ifndef _GROCONFIGREADER_H
#define _GROCONFIGREADER_H

#include <string>
#include <iostream>
#include "ldmm.h"
#include "configreader.h"
#include "psp.h"
#include "lj12_6.h"

class GroConfigReader: public ConfigReader
{
    public:
        GroConfigReader(const char* file)
            :ConfigReader(file, '#'), _psp(NULL)
        {
            for(int i=0; i<9; ++i)
                _box[i] = 0.0;
            _initV = true;
            _state = GROTITLE;
            
            _np = 0;
        }
        ~GroConfigReader(){}

        REAL GetBoxPara(int i){return (i>=0 && i<9)?(_box[i]):0.0;}
        REAL GetTime()const {return _time;} 
        std::string GetDesc()const {return _desc;}
        PSP *GetPSP() const {return _psp;}
        bool IsVInit() const {return _initV;}
        void InitTypes(FFLJ12_6 * ff);
    protected:
        void Process(std::string);
        void ParseAtom(std::string);

        enum GroState{GROEND, GROTITLE, GRONUM, GROCOORD, GROPBC};

    private:
        PSP * _psp;
        FFLJ12_6 *_ff;

        bool _initV;
        std::string _desc;
        REAL _time;
        REAL _box[9];
        std::string *_atomNames;

        GroState _state;
        int _np;
};

#endif
