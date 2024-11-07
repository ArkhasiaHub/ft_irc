// Copyright 2024 <sbriche@42lyon.fr>
// Code by: Samuel (sbriche) Briche
// Created: 04/07/2024 15:09:00
// Last updated: 07/07/2024 16:30:00

#include "../includes/Channel.hpp"

Channel::Channel() : _flags(0), _limit_user(0) {}

Channel::Channel(const std::string& name) :
_name(name), _flags(0), _limit_user(0) {}

Channel::Channel(const Channel& other) :
_name(other._name), _topic(other._topic), _user(other._user),
_operators(other._operators), _flags(other._flags),
_limit_user(other._limit_user) {}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel& other) {
  if (this != &other) {
    _name = other._name;
    _topic = other._topic;
    _user = other._user;
    _operators = other._operators;
  }
  return *this;
}

std::string Channel::getTopic() const {
  return _topic;
}

std::string Channel::getName() const {
  return _name;
}

Client* Channel::getClient(const std::string& user) const {
  strclientMap::const_iterator it = _user.find(user);
  if (it != _user.end()) {
    return it->second;
  }
  return NULL;
}

size_t Channel::getNumberClientOperator() const {
  return _operators.size();
}

int Channel::getModeFlag(const int flag) const {
  return _flags & flag;
}

size_t Channel::getLimitUser() const {
  return _limit_user;
}

size_t Channel::getNumberUser() const {
  return _user.size();
}

std::string Channel::getPassword() const {
  return _password;
}

std::string Channel::getAllUser() {
  std::ostringstream str;
  for (strclientMap::iterator it = _user.begin(); it != _user.end(); it++) {
    if (this->isClientOperator(it->first)) {
      str << " @" << it->first;
    } else {
      str << " " << it->first;
    }
  }
  return str.str();
}

strclientMap Channel::getUserMap() {
  return _user;
}

const strclientMap& Channel::getClients() const {
  return _user;
}

bool Channel::isClientOperator(const std::string& user) const {
  std::vector<std::string>::const_iterator it;
  for (it = _operators.begin(); it != _operators.end(); it++) {
    if (user == *it) {
      return true;
    }
  }
  return false;
}

bool Channel::isUserInvited(const std::string& user) const {
  std::vector<std::string>::const_iterator it;
  for (it = _invitedUsers.begin(); it != _invitedUsers.end(); it++) {
    if (*it == user) {
      return true;
    }
  }
  return false;
}

void Channel::setTopic(const std::string& topic) {
  _topic = topic;
}

void Channel::addClient(const std::string& user, Client* client) {
  if (_user.find(user) == _user.end()) {
    _user[user] = client;
  }
}

void Channel::eraseClient(const std::string& user) {
  if (_user.find(user) != _user.end()) {
    _user.erase(user);
  }
}

void Channel::addClientOperator(const std::string& user) {
  if (_user.find(user) != _user.end() && !isClientOperator(user)) {
    _operators.push_back(user);
  }
}

void Channel::eraseClientOperator(const std::string& user) {
  std::vector<std::string>::iterator it;
  for (it = _operators.begin(); it != _operators.end(); it++) {
    if (user == *it) {
      it = _operators.erase(it);
      break;
    }
  }
}

void Channel::addModeFlag(const int flag) {
  if (!getModeFlag(flag)) {
    _flags |= flag;
  }
}

void Channel::removeModeFlag(const int flag) {
  _flags &= ~flag;
}

void Channel::setLimitUser(const size_t newVal) {
  _limit_user = newVal;
}

void Channel::setPassword(const std::string& newPassword) {
  _password = newPassword;
}

void Channel::inviteUser(const std::string& user) {
  if (!isUserInvited(user)) {
    _invitedUsers.push_back(user);
  }
}

void Channel::eraseInvitedUser(const std::string& user) {
  std::vector<std::string>::iterator userIt = std::find(_invitedUsers.begin(), _invitedUsers.end(), user);
  if (userIt != _invitedUsers.end()) {
    _invitedUsers.erase(userIt);
  }
}
