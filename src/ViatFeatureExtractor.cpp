/**
 * @file		ViatFeatureExtraktor.cpp
 * @date		Oct 12, 2011
 * @author  	Gary Grutzek
 * @brief		Fingerprint Generator VIAT, main
 * @details		Mel STFT and MFCC, Generate hashvalues from STFT- and MFCC-derivatives and writes to CSV or pushes to TCP socket
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

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <string>

#ifndef __APPLE__
#include <malloc.h>
#include <stdlib.h>
#endif

#include "Wave.h"
#include "Audio.h"
#include "melFB.h"  		// Mel-Filterbank
#include "YapHash.h"
#include "AudioQ.h"
#include "HashQ.h"
#include "parseConfig.h"	// parse config file
#include "VIATUtilities.h"	// some helper functions
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "TCPServer.cpp"

// GLOBAL Q
AudioQ *audioQ;


#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
// TODO: debug level as parameter?
// Debug Level > 0 = timestamp
//             > 2 = write to .csv
#define DEBUG_LEVEL 1

#define HEADER_SIZE 2

using namespace std;  
using boost::asio::ip::tcp;

// calls boost::sleep 
void wait(int seconds)
{
	boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

// the tcp socket 
void tcpServerWorkerThread(Parameter *param)
{
	std::stringstream out;
	out << param->callxPort;
	TCPServer* tcpServerWorker = new TCPServer(param->callxIpAddress, out.str());
	tcpServerWorker->run();
	delete tcpServerWorker;
}

void audioHash(Parameter *param, HashQ *outputQ)
{
	// create mel Filterbank
	MelFb *melBank = new MelFb(param->windowSize, param->melCoeffs+1, param->loCut, param->hiCut);
	Audio *audio;
	YapHash *pHash;

	audioQueueData *call; 	// pointer to last audio buffer in InputQueue
	hashQueueData *hash;

	if (param->debugLevel > 2)
		cout << "Hash Thread." << endl;

	while(true){

		if (param->debugLevel > 2)
			cout << "Audio Q size: " << audioQ->size() << endl;
		if(!audioQ->empty()){

			// get last pointer to buffer
			call = audioQ->front();
			audioQ->pop();	// remove from queue
			cout  << call->id << " id " << endl;

			// new audio object from extracted call data
			audio = new Audio(call->data, call->numSamples, param);

			// calculate the hashvalues
			cout << "calc hash" << endl;
			pHash = new YapHash(*audio, *melBank, param);

			if (param->debugLevel > 2) {
				cout << pHash << " HashLen: " << pHash->length() << endl;
				for (int i=0; i < 5; i+=2) {
					cout << i << " Pos:" << pHash->index[i] << "  Feature:" << pHash->index[i+1] << endl;
				}
			}

			// send to client server thread
			hash = new hashQueueData;
			hash->id = call->id;

			cout << hash->id << " " << call->id << endl;

			hash->length = pHash->length(); // length = hash+position
			hash->data = pHash->index;

			outputQ->push(hash);

			free(call->data); // delete call from input thread
			delete call;
			delete audio;
			audio = NULL;

		}// end if
		// TODO: remove me
		wait(1);
		// 
	} // end while(true)
	delete melBank;
}

void socketClient(Parameter *param, HashQ *outputQ){

	hashQueueData *hash;

	while(true){
		// TODO: remove me
		wait(1);

		if (param->debugLevel > 2)
			cout << "Out Q size: " << outputQ->size() << endl;

		if(!outputQ->empty()){

			hash = outputQ->front(); // get last pointer to queue hash struct
			outputQ->pop();			// remove from queue

			int len = MIN(hash->length, param->maxHashLen); // limit to maximum allowed hashvalues

			if (param->debugLevel > 1)
				cout << ": " << hash->id << " " << hash->length << endl;

			if (param->writeToCsv) {
				std::stringstream ss;
				ss << hash->id << ".csv";
				std::cout << "csvname: "<< ss.str() << std::endl;
				const std::string tmp = ss.str();
				const char* cstr = tmp.c_str();
				writeIndexToCSV(cstr,hash->data, hash->length);
			}

			if (param->writeToSocket) {
				// socket client
				boost::asio::io_service ioService;

				// socket client to search
				boost::asio::ip::tcp::endpoint server( boost::asio::ip::address::from_string(param->indexdIpAddress)
				, param->indexdPort);
				boost::asio::ip::tcp::socket socket(ioService);
				socket.connect(server);

				if (param->debugLevel > 0) {
					cout << "Local endpoint:  " << socket.local_endpoint() << endl;
					cout << "Remote endpoint " << socket.remote_endpoint() << endl;
				}

				unsigned long *buf = NULL;
				if (!(buf = (unsigned long*) malloc(HEADER_SIZE * sizeof(long) + len * (sizeof(long) + sizeof(long))))) {
					fprintf(stderr, "main.cpp: <ERROR> buf:Memory allocation failed!\n");
					return;
				}
				buf[0] = hash->id;  // Call ID
				buf[1] = len; 		// length of payload / number of elements
				cout << buf[0] << " " << buf[1] << endl;

				// copy to output socket buffer
				for (int i = 0; i < len; i++) {
					buf[2 * i + HEADER_SIZE + 0] = hash->data[2 * i + 0]; // position
					buf[2 * i + HEADER_SIZE + 1] = hash->data[2 * i + 1]; // hash
				}

				// push data to index daemon
				pushToSocket(socket, buf, HEADER_SIZE * sizeof(long) + len * (sizeof(long) + sizeof(long)));

				if (socket.is_open())
					socket.close();
			}

			free(hash->data); 	// delete pHash data from previous thread
			delete hash;		// delete outputQ data struct
		}
	}
}

int main(int argc, const char * argv[]) {
	//  ippStaticInit();
	if (argc < 2) {
		//if(!strcmp(argv[1],"--help")){
		fprintf(
				stderr,
				"usage: %s <config file> (optional)>\n",
				argv[0]);
		return -1;
	}
#ifdef DEBUG
	Fw64u timeStamp1 = fwGetCpuClocks();
#endif

	try {
		Parameter param = { 0 };
		if(parseConfig(argv[1], &param)) {
			cout << "Config Error" << endl;
			return -1;
		}

		// setup threadsafe queues
		audioQ = new AudioQ();
		HashQ *hashQ = new HashQ();

		// start threads
		boost::thread tsThread(tcpServerWorkerThread, &param);
		boost::thread hashThread(audioHash, &param, hashQ);
		boost::thread socketClientThread(socketClient, &param, hashQ);
		//
		tsThread.join();
		hashThread.join();
		socketClientThread.join();

		cout << "you should never read this" << endl;

	} catch (const char* err) {
		cout << "Exception: " << err << endl;
		return -1;
	}
#ifdef DEBUG
	// get timestamp
	Fw64u timeStamp2 = fwGetCpuClocks();
	cout << (timeStamp2 - timeStamp1)/(2.4*1E9) << endl;
#endif

	return 0;
}


