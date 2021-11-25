#ifndef AUDIO_H_
#define AUDIO_H_

#include "SDL_mixer.h"

namespace audio
{
    void Init(void);
    void Quit(void);

    namespace BGM
    {
        void Init(void);
        void Quit(void);
        void Loop(void);
        void Stop(void);
    }
}


#endif /* AUDIO_H_ */
