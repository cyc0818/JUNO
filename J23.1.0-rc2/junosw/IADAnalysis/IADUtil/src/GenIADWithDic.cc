#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "IADFormat/Block.h"
#include "IADFormat/DummyData.h"
#include "IADFormat/IADFileWriter.h"
#include "TTimeStamp.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavigator.h"
#include <Event/OecHeader.h>
#include "IADFormat/MetaBlock.h"

using namespace IAD;
using namespace std;

class GenIADWithDic: public AlgBase
{
private:
    int m_count = 0;//Count the number of the events in the current block
    Block m_block;
    IADFileWriter* m_writer;
    uint32_t m_entry = 0;
    TTimeStamp m_lastTime;
    
    JM::NavBuffer* m_nbuf;

    string m_inputFile;
    string m_outputFile;
public:
    GenIADWithDic(const string& name);
    ~GenIADWithDic();
    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize(){return true;};

    std::map<uint32_t, std::string> m_map;
};

DECLARE_ALGORITHM(GenIADWithDic);

GenIADWithDic::GenIADWithDic(const string& name):
AlgBase(name),
m_block(1024*1024*1024)
{
    declProp("inputFile", m_inputFile);
    declProp("outputFile", m_outputFile);
}

bool GenIADWithDic::initialize(){
    m_map[0] = m_inputFile;
    m_writer = new IADFileWriter(m_outputFile);

    //get the instance of NavBuffer
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_nbuf = navBuf.data();

    m_lastTime = TTimeStamp(0,0);
    return true;
}

bool GenIADWithDic::execute(){
    //get OecEvt
    auto oecNav = m_nbuf->curEvt();
    auto evtHeader = dynamic_cast<JM::OecHeader*>(oecNav->getHeader("/Event/Oec"));
    auto oecEvt = dynamic_cast<JM::OecEvt*>(evtHeader->event("JM::OecEvt"));

    DummyData dummyEvt;
    TTimeStamp time = oecEvt->getTime();
    if(m_lastTime > time)   LogError<<"Fatal Error**************************Wrong Time"<<std::endl;
    m_lastTime = time;

    dummyEvt.u_sec = time.GetSec();
    dummyEvt.u_nanoSec = time.GetNanoSec();
    dummyEvt.f_energy = oecEvt->getEnergy();
    dummyEvt.f_vertexX = oecEvt->getVertexX();
    dummyEvt.f_vertexY = oecEvt->getVertexY();
    dummyEvt.f_vertexZ = oecEvt->getVertexZ();
    dummyEvt.i_tag = oecEvt->getTag();

    if(m_count >= 100000){//Block contans 100000 events 
        m_block.zip();
        m_writer->writeBuffer(m_block.getBuffer());//Write Block to file
        m_block.reset();
        m_count = 0;
    }
    //Add event to Block
    //Parameters are: event, file number, entry, sec, nanoSec
    m_block.addElem(dummyEvt, 0, m_entry, dummyEvt.u_sec, dummyEvt.u_nanoSec);
    m_count++;
    m_entry++;
    

    return true;
}

GenIADWithDic::~GenIADWithDic()
{
    m_block.zip();
    m_writer->writeBuffer(m_block.getBuffer());
    std::cout<<"Wrote a Buffer....Size is "<<(m_block.getBuffer())->size()<<std::endl;

    map<uint32_t, string> m;
    MetaBlock metaBlock(m_map);
    m_writer->writeMetaBlock(metaBlock);
}
