#ifndef ICLASSTOOL_H
#define ICLASSTOOL_H
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavigator.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/OecHeader.h"

#include <vector>
#include <string>

class classifyQuantity{
    public:
        classifyQuantity();
        classifyQuantity(const std::vector<std::string>&);
        ~classifyQuantity();

        //Set functions
        void addName(std::string);
        void addName(const std::vector<std::string>&);
        bool setValue(std::string, float);
        bool setValue(const std::map<std::string, float>&);

        //Get functions
        int getN() const {return m_classQuantity.size();}
        const std::vector<std::string>& getName() const{return v_qname;}
        float getValue(std::string) const;
        std::vector<float> getValue() const;

    protected:
        std::map<std::string, float> m_classQuantity;
        std::vector<std::string> v_qname;
};

class IClassTool{
    public:
        IClassTool();
        virtual ~IClassTool();

        //type define
        typedef std::pair<JM::OecEvt*, JM::OecEvt*> EventPair;

        //Get functions
        const std::vector<EventPair>& getEventPair(){return m_eventPair;}
        const std::vector<classifyQuantity>& getClassQuantity(){return m_classQuantity;}

        virtual bool isVetoed(JM::NavBuffer::Iterator) = 0;
        virtual bool findCorrelation(JM::NavBuffer::Iterator, std::vector<JM::OecEvt*>&) = 0;//Find the events correlated to this event
        virtual bool calClassifyQuantity(const std::vector<JM::OecEvt*>&) = 0;//Input:the events correlated in time
        virtual bool userFunc(JM::EvtNavigator*, const EventPair&) = 0;

    protected:
        std::vector<EventPair> m_eventPair;
        std::vector<classifyQuantity> m_classQuantity;
};
#endif
