//
// Created by jianddongguo on 2018/12/3.
//
#ifndef FFMPEG4ANDROID_OPENSLES_H
#define FFMPEG4ANDROID_OPENSLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "android/log.h"
#include <stdint.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>

#define TAG "PlayAudio"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,"%s",__VA_ARGS__)
#define LOG_I(format,...) __android_log_print(ANDROID_LOG_INFO,TAG,format,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,"%s",__VA_ARGS__)
#define LOG_E(format, ...) __android_log_print(ANDROID_LOG_ERROR,TAG,format,__VA_ARGS__)

struct PCMData {
    char *pcm;
    int size;
};

struct PCMDataNode {
    PCMData pkt;
    struct PCMDataNode *next;
};

struct PCMDataQueue {
    PCMDataNode *first_pkt, *last_pkt;
    int nb_packets;

    pthread_mutex_t mutex;    // 互斥锁
};

typedef struct OpenSLES {
    // 引擎
    SLObjectItf pEngineObject = NULL;
    SLEngineItf pEngineItf = NULL;
    // 混音器
    SLObjectItf pOutputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    // 播放器
    SLObjectItf pPlayerObject = NULL;
    SLPlayItf pPlayerItf = NULL;
    SLVolumeItf pVolumeItf = NULL;
    SLAndroidSimpleBufferQueueItf pBufferItf = NULL;

    PCMDataQueue queue_play;
    int isExit = 0;
} Global_OpenSL;

int createOpenSLEngine(int nbChannels,int sampleRate,int sampleFormat);
void destoryOpenSLEngine();

void playQueueInit(PCMDataQueue *pcmQueue);
int playQueuePut(PCMDataQueue *pcmQueue, PCMData *pcmPkt);
int playQueueGet(PCMDataQueue *pcmQueue, PCMData *pcmPkt);
int getQueueSize(PCMDataQueue *pcmQueue);

extern Global_OpenSL global_openSL;

#ifdef __cplusplus
}
#endif
#endif