// Copyright 2024 <sbriche@42lyon.fr>
// Code by: Samuel (sbriche) Briche
// Created: 04/07/2024 15:09:00
// Last updated: 07/07/2024 16:30:00

#include "../../includes/Server.hpp"

void Server::sendToClient(Client* client, const std::string& target, const std::string& msg)  {
  if (!isClientStored(target)) {
    nr::rpl_NoRecipient(client);
    return;
  }
  Client* targetClient = getClient(target);
  if (targetClient != NULL) {
    nr::rpl_MsgClientNotify(*client, targetClient, msg);
  }
}

void Server::sendToChannel(Client* client, const std::string& target, const std::string& msg) const {
  Channel* channel = getChannel(target);
  if (channel != NULL) {
    if (channel->getClient(client->getNickname())) {
      nr::rpl_MsgChannelNotify(client, channel, target, msg);
    } else {
      nr::rpl_CannotSendToChannel(client, channel);
    }
  } else {
    nr::rpl_NoRecipient(client);
  }
}

void Server::privmsg(const std::string& args, Client* client) {
  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }
  std::stringstream ss(args);
  std::vector<std::string> targets;
  std::string msg;
  std::string tmp;

  if (getline(ss, tmp, ' ')) {
    std::stringstream targetStream(tmp);
    std::string target;
    while (getline(targetStream, target, ',')) {
      targets.push_back(target);
    }
  }
  if (std::getline(ss, msg)) {
    parseArgs(&msg);
  }

  if (targets.empty()) {
      nr::rpl_NoRecipient(client);
      return;
  }
  if (msg.empty()) {
      nr::rpl_NoTextToSend(client);
      return;
  }
  for (size_t i = 0; i < targets.size(); i++) {
    if (targets[i][0] == '#') {
    sendToChannel(client, targets[i], msg);
    } else {
      sendToClient(client, targets[i], msg);
    }
  }
}
