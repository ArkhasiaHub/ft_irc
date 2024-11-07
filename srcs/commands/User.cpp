// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 17/07/2024 23:17:00
// Last updated: 17/07/2024 23:17:00

#include "../../includes/Server.hpp"

static bool checkUserStr(const std::string& userStr) {
  if (userStr.find("0 * ") == std::string::npos) {
    return false;
  }
  return true;
}

void Server::user(const std::string& userStr, Client* client) {
  if (client->getIsPasswordSet() == false) {
    nr::rpl_NotRegistered(client);
    return ;
  }
  if (clientIsLogged(client)) {
    nr::rpl_AlreadyRegister(client);
    return;
  }
  size_t posUser = userStr.find_first_of(' ');
  size_t posMask = userStr.find_first_of('*');
  size_t posRealName = userStr.find_first_of(' ', posMask);

  if (posUser == std::string::npos || posRealName == std::string::npos || !checkUserStr(userStr.substr(posUser, 6))) {
    nr::rpl_NeedMoreParams(client, "USER");
    return;
  }
  std::string user = userStr.substr(0, posUser);
  std::string realName = userStr.substr(posRealName + 1);
  parseArgs(&realName);

  if (user.empty()) {
    nr::rpl_NeedMoreParams(client, "USER");
    return;
  }
  if (user.size() > 12) {
    user = user.substr(0, 12);
  }
  client->setUsername(user);
  client->setRealName(realName);
  clientIsLogged(client);
}
