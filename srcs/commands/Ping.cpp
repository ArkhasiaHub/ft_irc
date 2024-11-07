// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 19/07/2024 09:29:00
// Last updated: 19/07/2024 09:29:00

#include "../../includes/Server.hpp"

void Server::ping(const std::string& args, Client* client) {
  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }
  if (args.empty()) {
    nr::rpl_NeedMoreParams(client, "PING");
    return;
  }
  pong(args,client);
}
