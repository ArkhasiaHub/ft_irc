// Copyright 2024 <earaujo@42lyon.fr>
// Code by: Evan (earaujo) Araujo
// Created: 12/07/2024 10:06:00
// Last updated: 12/07/2024 12:52:00

#include  "../includes_bonus/Bot.hpp"

bool Bot::_signals = false;

Bot::Bot(const int port, const std::string& password) :
_port(port), _password(password), _connected(false) {}

int Bot::getPort() const {
  return _port;
}

void Bot::appendBuffer(std::string buffer) {
  _buffer.append(buffer);
}

const std::string& Bot::getBuffer() const {
  return (_buffer);
}

void Bot::clearBuffer() {
  _buffer.clear();
}

void Bot::signalHandler(int signal) {
  if (signal == SIGINT || signal == SIGTERM) {
    _signals = true;
  }
}

void  Bot::bugshot(std::vector<std::string> args) {
  std::srand(std::time(NULL));

  int  randNumber = std::rand() % 8;

  if (randNumber == 0) {
    std::string  command = "KICK " + args[2] + " " + args[0].substr(1) + " :You play against me, you die. Try again in an another life you dumb !\r\n";
    sendCommand(_bot_socket, command);
  } else {
    std::string command = "PRIVMSG " + args[2] +
      " :You are lucky. I don't succeed to kill you. Would you try again ?\r\n";
    sendCommand(_bot_socket, command);
  }
}

int  Bot::handleCommand(std::vector<std::string> commandTab) {
  std::vector<std::string>::iterator begin;
  for (begin = commandTab.begin(); begin != commandTab.end(); begin++) {
    std::string command;
    std::vector<std::string> args;
    size_t pos = 0;
    size_t start = 0;
    std::stringstream ss;

    while (pos != begin->npos) {
      pos = begin->find(" ", start);
      args.push_back(begin->substr(start, pos - start));
      start = pos + 1;
    }
    if (args.size() >= 2 && args[0][0] == ':' && args[1] == "001") {
      std::cout << _newNickname << std::endl;
    }
    else if (args.size() >= 4 && args[0][0] == ':' &&
        args[1] == "INVITE" && args[3][0] == '#') {
      command = "JOIN " + args[3] + "\r\n";
      sendCommand(_bot_socket, command);
      command = "PRIVMSG " + args[3] + " :Hey I am BugShot. " +
          "Do you want play with me ? For that I need to be an operator " +
          "then if you use !bugshot inside your message I will launch the " +
          "roulette. 1 chance on 8 for you to be kicked. Good luck.\r\n";
      sendCommand(_bot_socket, command);
    }  else if (args.size() >= 3 && args[0][0] == ':' &&
         args[1] == "PRIVMSG" && args[2][0] == '#') {
    if (args[3][0] == ':')
      args[3] = args[3].substr(1);
    for (size_t i = 3; i < args.size(); i++)  {
      if (args[i] == "!bugshot")
        bugshot(args);
      }
    }  else if (args.size() >= 2 && args[0] == ":localhost" && args[1] == "482") {
      command = "PRIVMSG " + args[3] + " :You are lucky. I am handcuffed and" +
          " can't kill you now. Try to put me operator and play again.\r\n";
      sendCommand(_bot_socket, command);
    }  else if (args.size() >= 2 && args[0] == ":localhost" && args[1] == "433") {
      ss << _addNickname;
      _newNickname = _nickname + ss.str();
      command = "NICK " + _newNickname + "\r\n";
      sendCommand(_bot_socket, command);
      command = "USER " + _newNickname + " 0 * :" + _newNickname + "\r\n";
      sendCommand(_bot_socket, command);
      if (_addNickname != 99999) {
        _addNickname++;
      }
      else
        return (-1);
    }
  }
  return (0);
}

int  Bot::handleData(const std::string &buff) {
  size_t pos = 0;
  size_t start = 0;
  std::vector<std::string> commandTab;

  if (!buff.empty()) {
    appendBuffer(buff);
  }
  pos = getBuffer().find("\r\n", pos);
  while (pos != getBuffer().npos) {
    commandTab.push_back(getBuffer().substr(start, pos - start));
    pos += 2;
    start = pos;
    pos = getBuffer().find("\r\n", pos);
  }
  if (commandTab.size() > 0) {
    if (handleCommand(commandTab) == -1) {
      clearBuffer();
      return (-1);
    }
    clearBuffer();
  }
  return (0);
}

int  Bot::receiveData() {
  std::string buff(2048, '\0');

  ssize_t bytes = recv(_pollfdBot.fd, &buff[0], sizeof(buff) - 1, 0);

  if (bytes == -1) {
    std::cerr << "Error: recv() failed" << std::endl;
  } else {
    buff.resize(bytes);
    if (handleData(buff) == -1)
      return (-1);
  }
  return (0);
}

void  Bot::sendCommand(int _bot_socket, std::string command) {
  send(_bot_socket, command.c_str(), command.size(), 0);
}

void  Bot::communication() {
  while (!_signals) {
    if (poll(&_pollfdBot, 1, -1) > 0) {
      if (_connected == false && _pollfdBot.revents & POLLOUT) {
        _nickname = "BugShot";
        _newNickname = "BugShot";
        _addNickname = 0;
        std::string  pass = "PASS " + _password + "\r\n";
        std::string  nick = "NICK BugShot\r\n";
        std::string  user = "USER BugShot 0 * :BugShot\r\n";
        sendCommand(_bot_socket, pass);
        sendCommand(_bot_socket, nick);
        sendCommand(_bot_socket, user);
        _connected = true;
      }  else if (_pollfdBot.revents & POLLIN) {
         if (receiveData() == -1)
          break;
      }
    }
  }
  sendCommand(_bot_socket, "QUIT :Leaving\r\n");
}

void Bot::createBot() {
  _bot_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_bot_socket == -1) {
    throw std::runtime_error("Error: socket creation failed");
  }
  int opt = 1;
  if (setsockopt(_bot_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    close(_bot_socket);
    throw std::runtime_error("Error: setsockopt failed");
  }
  _bot_addr.sin_family = AF_INET;
  _bot_addr.sin_port = htons(_port);
  _bot_addr.sin_addr.s_addr = INADDR_ANY;

  if (connect(_bot_socket, (struct sockaddr *)&_bot_addr, sizeof(_bot_addr)) < 0) {
    close(_bot_socket);
    throw std::runtime_error("Error: connection failed");
  }
  _pollfdBot.fd = _bot_socket;
  _pollfdBot.events = POLLIN | POLLOUT;
  _pollfdBot.revents = 0;
  communication();
  close(_bot_socket);
}
