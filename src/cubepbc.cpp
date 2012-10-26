#include "cubepbc.h"
#include <cstdio>
#include <cmath>

inline REAL CubePBC::Restrict(REAL r)
{
    return (r - _L*(int)(r/_L+(r<0?-0.5:0.5)));
}

inline REAL CubePBC::RestrictX(REAL r)
{
    return (r - _L*(int)(r/_L+(r<0?-_L:0)));
}

inline REAL CubePBC::GetL()
{
    return _L;
}
inline REAL CubePBC::GetV()
{
    return _L*_L*_L;
}

REAL CubePBC::Dist2(Particle& p1, Particle& p2)
{
    REAL dist2 = 0.0, temp;
    for(int i=0; i<DIM; ++i)
    {
        temp = Restrict(p1.x[i] - p2.x[i]);
        dist2 += temp * temp;
    }
    return dist2;
}

REAL CubePBC::Dist(Particle& p1, Particle& p2, int k)
{
    return Restrict(p1.x[k] - p2.x[k]);
}

void CubePBC::UpdateCoord()
{
    if(_psp){
        for(int i=1; i<=_psp->GetNP(); ++i)
            for(int j=0; j<DIM; ++j)
                (*_psp)[i].x[j] = RestrictX((*_psp)[i].x[j]);
    }
}

void CubePBC::InitPos(string type)
{
    if("fcc" == type)
        SetupFCC();
    else if("cubic" == type)
        SetupCubic();
    UpdateCoord();
}

void CubePBC::SetupFCC()
{
    // Number of primary FCC cells in one dimension
    static REAL _FCC[4][3] = {
        {0.25, 0.25, 0.25}, {0.75, 0.75, 0.25},
        {0.75, 0.25, 0.75}, {0.25, 0.75, 0.75}};
    int M = 2;
    while(4*M*M*M < _psp->GetNP())
        ++M;
    REAL m = _L/M;

    printf("Initial Position: FCC with %dx%dx%d Grids of Length %.3f.\n", M, M, M, m);
    int n = 1;
    for(int x=0; x < M; ++x)
        for(int y=0; y < M; ++y)
            for(int z=0; z < M; ++z)
            {
                for(int i=0; n <= _psp->GetNP() && i < 4; ++i, ++n)
                {
                    (*_psp)[n].x[0] = (_FCC[i][0] + x)*m;
                    (*_psp)[n].x[1] = (_FCC[i][1] + y)*m;
                    (*_psp)[n].x[2] = (_FCC[i][2] + z)*m;
                }
            }
}

void CubePBC::SetupCubic()
{
    int M = 2;
    while(M*M*M < _psp->GetNP())
        ++M;
    REAL m = _L/M;
    printf("Initial Position: Cubic %dx%dx%d with spacing %.3f.\n", M, M, M, m);

    int n = 1;
    for(int x=0; x < M; ++x)
        for(int y=0; y < M; ++y)
            for(int z=0; z < M; ++z)
            {
                if(n <= _psp->GetNP())
                {
                    (*_psp)[n].x[0] = (0.5 + x) * m;
                    (*_psp)[n].x[1] = (0.5 + y) * m;
                    (*_psp)[n].x[2] = (0.5 + z) * m;
                }
                ++n;
            }
}
