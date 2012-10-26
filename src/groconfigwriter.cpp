#include "groconfigwriter.h"


void GroConfigWriter::AppendTrace(const char * title, REAL t, REAL boxL)
{
    FILE * out = fopen(_trfile.c_str(), "a+");
    fprintf(out, "%s, t= %8.3f\n", title, t);
    fprintf(out, "%5d\n", _psp->GetNP());
    for(int i=1; i<=_psp->GetNP(); ++i)
    {
        fprintf(out, "%5d  SOL   AR%5d%8.3f%8.3f%8.3f%8.4f%8.4f%8.4f\n", i, i,
               (*_psp)[i].x[0],
               (*_psp)[i].x[1],
               (*_psp)[i].x[2],
               (*_psp)[i].v[0],
               (*_psp)[i].v[1],
               (*_psp)[i].v[2]);
    }
    fprintf(out, "%8.3f%8.3f%8.3f\n", boxL, boxL, boxL);
    fclose(out);
}

void GroConfigWriter::AppendStatistics(ITRVVerlet* v, REAL t)
{
    FILE * out = fopen(_stfile.c_str(), "a+");
    REAL Ek = v->GetKinetic();
    REAL Ep = v->GetPotential();
    REAL T = v->GetInstT();
    REAL P = v->GetInstP();
    fprintf(out, "%8.3f  %8.3f  %8.3f  %8.4f  %8.4f  %8.4f\n", t, T, P, Ep, Ek, Ek+Ep);
    fclose(out);
}
