/*
 * InQueue_MT.h
 *
 *  Created on: Jul 2, 2012
 *      Author: grutzek
 */

#ifndef OUTQUEUE_MT_H_
#define OUTQUEUE_MT_H_

#include <queue>
#include <boost/thread.hpp>

struct hashQueueData
{
	unsigned long id;	 // call id
	unsigned long length; // number of hashes
	unsigned long *data;  // hashes
};

class HashQ {
public:

    void push(hashQueueData* data);
    bool empty();
    hashQueueData* front();
    void pop();
    long size();

private:

    std::queue<hashQueueData*> queueMT;
    boost::mutex queueMTMutex;
};

#endif /* OUTQUEUEMT_H_ */
