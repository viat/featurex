/*
 * AudioQ.cpp
 *
 *  Created on: Jun 13, 2012
 *      Author: grutzek
 */

#include "AudioQ.h"

void AudioQ::push(audioQueueData *data) {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    queueMT.push(data);
}

bool AudioQ::empty() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    return queueMT.empty();
}

audioQueueData* AudioQ::front() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    return queueMT.front();
}

void AudioQ::pop() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    queueMT.pop();
}

long AudioQ::size() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    return queueMT.size();
}

