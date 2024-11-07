// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 10/07/2024 12:42:00
// Last updated: 10/07/2024 12:42:00

#include "../../includes/Server.hpp"

void Server::parseKick(const std::string& args, std::vector<std::string>* targets, std::string* channelTarget, std::string* msg) {
  std::stringstream ss(args);
  std::string tmp;

  std::getline(ss, *channelTarget, ' ');
  if (std::getline(ss, tmp, ' ')) {
    std::stringstream ssTarget(tmp);
    while (std::getline(ssTarget, tmp, ','))
      targets->push_back(tmp);
  }
  if (std::getline(ss, *msg)) {
    parseArgs(msg);
  }
}

static void KickGetMsg(const std::string& tmpMsg, const std::string& target, const std::string& channelTarget, std::string& msg) {
  if (tmpMsg.empty())
    msg = target + " was kicked from " + channelTarget;
  else
    msg = tmpMsg;
}

bool Server::KickCheckData(Channel** channel, Client* client, const std::string& channelTarget) const {
  *channel = getChannel(channelTarget);

  if (!*channel) {
    nr::rpl_NoSuchChannel(client, channelTarget);
    return false;
  }
  if (!(*channel)->getClient(client->getNickname())) {
    nr::rpl_NoToChannel(client, *channel);
    return false;
  }
  if (!(*channel)->isClientOperator(client->getNickname())) {
    nr::rpl_ChanoPrivsNeeded(client, *channel);
    return false;
  }
  return true;
}

void Server::KickLogic(Client* client, Channel* channel, Client* target, const std::string& msg, const std::string& userTarget) {
  channel->eraseClient(userTarget);
  nr::rpl_KickNotify(*client, channel, *target, msg, getClient(userTarget));
  if (channel->isClientOperator(userTarget)) {
    channel->eraseClientOperator(userTarget);
  }

  if (channel->getNumberUser() > 0) {
    if (channel->getNumberClientOperator() == 0) {
      channel->addClientOperator(channel->getClients().begin()->second->getNickname());
      strclientMap strclientMap  = channel->getClients();
      Client* newOperator = channel->getClients().begin()->second;
      strclientMap::iterator it;
      for (it = strclientMap.begin(); it != strclientMap.end(); it++) {
        nr::rpl_OperatorNotify(it->second, channel, newOperator->getNickname(), true, *client);
        nr::rpl_KickNotify(*client, channel, *target, msg, it->second);
        nr::rpl_Namrply(it->second, channel);
        nr::rpl_EndOfName(it->second, *channel);
      }

    } else {
      strclientMap userToNotify = channel->getUserMap();
      strclientMap::iterator it;
      for (it = userToNotify.begin(); it != userToNotify.end(); it++) {
        nr::rpl_KickNotify(*client, channel, *target, msg, it->second);
      }
    }
  }
}

void Server::kick(const std::string& args, Client* client) {
  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }

  if (args.find(' ') == std::string::npos) {
    nr::rpl_NeedMoreParams(client, "KICK");
    return;
  }

  std::vector<std::string> targets;
  std::string channelTarget;
  std::string tmpMsg;
  parseKick(args, &targets, &channelTarget, &tmpMsg);

  Channel* channel = NULL;
  if (!KickCheckData(&channel, client, channelTarget)) {
    return;
  }

  for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++) {
    Client* target = channel->getClient(*it);
    if (!target) {
      nr::rpl_UserNotInChannel(client, channel, *it);
      continue;
    }

    std::string msg;
    KickGetMsg(tmpMsg, *it, channelTarget, msg);

    if (channel->getNumberUser() == 1) {
      part(channel->getName(), client);
      return;
    }

    KickLogic(client, channel, target, msg, *it);
  }
}
