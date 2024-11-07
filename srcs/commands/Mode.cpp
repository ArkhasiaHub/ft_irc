// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 10/07/2024 13:03:00
// Last updated: 10/07/2024 13:03:00

#include "../../includes/Server.hpp"

static bool modeInvite(Client* client, Channel* channel, const bool& adding) {
  if (!channel->isClientOperator(client->getNickname())) {
    nr::rpl_ChanoPrivsNeeded(client, channel);
    return false;
  }
  if (adding) {
    channel->addModeFlag(MODE_INVITE_ONLY);
  } else {
    channel->removeModeFlag(MODE_INVITE_ONLY);
  }
  return true;
}

static bool modeOperator(Client* client, Channel* channel, const bool& adding, const std::vector<std::string>& modeParams, size_t* paramIndex) {
  if (modeParams.empty()) {
    nr::rpl_NeedMoreParams(client, "MODE");
    return false;
  }
  if (!channel->getClient(client->getNickname())) {
    nr::rpl_UserNotInChannel(client, channel, client->getNickname());
    return false;
  }
  if (channel->isClientOperator(client->getNickname())) {
    if (*paramIndex < modeParams.size()) {
      const std::string& nick = modeParams[(*paramIndex)++];
      if (!channel->getClient(nick)) {
        nr::rpl_NoSuchNick(client, nick);
        return false;
      }
      if (adding) {
        channel->addClientOperator(nick);
      } else {
        channel->eraseClientOperator(nick);
      }
      strclientMap strclientMap  = channel->getClients();
      strclientMap::iterator it;
      for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
        nr::rpl_OperatorNotify(it->second, channel, nick, adding, *client);
        nr::rpl_Namrply(it->second, channel);
        nr::rpl_EndOfName(it->second, *channel);
      }
    }
  } else {
    nr::rpl_ChanoPrivsNeeded(client, channel);
    return false;
  }
  return true;
}

static bool modeTopic(Client* client, Channel* channel, const bool& adding) {
  if (!channel->getClient(client->getNickname())) {
    nr::rpl_UserNotInChannel(client, channel, client->getNickname());
    return false;
  }
  if (!channel->isClientOperator(client->getNickname())) {
    nr::rpl_ChanoPrivsNeeded(client, channel);
    return false;
  }
  if (adding) {
    channel->addModeFlag(MODE_TOPIC);
  } else {
    channel->removeModeFlag(MODE_TOPIC);
  }
  return true;
}

static bool modeLimitUser(Client* client, Channel* channel, const bool& adding, const std::vector<std::string>& modeParams, size_t* paramIndex) {
  std::string tmp;

  if (adding && modeParams.empty()) {
    nr::rpl_NeedMoreParams(client, "MODE");
    return false;
  }
  if (!channel->getClient(client->getNickname())) {
    nr::rpl_UserNotInChannel(client, channel, client->getNickname());
    return false;
  }
  if (!channel->isClientOperator(client->getNickname())) {
    nr::rpl_ChanoPrivsNeeded(client, channel);
    return false;
  }
  if (adding && *paramIndex < modeParams.size()) {
    tmp = modeParams[(*paramIndex)++];
    
    errno = 0;
    char *end;
    size_t limit = std::strtol(tmp.c_str(), &end, 10);
    if (errno == ERANGE || *end != '\0') {
      nr::rpl_InvalidModeParam(client, *channel, "l", tmp);
      return false;
    }

    channel->addModeFlag(MODE_LIMIT);
    channel->setLimitUser(limit);
  } else {
    channel->removeModeFlag(MODE_LIMIT);
  }
  strclientMap strclientMap  = channel->getClients();
  strclientMap::iterator it;
  for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
    nr::rpl_LimitNotify(it->second, channel, tmp, adding);
  }
  return true;
}

static bool modePassword(Client* client, Channel* channel, const bool& adding, const std::vector<std::string>& modeParams, size_t* paramIndex) {
  if (!channel->getClient(client->getNickname())) {
    nr::rpl_UserNotInChannel(client, channel, client->getNickname());
    return false;
  }
  if (!channel->isClientOperator(client->getNickname())) {
    nr::rpl_ChanoPrivsNeeded(client, channel);
    return false;
  }
  if (adding && modeParams.empty()) {
    nr::rpl_NeedMoreParams(client, "MODE");
    return false;
  }
  if (adding && *paramIndex < modeParams.size()) {
    const std::string& key = modeParams[(*paramIndex)++];
    channel->addModeFlag(MODE_PASSWORD);
    channel->setPassword(key);
  } else {
    channel->removeModeFlag(MODE_PASSWORD);
  }
  strclientMap clientMap  = channel->getClients();
  strclientMap::iterator it;
  for (it = clientMap.begin(); it != clientMap.end(); it++) {
    nr::rpl_PasswordNotify(*client, channel, channel->getPassword(), adding, it->second);
  }
  return true;
}

void Server::applyModeChanges(Client* client, Channel* channel, const std::string& modeChanges, const std::vector<std::string>& modeParams) {
  bool adding = true;
  size_t paramIndex = 0;

  for (size_t i = 0; i < modeChanges.size(); i++) {
    char c = modeChanges[i];
    if (c == '+') {
      adding = true;
      continue;
    } else if (c == '-') {
      adding = false;
      continue;
    }
    switch (c) {
    case 'i':
      if (!modeInvite(client, channel, adding)) {
        return;
      }
      break;
    case 'o':
      if (!modeOperator(client, channel, adding, modeParams, &paramIndex)) {
        return;
      }
      break;
    case 't':
      if (!modeTopic(client, channel, adding)) {
        return;
      }
        break;
    case 'l':
      if (!modeLimitUser(client, channel, adding, modeParams, &paramIndex)) {
        return;
      }
      break;
    case 'k':
      if (!modePassword(client, channel, adding, modeParams, &paramIndex)) {
        return;
      }
      break;
    default:
      nr::rpl_UnknownMode(client, std::string(1, c));
      break;
    }
  }
}

void Server::mode(const std::string& args, Client* client) {
  std::stringstream ss(args);
  std::vector<std::string> tokens;
  std::string tmp;

  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }

  while (std::getline(ss, tmp, ' ')) {
    tokens.push_back(tmp);
  }

  if (tokens.empty()) {
    nr::rpl_NeedMoreParams(client, "MODE");
    return;
  }

  std::string channel = tokens[0];
  Channel* chan = getChannel(channel);
  if (!chan) {
    nr::rpl_NoSuchChannel(client, channel);
    return;
  }

  std::string modeChanges;
  std::vector<std::string> modeParams;

  if (tokens.size() > 1) {
    modeChanges = tokens[1];
    modeParams = std::vector<std::string>(tokens.begin() + 2, tokens.end());
  }

  if (modeChanges.empty()) {
    if (!chan->getClient(client->getNickname())) {
      nr::rpl_NoToChannel(client, chan);
      return;
    }
    nr::rpl_ChannelModeIs(client, chan);
    return;
  }
  applyModeChanges(client, chan, modeChanges, modeParams);
}
