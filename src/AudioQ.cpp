/**
 * @file		AudioQ.cpp
 * @date		Jun 13, 2012
 * @author  	Gary Grutzek
 *
 * @copyright  	Copyright (c) 2012 Gary Grutzek<br>
 * 				Cologne University of Applied Sciences<br>
 * 				<br>
 * 				This program is free software: you can redistribute it and/or modify
 *				it under the terms of the GNU General Public License as published by
 *				the Free Software Foundation, either version 3 of the License, or
 * 				(at your option) any later version.<br>
 *				This program is distributed in the hope that it will be useful,
 *				but WITHOUT ANY WARRANTY; without even the implied warranty of
 *				MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *				GNU General Public License for more details.<br>
 *				You should have received a copy of the GNU General Public License
 *				along with this program. If not, see <http://www.gnu.org/licenses/>.
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

