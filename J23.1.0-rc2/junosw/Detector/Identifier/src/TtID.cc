//
//  Author: Kaijie Li
//

#include "Identifier/TtID.h"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdexcept>

unsigned int TtID::WALLID_MAX = 255;
unsigned int TtID::WALLID_CHANNEL_MIN = 0;
unsigned int TtID::WALLID_CHANNEL_MAX = 32256;

TtID::TtID(void)
{
}

TtID::~TtID(void)
{
}

//----------------------------------------------------------------------------
bool TtID::valuesOk ( const unsigned int wall_id,
                      const unsigned int channel
		    )
{
    if ( wall_id > WALLID_MAX)  return false;
    if ( channel > WALLID_CHANNEL_MAX)  return false;

    return true;
}

//----------------------------------------------------------------------------
int TtID::wall_id (const Identifier& id)
{
    return ((id.getValue() & TtID::WALLID_MASK) >>  TtID::WALLID_INDEX);
}

//----------------------------------------------------------------------------
int TtID::channel(const Identifier& id)
{
    return ((id.getValue() & TtID::CHANNEL_MASK) >>  TtID::CHANNEL_INDEX);
}

//----------------------------------------------------------------------------
int TtID::layer  (const Identifier& id)
{
    return ((id.getValue() & LAYER_MASK ) >>  LAYER_INDEX );
}

//----------------------------------------------------------------------------
int TtID::column (const Identifier& id)
{
    return ((id.getValue() & COLUMN_MASK) >>  COLUMN_INDEX);
}

//----------------------------------------------------------------------------
int TtID::row    (const Identifier& id)
{
    return ((id.getValue() & ROW_MASK   ) >>  ROW_INDEX   );
}

//----------------------------------------------------------------------------
int TtID::pmt    (const Identifier& id)
{
    return ((id.getValue() & PMT_MASK   ) >>  PMT_INDEX   );
}

//----------------------------------------------------------------------------
int TtID::strip  (const Identifier& id)
{
    return ((id.getValue() & STRIP_MASK ) >>  STRIP_INDEX );
}

//----------------------------------------------------------------------------
const int TtID::s_pmt_other_end[] =
    { 11, 10,  9,  8,
      15, 14, 13, 12,
       3,  2,  1,  0,
       7,  6,  5,  4 };

//----------------------------------------------------------------------------
int TtID::pmt_other_end(const int pmt)
{
    return s_pmt_other_end[pmt];
}

//----------------------------------------------------------------------------
int TtID::lower_pmt_index(const int pmt)
{
    return std::min(pmt, s_pmt_other_end[pmt]);
}

//----------------------------------------------------------------------------
Identifier TtID::id_other_end (const Identifier & id )
{
    int layer = TtID::layer(id);
    int column = TtID::column(id);
    int row = TtID::row(id);
    int pmt = TtID::pmt(id);
    int strip = TtID::strip(id);
    return TtID::id(layer, column, row, TtID::pmt_other_end(pmt), strip);
}

//----------------------------------------------------------------------------
const int TtID::s_pmt_perp[2][8] = {
    { 4, 5, 6, 7, 12, 13, 14, 15 },
    { 0, 1, 2, 3,  8,  9, 10, 11 }
};
//----------------------------------------------------------------------------
const int * TtID::pmts_perp(const int pmt){
    return s_pmt_perp[(pmt/4)%2];
}

//----------------------------------------------------------------------------
int TtID::pmt_from_geant(const int side, const int plane, const int panel){
    int pmt = -1;
    if     (side == 0 && plane == 1) pmt = panel;
    else if(side == 0 && plane == 0) pmt = 4+panel;
    else if(side == 1 && plane == 1) pmt = 11-panel;
    else if(side == 1 && plane == 0) pmt = 15-panel;
    return pmt;
}

//----------------------------------------------------------------------------
int TtID::wallIDMax()
{
    return WALLID_MAX;
}

//----------------------------------------------------------------------------
int TtID::wallIDMin()
{
    return WALLID_MIN;
}

//----------------------------------------------------------------------------
int TtID::wallIDChannelMax()
{
    return WALLID_CHANNEL_MAX;
}

//----------------------------------------------------------------------------
int TtID::wallIDChannelMin()
{
    return WALLID_CHANNEL_MIN;
}

//----------------------------------------------------------------------------
int TtID::wallIDChannelNumber()
{
    return wallIDChannelMax() - wallIDChannelMin() + 1;
}

//----------------------------------------------------------------------------
Identifier TtID::id ( unsigned int layer,
                      unsigned int column,
                      unsigned int row,
                      unsigned int pmt,
                      unsigned int strip
                    )
{
    return Identifier(getIntID(layer, column, row, pmt, strip));
}

