实现pcm转opus 实现48000hz 单通道 16bit转 48000hz 单通道 16bit opus 相互转换

输入为10ms一帧的pcm pcm需要包大小为 48000 * 0.01 * channel（1）* 采样深度16bit / 8 = 960 byte