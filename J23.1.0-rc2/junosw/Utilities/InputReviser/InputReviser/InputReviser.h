#ifndef COMMON_INPUT_REVISER_H
#define COMMON_INPUT_REVISER_H

#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"

class IInputStream;

class InputReviser : public Incident
{
    public :

        InputReviser(Task& task, const std::string& msg, bool infinite = false);

        virtual ~InputReviser();

        virtual int fire(Task& domain);

        bool  reset(int entry);

        int   getEntries();
        int   getEntry();
        std::string getFileName();

        const std::string& objName();

    private :

        bool init();

        Task&             m_par; // root task
        bool              m_infinite;
        bool              m_revise;
        int               m_entries;
        IInputStream*     m_is;

        std::string m_objname;
};

#endif