//----------------------------------------------------------------------------
Identifier TtID::id ( unsigned int wall_id,
                      unsigned int channel
					)
{
	assert ( valuesOk(wall_id, channel) );
	unsigned int value = (JunoDetectorID::TT_ID << SUB_INDEX) |
			     (wall_id << WALLID_INDEX) |
			     (channel << CHANNEL_INDEX);
	return Identifier(value);
}

//----------------------------------------------------------------------------
Identifier TtID::id(int value)
{
    return Identifier(TT|value);
}

//----------------------------------------------------------------------------
Identifier TtID::id(const char * path)
{
    int wall   = -1;
    int pmt    = -1;
    int strip  = -1;
    int panel  = -1;
    int plane  = -1;

    std::string spath = path;
    std::replace( spath.begin(), spath.end(), '_', ' '); // needed to use stringstream
    int slash = spath.find('/');
    int next_slash = spath.find('/',slash+1);
    while(slash != std::string::npos){
        std::string node_name = spath.substr(slash+1,next_slash-slash-1);
        slash = next_slash;
        next_slash = spath.find('/',slash+1);

        std::istringstream iss(node_name);
        std::string node_type;
        int value;
        iss >> node_type >> value;
        if(iss.fail()) continue;

        if(node_type.find("Wall") != std::string::npos){
            wall = value;
            if(node_type[0] == 'l'){
                // logical numbering doesn't work out of the box
                wall -= 76;
                static int lwall_correspondence[] = {
                     0, 64, 128,
                     1, 65, 129,
                     2, 66, 130,
                     3, 67, 131,
                     4, 68, 132,
                     5, 69, 133,
                     6, 70, 134,
                    16, 80, 144,
                    17, 81, 145,
                    18, 82, 146,
                    19, 83, 147,
                    20, 84, 148,
                    21, 85, 149,
                    22, 86, 150,
                     8, 72, 136,
                     9, 73, 137,
                    10, 74, 138,
                    12, 76, 140,
                    13, 77, 141,
                    14, 78, 142,
                    11, 75, 139
                };
                wall = lwall_correspondence[wall];
            }
        }
        else if(node_type.find("Plane") != std::string::npos){
            plane = value;
            if(node_type[0] == 'l'){
                // logical numbering doesn't work out of the box
                plane -= 74;
            }
        }
        else if(node_type.find("Panel") != std::string::npos){
            if(node_type.find("Tape") == std::string::npos) {
                panel = value;
                if(node_type[0] == 'l'){
                    // logical numbering doesn't work out of the box
                    panel -= 70;
                }
            }
        }
        else if(node_type.find("Coating") != std::string::npos){
            strip = value;
            if(node_type[0] == 'l'){
                // logical numbering doesn't work out of the box
                strip -= 5;
            }
        }
    }

    if(plane >= 0 && panel >= 0){
        pmt = pmt_from_geant(0, plane, panel);
    }

    if(wall < 0 || pmt < 0 || strip < 0){
        // Identification did not work
        std::cerr << "Element path \"" << path << "\" not in TT" << std::endl;
        throw std::invalid_argument("Could not decode path");
    }

    return Identifier(getIntID(wall,pmt,strip));
}

//----------------------------------------------------------------------------
unsigned int TtID::getIntID ( unsigned int wall_id,
                              unsigned int pmt,
                              unsigned int strip
                            )
{
    wall_id= (wall_id<< WALLID_INDEX) & WALLID_MASK;
    pmt    = (pmt    << PMT_INDEX   ) & PMT_MASK;
    strip  = (strip  << STRIP_INDEX ) & STRIP_MASK;
    return (TT|wall_id|pmt|strip);
}
//----------------------------------------------------------------------------
unsigned int TtID::getIntID ( unsigned int layer,
                              unsigned int column,
                              unsigned int row,
                              unsigned int pmt,
                              unsigned int strip
                            )
{
    layer  = (layer  << LAYER_INDEX ) & LAYER_MASK;
    column = (column << COLUMN_INDEX) & COLUMN_MASK;
    row    = (row    << ROW_INDEX   ) & ROW_MASK;
    pmt    = (pmt    << PMT_INDEX   ) & PMT_MASK;
    strip  = (strip  << STRIP_INDEX ) & STRIP_MASK;
    return (TT|layer|column|row|pmt|strip);
}
//----------------------------------------------------------------------------
unsigned int TtID::getIntID ( unsigned int wall_id,
                              unsigned int channel
							)
{
    unsigned int value = (JunoDetectorID::TT_ID << SUB_INDEX) |
                         (wall_id << WALLID_INDEX) |
                         (channel << CHANNEL_INDEX);
    return value;
}

