#ifndef JUNO_VIS_STATUS_H
#define JUNO_VIS_STATUS_H

//
//  Juno Visualization Status
//
//  Author: Zhengyun You  2014-10-27
//

class JVisStatus
{
    public :

        JVisStatus();
        ~JVisStatus();

        bool init();

        bool m_simInit;
        bool m_calibInit;
        bool m_recInit; 
};

#endif // JUNO_VIS_STATUS_H
