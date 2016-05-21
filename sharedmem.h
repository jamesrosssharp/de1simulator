/*
		   Shared memory layout
										*/

#pragma once

struct __attribute__((__packed__)) SharedMemoryLayout 
{

    int redLEDs[10];
    int greenLEDs[8];
    int slideSwitches[10];
    int keys[4];
    int sevenSegmentData[4][8];
    // RGB 32 video data
    int VGAData[480][640];
    int verticalRetrace;
	int requestUIExit;
	int uiExited;

};

#define SHAREDMEMORYSIZE sizeof(struct SharedMemoryLayout)
#define SHAREDMEMORYFILE "/tmp/de1.mem"
