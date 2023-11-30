#include "Client.hpp"
#include <sys/_types/_ssize_t.h>

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


void Client::getREQ(std::string buffer)
{
	_httpBuffer = buffer;
	if (_httpBuffer.find("\r\n\r\n") == std::string::npos)
		return;

	// Header buffer is ready, proceed to parse
	request.setRequestString(buffer);
	request.parseRequestLine(buffer.substr(0, buffer.find("\r\n")));
	request.parseRequestHeaders();
	if (!request.getStatusCode())
		_requestParsed = true;
// Send error response	
	if (request.getRequestMethod() != "POST")
	{
		_requestIsReady = true;
		return; // success exit
	}
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

void Client::getBody(std::string buffer)
{
	if (_bytesExpected > server.getMaxBodySize())
		throw std::runtime_error("413");
	
	if (_recvChunk)
	{
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
			
			_body.append(_chunkedBuffer.substr(pos + 2, size));
			_chunkedBuffer.erase(0, pos + 2 + size + 2);
			if (_body.size() >= (unsigned long)server.getMaxBodySize())
				throw std::runtime_error("413");
		}
	}
	else 
	{
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

void	Client::createUploadFile(std::string filename, std::string content)
{
	std::ofstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cout << "Error opening file" << std::endl;
		// _errorCode = 500;
		return;
	}
	file << content;
	file.close();
	// _errorCode = 201;
}



void Client::handleRequestMethod(){

	Request r = this->request;
	if (r._headers["Method"] == POST)
	{
		if (r._headers["content-type"].find("multipart/form-data") != std::string::npos)
		{
			for (std::vector<BoundRequest>::iterator it = r._Boundaries.begin(); it != r._Boundaries.end(); ++it)
			{
				if (it->_headers["content-disposition"].find("filename=") != std::string::npos)
				{
					std::string filename = "/tmp/" + it->_headers["content-disposition"].substr(it->_headers["content-disposition"].find("filename=") + 10, it->_headers["content-disposition"].rfind('\"', std::string::npos) - (it->_headers["content-disposition"].find("filename=") + 10));
					std::string content = it->_body;
					createUploadFile(filename, content);
				}
				else
					createUploadFile("RandomFile", it->_body);
			}
		}
	}
	else if (r._headers["Method"] == GET)
	{
		// std::cout << "GET" << std::endl;
	}
	else if (r._headers["Method"] == DELETE)
	{
		
	}
	else
		response.setStatusCode(501);
}

bool Client::isMethodAllowed()
{
	return true;
}



