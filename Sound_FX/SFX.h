#include "../deps/include/SDL/SDL.h"
#include "../deps/include/SDL/SDL_mixer.h"

class SFX {
    private:

    public:
        // constructor method, which inits all sound effects 
        SFX();

        // destructor
        ~SFX();

        // functions to initialize the FX and close the interface once done
        void closeFX();
};