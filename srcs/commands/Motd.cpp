// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 19/07/2024 09:29:00
// Last updated: 19/07/2024 09:29:00

#include "../../includes/Server.hpp"
#include <fstream>

void Server::motd(const std::string& target, Client* client) {
  (void) target;
  std::fstream motd;
  motd.open("Motd.txt");
  if (motd.is_open()) {
    std::string line;
    nr::rpl_MotdStart(client);
    while (std::getline(motd, line)) {
      nr::rpl_Motd(client, line);
      line.clear();
    }
    nr::rpl_EndOfMotd(client);
    motd.close();
  } else {
    nr::rpl_NoMotd(client);
  }
}
