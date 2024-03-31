#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "IADUtil/IADBlockContext.h"
#include "SniperMuster/GlobalStream.h"
#include "EvtNavigator/EvtNavigator.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/Task.h"
#include <fstream> 
#include <stdio.h>

#include <memory>

using namespace std;
using namespace JM;



class ClearBufferAlg: public AlgBase{
public:
    ClearBufferAlg(const string& name);
    virtual ~ClearBufferAlg();

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

private:
    GlobalBuffer<IADBlockContext>* m_gbuf;

};


DECLARE_ALGORITHM(ClearBufferAlg);

ClearBufferAlg::ClearBufferAlg(const string& name):
    AlgBase(name){

    }

ClearBufferAlg::~ClearBufferAlg(){

}

bool ClearBufferAlg::initialize(){
    m_gbuf = GlobalStream<IADBlockContext>::GetBufferFrom("IADGlobalStream");
    return true;
}

bool ClearBufferAlg::execute(){
    /*{
        auto bufSize = m_gbuf->rough_size();
        ofstream output("buffer_status.txt", ofstream::app);
        output<<bufSize<<std::endl;
    }*/
    //std::cout<<"output thread execute"<<std::endl;

    auto result = m_gbuf->pop_front();
    //std::cout<<"clear one elem"<<std::endl;

    if(result == nullptr){
        m_par->stop();
    }

    return true;
}


bool ClearBufferAlg::finalize(){
    return true;
}
