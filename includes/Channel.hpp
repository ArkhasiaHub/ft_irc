// Copyright 2024 <sbriche@42lyon.fr>
// Code by: Samuel (sbriche) Briche
// Created: 04/07/2024 15:09:00
// Last updated: 07/07/2024 16:42:00

#ifndef INCLUDES_CHANNEL_HPP_
#define INCLUDES_CHANNEL_HPP_

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "../includes/Client.hpp"

#define MODE_INVITE_ONLY (1 << 0)
#define MODE_PASSWORD (1 << 2)
#define MODE_TOPIC (1 << 3)
#define MODE_LIMIT (1 << 4)

class Client;

typedef  std::map<std::string, Client*> strclientMap;

class Channel {
 private:
  std::string _name;
  std::string _topic;
  strclientMap _user;
  std::vector<std::string> _operators;
  std::vector<std::string> _invitedUsers;
  int _flags;
  size_t _limit_user;
  std::string _password;

 public:
  Channel();
  explicit Channel(const std::string& name);
  Channel(const Channel& other);
  ~Channel();

  Channel& operator=(const Channel& other);

  std::string getTopic() const;
  std::string getName() const;
  Client* getClient(const std::string& user) const;
  bool isClientOperator(const std::string& user) const;
  size_t getNumberClientOperator() const;
  int getModeFlag(int flag) const;
  size_t getLimitUser() const;
  size_t getNumberUser() const;
  std::string getPassword() const;
  bool isUserInvited(const std::string& user) const;
  std::string getAllUser();
  strclientMap getUserMap();
  const strclientMap& getClients() const;

  void setTopic(const std::string& topic);
  void addClient(const std::string& user, Client* client);
  void eraseClient(const std::string& user);
  void addClientOperator(const std::string& user);
  void eraseClientOperator(const std::string& user);
  void addModeFlag(int flag);
  void removeModeFlag(int flag);
  void setLimitUser(size_t newVal);
  void setPassword(const std::string& newPassword);
  void inviteUser(const std::string& user);
  void eraseInvitedUser(const std::string& user);
};

#endif  // INCLUDES_CHANNEL_HPP_
