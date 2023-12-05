#include "Client.hpp"
#include <cstdio>
#include <ctime>
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <unistd.h>

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
	_isCGI = false;
	_lastTime = std::clock();
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
	if((size_t)(std::clock() - _lastTime) > TIMEOUT_T)
		throw std::runtime_error("408");
	return false; 
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
	if (request.getContentLength() >  server.getMaxBodySize())
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
			int size = std::strtol(sizeStr.c_str(), NULL, 16);
			if (size == 0)
			{
				_requestIsReady = true;
				_chunkedBuffer.clear();
				break;
			}
			
			_body.append(_chunkedBuffer.substr(pos + 2, size));
			_chunkedBuffer.erase(0, pos + 2 + size + 2);
			_bytesRecved += size;
			if ((ssize_t)_body.size() > server.getMaxBodySize())
				throw std::runtime_error("413");
		}
	}
	else 
	{
		_body.append(buffer);
		_bytesRecved += buffer.length();
		if ((ssize_t)_body.size() > server.getMaxBodySize())
			throw std::runtime_error("413");
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
	int resourceType = getResourceType(filename);
	if (resourceType == ISFILE)
		throw std::runtime_error("409");
	std::ofstream file(filename.c_str());
	if (!file.is_open()){
		puts("Error opening file");
		throw std::runtime_error("500");
	}
	response.uploadFilePath = filename;
	file << content;
	file.close();
	// _errorCode = 201;
}

void Client::generateResponse(Request& request, std::string path, int code)
{
	response.setStatusCode(code);
	if (path != EMPTY)
	{
		response.filePath = path;
		return ;
	}	
	response.initResponseHeaders(request);
	response.readyToSend = true;
}

void Client::generateRedirectionResponse(Request& request, std::string path, int code)
{
	response.setStatusCode(code);
	response.initRedirectHeaders(request, path);
	response.readyToSend = true;
}

std::string normalizePath(const std::string& path) {
    std::string result;
    char lastChar = '\0';

    for (std::string::const_iterator it = path.begin(); it != path.end(); ++it) {
        if (*it == '/' && lastChar == '/') {
            continue;
        }
        result += *it;
        lastChar = *it;
    }

    return result;
}

void Client::handleGetRequest()
{
	Request r = this->request;
	std::string fullPath = normalizePath(getPath());

	if (fullPath.find(server.getRoot()) == std::string::npos)
		throw std::runtime_error("403");
	int resourceType = getResourceType(fullPath);	
    if (resourceType == ISNOTEXIST)
		throw std::runtime_error("404");
	if (resourceType == ISFILE)
	{
		struct stat path_stat;
   		stat(fullPath.c_str(), &path_stat);
		response._contentLength = std::to_string(path_stat.st_size);
		return generateResponse(request, fullPath, 200);
	}
	else if (resourceType == ISDIR)
	{
		if (fullPath[fullPath.length() - 1] != '/')
			return generateRedirectionResponse(r, HTTP + request._headers["host"] + request.uri + "/", 301);
		std::string index = normalizePath(fullPath + "/" + _config_location.index);
		struct stat path_stat;
   		stat(index.c_str(), &path_stat);
		if (S_ISREG(path_stat.st_mode))
		{
			std::ifstream file(index.c_str());
			if (!file.is_open())
				throw std::runtime_error("500");
			response._contentLength = std::to_string(path_stat.st_size);
			file.close();		
			return generateResponse(request, index, 200);
		}
		if (_config_location.autoindex)
		{
			response.filePath = fullPath;
			response.root = server.getRoot();
			return response.generateAutoIndex(r);
		}
		else if (_config_location.autoindex == false)
			throw std::runtime_error("403");
		throw std::runtime_error("404");
	}
}


void Client::handlePostRequest(){
	
	Request r = this->request;
	if (server.uploadEnabled() && r._headers["content-type"].find("multipart/form-data") != std::string::npos)
	{
			std::string root;
			for (std::vector<BoundRequest>::iterator it = r._Boundaries.begin(); it != r._Boundaries.end(); ++it)
			{
				if (it->_headers["content-disposition"].find("filename=") != std::string::npos)
				{
					if (_config_location.uploadPath.empty()){
						puts("Error: upload path not set");
						throw std::runtime_error("500");
					}
					std::string filename = _config_location.uploadPath + "/" + it->_headers["content-disposition"].substr(it->_headers["content-disposition"].find("filename=") + 10, it->_headers["content-disposition"].rfind('\"', std::string::npos) - (it->_headers["content-disposition"].find("filename=") + 10));
					std::string content = it->_body;
					createUploadFile(filename, content);
				}
				else createUploadFile("RandomFile", it->_body);
			}
			return generateResponse(r, EMPTY, 204);
	}
	else
	{
		std::string fullPath = getPath();
		int resourceType = getResourceType(fullPath);
		if (resourceType == ISFILE)
			throw std::runtime_error("403"); // check if CGI after
		else if (resourceType == ISDIR)
		{
			if (fullPath[fullPath.length() - 1] != '/')
				return generateRedirectionResponse(r, HTTP + request._headers["host"] + request.uri + "/", 301);
			if (_config_location.index.size() <= 0)
				throw std::runtime_error("403");
			throw std::runtime_error("403"); // check if CGI after
		}
			throw std::runtime_error("404");
	}
	throw std::runtime_error("403");
}

