// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 04/07/2024 15:09:00
// Last updated: 04/07/2024 15:09:00

#ifndef INCLUDES_NUMERICREPLY_HPP_
#define INCLUDES_NUMERICREPLY_HPP_

#include <string>
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

class Client;
class Channel;

namespace nr {
  void SendMessage(Client* client, const std::string& message);

  void rpl_Welcome(Client* client);
  void rpl_YourHost(Client* client);
  void rpl_Created(Client* client);
  void rpl_MyInfo(Client* client);

  void rpl_NeedMoreParams(Client* client, const std::string& command);
  void rpl_UnknownMode(Client* client, const std::string& modeChar);

  void rpl_NoNicknameGiven(Client* client);
  void rpl_ErroneusNickname(Client* client, const std::string& nickname);
  void rpl_NicknameInUse(Client* client, const std::string& nickname);
  void rpl_NicknameChangedNotify(Client* client, const std::string& oldNickname, Client* clientToSend);

  void rpl_AlreadyRegister(Client* client);
  void rpl_NotRegistered(Client* client);
  void rpl_NoTextToSend(Client* client);

  void rpl_Topic(Client* client, const Channel& channel);
  void rpl_NoTopic(Client* client, const Channel& channel);
  void rpl_Inviting(Client* client, const Channel& channel, const std::string& nickname);
  void rpl_JoinNotify(Client* client, const Channel& channel);
  void rpl_InviteNotify(const Client& client, const Channel& channel, Client* invitedClient);
  void rpl_Namrply(Client* client, Channel* channel);
  void rpl_EndOfName(Client* client, const Channel& channel);

  void rpl_WhoReply(Client* client, const Client& whoClient, const Channel& channel);
  void rpl_EndOfWhoReply(Client* client, const std::string& request);

  void rpl_MsgClientNotify(const Client& client, Client* targetClient, const std::string& msg);
  void rpl_MsgChannelNotify(Client* client, Channel* channel, const std::string& target, const std::string& msg);
  void rpl_NoSuchNick(Client* client, const std::string& target);
  void rpl_NoSuchChannel(Client* client, const std::string& target);
  void rpl_NoRecipient(Client* client);
  void rpl_CannotSendToChannel(Client* client, Channel* channel);

  void rpl_InviteOnlyChan(Client* client, Channel* channel);
  void rpl_ChannelIsFull(Client* client, Channel* channel);
  void rpl_BadChannelKey(Client* client, Channel* channel);
  void rpl_BadChanMask(Client* client, const std::string& channel);

  void rpl_UserNotInChannel(Client* client, Channel* channel, const std::string& target);
  void rpl_UserOnChannel(Client* client, Channel* channel, const std::string& nickname);
  void rpl_NoToChannel(Client* client, Channel* channel);
  void rpl_ChanoPrivsNeeded(Client* client, Channel* channel);
  void rpl_KickNotify(const Client& client, Channel* channel, const Client& target, const std::string& msg, Client* clientToSend);
  void rpl_ErrorNotify(Client* client, const std::string& msg);

  void rpl_OperatorNotify(Client* client, Channel* channel, const std::string& nickname, bool adding, const Client& clientGivingOp);
  void rpl_PasswordNotify(const Client& client, Channel* channel, const std::string& password, bool adding, Client* clientToSend);
  void rpl_LimitNotify(Client* client, Channel* channel, const std::string& channelLimit, bool adding);
  void rpl_PartNotify(Client* client, Channel* channel, const Client& clientLeaving, const std::string& msg);
  void rpl_InvalidModeParam(Client* client, const Channel& channel, const std::string& mode, const std::string& param);
  void rpl_PassWdMismatch(Client* client);

  void rpl_ChannelModeIs(Client* client, Channel* channel);

  void rpl_MotdStart(Client *client);
  void rpl_Motd(Client *client, const std::string& motdLine);
  void rpl_EndOfMotd(Client *client);
  void rpl_NoMotd(Client *client);
}  // namespace nrS

#endif  // INCLUDES_NUMERICREPLY_HPP_
