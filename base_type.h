//
// Created by Faker on 2024/1/23.
//

#ifndef OPUS_DEMO_BASE_TYPE_H
#define OPUS_DEMO_BASE_TYPE_H

typedef struct StreamInfo
{
    unsigned char *data;
    int len;
    int dts;
}StreamInfo;

#endif //OPUS_DEMO_BASE_TYPE_H
