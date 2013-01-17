#ifndef _DIST_H
#define _DIST_H

#include <cstdlib>
#include <ctime>
#include "ldmm.h"

namespace dist
{
    class Dist
    {
        public:
            Dist()
                :_max((REAL)RAND_MAX)
            {
                srand((unsigned)time(0));
            }
            virtual ~Dist(){}
            virtual REAL Get() = 0;
        protected:
            virtual REAL GetOne()
            {
                return rand()/_max;
            }
        private:
            REAL _max;
    };

    class Normal: public Dist
    {
        public:
            REAL Get()
            {
                REAL s = 0.0;
                for(int i=0; i<12; ++i)
                    s += GetOne();
                s -= 6;
                return s;
            }
    };
}

#endif
