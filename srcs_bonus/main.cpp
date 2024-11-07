// Copyright 2024 <earaujo@42lyon.fr>
// Code by: Evan (earaujo) Araujo
// Created: 12/07/2024 10:06:00
// Last updated: 12/07/2024 12:52:00

#include <netinet/in.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "../includes_bonus/Bot.hpp"

int check_port(const std::string &port) {
  if (port.find_first_not_of("0123456789") != std::string::npos) {
    throw std::invalid_argument("Port must be a number");
  }
  int port_int = std::atoi(port.c_str());
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
    Bot  botInfo(check_port(av[1]), av[2]);
    signal(SIGINT, Bot::signalHandler);
    signal(SIGQUIT, Bot::signalHandler);
    botInfo.serverAddr.sin_family = AF_INET;
    botInfo.serverAddr.sin_port = htons(botInfo.getPort());
    botInfo.serverAddr.sin_addr.s_addr = INADDR_ANY;
    botInfo.createBot();
  }
  catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
