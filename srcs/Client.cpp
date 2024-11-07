// Copyright 2024 <sbriche@42lyon.fr>
// Code by: Samuel (sbriche) Briche
// Created: 04/07/2024 15:09:00
// Last updated: 07/07/2024 16:42:00

#include "../includes/Client.hpp"

Client::Client() :_nickname("*"), _isLog(false), _isPassWordSet(false) {}

Client::~Client() {}

std::string Client::getNickname() const {
  return _nickname;
}

std::string Client::getUsername() const {
  return _username;
}

std::string Client::getRealName() const {
  return _realname;
}

std::string Client::getPassword() const {
  return _password;
}

pollfd Client::getPollFd() const {
  return (_pollfd);
}

const std::string& Client::getBuffer() const {
  return (_buffer);
}

int Client::getFd() const {
  return _pollfd.fd;
}

bool Client::getIsLog() const {
  return _isLog;
}

bool Client::getIsPasswordSet() const {
  return _isPassWordSet;
}


void Client::setIpAddress(const std::string& newVal) {
  _ipAddress = newVal;
}

void Client::setNickname(const std::string& newVal) {
  _nickname = newVal;
}
void Client::setUsername(const std::string& newVal) {
  _username = newVal;
}
void Client::setPassword(const std::string& newVal) {
  _password = newVal;
}

void Client::setRealName(const std::string& newVal) {
  _realname = newVal;
}

void Client::setPollFd(struct pollfd newPoll) {
  _pollfd = newPoll;
}

void Client::appendBuffer(const std::string& buffer) {
  _buffer.append(buffer);
}

void Client::clearBuffer() {
  _buffer.clear();
}

void Client::setIsLog(bool isLog) {
  _isLog = isLog;
}

void Client::setIsPasswordSet(bool isPasswordSet) {
  _isPassWordSet = isPasswordSet;
}
