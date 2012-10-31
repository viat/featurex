/**
 * @file		AudioQ.h
 * @date		Jun 13, 2012
 * @author  	Gary Grutzek
 * @brief		Input Queue
 * @details		Input Queue for input socket
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
