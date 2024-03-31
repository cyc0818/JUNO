#ifndef JUNO_DETECTOR_TT_ID_H
#define JUNO_DETECTOR_TT_ID_H

//
//  This class provide hierarchy and functions for Top Tracker Identifier
//
//  Author: Kaijie Li
//

#include "Identifier/JunoDetectorID.h"
#include <string>
#include <assert.h>

class TtID : public JunoDetectorID
{
    public:

        typedef Identifier::size_type  size_type; 
        typedef Identifier::value_type value_type; 

        /// constructor
        TtID();

        /// destructor 
        ~TtID(void);
 
        /// For a single channel
        static Identifier id ( unsigned int layer,
                               unsigned int column,
                               unsigned int row,
                               unsigned int pmt,
                               unsigned int strip
                             );
	static Identifier id ( unsigned int wall_id,
		               unsigned int channel
			     );
        static Identifier id ( int value );
        static Identifier id ( const char * path ); /* path of TGeoNode like /lWorld_1/.../pBar... */
        static unsigned int getIntID ( unsigned int wall_id,
                                       unsigned int pmt,
                                       unsigned int strip
                                     );
        static unsigned int getIntID ( unsigned int layer,
                                       unsigned int column,
                                       unsigned int row,
                                       unsigned int pmt,
                                       unsigned int strip
                                     );
        static value_type getIntID ( unsigned int wall_id,
		                     unsigned int channel
				   );
	static bool valuesOk ( const unsigned int wall_id,
	                       const unsigned int channel
		       	     );

        // Wall Id = layer-column-row
        static int wall_id (const Identifier& id);
        // Deprecated: please use wall_id.
        // The reasons for the deprecations are 2-fold:
        //  - wall_id is clearer a name than module
        //  - from C++20 module is designed an identifier with special meaning
        static inline int
#ifdef __GNUC__
                __attribute__ ((deprecated))
#endif
                module  (const Identifier& id) { return wall_id(id); }
        // Channel => ID in wall, ie PMT-strip
        static int channel (const Identifier& id);

        static int layer  (const Identifier& id);
        static int column (const Identifier& id);
        static int row    (const Identifier& id);
        static int pmt    (const Identifier& id);
        static int strip  (const Identifier& id);

        /// Every TT strip is seen by 2 PMTs, so getting the PMT number corresponding to
        /// the opposite side or from the lower numberd side is useful to check PMTs
        /// where a signal would be expected and have unique identifiers.
        static int pmt_other_end   (const int pmt);
        static int lower_pmt_index (const int pmt);
        static Identifier id_other_end (const Identifier & id );

        static const int * pmts_perp(const int pmt);

        /// Retrieve pmt id inside a wall from the information in Geant4
        /// side   => RO in 'left' (0) or 'right' (1) of the pannel
        /// plane  => top (X-strips: PMT_l => 0-4) or bottom (Y-strips: PMT_l => 4-7)
        /// pannel => 0-3 number of the TT pannel in plane
        static int pmt_from_geant(const int side, const int plane, const int pannel);

	/// Max/Min values for each field (error returns -999)
	static int wallIDMin();
	static int wallIDMax();
        static int wallIDChannelMin();
	static int wallIDChannelMax();

	static int wallIDChannelNumber();

	/// Set WallID Max(when geometry not fixed)
	static void setWallIDMax(unsigned int value) { WALLID_MAX = value; }
	static void setWallIDChannelMin(unsigned int value) { WALLID_CHANNEL_MIN = value; }
	static void setWallIDChannelMax(unsigned int value) { WALLID_CHANNEL_MAX = value; }
    private:

	typedef std::vector<Identifier> idVec;
	typedef idVec::const_iterator   idVecIt;

	static const unsigned int WALLID_MIN      = 0;
	static       unsigned int WALLID_MAX;
	
	static       unsigned int WALLID_CHANNEL_MIN;
	static       unsigned int WALLID_CHANNEL_MAX;

	//static const unsigned int WALLID_CHANNEL_MAX  = 0;
	//static const unsigned int WALLID_CHANNEL_MIN  = 0;

        static const unsigned int STRIP_INDEX  =  0;
        static const unsigned int STRIP_MASK   = 0x3F;              // 0x0000003F

        static const unsigned int PMT_INDEX    =  6;
        static const unsigned int PMT_MASK     = 0xF<<PMT_INDEX;    // 0x000003C0

        static const unsigned int ROW_INDEX    = 10;
        static const unsigned int ROW_MASK     = 0x7<<ROW_INDEX;    // 0x00001C00

        static const unsigned int COLUMN_INDEX = 13;
        static const unsigned int COLUMN_MASK  = 0x7<<COLUMN_INDEX; // 0x0000E000

        static const unsigned int LAYER_INDEX  = 16;
        static const unsigned int LAYER_MASK   = 0x7<<LAYER_INDEX;  // 0x00070000

        static const unsigned int TT = (TT_ID << SUB_INDEX) & SUB_MASK;

        static const unsigned int WALLID_INDEX  = ROW_INDEX;
        static const unsigned int WALLID_MASK   = ROW_MASK | COLUMN_MASK | LAYER_MASK;

        static const unsigned int CHANNEL_INDEX = STRIP_INDEX;
        static const unsigned int CHANNEL_MASK  = STRIP_MASK | PMT_MASK;

        static const int s_pmt_other_end[];
        static const int s_pmt_perp[2][8];

};

#endif // JUNO_DETECTOR_TT_ID_HYER_MASK
