// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 10/07/2024 12:43:00
// Last updated: 10/07/2024 12:43:00

#include "../../includes/Server.hpp"

void Server::invite(const std::string& args, Client* client) {
  std::stringstream ss(args);
  std::string channelName;
  std::string nickname;

  if (!clientIsLogged(client)) {
    nr::rpl_NotRegistered(client);
    return;
  }

  std::getline(ss, nickname, ' ');
  size_t pos = args.find_first_of('#');
  if (nickname[0] == '#' || pos == std::string::npos) {
    nr::rpl_NeedMoreParams(client, "INVITE");
    return;
  }
  
  channelName = args.substr(pos);
  Channel *channel = getChannel(channelName);
  if (!channel) {
    nr::rpl_NoSuchChannel(client, channelName);
    return;
  }
  if (!channel->getClient(client->getNickname())) {
    nr::rpl_NoToChannel(client, channel);
    return;
  }
  if (channel->getModeFlag(MODE_INVITE_ONLY) && !channel->isClientOperator(client->getNickname())) {
    nr::rpl_ChanoPrivsNeeded(client, channel);
    return;
  }

  Client  *newClient = getClient(nickname);
  if (!newClient) {
    nr::rpl_NoSuchNick(client, nickname);
    return;
  }
  if (channel->getClient(nickname)) {
    nr::rpl_UserOnChannel(client, channel, nickname);
    return;
  }

  nr::rpl_InviteNotify(*client, *channel, newClient);
  nr::rpl_Inviting(client, *channel, nickname);
  channel->inviteUser(nickname);
}
