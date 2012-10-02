/*
 * AudioQ.cpp
 *
 *  Created on: Jun 13, 2012
 *      Author: grutzek
 */

#include "HashQ.h"

void HashQ::push(hashQueueData *data) {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    queueMT.push(data);
}

bool HashQ::empty() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    return queueMT.empty();
}

hashQueueData* HashQ::front() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    return queueMT.front();
}

void HashQ::pop() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    queueMT.pop();
}

long HashQ::size() {
    // ============= enter critical section ================
    boost::lock_guard<boost::mutex> queueMTLock(queueMTMutex);
    // -----------------------------------------------------
    return queueMT.size();
}

