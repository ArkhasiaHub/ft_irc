// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 17/07/2024 23:20:00
// Last updated: 17/07/2024 23:20:00

#include "../../includes/Server.hpp"

void  Server::nick(const std::string& nickName, Client* client) {
  if (client->getIsPasswordSet() == false) {
    nr::rpl_NotRegistered(client);
    return ;
  }
  if (nickName.empty()) {
    nr::rpl_NoNicknameGiven(client);
    return;
  }

  std::string nick = nickName;
  parseArgs(&nick);

  if (nick.find_first_of("#: &") != std::string::npos) {
    nr::rpl_ErroneusNickname(client, nick);
    return;
  }
  for (size_t i = 0; i < _clientList.size(); i++) {
    if (_clientList[i]->getNickname() == nick) {
      nr::rpl_NicknameInUse(client, nick);
      return;
    }
  }
  std::string oldNickname = client->getNickname();
  client->setNickname(nick);
  if (clientIsLogged(client)) {
    for (size_t i = 0; i < _clientList.size(); i++) {
      nr::rpl_NicknameChangedNotify(client, oldNickname, _clientList[i]);
    }
    strchannelMap::iterator chanIt;
    for (chanIt = _channels.begin(); chanIt != _channels.end(); chanIt++) {
      Channel* channel = chanIt->second;
      if (channel->getClient(oldNickname)) {
        channel->eraseClient(oldNickname);
        channel->addClient(client->getNickname(), client);
        if (channel->isClientOperator(oldNickname)) {
          channel->eraseClientOperator(oldNickname);
          channel->addClientOperator(client->getNickname());
        }
        strclientMap strclientMap  = channel->getClients();
        strclientMap::iterator clientIt;
        for (clientIt = strclientMap.begin(); clientIt != strclientMap.end(); clientIt++) {
          nr::rpl_OperatorNotify(clientIt->second, channel, client->getNickname(), true, *client);
          nr::rpl_Namrply(clientIt->second, channel);
          nr::rpl_EndOfName(clientIt->second, *channel);
        }
      }
    }
  }
}
