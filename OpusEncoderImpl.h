//
// Created by Faker on 2024/1/23.
//

#ifndef OPUS_DEMO_OPUSENCODERIMPL_H
#define OPUS_DEMO_OPUSENCODERIMPL_H

#include "opus/opus.h"
#include <vector>
#include <mutex>
#include "base_type.h"
#include <queue>
#include <thread>


class OpusEncoderImpl
{
private:
    OpusEncoder *encoder;
    const int channel_num;
    int sample_rate;
    std::queue<StreamInfo> info_queue;
    std::queue <unsigned char> pcm_queue;
    std::mutex mutex;
    bool isRuning = true;
    std::mutex access_mutex;
    std::unique_ptr<std::thread> m_thread;
public:
    OpusEncoderImpl(int sampleRate, int channel);
    void Feed(unsigned char*data, int len);
    bool PopFrame(StreamInfo &info);
    void EncodeRun();
    void Stop();
    ~OpusEncoderImpl();
};
#endif //OPUS_DEMO_OPUSENCODERIMPL_H
