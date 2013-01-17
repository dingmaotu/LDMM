#ifndef _LJ12_6_H
#define _LJ12_6_H

#include "ldmm.h"
#include "psp.h"
#include "pbc.h"
#include <string>
#include <map>
using namespace std;

struct LJ12_6ParaEntry
{
    REAL m;
    REAL eps;
    REAL sig;
};

class FFLJ12_6
{
    public:
        // config file, pbc, psp, cutoff, pairmax
        FFLJ12_6(const char *, PBC *, PSP *, REAL, REAL);
        ~FFLJ12_6();

        void UpdateForce();

        REAL GetPotential()
        {
            return _potential;
        }
        REAL GetIVirial()
        {
            return _ivirial/3;
        }

        inline int GetTypeIndex(string ts)
        {
            return _typeIndex[ts];
        }

        REAL GetM(int i)
        {
            return _paraTable[(*_psp)[i].type].m;
        }

        void UpdateNeighbers();
        void UpdateDistances();

    private:
        void LoadParameters(const char*);
        REAL GetSig(int i, int j);
        REAL GetEps(int i, int j);

        PBC * _pbc;
        PSP * _psp;
        map<string, int> _typeIndex;
        LJ12_6ParaEntry *_paraTable;

        REAL _cutoff;
        REAL _maxSqr;
        int _npairs;
        int **_pairs; // _npairs*2 array: i j 
        REAL **_pairdists; // _npairs*4 array: dSqrd dx dy dz 
        REAL *_sig;
        REAL *_eps;
        REAL _potential;
        REAL _ivirial;
};
#endif
