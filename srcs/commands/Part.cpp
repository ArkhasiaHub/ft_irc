// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 10/07/2024 17:33:00
// Last updated: 12/07/2024 13:46:00

#include "../../includes/Server.hpp"

void Server::partChannel(Client* client, const std::string& channelName, const std::string& msg) {
  if (channelName.empty() || channelName[0] != '#') {
    nr::rpl_NeedMoreParams(client, "PART");
    return;
  }
  if (!isChannelStored(channelName)) {
    nr::rpl_NoSuchChannel(client, channelName);
    return;
  }
  Channel* channel = _channels.find(channelName)->second;
  if (!channel->getClient(client->getNickname())) {
    nr::rpl_NoToChannel(client, channel);
    return;
  }
  nr::rpl_PartNotify(client, channel, *client, msg);
  channel->eraseClient(client->getNickname());
  if (channel->isClientOperator(client->getNickname())) {
    channel->eraseClientOperator(client->getNickname());
  }
  if (channel->getNumberUser() > 0) {
    if (channel->getNumberClientOperator() == 0) {
      channel->addClientOperator(channel->getClients().begin()->second->getNickname());
      strclientMap strclientMap  = channel->getClients();
      Client* newOperator = channel->getClients().begin()->second;
      strclientMap::iterator it;
      for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
        nr::rpl_OperatorNotify(it->second, channel, newOperator->getNickname(), true, *client);
      }
    }
    strclientMap strclientMap  = channel->getClients();
    strclientMap::iterator it;
    for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
      nr::rpl_PartNotify(it->second, channel, *client, msg);
      nr::rpl_Namrply(it->second, channel);
      nr::rpl_EndOfName(it->second, *channel);
    }
  } else {
    delete channel;
    _channels.erase(channelName);
  }
}

void Server::part(const std::string& args, Client* client) {

  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }
  if (args.empty()) {
    nr::rpl_NeedMoreParams(client, "PART");
    return;
  }

  std::vector<std::string> channels;
  std::stringstream ss(args);
  std::string tmp;
  std::string msg;

  if (std::getline(ss, tmp, ' ')) {
    std::stringstream targetChannel(tmp);
    std::string chan;
    while (std::getline(targetChannel, chan, ',')) {
      channels.push_back(chan);
    }
  }
  if (std::getline(ss, msg)) {
    parseArgs(&msg);
  }

  for (size_t i = 0; i < channels.size(); i++) {
    partChannel(client, channels[i], msg);
  }
}
