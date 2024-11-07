// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 17/06/2024 17:32:00
// Last updated: 27/06/2024 11:22:00

#include <netinet/in.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "../includes/Server.hpp"

long check_port(const std::string &port) {
  if (port.find_first_not_of("0123456789") != std::string::npos) {
    throw std::invalid_argument("Port must be a number");
  }
  errno = 0;
  char *end;
  long port_int = std::strtol(port.c_str(), &end, 10);
  if (errno == ERANGE || *end != '\0') {
    throw std::invalid_argument("Port must be between 1024 and 65535");
  }
  if (port_int < 1024 || port_int > 65535 || port.length() > 11) {
    throw std::invalid_argument("Port must be between 1024 and 65535");
  }
  return (port_int);
}

int main(int ac, char **av) {
  if (ac != 3) {
    std::cerr << "Usage: ./ircserv [port] [password]" << std::endl;
    return 1;
  }
  try {
    Server  servInfo(check_port(av[1]), av[2]);
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, Server::signalHandler);
    signal(SIGPIPE, SIG_IGN);
    servInfo.serverAddr.sin_family = AF_INET;
    servInfo.serverAddr.sin_port = htons(servInfo.getPort());
    servInfo.serverAddr.sin_addr.s_addr = INADDR_ANY;
    servInfo.createServ();
    servInfo.createMap();
    servInfo.irc();
  }
  catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
  return 1;
  }
  return 0;
}
