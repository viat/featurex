/*
 * AudioQ.h -
 *
 *  Created on: Jul 2, 2012
 *      Author: grutzek
 */

#ifndef INQUEUE_MT_H_
#define INQUEUE_MT_H_

#include <queue>
#include <boost/thread.hpp>

struct audioQueueData
{
	unsigned long id;		// call id
	unsigned long numSamples;  // length of call < 6 seconds
	signed short *data;		// audio samples, 16 Bit Linear PCM
};

class AudioQ {
public:

    void push(audioQueueData *call);
    bool empty();
    audioQueueData* front();
    void pop();
    long size();

private:

    std::queue<audioQueueData*> queueMT;
    boost::mutex queueMTMutex;
};

#endif /* INQUEUEMT_H_ */
