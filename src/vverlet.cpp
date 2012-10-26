#include <cstdio>
#include <cmath>
#include <omp.h>

#include "ldmm.h"
#include "vverlet.h"
#include "dist.h"
using namespace std;

ITRVVerlet::ITRVVerlet(PSP &psp, PBC *pbc, FFLJ12_6 *ff, REAL dt)
    :_psp(psp),_pbc(pbc),_ff(ff), _dt(dt)
{}

void ITRVVerlet::InitVel(REAL T, bool init)
{
    if(init){
        REAL cmv[DIM] = {0.0, 0.0, 0.0};
        REAL cm = 0.0;

        dist::Normal nor;
        for(int i=1; i <= _psp.GetNP(); ++i)
        {
            REAL m = _ff->GetM(i);
            cm += m;
            for(int j=0; j<DIM; ++j){
                _psp[i].v[j] = nor.Get();
                cmv[j] += m * _psp[i].v[j];
            }
        }

        for(int j=0; j<DIM; ++j)
            cmv[j] /= cm;
        for(int i=1; i <= _psp.GetNP(); ++i)
            for(int j=0; j<DIM; ++j)
                _psp[i].v[j] -= cmv[j];

    }
    SetMV2();
    if(init)
        RescaleV(T, 1);
}

REAL ITRVVerlet::GetInstT()
{
    return _mv2/3/(_psp.GetNP()-1)*1000/GAS_R;
}
REAL ITRVVerlet::GetInstP()
{
    int N = _psp.GetNP();
    int V = _pbc->GetV();
    return GetInstT()*GAS_R/1000*N/V + _ff->GetIVirial()/V;
}

REAL ITRVVerlet::GetPotential()
{
    return _ff->GetPotential();
}

REAL ITRVVerlet::GetKinetic()
{
    return 0.5*_mv2;
}

void ITRVVerlet::SetMV2()
{
    _mv2 = 0.0;
    for(int i=1; i <= _psp.GetNP(); ++i)
    {
        REAL m = _ff->GetM(i);
        for(int j=0; j<DIM; ++j)
            _mv2 += m * _psp[i].v[j] * _psp[i].v[j];
    }
}

void ITRVVerlet::RescaleV(REAL T, REAL s)
{
    REAL lambda = sqrt(1 + s*(T/GetInstT()-1));
    for(int i=1; i <= _psp.GetNP(); ++i)
        for(int j=0; j<DIM; ++j)
            _psp[i].v[j] *= lambda;
    SetMV2();
}

void ITRVVerlet::UpdatePosAndVel()
{
    UpdatePos();
#pragma omp parallel sections
    {
#pragma omp section
    _pbc->UpdateCoord();
#pragma omp section
    UpdateVel();
    }
    _ff->UpdateForce();
    UpdateVel();

    SetMV2();
}

void ITRVVerlet::UpdatePos()
{
    for(int i=1; i <= _psp.GetNP(); ++i)
        for(int j=0; j<DIM; ++j)
            _psp[i].x[j] += _dt*_psp[i].v[j] + 0.5*_psp[i].f[j]*_dt*_dt/_ff->GetM(i);
}

void ITRVVerlet::UpdateVel()
{
    for(int i=1; i <= _psp.GetNP(); ++i)
        for(int j = 0; j < DIM; ++j)
            _psp[i].v[j] += 0.5*_psp[i].f[j]*_dt/_ff->GetM(i);
}
