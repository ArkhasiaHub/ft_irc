// Copyright 2024 <earaujo@42lyon.fr>
// Code by: Evan (earaujo) Araujo
// Created: 27/06/2024 10:06:00
// Last updated: 27/06/2024 11:17:00

#ifndef INCLUDES_SERVER_HPP_
#define INCLUDES_SERVER_HPP_

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <csignal>
#include <cstring>
#include <sstream>
#include <istream>
#include <map>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include "../includes/Client.hpp"
#include "../includes/NumericReply.hpp"
#include "../includes/Channel.hpp"

class Client;
class Channel;

typedef  std::map<std::string, Channel*> strchannelMap;

class Server {
 private:
  typedef void (Server::*Handler)(const std::string&, Client*);

  std::map<std::string, Handler> func;
  std::vector<Client *> _clientList;
  std::vector<pollfd> _pollfdList;
  const long _port;
  int _serverSocket;
  const std::string _password;
  static bool _signals;
  pollfd _pollfd;
  strchannelMap _channels;

 public:
  struct sockaddr_in  serverAddr;

  Server(long port, const std::string& password);

  long getPort() const;
  std::string getPassword() const;
  Client* getClient(const std::string& nickName);
  Channel* getChannel(const std::string& channel) const;

  void acceptClient();
  void receiveData(class Client* client);
  void HandleData(const std::string& buff, class Client* client);
  void createMap();
  bool clientIsLogged(Client* client);
  void closeServer();

  void join(const std::string& args, Client* client);
  void who(const std::string& args, Client* client);
  void kick(const std::string& args, Client* client);
  void topic(const std::string& args, Client* client);
  void invite(const std::string& args, Client* client);
  void privmsg(const std::string& args, Client* client);
  void pass(const std::string& testPassword, Client* client);
  void nick(const std::string& nickName, Client* client);
  void user(const std::string& userStr, Client* client);
  void mode(const std::string& args, Client* client);
  void part(const std::string& args, Client* client);
  void quit(const std::string& args, Client* client);
  void motd(const std::string& target, Client* client);
  void ping(const std::string& args, Client* client);
  void pong(const std::string& args, Client* client);

  void applyModeChanges(Client* client, Channel* channel, const std::string& modeChanges, const std::vector<std::string>& modeParams);
  void parseJoin(Client* client, const std::string& args, std::vector<std::string>* channelName, std::vector<std::string>* channelKey);
  void partChannel(Client* client, const std::string& channelName, const std::string& msg);
  bool KickCheckData(Channel** channel, Client* client, const std::string& channelTarget) const;
  void KickLogic(Client* client, Channel* channel, Client* target, const std::string& msg, const std::string& userTarget);
  void parseKick(const std::string& args, std::vector<std::string>* targets, std::string* channelTarget, std::string* msg);
  void parseArgs(std::string* args);

  void checkChannel(const std::string& channelName, const std::string& key, Client* client);
  void ClientQuitAllChannel(Client* client);
  bool isChannelStored(const std::string& topic) const;

  bool isClientStored(const std::string& nickName);

  void sendToClient(Client* client, const std::string& target, const std::string& msg);
  void sendToChannel(Client* client, const std::string& target, const std::string& msg) const;
  void addNewChanClient(Channel* channel, Client* client);

  void createServ();
  void irc();
  void do_command(std::vector<std::string> commandTab, class Client* client);

  static void signalHandler(int signal);
};

#endif  // INCLUDES_SERVER_HPP_
