#ifndef JUNO_VIS_OPTRACK_H
#define JUNO_VIS_OPTRACK_H

//
//  Juno Visualization Optical Photon Track
//
//  Author: Zhengyun You  2016-04-16
//

#include "Identifier/Identifier.h"

#include "TEveTrack.h"
#include <vector>
#include <map>

class JVisOpTrack : public TEveTrack 
{
    public :

        JVisOpTrack();
        virtual ~JVisOpTrack();

        bool init();

    private :

};

#endif // JUNO_VIS_OPTRACK_H


