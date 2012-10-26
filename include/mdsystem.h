#ifndef _MDSYSTEM_H
#define _MDSYSTEM_H

#include "ldmm.h"
#include "msystem.h"
#include "mdconfigreader.h"
#include "psp.h"
#include "vverlet.h"
#include "groconfigwriter.h"

class MDSystem: public MSystem
{
    public:
        MDSystem(MDConfigReader *);
        ~MDSystem();

        void Sample();
        void Update();
        void Output();
        bool IsEnd() const;

    private:
        void ShowProgress() const;

        MDConfigReader *_config;
        GroConfigWriter *_writer;
        PSP *_psp;
        PBC *_pbc;
        FFLJ12_6 *_ff;
        ITRVVerlet *_vv;
        bool _fail;

        string _title;
        int _curstep;
        int _totalsteps;
        REAL _dt;
        REAL _initT;
        REAL _endT;

        int _batchmode;
        int _batchpass;
        REAL _batchtstep;
        int _batchcollect;
        int _curpass;
        REAL _colT;
        REAL _colP;

        int _nstrace;
        int _nsstatistics;

        int _nsscale;
        REAL _scaleFactor;

        REAL _pairmax;
        REAL _cutoff;
        int _nsneighberupdate;
};

#endif

