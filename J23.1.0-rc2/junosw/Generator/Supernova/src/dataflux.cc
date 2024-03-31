#include <iostream>
#include <fstream>
#include <cstdlib>
#include "dataflux.h"

using namespace std;

TDataFlux::TDataFlux(const char *filename, double _D, int _isNH,bool _isSN)
{
    ifstream ifs;
    NODE node;

    D = _D;
    isNH = _isNH;

    if(_isSN) Nepos = 20;
    else Nepos = 60;
    ifs.open(filename);

    if(!ifs.good())
    {
        cout<<"Fail to open file "<<endl;
        exit(1);
    }

    while(true)
    {
        ifs>>node.time;
        if(!ifs.good())
        {
            break;
        }
        for(int i = 0; i < Nepos; i++)
        {
            ifs>>node.spectrum[i].eStart
                >>node.spectrum[i].eEnd
                >>node.spectrum[i].fluxNuE
                >>node.spectrum[i].fluxNuBarE
                >>node.spectrum[i].fluxNuX
                >>node.spectrum[i].lumNuE
                >>node.spectrum[i].lumNuBarE
                >>node.spectrum[i].lumNuX;
        }
        data.push_back(node);
    }
    ifs.close();
}

TDataFlux::~TDataFlux()
{
    data.clear();
}

double TDataFlux::GetLocalFlux(double t, double e, NuType type)
{
    double pBarEBarE, pEE, flux1, flux2, flux;

    //no osc
    if(isNH == -1){
        pEE = 1;
        pBarEBarE = 1;
    }
    //MSW effect 
    //Dighe & Smirnov 
    //Phys.Rev. D62 (2000) 033007
    else if(isNH == 1)
    {
        pEE = 0.022;//sin^{2}theta13 
        pBarEBarE = 0.687;//cos^{2}theta12cos^{2}theta13
    }
    else
    {
        pEE = 0.291;//sin^{2}theta12cos^{2}theta13
        pBarEBarE = 0.022;//sin^{2}theta13
    }

    switch(type)
    {
        case nuE:
            flux1 = interpolate(t, e, nuE);
            flux2 = interpolate(t, e, nuX);
            flux = flux1 * pEE + (1 - pEE) * flux2;
            break;
        case nuAE:
            flux1 = interpolate(t, e, nuAE);
            flux2 = interpolate(t, e, nuAX);
            flux = flux1 * pBarEBarE + (1 - pBarEBarE) * flux2;
            break;
        case nuX:
            flux1 = interpolate(t, e, nuE);
            flux2 = interpolate(t, e, nuX);
            flux = flux2 * (1 + pEE) + (1 - pEE) * flux1;
            break;
        case nuAX:
            flux1 = interpolate(t, e, nuAE);
            flux2 = interpolate(t, e, nuAX);
            flux = flux2 * (1 + pBarEBarE) + (1 - pBarEBarE) * flux1;
            break;
        default:
            cerr<<"Invalid flux type."<<endl;
            exit(1);			
    }
    flux /= (4 * M_PI * pow(3.08567758149e21 * D, 2));	
    return flux;
}

double onedimintp(double x1, double x2, double f1, double f2, double x)
{
    if(f1 == f2)
    {
        return f1;
    }
    return (f2 - f1) / (x2 - x1) * (x - x1) + f1;
}

//spec = 0,1,2 means nue,nuebar,nux in original data
double TDataFlux::interpolate(double t, double e, NuType type)
{
    double t1, t2, e1, e2, f11, f12, f21, f22, f;
    int tpos, epos, tpos1, tpos2, epos1, epos2, size = data.size();

    if(t < data[0].time || t > data[size - 1].time)
        //	if(t < 0.0 || t > data[size - 1].time)
    {
        return 0.0;
    }
    if(e < data[0].spectrum[0].eStart || e > data[0].spectrum[Nepos-1].eEnd)
    {
        return 0.0;
    }

    for(tpos = 0;tpos < size; tpos ++)
    {
        if(t <= data[tpos].time)
        {
            break;
        }
    }
    for(epos = 0;epos < Nepos; epos ++)
    {
        if(e <= data[0].spectrum[epos].eEnd)
        {
            break;
        }
    }

    tpos2 = tpos;
    tpos1 = tpos == 0 ? 0 : tpos - 1;
    t2 = data[tpos2].time;
    t1 = data[tpos1].time;

    if(e >= (data[0].spectrum[epos].eStart + data[0].spectrum[epos].eEnd) / 2)
    {
        epos1 = epos;
        epos2 = epos + 1;
        e1 = (data[0].spectrum[epos1].eStart + data[0].spectrum[epos1].eEnd) / 2;
        e2 = epos2 > (Nepos-1) ? data[0].spectrum[Nepos-1].eEnd 
            : (data[0].spectrum[epos2].eStart + data[0].spectrum[epos2].eEnd) / 2;
    }
    else
    {
        epos2 = epos;
        epos1 = epos - 1;
        e2 = (data[0].spectrum[epos2].eStart + data[0].spectrum[epos2].eEnd) / 2;
        e1 = epos1 < 0 ? data[0].spectrum[0].eStart 
            : (data[0].spectrum[epos1].eStart + data[0].spectrum[epos1].eEnd) / 2;

    }

    switch(type)
    {
        case nuE:
            f11 = epos1 < 0 ? 0 : data[tpos1].spectrum[epos1].fluxNuE;
            f21 = epos1 < 0 ? 0 : data[tpos2].spectrum[epos1].fluxNuE;
            f12 = epos2 > (Nepos-1) ? 0 : data[tpos1].spectrum[epos2].fluxNuE;
            f22 = epos2 > (Nepos-1) ? 0 : data[tpos2].spectrum[epos2].fluxNuE;
            break;
        case nuAE:
            f11 = epos1 < 0 ? 0 : data[tpos1].spectrum[epos1].fluxNuBarE;
            f21 = epos1 < 0 ? 0 : data[tpos2].spectrum[epos1].fluxNuBarE;
            f12 = epos2 > (Nepos-1) ? 0 : data[tpos1].spectrum[epos2].fluxNuBarE;
            f22 = epos2 > (Nepos-1) ? 0 : data[tpos2].spectrum[epos2].fluxNuBarE;
            break;
        case nuX:
        case nuAX:
            f11 = epos1 < 0 ? 0 : data[tpos1].spectrum[epos1].fluxNuX;
            f21 = epos1 < 0 ? 0 : data[tpos2].spectrum[epos1].fluxNuX;
            f12 = epos2 > (Nepos-1) ? 0 : data[tpos1].spectrum[epos2].fluxNuX;
            f22 = epos2 > (Nepos-1) ? 0 : data[tpos2].spectrum[epos2].fluxNuX;
            break;
        default:
            cerr<<"Wrong NuType in interpolation."<<endl;
            f11 = 0.0;
            f12 = 0.0;
            f21 = 0.0;
            f22 = 0.0;
    }
    f = onedimintp(e1, e2, onedimintp(t1, t2, f11, f21, t), onedimintp(t1, t2, f12, f22, t), e);
    //std::cout << t << "s, " << e << "MeV, flux "<< f << std::endl;
    return f;
}
