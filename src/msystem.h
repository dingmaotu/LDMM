#ifndef _MSYSTEM_H
#define _MSYSTEM_H

#include <string>
using namespace std;

class MSystem
{
    public:
        virtual ~MSystem() {};
        virtual void Sample() = 0;
        virtual void Update() = 0;
        virtual void Output() = 0;
        virtual bool IsEnd() const = 0;
};


#endif
