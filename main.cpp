#include <iostream>
#include "base_type.h"
#include "OpusEncoderImpl.h"
#include "OpusDecoderImpl.h"
using namespace std;
int main() {
//
    OpusEncoderImpl* opusEncoder = new OpusEncoderImpl(48000, 1);
    OpusDecoderImpl* opusDecoder = new OpusDecoderImpl(48000,1);
    int readsize = 960;
    char *pcm = new char[readsize];

    char* inPath = "/Users/faker/Desktop/code/opus-demo/out.pcm";
    FILE *fp = fopen(inPath, "rb");
    if (fp == NULL) {
        std::cout << "无法打开文件: " << inPath << std::endl;
        delete[] pcm;
        return 0;
    }
    while(1)
//    for(int i = 0;i < 10000;++i)
    {
        int len = fread(pcm, 1,readsize, fp);
        if (len <= 0){
            break;
        }
        opusEncoder->Feed(reinterpret_cast<unsigned char *>(pcm), len);//送pcm 数据编码
    }
    StreamInfo info;
    while (opusEncoder->PopFrame(info))
    {
        opusDecoder->Decode(info.data,info.len);
    }

    opusEncoder->Stop();


}
