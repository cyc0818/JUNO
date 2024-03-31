#ifndef TRIGGER_TT_L1
#define TRIGGER_TT_L1

#include <deque>

enum TT_L1_Trigger_mode{
	kXY=0,
	kXYT,
	kXYX
};

class TriggerTT_L1{
	public:
		TriggerTT_L1();
		~TriggerTT_L1();

		void SetSteppingWindow(int _SW);
		void SetTimeWindow(int _TW);
		void SetTriggerMode(enum TT_L1_Trigger_mode _TM);

		void Clear();
		/* AddHit tries adding a hit to this L1 trigger.
		 * It will succeed if this hit can still be used in this trigger window
		 * and will fail if not (which would means the time difference between this hit
		 * and the first is larger than the TW provided).
		 * This is done to guarantee we know which hits HasTrigger is using.
		 */
		bool AddHit(int PMTnumber, int time);
		void RemoveFirst();
		/* HasTrigger checks the trigger of the added hits.
		 * trigger_time is a return parameter saving the time
		 * at which this L1 trigger was decided upon. It no trigger
		 * found, trigger_time will not be changed.
		 */
		bool HasTrigger(int * trigger_time = nullptr);

	private:
		enum TT_L1_Trigger_mode TM;
		int TW;
		int SW;

		std::deque<int> PMT_list;
		std::deque<int> time_in_step;
};

#endif // TRIGGER_TT_L1
