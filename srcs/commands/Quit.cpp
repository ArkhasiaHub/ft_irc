// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 10/07/2024 21:50:00
// Last updated: 11/07/2024 10:40:00

#include "../../includes/Server.hpp"

void Server::quit(const std::string& args, Client* client) {
  std::string message = args;
  parseArgs(&message);
  nr::rpl_ErrorNotify(client, message);
  strchannelMap::iterator chanMap = _channels.begin();

  while (chanMap != _channels.end()) {
    Channel* channel = chanMap->second;
    if (channel->getClient(client->getNickname())) {
      channel->eraseClient(client->getNickname());
      if (channel->isClientOperator(client->getNickname())) {
        channel->eraseClientOperator(client->getNickname());
      }
      if (channel->getNumberUser() > 0) {
        if (channel->getNumberClientOperator() == 0) {
          channel->addClientOperator(channel->getClients().begin()->second->getNickname());
          std::map<std::string, Client *> strclientMap = channel->getClients();
          Client* newOperator = channel->getClients().begin()->second;
          strclientMap::iterator it;
          for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
            nr::rpl_OperatorNotify(it->second, channel, newOperator->getNickname(), true, *client);
          }
        }
        std::map<std::string, Client *> strclientMap = channel->getClients();
        strclientMap::iterator it;
        for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
          nr::rpl_PartNotify(it->second, channel, *client, message);
          nr::rpl_Namrply(it->second, channel);
          nr::rpl_EndOfName(it->second, *channel);
        }
        chanMap++;
      } else {
        strchannelMap::iterator tmp = chanMap;
        tmp++;
        delete chanMap->second;
        _channels.erase(chanMap->first);
        if (tmp == _channels.end()) {
          break;
        } else {
          chanMap = tmp;
        }
      }
    } else {
      chanMap++;
    }
  }
  std::vector<pollfd>::iterator it;
  for (it = _pollfdList.begin(); it != _pollfdList.end(); it++) {
    if (it->fd == client->getFd()) {
      close(it->fd);
      _pollfdList.erase(it);
      break;
    }
  }
  std::vector<Client *>::iterator clientVec;
  for (clientVec = _clientList.begin(); clientVec != _clientList.end(); clientVec++) {
    if ((*clientVec)->getNickname() == client->getNickname()) {
      delete *clientVec;
      _clientList.erase(clientVec);
      break;
    }
  }
}
