// Copyright 2024 <earaujo@42lyon.fr>
// Code by: Evan (earaujo) Araujo
// Created: 12/07/2024 10:06:00
// Last updated: 12/07/2024 12:52:00

#ifndef INCLUDES_BONUS_BOT_HPP_
#define INCLUDES_BONUS_BOT_HPP_

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <csignal>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

class Bot {
 private:
    int _bot_socket;
    struct sockaddr_in _bot_addr;
    static bool  _signals;
    int  _port;
    std::string  _password;
    bool  _connected;
    pollfd  _pollfdBot;
    std::string  _nickname;
    std::string  _newNickname;
    int  _addNickname;
    std::vector<std::string>  commandTab;
    std::string  _buffer;

 public:
    struct sockaddr_in  serverAddr;
    Bot(const int port, const std::string& password);
    void  createBot();
    static void  signalHandler(int signal);
    int  getPort() const;
    void  communication();
    void  sendCommand(int bot_socket, std::string command);
    int  receiveData();
    int  handleData(const std::string &buff);
    void appendBuffer(std::string buffer);
    const std::string& getBuffer() const;
    void clearBuffer();
    int handleCommand(std::vector<std::string> commandTab);
    void bugshot(std::vector<std::string> args);
};

#endif  // INCLUDES_BONUS_BOT_HPP_
