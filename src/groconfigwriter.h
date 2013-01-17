#ifndef _GROCONFIGWRITER_H
#define _GROCONFIGWRITER_H

#include <cstdio>
#include <string>
using namespace std;

#include "ldmm.h"
#include "psp.h"
#include "vverlet.h"

class GroConfigWriter
{
    public:
        GroConfigWriter(PSP *psp, string trfile, string stfile)
        :_psp(psp), _trfile(trfile), _stfile(stfile)
        {}

        void AppendTrace(const char*, REAL, REAL);
        void AppendStatistics(ITRVVerlet*, REAL);
    private:
        PSP *_psp;
        string _trfile;
        string _stfile;
};
#endif
