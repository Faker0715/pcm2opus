cmake_minimum_required(VERSION 3.27)
project(opus_demo)

set(CMAKE_CXX_STANDARD 17)
set(OPUS_DIR /usr/local/opt/opus)
include_directories(${OPUS_DIR}/include/)
link_directories(${OPUS_DIR}/lib/)

add_executable(opus_demo main.cpp
        base_type.h
        OpusEncoderImpl.cpp
        OpusEncoderImpl.h
        OpusDecoderImpl.cpp
        OpusDecoderImpl.h)
target_link_libraries(opus_demo opus)