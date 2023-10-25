#include "Client.hpp"

Client::Client(int fd, Listen_Addr Client, Listen_Addr Server) 
{
	static int clientID = 1;
	_id = clientID;
	clientID++;

	_fd = fd;
	_serverAddr = Server;
	_clientAddr = Client;
	_ready = false;
	_Connected = true;
	_requestIsReady = false;
	_requestParsed = false;
	_recvChunk = false;
	_httpBuffer.clear();
	_chunkedBuffer.clear();
	_body.clear();
	_bytesRecved = 0;
	_bytesExpected = 0;


	_lastTime = std::time(NULL);

}

int Client::getId() const
{ 
	return _id;
}

int Client::getFd() const 
{
	return _fd;
}

int Client::getServerPort() const 
{
	return _serverAddr.port;
}

in_addr_t Client::getServerIp() const 
{
	return _serverAddr.ip;
}

in_addr_t Client::getClientIp() const 
{
	return _clientAddr.ip;
}

bool Client::is_Connected() const 
{
	return _Connected;
}

bool Client::Timeout() const 
{
	return (std::time(NULL) - _lastTime) > TIMEOUT_T; 
}


void Client::set_Connect(bool status)
{
	_Connected = status;
}

void Client::setReady(bool status)
{
	_ready = status;
}

bool Client::isReady() const
{
	return _ready;
}


void Client::getREQ(std::string& buffer)
{
	_httpBuffer.append(buffer);
	if (_httpBuffer.find("\r\n\r\n") == std::string::npos)
		return;

	// Header buffer is ready, proceed to parse
	request.setRequestString(buffer);
	request.parseRequestLine(buffer.substr(0, buffer.find("\r\n")));
	request.parseRequestHeaders();
	if (!request.getErrorCode())
		_requestParsed = true;
// Send error response	
	if (request.getRequestMethod() != "POST")
	{
		_requestIsReady = true;
		return; // success exit
	}

	buffer = buffer.substr(buffer.find("\r\n\r\n") + 4);

	// Check Content-Length and set expecting bytes
	if (request.getContentLength() != -1)
	{
		_bytesExpected = request.getContentLength();
		_requestIsReady = false;
		return;
	}
	if (request._headers.find("Transfer-Encoding") != request._headers.end() && request._headers["Transfer-Encoding"] == "chunked")
	{
		_recvChunk = true;
		_requestIsReady = false;
		return;
	}
}

void Client::getBody(std::string& buffer)
{
	if (_recvChunk)
	{
		// Handle chunked encoding
		_chunkedBuffer.append(buffer);
		while (true)
		{
			size_t pos = _chunkedBuffer.find("\r\n");
			if (pos == std::string::npos)
				break;

			std::string sizeStr = _chunkedBuffer.substr(0, pos);
			int size = strtol(sizeStr.c_str(), NULL, 16);
			if (size == 0)
			{
				_requestIsReady = true;
				_chunkedBuffer.clear();
				break;
			}
			// 1894198616\r\n0\r\n\r\n
			// if (_chunkedBuffer.size() < pos + 2 + size + 2)
			// 	break;

			_body.append(_chunkedBuffer.substr(pos + 2, size));
			_chunkedBuffer.erase(0, pos + 2 + size + 2);
		}
	}
	else {
		// Handle multipart/form-data
		_body.append(buffer);
		_bytesRecved += buffer.length();
		if (_bytesRecved >= _bytesExpected)
			_requestIsReady = true;
	}
}



void Client::reset()
{
	_bytesExpected = 0;
	_bytesRecved = 0;
	_requestIsReady = true;
	_requestParsed = false;
	_httpBuffer.clear();
	_recvChunk = false;
	_chunkedBuffer.clear();
	_body.clear();

}

void Client::setServer(Server& server)
{
	_server = server;
	std::cout << "Client " << _id << " connected to server " << _server.getName() << std::endl;
	std::cout << "Client " << _id << " bound to " << toIPString(_clientAddr.ip) << ":" << _clientAddr.port << std::endl;
	std::cout << "Client " << _id << " connected to " << toIPString(_serverAddr.ip) << ":" << _serverAddr.port << std::endl;
}