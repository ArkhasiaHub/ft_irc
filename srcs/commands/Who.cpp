// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 17/07/2024 23:17:00
// Last updated: 17/07/2024 23:17:00

#include "../../includes/Server.hpp"

void Server::who(const std::string& args, Client* client) {
  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }
  if (!args.empty() && args[0] == '#') {
    strchannelMap::iterator it = _channels.find((args));
    if (it != _channels.end()) {
      strclientMap clientIt = it->second->getUserMap();
      strclientMap::iterator itMap;
      for (itMap = clientIt.begin(); itMap != clientIt.end(); itMap++) {
        nr::rpl_WhoReply(client, *itMap->second, *it->second);
      }
      nr::rpl_EndOfWhoReply(client, args);
    }
  }
}
