// Copyright 2024 <earaujo@42lyon.fr>
// Code by: Evan (earaujo) Araujo
// Created: 27/06/2024 10:06:00
// Last updated: 27/06/2024 11:17:00

#include <complex>
#include <algorithm>
#include "../includes/Server.hpp"

bool Server::_signals = false;

Server::Server(const long port, const std::string& password) :
_port(port), _password(password) {}

long Server::getPort() const {
  return _port;
}

std::string Server::getPassword() const {
  return _password;
}

void Server::signalHandler(int signal) {
  if (signal == SIGINT || signal == SIGTERM) {
    _signals = true;
  }
}

void    Server::createServ() {
  int option = 1;
  _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (_serverSocket == -1) {
    throw std::runtime_error("Fail of socket function");
  }
  if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1) {
    close(_serverSocket);
    throw std::runtime_error("Fail of setsockopt function");
  }
  if (bind(_serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
    close(_serverSocket);
    throw std::runtime_error("Fail of bind function");
  }
  if (listen(_serverSocket, 10) == -1) {
    close(_serverSocket);
    throw std::runtime_error("Fail of listen function");
  }
  _pollfd.fd = _serverSocket;
  _pollfd.events = POLLIN;
  _pollfdList.push_back(_pollfd);
}

void  Server::acceptClient() {
  struct sockaddr_in sockAddr = {};
  struct pollfd newPollFd = {};
  socklen_t len = sizeof(sockAddr);

  int fd = accept(_serverSocket, reinterpret_cast<sockaddr *>(&(sockAddr)), &len);
  if (fd == -1) {
    std::cout << "accept() failed" << std::endl;
    return;
  }
  if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
    std::cout << "fcntl() failed" << std::endl;
  }

  newPollFd.fd = fd;
  newPollFd.events = POLLIN;
  newPollFd.revents = 0;
  try {
    Client* cli = new Client();
    cli->setIpAddress(inet_ntoa(sockAddr.sin_addr));
    cli->setPollFd(newPollFd);
    _clientList.push_back(cli);
    _pollfdList.push_back(newPollFd);
    if (_password.empty())
    {
      cli->setIsPasswordSet(true);
      cli->setPassword("");
    }
  }
  catch (std::exception& e) {
    for (size_t i = 0; i < _clientList.size(); i++) {
      nr::rpl_ErrorNotify(_clientList[i], "Fatal Error server closing.");
    }
    _signals = true;
    return;
  }
}

void  Server::receiveData(class Client* client) {
  std::string buffer(2048, '\0');

  ssize_t bytes = recv(client->getPollFd().fd, &buffer[0], buffer.size() - 1, 0);
  if (bytes > 0) {
    buffer.resize(bytes);
    std::cout << buffer << std::endl;
    HandleData(buffer, client);
  } else {
    quit("Client Timeout", client);
  }
}

void Server::parseArgs(std::string* args) {
  if (args->empty())
    return;

  if ((*args)[0] == ':') {
    *args = args->substr(1);
  } else {
    size_t pos = args->find_first_of(' ');
    if (pos != std::string::npos) {
      *args = args->substr(0, pos);
    }
  }
}

void Server::do_command(std::vector<std::string> commandTab, class Client* client) {
  std::vector<std::string>::iterator begin;
  for (begin = commandTab.begin(); begin != commandTab.end(); begin++) {
    std::string command;
    std::string args;
    size_t pos = 0;
    size_t start = 0;

    pos = begin->find(" ", pos);
    if (pos != std::string::npos) {
      command = begin->substr(start, pos);
      pos += 1;
      args = begin->substr(pos, begin->size() - pos);
    } else {
      command = *begin;
    }



    std::map<std::string, Handler>::const_iterator it;
    it = func.find((command));
    if (it != func.end()) {
      std::vector<Client *>::iterator clientIt;
      for (clientIt = _clientList.begin(); clientIt != _clientList.end(); clientIt++) {
        if ((*clientIt)->getFd() == client->getPollFd().fd) {
          break;
        }
      }
      (this->*(it->second))(args, *clientIt);
      if (it->first != "QUIT") {
        client->clearBuffer();
      }
    }
  }
}

