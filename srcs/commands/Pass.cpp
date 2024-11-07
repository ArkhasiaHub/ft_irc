// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 17/07/2024 23:17:00
// Last updated: 17/07/2024 23:17:00

#include "../../includes/Server.hpp"

void Server::pass(const std::string& testPassword, Client* client) {
  if (clientIsLogged(client) || client->getIsPasswordSet() == true) {
    nr::rpl_AlreadyRegister(client);
    return;
  }

  std::string password = testPassword;
  parseArgs(&password);
  client->setPassword(password);
  if (password == _password) {
    client->setIsPasswordSet(true);
    clientIsLogged(client);
  } else {
    nr::rpl_PassWdMismatch(client);
  }
}
