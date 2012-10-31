/**
 * @file		HashQ.h
 * @date		Jun 13, 2012
 * @author  	Gary Grutzek
 * @brief		Queue to next thread
 * @details		Queue to push data to hash calculation thread
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
