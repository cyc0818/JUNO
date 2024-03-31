#ifndef JUNO_VIS_HELP_H
#define JUNO_VIS_HELP_H

//
//  Juno Visualization Help
//
//  Author: Yumei Zhang   2014-11-23
//

#include "TObject.h"

class VisHelp
{
    public :

        VisHelp() {;} 
        virtual ~VisHelp() {;}
        
        static const char* m_authorText;
        static const char* m_helpText1;
        static const char* m_helpText2;

    ClassDef(VisHelp, 0)  // Vis Help
};

#endif // JUNO_VIS_HELP_H

