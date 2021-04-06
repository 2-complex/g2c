#ifndef _WAVE_
#define _WAVE_

#include <stdint.h>
#include <stddef.h>

/*! A utility class for Wave (as in .wav file) sound data.  To use Wave, open
    a .wav file, and read in all the data to memory.  Then initialize the Wave
    object using initWithData().  That function will populate the public members
    with the sample-rate number of channels number of samples and bytes-per-sample
    from the file.  The member data will point to the first byte in the raw PCM
    sound data.*/
class Wave {
public:
    /*! The number of samples per second.  44100 for CD-quality sound stereo or mono.*/
    int sampleRate;

    /*! 1 for mono 2 for stereo. */
    int numChannels;

    /*! The number of samples.  Each sample is a a packet of data consisting of one
       number for each channel.  So, for CD-quality stero a sample is two numebrs,
       a total of 4 bytes.*/
    int numSamples;

    /*! 1 for 8-bit sound, 2 for 16-bit sound.*/
    int bytesPerSample;

    /*! A pointer to the raw pcm audio data.*/
    uint8_t* data;
    
    /*! Initialize the Wave file with a pointer to .wav file data.  This populates
        the member variables.*/
    void initWithData(const uint8_t* data, size_t size);
};

#endif

