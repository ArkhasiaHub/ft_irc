// Copyright 2024 <sbriche@42lyon.fr>
// Code by: Samuel (sbriche) Briche
// Created: 04/07/2024 15:09:00
// Last updated: 07/07/2024 17:04:00

#ifndef INCLUDES_CLIENT_HPP_
#define INCLUDES_CLIENT_HPP_

# include <vector>
# include <iostream>
# include <string>
# include "Server.hpp"

class Client {
 private:
  std::string _buffer;
  std::string _ipAddress;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  std::string _password;
  struct pollfd _pollfd;
  bool _isLog;
  bool _isPassWordSet;

 public:
  Client();
  ~Client();

  int getFd() const;
  pollfd getPollFd() const;
  const std::string& getBuffer() const;
  std::string getNickname() const;
  std::string getUsername() const;
  std::string getRealName() const;
  std::string getPassword() const;
  bool getIsLog() const;
  bool getIsPasswordSet() const;

  void setPollFd(struct pollfd newPoll);
  void appendBuffer(const std::string& buffer);
  void setIpAddress(const std::string& newVal);
  void setNickname(const std::string& newVal);
  void setUsername(const std::string& newVal);
  void setRealName(const std::string& newVal);
  void setPassword(const std::string& newVal);
  void setIsLog(bool isLog);
  void setIsPasswordSet(bool isPasswordSet);
  void clearBuffer();
};

#endif  // INCLUDES_CLIENT_HPP_
