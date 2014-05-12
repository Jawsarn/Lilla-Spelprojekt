#include "fmod.hpp"
#include <vector>

struct AudioHolder
{
	FMOD::Sound* sound;
	FMOD::Channel* channel;
};