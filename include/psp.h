#ifndef _PSP_H
#define _PSP_H
#include "particle.h"

// Phase Space Point
class PSP
{
    public:
        PSP(int np)
            :_np(np)
        {
            _p = new Particle[np];
        }
        ~PSP()
        {
            if(_p)
                delete [] _p;
        }
        int GetNP()const {return _np;};
        Particle& operator[](int i){return _p[i-1];}
    private:
        int _np;
        Particle *_p;
};

#endif
