#include "AudioQ.h"
#include "HashQ.h"
#include "parseConfig.h"	// parse config file
#include "VIATUtilities.h"	// some helper functions
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

#define TUPLE_SIZE 2

extern AudioQ *audioQ;

class TCPConnection: public boost::enable_shared_from_this<TCPConnection>
{
public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new TCPConnection(io_service));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
	//	int k=0;
		for (;;)
		{
			boost::system::error_code error;

			// read call identifier
			bufferedbytes_ = 0;
			do {
				bufferedbytes_ = socket_.read_some(boost::asio::buffer(buffer_, sizeof(unsigned long)), error);
			} while(bufferedbytes_ < 1 && !error);


			// copy callID to buffer, 8 Byte !
			// 		++++ which is copied to buffer like this: +++++
			//			buffer[0] = (callId >> 0) & 0xffff;
			//			buffer[1] = (callId >> 16) & 0xffff;
			//			buffer[2] = (callId >> 32) & 0xffff;
			//			buffer[3] = (callId >> 48) & 0xffff;
			//
			unsigned long tmp=0;
			tmp = (buffer_[3] & 0xffff);
			tmp = tmp << 16;
			tmp |= (buffer_[2] & 0xffff);
			tmp = tmp << 16;
			tmp |= (buffer_[1] & 0xffff);
			tmp = tmp << 16;
			tmp |= (buffer_[0] & 0xffff);
			callID_ = tmp;

			// read call data as 16 Bit LPCM audio samples
			bufferedbytes_ = 0;
			int i = 0;
			do {
				i++;
				bufferedbytes_ += socket_.read_some(boost::asio::buffer(&buffer_[bufferedbytes_ / sizeof(short)], maxLen_ - bufferedbytes_), error);
			} while(bufferedbytes_ < maxLen_ && !error);

			if (bufferedbytes_)
				std::cout << bufferedbytes_ << std::endl;

			//			if (error && error != boost::asio::error::eof)//
			//				throw boost::system::system_error(error); // Some other error than eof
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			if (bufferedbytes_){
//				std::cout << error << std::endl;
//				std::cout << k << std::endl;
//				k++;

				call_ = new audioQueueData;
				call_->data = (short*) malloc(maxLen_);
				if (!call_->data)
					std::cout << "malloc error" << std::endl;

				call_->id = callID_; // call id
				call_->numSamples = bufferedbytes_/sizeof(short); // length (eof or maxLen)
//				std::cout << "numSamples: " << call_->numSamples << std::endl;

				for(unsigned int i=0; i< call_->numSamples; i++)
					call_->data[i] = buffer_[i];
//				for (int i=0; i < 5; i++) {
//					std::cout << call_->data << " input socket:" << call_->data[i] << std::endl;
//				}
				audioQ->push(call_);
			}
		}
	}

private:
	TCPConnection(boost::asio::io_service& io_service) :
		buffer_(NULL),
		socket_(io_service),
		bufferedbytes_(0),
		datalength_(0),
		callID_(0),
		call_(NULL)
	{
		// input socket buffer
		//int maxLen = param->maxAudioLen*param->samplerate * sizeof(short); // MAX_LEN samples
		maxLen_ = 18000*sizeof(short);
		if (!(buffer_ = (short*) malloc(maxLen_))) { // alloc
			// fprintf(stderr, "SocketServer.cpp: <ERROR> buf:Memory allocation failed!\n");
			return;
		}
	}

	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{
	}

	//boost::array<unsigned long, 1000> buffer_;
	short *buffer_;

	///
	tcp::socket socket_;

	///
	size_t bufferedbytes_;

	///
	unsigned long datalength_;

	///
	unsigned long callID_;

	///
	unsigned int maxLen_;

	///
	audioQueueData *call_; // call data from input socket
};

class TCPServer
{
public:
	TCPServer(const std::string& addr, const std::string& port) :
			io_service_(), signals_(io_service_), acceptor_(io_service_, tcp::endpoint(boost::asio::ip::address::from_string(addr), atoi(port.c_str())))
	{
		// Register to handle the signals that indicate when the server should exit.
		// It is safe to register for the same signal multiple times in a program,
		// provided all registration for the specified signal is made through Asio.
		//	signals_.add(SIGTERM);
		//	signals_.async_wait(boost::bind(&TCPServer::handle_stop, this));
		std::cout << "Acceptor ready at '" + addr + ":" + port + "'!" << std::endl;

		start_accept();
	}

	void run()
	{
		// The io_service::run() call will block until all asynchronous operations
		// have finished. While the server is running, there is always at least one
		// asynchronous operation outstanding: the asynchronous accept call waiting
		// for new incoming connections.
		io_service_.run();
	}

private:
	void start_accept()
	{
		TCPConnection::pointer new_connection = TCPConnection::create(acceptor_.get_io_service());

		acceptor_.async_accept(new_connection->socket(), boost::bind(&TCPServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
	}

	void handle_accept(TCPConnection::pointer new_connection, const boost::system::error_code& error)
	{
		// Check whether the server was stopped by a signal before this completion
		// handler had a chance to run.
		if (!acceptor_.is_open())
		{
			return;
		}

		if (!error)
		{
			new_connection->start();
		}

		start_accept();
	}

	void handle_stop()
	{
		// The server is stopped by canceling all outstanding asynchronous
		// operations. Once all operations have finished the io_service::run() call
		// will exit.
		acceptor_.close();

		//L_i << "Acceptor closed successfully!";
	}

	/// The io_service used to perform asynchronous operations.
	boost::asio::io_service io_service_;

	/// The signal_set is used to register for process termination notifications.
	boost::asio::signal_set signals_;

	/// Acceptor used to listen for incoming connections.
	boost::asio::ip::tcp::acceptor acceptor_;

};