void  Server::HandleData(const std::string& buff, class Client* client) {
  size_t pos = 0;
  size_t start = 0;
  std::vector<std::string> commandTab;

  if (!buff.empty()) {
    client->appendBuffer(buff);
  }
  pos = client->getBuffer().find("\r\n", pos);
  
  while (pos != std::string::npos) {
    commandTab.push_back(client->getBuffer().substr(start, pos - start));
    pos += 2;
    start = pos;
    pos = client->getBuffer().find("\r\n", pos);
  }

  if (!commandTab.empty()) {
    do_command(commandTab, client);
  }
}

void Server::irc() {
  while (!_signals) {
    if (poll(&_pollfdList[0], _pollfdList.size(), -1) > 0) {
      for (size_t i = 0; i < _pollfdList.size(); i++) {
        if (_pollfdList[i].revents & POLLIN) {
          if (_pollfdList[i].fd == _serverSocket) {
            acceptClient();
          } else {
            for (size_t j = 0; j < _clientList.size(); j++) {
              if (_clientList[j]->getFd() == _pollfdList[i].fd) {
                receiveData(_clientList[j]);
                break;
              }
            }
          }
        }
      }
    }
  }
  closeServer();
}


void Server::closeServer() {
  close(_serverSocket);
  for (size_t i = 0; i < _clientList.size(); ++i) {
    close(_clientList[i]->getFd());
    delete _clientList[i];
  }
  strchannelMap::iterator it;
  for (it = _channels.begin(); it != _channels.end(); it++) {
    delete it->second;
  }
  _clientList.clear();
  _pollfdList.clear();
}

bool Server::isChannelStored(const std::string& topic) const {
  if (_channels.find(topic) != _channels.end()) {
    return true;
  }
  return false;
}

bool Server::isClientStored(const std::string& nickName) {
  std::vector<Client*>::iterator it;
  for (it = _clientList.begin(); it < _clientList.end(); it++) {
    if ((*it)->getNickname() == nickName) {
      return true;
    }
  }
  return false;
}

Client* Server::getClient(const std::string& nickName) {
  std::vector<Client*>::iterator it;
  for (it = _clientList.begin(); it < _clientList.end(); it++) {
    if ((*it)->getNickname() == nickName) {
      return *it;
    }
  }
  return NULL;
}

Channel* Server::getChannel(const std::string& channel) const {
  strchannelMap::const_iterator it = _channels.find(channel);
  if (it != _channels.end()) {
    return it->second;
  }
  return NULL;
}

void  Server::createMap() {
  func["JOIN"] = &Server::join;
  func["WHO"] = &Server::who;
  func["PRIVMSG"] = &Server::privmsg;
  func["MODE"] = &Server::mode;
  func["KICK"] = &Server::kick;
  func["INVITE"] = &Server::invite;
  func["TOPIC"] = &Server::topic;
  func["PASS"] = &Server::pass;
  func["NICK"] = &Server::nick;
  func["USER"] = &Server::user;
  func["PART"] = &Server::part;
  func["QUIT"] = &Server::quit;
  func["PING"] = &Server::ping;
}

bool  Server::clientIsLogged(Client* client) {
  if (!client->getIsLog()) {
    if (client->getNickname().empty() || client->getNickname() == "*") {
      return false;
    }
    if (client->getUsername().empty() || client->getRealName().empty()) {
      return false;
    }
    if (client->getPassword() != getPassword()) {
      return false;
    }
    client->setIsLog(true);
    nr::rpl_Welcome(client);
    nr::rpl_YourHost(client);
    nr::rpl_Created(client);
    nr::rpl_MyInfo(client);
    motd("", client);
  }
  return true;
}
