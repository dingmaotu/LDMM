#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ldmm.h"
#include "mdsystem.h"
#include "lj12_6.h"
#include "util.h"
#include "cubepbc.h"
#include "groconfigreader.h"

using namespace std;

MDSystem::MDSystem(MDConfigReader *r)
    :_config(r),_curstep(0), _fail(false)
{
    cout << "Setting up system..." << endl;
    _title = _config->GetOption("name");
    _dt = _config->GetNumber<REAL>("dt");
    _nstrace = _config->GetNumber<int>("nstrace");
    _nsstatistics = _config->GetNumber<int>("nsstatistics");
    
    _initT = _config->GetNumber<REAL>("initt");
    _endT = _config->GetNumber<REAL>("endt");
    _totalsteps = _config->GetNumber<int>("totalsteps");
    
    _batchmode = _config->GetNumber<int>("batchmode");
    _batchpass = _config->GetNumber<int>("batchpass");
    _batchtstep = _config->GetNumber<REAL>("batchtstep");
    _batchcollect = _config->GetNumber<int>("batchcollect");
    _curpass = 1;
    _colT = 0.0;
    _colP = 0.0;

    _nsscale = _config->GetNumber<int>("nsscale");
    _scaleFactor = _config->GetNumber<REAL>("scalefactor");

    _cutoff = _config->GetNumber<REAL>("cutoff");
    _pairmax = _config->GetNumber<REAL>("pairmax");
    _nsneighberupdate = _config->GetNumber<int>("nsneighberupdate");
    string sf = _config->GetOption("structurefile");
    string tr = _config->GetOption("tracefile");
    string st = _config->GetOption("statisticsfile");
    
    GroConfigReader *gro = new GroConfigReader(sf.c_str());
    gro->LoadConfig();

    _psp = gro->GetPSP();
    _pbc = new CubePBC(gro->GetBoxPara(0));
    _pbc->SetPSP(_psp);

    if(_config->GetOption("initpos") == "fcc")
        _pbc->InitPos("fcc");

    else if(_config->GetOption("initpos") == "cubic")
        _pbc->InitPos("cubic");

    string exedir = exe_dir();
    if(!exedir.empty())
        _ff = new FFLJ12_6((exedir+"share/fflj12_6.config").c_str(), _pbc, _psp, _cutoff, _pairmax);
    else
        _fail = true;

    gro->InitTypes(_ff);

    _vv = new ITRVVerlet(*_psp, _pbc, _ff, _dt);
    if(!gro->IsVInit())
        _vv->InitVel(_initT, true);
    else
        _vv->InitVel(_initT, false);


    _ff->UpdateForce();

    REAL BL = _pbc->GetL();
    cout << "Get " << _psp->GetNP() << " Atoms." << endl;
    cout << "Init Temperature: " << _initT << " K." << endl;
    cout << "Box Side Length: " << BL << " nm." << endl;
    cout << "Density: " << _ff->GetM(2)*_psp->GetNP()*10/6.022/(BL*BL*BL) << " g/L." << endl;
    cout << "Volume: " << 6.022*(BL*BL*BL)/_psp->GetNP()/10 << " L." << endl;

    _writer = new GroConfigWriter(_psp, tr, st);
    if(_nstrace > 0)
        _writer->AppendTrace("LDMM Sample", _curstep*_dt, _pbc->GetL());
    _writer->AppendStatistics(_vv, _curstep*_dt);

    if(_fail)
        cout << "Initialization Failed. Exit..." << endl;
    else
        cout << "Initialization Done." << endl;
    cout << endl;

    ShowProgress();
}
MDSystem::~MDSystem()
{
    if(_config)
        delete _config;
    if(_vv)
        delete _vv;
    if(_psp)
        delete _psp;
    if(_writer)
        delete _writer;
    if(_pbc)
        delete _pbc;
    if(_ff)
        delete _ff;

    cout << "Simulation Completed." << endl;
}

void MDSystem::Sample()
{
    ++_curstep;
    _vv->UpdatePosAndVel();
}

void MDSystem::Update()
{
    if(_nsscale > 0)
        if(_curstep%_nsscale == 0)
            _vv->RescaleV(_endT, _scaleFactor);

    if(_nsneighberupdate > 0)
        if(_curstep%_nsneighberupdate == 0)
            _ff->UpdateNeighbers();

    _ff->UpdateDistances();

    if(_curstep == 1){
        _colT = 0.0;
        _colP = 0.0;
    }

    if(_curstep > _batchcollect){
        _colT += _vv->GetInstT();
        _colP += _vv->GetInstP();
    }
}

void MDSystem::Output()
{
    if(_nstrace > 0)
        if(_curstep%_nstrace == 0)
            _writer->AppendTrace(_title.c_str(), _curstep*_dt, _pbc->GetL());

    if(_nsstatistics > 0)
        if(_curstep%_nsstatistics == 0)
            _writer->AppendStatistics(_vv, _curstep*_dt);

    ShowProgress();

    if(_curstep == _totalsteps){
        int N = _totalsteps - _batchcollect;
        printf("\nT: %6.2f K, P: %10.6f bar.\n",
               _colT/N, 16.6054*_colP/N);

        if(_batchmode && _curpass < _batchpass){
            printf("Pass %d Finished.\n", _curpass);

            ++_curpass;
            _endT += _batchtstep;
            _curstep = 0;

            ShowProgress();
        }
    }
}

bool MDSystem::IsEnd() const
{
    return (_fail ? true : (_curstep == _totalsteps));
}

void MDSystem::ShowProgress() const
{
    if(_totalsteps < 1000 || (_curstep%(_totalsteps/1000)) == 0){
        float progress = (float)_curstep/(float)_totalsteps;
        if(_totalsteps < 50 || (_curstep%(_totalsteps/50)) == 0){
            printf("\r[");
            int i = (int)(progress*50);
            for(int j=0; j<50; ++j)
                if(j < i)
                    putchar('=');
                else
                    putchar(' ');
            printf("] ");
        }else
            printf("\b\b\b\b\b");
        printf("%4.1f%%", progress*100);
        fflush(stdout);
    }
    if(_curstep == _totalsteps) printf("\n");
}

