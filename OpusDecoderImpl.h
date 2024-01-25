//
// Created by Faker on 2024/1/23.
//

#ifndef OPUS_DEMO_OPUSDECODERIMPL_H
#define OPUS_DEMO_OPUSDECODERIMPL_H


#include <stdio.h>
#include "opus/opus.h"
#include <vector>
#include <mutex>
#include "base_type.h"
#include <queue>
#include <thread>

class OpusDecoderImpl
{
private:
    /* data */
    OpusDecoder *decoder;
    int sample_rate;
    int channel_num;
    FILE *pcm_file;
public:
    bool Decode(unsigned char* in_data, int len);
    OpusDecoderImpl(int sampleRate, int channel);
    ~OpusDecoderImpl();
};


#endif //OPUS_DEMO_OPUSDECODERIMPL_H
