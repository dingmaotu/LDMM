#include <cmath>
#include <map>
#include <sstream>
#include <cstdio>
#include <omp.h>
#include "lj12_6.h"
#include "mdconfigreader.h"

using namespace std;

class LJConfigReader: public MDConfigReader
{
    public:
        LJConfigReader(const char *file, char c)
            :MDConfigReader(file, c)
        {}
        const map<string, string>& GetAll()const
        {
            return _options;
        }
};

FFLJ12_6::FFLJ12_6(const char * file, PBC *pbc, PSP *psp, REAL cutoff, REAL pairmax)
    :_pbc(pbc), _psp(psp), _cutoff(cutoff), _maxSqr(pairmax*pairmax)
{
    LoadParameters(file);

    int N = _psp->GetNP();
    N = N*(N-1)/2;
    _pairs = new int*[N];
    _pairdists = new REAL*[N];
    _sig = new REAL[N];
    _eps = new REAL[N];
    for(int i=0; i<N; ++i)
    {
        _pairs[i] = new int[2];
        _pairdists[i] = new REAL[4];
    }

    UpdateNeighbers();
    UpdateDistances();
}

FFLJ12_6::~FFLJ12_6()
{
    if(_paraTable)
        delete _paraTable;
    if(_pairs && _pairdists){
        int N = _psp->GetNP();
        N = N*(N-1)/2;
        for(int i=0; i<N; ++i)
        {
            delete _pairs[i];
            delete _pairdists[i];
        }
        delete _pairs;
        delete _pairdists;
    }
    if(_sig && _eps){
        delete _sig;
        delete _eps;
    }
}

void FFLJ12_6::UpdateNeighbers()
{
    _npairs = 0;
    int NP = _psp->GetNP();
#pragma omp parallel for num_threads(4)
    for(int i = 1; i <= NP; ++i)
        for(int j = i+1; j <= NP; ++j)
            if(_pbc->Dist2((*_psp)[i], (*_psp)[j]) < _maxSqr)
#pragma omp critical
            {
                _pairs[_npairs][0] = i;
                _pairs[_npairs][1] = j;
                _sig[_npairs] = GetSig(i, j);
                _eps[_npairs] = GetEps(i, j);
                ++_npairs;
            }
}

void FFLJ12_6::UpdateDistances()
{
#pragma omp parallel for
    for(int p = 0; p < _npairs; ++p)
    {
        _pairdists[p][0] = _pbc->Dist2((*_psp)[_pairs[p][0]], (*_psp)[_pairs[p][1]]);
        for(int k = 0; k < DIM; ++k)
            _pairdists[p][k+1] = _pbc->Dist((*_psp)[_pairs[p][0]], (*_psp)[_pairs[p][1]], k);
    }
}

void FFLJ12_6::UpdateForce()
{
    _ivirial = 0.0;
    _potential = 0.0;

    for(int i = 1; i <= _psp->GetNP(); ++i)
        for(int j = 0; j < DIM; ++j)
            (*_psp)[i].f[j] = 0.0;
    for(int p = 0; p < _npairs; ++p)
    {
        REAL Rij2 = _pairdists[p][0];
        REAL SR2 = (_sig[p]*_sig[p])/Rij2;
        REAL SR6 = SR2*SR2*SR2;

        _potential += 4*_eps[p]*SR6*(SR6-1);

        REAL tmp = 24*_eps[p]*SR6*(2*SR6-1)/Rij2;

        for(int k = 0; k < DIM; ++k)
        {
            (*_psp)[_pairs[p][0]].f[k] += tmp * _pairdists[p][k+1];
            (*_psp)[_pairs[p][1]].f[k] -= tmp * _pairdists[p][k+1];
            _ivirial += tmp * _pairdists[p][k+1] * _pairdists[p][k+1];
        }
    }
}

void FFLJ12_6::LoadParameters(const char *file){
    LJConfigReader reader(file, '#');
    reader.LoadConfig();
    const map<string, string>& all = reader.GetAll();
    _paraTable = new LJ12_6ParaEntry[all.size()];
    int i = 0;
    for(map<string, string>::const_iterator it = all.begin();
            it != all.end(); ++it, ++i)
    {
        _typeIndex[it->first] = i;
        istringstream iss(it->second);
        iss >> _paraTable[i].eps;
        iss >> _paraTable[i].sig;
        iss >> _paraTable[i].m;
        _paraTable[i].eps *= (GAS_R/1000); 
    }
}
REAL FFLJ12_6::GetEps(int i, int j)
{
    if((*_psp)[i].type == (*_psp)[j].type)
        return _paraTable[(*_psp)[i].type].eps;
    else
        return sqrt(_paraTable[(*_psp)[i].type].eps
                    *_paraTable[(*_psp)[j].type].eps);
}
REAL FFLJ12_6::GetSig(int i, int j)
{
    if((*_psp)[i].type == (*_psp)[j].type)
        return _paraTable[(*_psp)[i].type].sig;
    else
        return 0.5*(_paraTable[(*_psp)[i].type].sig
                    +_paraTable[(*_psp)[j].type].sig);
}
