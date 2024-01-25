//
// Created by Faker on 2024/1/23.
//

#include "OpusEncoderImpl.h"
#include "OpusDecoderImpl.h"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#define MAX_PACKET_SIZE 3*1276

/*
* sampleRate:采样率
* channel：通道数
*/

OpusEncoderImpl::OpusEncoderImpl(int sampleRate, int channel):channel_num(channel),sample_rate(sampleRate)
{
    int err;
    int applications[3] = {OPUS_APPLICATION_AUDIO, OPUS_APPLICATION_VOIP, OPUS_APPLICATION_RESTRICTED_LOWDELAY};

    encoder = opus_encoder_create(sampleRate, channel_num, applications[0], &err);

    if(err != OPUS_OK || encoder == NULL) {
        printf("打开opus 编码器失败\n");
    }

    opus_encoder_ctl(encoder, OPUS_SET_VBR(0));//0:CBR, 1:VBR
    opus_encoder_ctl(encoder, OPUS_SET_VBR_CONSTRAINT(true));
    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(96000));
    opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(8));//8    0~10
    opus_encoder_ctl(encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
    opus_encoder_ctl(encoder, OPUS_SET_LSB_DEPTH(16));//每个采样16个bit，2个byte
    opus_encoder_ctl(encoder, OPUS_SET_DTX(0));
    opus_encoder_ctl(encoder, OPUS_SET_INBAND_FEC(0));

    EncodeRun();
}
//每一帧pcm 是23ms
void OpusEncoderImpl::Feed(unsigned char *data, int len) {
    mutex.lock();
    for(auto i = 0;i < len;i++) {
        pcm_queue.emplace(data[i]);
    }
    mutex.unlock();
}

bool OpusEncoderImpl::PopFrame(StreamInfo &info) {
    if(info_queue.size() > 0) {
        access_mutex.lock();
        info = info_queue.front();
        info_queue.pop();
        access_mutex.unlock();
        return true;
    }

    return false;
}


//48000 采样率，48个样本/ms * 10ms * 1 channel = 480
void OpusEncoderImpl::EncodeRun() {
    m_thread = std::make_unique<std::thread>([this](){
        const int frame_size = 480;
        const int input_len = sizeof(opus_int16) * frame_size * 1;
//        std::cout << "input_len: " << input_len << std::endl;

        FILE *opus_file = fopen("/Users/faker/Desktop/code/opus-demo/out-mult.opus", "wb+");
        OpusDecoderImpl decoder(48000, 1);

        opus_int16 input_data[frame_size] = {0};//frame_size*channels*sizeof(opus_int16)
        unsigned char input_buffer[frame_size*2] = {0};//每一帧的数据量
        unsigned char out_data[input_len] = {0};

        int k = 0;
        while (isRuning) {
            if(pcm_queue.size() >= input_len) {
                mutex.lock();
//                cout << pcm_queue.size() << endl;
                for(int i = 0;i < input_len;i++)
                {
                    input_buffer[i] = pcm_queue.front();
                    pcm_queue.pop();
                }
                memset(out_data,0,sizeof(out_data));
                memcpy(input_data, input_buffer, input_len);

                int len = opus_encode(encoder, input_data, frame_size, out_data, sizeof(out_data));

                if(len < 0) {
                    printf("opus编码失败, %d\n", len);
                    break;
                }

                fwrite(&len, sizeof(len),1, opus_file);
                fwrite(out_data, 1, len, opus_file);
//                fflush(opus_file);

                unsigned char* opus_buffer = (unsigned char*)malloc(len);
                memcpy(opus_buffer, out_data, len);
                // 边编码边解码
//                decoder.Decode(opus_buffer,len);

                StreamInfo info;
                info.data = opus_buffer;
                info.len = len;
                info.dts = 10;
                access_mutex.lock();
                info_queue.push(info);
                access_mutex.unlock();
                mutex.unlock();
            }else {
//                usleep(1000);
            }
        }
        fclose(opus_file);

    });

}

void OpusEncoderImpl::Stop() {
    isRuning = false;
    m_thread->join();

    while (pcm_queue.size() > 0)
    {
        pcm_queue.pop();
    }

    opus_encoder_destroy(encoder);

}

OpusEncoderImpl::~OpusEncoderImpl() {

}