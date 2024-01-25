//
// Created by Faker on 2024/1/23.
//

#include "OpusDecoderImpl.h"
#include <iostream>

#define CHANNELS 2

OpusDecoderImpl::OpusDecoderImpl(int sampleRate, int channel)
{
    int err;
    decoder = opus_decoder_create(sampleRate, channel, &err);
    opus_decoder_ctl(decoder, OPUS_SET_LSB_DEPTH(16));
//    sample_rate = sample_rate;
    channel_num = channel;
    if(err < 0 || decoder == NULL)
    {
        printf("创建解码器失败\n");
        return;
    }

    pcm_file = fopen("/Users/faker/Desktop/code/opus-demo/out-mult.pcm", "wb+");
}

bool OpusDecoderImpl::Decode(unsigned char* in_data, int len)
{
    opus_int16 out[480];
    auto frame_size = opus_decode(decoder, in_data, len, out, 480, 0);
    if (frame_size < 0)
    {
        printf("解码失败\n");
        return false;
    }
//    std::cout << "decode " << len << std::endl;

    fwrite(out, sizeof(opus_int16), 480 , pcm_file);
    fflush(pcm_file);
    return true;
}

OpusDecoderImpl::~OpusDecoderImpl()
{
    fclose(pcm_file);
}