void Client::handleDeleteRequest(){
	
	Request r = this->request;
	std::string path = server.getRoot() + request.getRequestURI();
	int resourceType = getResourceType(path);


	if (resourceType == ISFILE){
		if (::remove(path.c_str()) == 0)
			return generateResponse(r, EMPTY, 204);
		throw std::runtime_error("500");
			// return a response of 500
	}
	else if (resourceType == ISDIR)
	{
		if (path[path.length() - 1] != '/')
			throw std::runtime_error("409");
		else
		{
			if (::rmdir(path.c_str()) == 0)
				return generateResponse(r, EMPTY, 204);
			else
			{
				if(access(path.c_str(), W_OK) == 0)
				 	throw std::runtime_error("500");
				throw std::runtime_error("403");
			}
		}
	}
	else throw std::runtime_error("404");

}

void Client::handleRequestMethod(){

    Request r = this->request;
	if (_config_location.root.empty() == false && _config_location.root.substr(0, server.getRoot().length()) != server.getRoot())
		throw std::runtime_error("403");
    std::string path = server.getRoot() + request.getRequestURI();
    if (_config_location.uri[0] == '.' && getResourceType(path) == ISFILE)
        handleCGI();
    else if (r._headers["Method"] == GET)
        return handleGetRequest();
    else if (r._headers["Method"] == POST)
        return handlePostRequest();
    else if (r._headers["Method"] == DELETE)
        return handleDeleteRequest();
    else
        throw std::runtime_error("501");
}



std::string Client::getPath()
{
	std::string root = server.getRoot();
	request._headers["URI"].erase(0,_config_location.uri.length());

	if (_config_location.alias.empty() == false)
		return normalizePath(root + _config_location.alias + request.getRequestURI());
	else if (_config_location.root.empty() == false)
		root = _config_location.root;
	return normalizePath(root + request.getRequestURI());
}

 stringMap Client::fetchCGIEnv()
{
	stringMap env;

	env["CONTENT_TYPE"] = request._headers["Content-Type"];
	
	if (request._headers["Method"] == "POST")
	{
		_body = _body.substr(_body.find("\r\n\r\n") + 4);
		env["CONTENT_LENGTH"] = request._body.size();
	}
	else
		env["QUERY_STRING"] = request._headers["Queries"];
	env["HTTP_COOKIE"] = request._headers["cookie"];
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["PATH_INFO"] = request.getRequestURI();
	env["PATH_TRANSLATED"] =  server.getRoot() + request.getRequestURI();
	env["REMOTE_ADDR"] = toIPString(server._config.address.ip);
	env["REQUEST_METHOD"] = request._headers["Method"];
	env["SCRIPT_FILENAME"] = _config_location.cgiPath;
	env["SERVER_NAME"] = server.getName();
	env["SERVER_PORT"] = std::to_string(server._config.address.port);
	env["SERVER_SOFTWARE"] = "Webserv/1.0.0 (mechane-azari)";
	env["REDIRECT_STATUS"] = "200";
	return env;
}

void Client::handleCGI()
{
	Request r = this->request;
	std::string path = server.getRoot() + request.getRequestURI();
	
	int pipeIn[2];
	int pipeOut[2];
	pid_t pid;

	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
		throw std::runtime_error("500");

	if ((pid = fork()) == -1)
		throw std::runtime_error("500");
	
	fcntl(pipeOut[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	std::string result= "";
	if (pid == 0) {
		try {
				stringMap env = fetchCGIEnv();
				char **envp = new char*[env.size() + 1];
				int i = 0;
				for (stringMap::iterator it = env.begin(); it != env.end(); ++it)
				{
					std::string tmp = it->first + "=" + it->second;
					envp[i] = new char[tmp.size() + 1];
					std::strcpy(envp[i], tmp.c_str());
					i++;
				}
				envp[i] = NULL;
				char **argv = new char*[3];
				argv[0] = new char[_config_location.cgiPath.size() + 1];
				std::strcpy(argv[0], _config_location.cgiPath.c_str());
				argv[1] = new char[path.size() + 1];
				std::strcpy(argv[1], path.c_str());
				argv[2] = NULL;
	

			chdir(std::string(argv[1]).substr(0, std::string(argv[1]).find_last_of('/')).c_str());
			dup2(pipeIn[0], 0), dup2(pipeOut[1], 1);
			close(pipeIn[1]), close(pipeOut[0]);
	
			if (execve(argv[0], argv, envp) == -1)
				throw std::runtime_error("500");
		}
		catch (const std::exception& e)
		{
			std::exit(1);
		}
	}
	else {
		int status;

		_lastTime = std::clock();
		if (request._headers["Method"] == "POST")
		{
			_body = _body.substr(_body.find("\r\n\r\n") + 4);
			write(pipeIn[1], _body.c_str(), request._body.size());
		}
		close(pipeOut[1]), close(pipeIn[0]);
		close(pipeIn[1]);

		// Pipe stdout
		char buffer[1024];
		FILE* file = fdopen(pipeOut[0], "r");
		if (!file)
			throw std::runtime_error("500");
		while (!Timeout())
		{
   	 		if (!std::fgets(buffer, sizeof(buffer), file))
			{
				if (feof(file))
        			break;
				else
					continue;
			}
			result += buffer;
		}
		
		close(pipeOut[0]);
		waitpid(pid, &status, 0);
		if (!(WIFEXITED(status) && WEXITSTATUS(status) == 0)) 
			throw std::runtime_error("500");

	}

	response.response =  "HTTP/1.1 200 script output follows\r\n Server: Webserv/1.0.0 (mechane-azari)\r\n"  + result;
	response.readyToSend = true;
	_isCGI = true;
	
}

