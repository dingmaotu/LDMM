#include <iostream>
#include <fstream>
#include "mdconfigreader.h"
#include "mdsystem.h"
#include "util.h"

int main(int argc, char *argv[])
{
    vermsg();
    if(argc != 2)
    {
        usage();
        return 0;
    }
    
    MDConfigReader *reader = new MDConfigReader(argv[1], '#');
    reader->LoadConfig();
    if(!reader->IsValid()){
        cerr << "Config file does not exist or is invalid!" << endl;
        return 0;
    }

    MDSystem sys(reader);

    while(!sys.IsEnd())
    {
        sys.Sample();
        sys.Update();
        sys.Output();
    }

    return 0;
}
