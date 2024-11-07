// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 10/07/2024 12:40:00
// Last updated: 10/07/2024 12:40:00

#include "../../includes/Server.hpp"

void  Server::topic(const std::string& args, Client* client) {
  std::stringstream ss(args);
  std::string channelName;

  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }
  if (args.empty()) {
    nr::rpl_NeedMoreParams(client, "TOPIC");
    return;
  }

  std::getline(ss, channelName, ' ');

  Channel* channel = getChannel(channelName);

  if (!channel) {
    nr::rpl_NoSuchChannel(client, channelName);
    return;
  }
  if (!channel->getClient(client->getNickname())) {
    nr::rpl_NoToChannel(client, channel);
    return;
  }

  std::string newTopic;
  if (std::getline(ss, newTopic)) {
    parseArgs(&newTopic);
  }

  if (!newTopic.empty() &&
  ((channel->getModeFlag(MODE_TOPIC) &&
    channel->isClientOperator(client->getNickname())) ||
    !channel->getModeFlag(MODE_TOPIC))) {
    channel->setTopic(newTopic);
    strclientMap userToNotify = channel->getUserMap();
    if (!channel->getTopic().empty()) {
      strclientMap::iterator it;
      for (it = userToNotify.begin(); it != userToNotify.end(); it++) {
        nr::rpl_Topic(it->second, *channel);
      }
    } else {
      strclientMap::iterator it;
      for (it = userToNotify.begin(); it != userToNotify.end(); it++) {
        nr::rpl_NoTopic(it->second, *channel);
      }
    }
  } else if (!newTopic.empty() && channel->getModeFlag(MODE_TOPIC) &&
              !channel->isClientOperator(client->getNickname())) {
    nr::rpl_ChanoPrivsNeeded(client, channel);
    if (!channel->getTopic().empty()) {
      nr::rpl_Topic(client, *channel);
    } else {
      nr:: rpl_NoTopic(client, *channel);
    }
  } else if (newTopic.empty()) {
    if (!channel->getTopic().empty()) {
      nr::rpl_Topic(client, *channel);
    } else {
      nr:: rpl_NoTopic(client, *channel);
    }
  }
}
