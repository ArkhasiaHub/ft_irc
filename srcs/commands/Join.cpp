// Copyright 2024 <sbriche@42lyon.fr>
// Code by: Samuel (sbriche) Briche
// Created: 04/07/2024 15:09:00
// Last updated: 07/07/2024 16:38:00

#include "../../includes/Server.hpp"

void Server::parseJoin(Client* client, const std::string& args, std::vector<std::string>* channelName, std::vector<std::string>* channelKey) {
  std::stringstream ss(args);
  std::string channels, keys;

  std::getline(ss, channels, ' ');
  std::getline(ss, keys);

  std::stringstream channelStream(channels);
  std::string channel;
  while (std::getline(channelStream, channel, ',')) {
      if (!channel.empty() && (channel[0] == '#' || channel == "0")) {
        channelName->push_back(channel);
      } else {
        nr::rpl_BadChanMask(client, channel);
      }
  }

  if (!keys.empty()) {
    std::stringstream keyStream(keys);
    std::string key;
    while (std::getline(keyStream, key, ',')) {
      parseArgs(&key);
      channelKey->push_back(key);
    }
  }
}

void Server::addNewChanClient(Channel* channel, Client* client) {
  channel->addClient(client->getNickname(), client);
  nr::rpl_JoinNotify(client, *channel);
  if (channel->getTopic().empty()) {
    nr::rpl_NoTopic(client, *channel);
  } else {
    nr::rpl_Topic(client, *channel);
  }

  strclientMap strclientMap  = channel->getClients();
  strclientMap::iterator it;
  for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
    nr::rpl_Namrply(it->second, channel);
    nr::rpl_EndOfName(it->second, *channel);
  }
}

void Server::checkChannel(const std::string& channelName, const std::string& key, Client* client) {
  if (!channelName.empty() && (channelName[0] != '#' || (channelName[0] == '#' && channelName.size() == 1))) {
    nr::rpl_BadChanMask(client, channelName);
    return;
  }
  if (!isChannelStored(channelName)) {
    try {
      Channel* newChannel = new Channel(channelName);
      if (!key.empty()) {
        newChannel->addModeFlag(MODE_PASSWORD);
        newChannel->setPassword(key);
      }
      newChannel->addClient(client->getNickname(), client);

      nr::rpl_JoinNotify(client, *newChannel);
      if (newChannel->getTopic().empty()) {
        nr::rpl_NoTopic(client, *newChannel);
      } else {
        nr::rpl_Topic(client, *newChannel);
      }
      newChannel->addClientOperator(client->getNickname());
      _channels.insert(std::make_pair(channelName, newChannel));
      nr::rpl_OperatorNotify(client, newChannel, client->getNickname(), true, *client);
      nr::rpl_Namrply(client, newChannel);
      nr::rpl_EndOfName(client, *newChannel);
    }
    catch (std::exception& e) {
      for (size_t i = 0; i < _clientList.size(); i++) {
        nr::rpl_ErrorNotify(_clientList[i], "Fatal Error server closing.");
      }
      _signals = true;
    }
    return;
  }

  Channel* channel = getChannel(channelName);

  if (channel->getClient(client->getNickname())) {
    nr::rpl_UserOnChannel(client, channel, client->getNickname());
    return;
  }

  if (channel->getModeFlag(MODE_INVITE_ONLY)) {
    if (!channel->isUserInvited(client->getNickname())) {
      nr::rpl_InviteOnlyChan(client, channel);
      return;
    } else {
      channel->eraseInvitedUser(client->getNickname());
    }
  }
  
  if (channel->getModeFlag(MODE_LIMIT)) {
    if (channel->getNumberUser() + 1 > channel->getLimitUser()) {
      nr::rpl_ChannelIsFull(client, channel);
      return;
    }
  }

  if (channel->getModeFlag(MODE_PASSWORD) && channel->getPassword() != key) {
    nr::rpl_BadChannelKey(client, channel);
    return;
  }
  addNewChanClient(channel, client);
}

void Server::ClientQuitAllChannel(Client* client) {
  strchannelMap::iterator chanIt = _channels.begin();
  while (chanIt != _channels.end()) {
    strchannelMap::iterator tmp = chanIt;
    tmp++;
    Channel* channel = chanIt->second;
    if (channel->getClient(client->getNickname())) {
      part(channel->getName(), client);
    }
    chanIt = tmp;
  }
}

void Server::join(const std::string& args, Client* client) {
  std::vector<std::string> channelName;
  std::vector<std::string> channelKey;

  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }
  if (args.empty()) {
    nr::rpl_NeedMoreParams(client, "JOIN");
    return;
  }
  parseJoin(client, args, &channelName, &channelKey);
  if (channelName.size() == 1) {
    if (channelName.at(0) == "0") {
      ClientQuitAllChannel(client);
      return;
    }
  }
  for (size_t i = 0; i < channelName.size(); i++) {
    if (i < channelKey.size()) {
      checkChannel(channelName[i], channelKey[i], client);
    } else {
      checkChannel(channelName[i], "", client);
    }
  }
}
