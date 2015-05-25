
#define USE_SOUND
#ifdef  USE_SOUND
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include "fmod.h"


class audio 
{
	public:
		void create_sounds();
		//void music_play();
		//bool times_play = 1;
		int  stop = 0;
	private:
};
#endif //AUDIO
