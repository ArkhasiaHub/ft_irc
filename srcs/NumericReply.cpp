// Copyright 2024 <ccharton@42lyon.fr>
// Code by: Corentin (ccharton) Charton
// Created: 04/07/2024 15:09:00
// Last updated: 07/07/2024 16:33:00

#include "../includes/NumericReply.hpp"

void nr::SendMessage(Client* client, const std::string& message) {
  send(client->getFd(), message.c_str(), message.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

void nr::rpl_Welcome(Client* client) {
  std::string message =  ":localhost 001 " + client->getNickname() +
      " :Welcome to the ft_irc Network, " + client->getNickname()  +
      "[" + client->getUsername()+ "@" + "ft_irc]\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_YourHost(Client* client) {
  std::string message =  ":localhost 002 " + client->getNickname() +
      " :Your host is localhost, running version idontknowwhatimdoing\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_Created(Client* client) {
  std::string message =  ":localhost 003 " + client->getNickname() +
      " :This server was created on le computer\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_MyInfo(Client* client) {
  std::string message =  ":localhost 004 " + client->getNickname() +
      " :localhost idontknowwhatimdoing [insert available umodes]" +
      "[insert available cmodes][insert cmodes with param]\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NeedMoreParams(Client* client, const std::string& command) {
  std::string message =  ":localhost 461 " + client->getNickname() +
      " " + command + " :Not enough parameters\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_UnknownMode(Client* client, const std::string& modeChar) {
  std::string message =  ":localhost 472 " + modeChar +
      " :is unknown mode char to me\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NoNicknameGiven(Client* client) {
  std::string message =  ":localhost 431 :No nickname given\r\n";
  nr::SendMessage(client, message);
}

void nr::rpl_ErroneusNickname(Client* client, const std::string& nickname) {
  std::string message =  ":localhost 432 " + nickname + " :Erroneus nickname\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NicknameInUse(Client* client, const std::string& nickname) {
  std::string message =  ":localhost 433 " + nickname +
      " :Nickname is already in use\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NicknameChangedNotify(Client* client, const std::string& oldNickname, Client* clientToSend) {
  std::string message = ":" + oldNickname + " NICK "
                        + client->getNickname() + "\r\n";

  nr::SendMessage(clientToSend, message);
}

void nr::rpl_AlreadyRegister(Client* client) {
  std::string message =  ":localhost 462 : You may not reregister\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_Topic(Client* client, const Channel& channel) {
  std::string message =  ":localhost 332 " + client->getNickname() +
      " " + channel.getName() + " " + channel.getTopic() + "\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NotRegistered(Client* client) {
  std::string message =  ":localhost 451 " + client->getNickname() + " :You have not registered\r\n";
  nr::SendMessage(client, message);
}

void nr::rpl_NoTextToSend(Client* client) {
  std::string message =  ":localhost 451 " + client->getNickname() + " :No text to send\r\n";
  nr::SendMessage(client, message);
}

void nr::rpl_NoTopic(Client* client, const Channel& channel) {
  std::string message =  ":localhost 331 " + client->getNickname() +
      " " + channel.getName() + " No topic is set\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_JoinNotify(Client* client, const Channel& channel) {
  std::string message =  ":" + client->getNickname() + " JOIN " +
      channel.getName()+ "\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_InviteNotify(const Client& client, const Channel& channel, Client* invitedClient) {
  std::string message =  ":" + client.getNickname() + " INVITE " +
      invitedClient->getNickname() + " " + channel.getName()+ "\r\n";

  nr::SendMessage(invitedClient, message);
}

void nr::rpl_Inviting(Client* client, const Channel &channel, const std::string& nickname) {
  std::string message =  ":localhost 341 " + client->getNickname() +
      " " + nickname + " " + channel.getName() + "\r\n";

  nr::SendMessage(client, message);
}


void nr::rpl_Namrply(Client* client, Channel* channel) {
  std::string message =  ":localhost 353 " + client->getNickname() +
      " = " + channel->getName() + " :" + channel->getAllUser() + "\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_EndOfName(Client* client, const Channel& channel) {
  std::string message =  ":localhost 366 " + client->getNickname() +
      " " + channel.getName() + " :End of /NAMES list.\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_WhoReply(Client* client, const Client& whoClient, const Channel& channel) {
  std::string message =  + ":localhost 352 "
     + client->getNickname() + " " + channel.getName() + " " +
     whoClient.getUsername() + " " + "localhost " + "localhost " +
     whoClient.getNickname() + " " + "H" +
     (channel.isClientOperator(whoClient.getNickname()) ? "@" : "") + " :0 "
     + whoClient.getRealName() + "\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_EndOfWhoReply(Client* client, const std::string& request) {
  std::string message =  ":localhost 315 " + client->getNickname() +
      " " + request + " :End of WHO list\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_MsgClientNotify(const Client& client, Client* targetClient, const std::string& msg) {
  std::string message = ":" + client.getNickname() + " PRIVMSG " +
      client.getNickname() + " " + msg + "\r\n";

  nr::SendMessage(targetClient, message);
}

void nr::rpl_MsgChannelNotify(Client* client, Channel* channel, const std::string& target, const std::string& msg) {
  std::string message = ":" + client->getNickname() + " PRIVMSG " +
      target + " " + msg + "\r\n";

  const strclientMap& clients = channel->getClients();
  strclientMap::const_iterator it;
  for (it = clients.begin(); it != clients.end(); ++it) {
    if (client->getFd() != it->second->getFd()) {
      nr::SendMessage(it->second, message);
    }
  }
}

void nr::rpl_NoRecipient(Client* client) {
  std::string message = ":localhost 411 " + client->getNickname() +
      " :No recipient given (PRIVMSG)\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_CannotSendToChannel(Client* client, Channel* channel) {
  std::string message = ":localhost 404 " + client->getNickname() + " "
                        + channel->getName() + " :Cannot send to channel\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NoSuchNick(Client* client, const std::string& target) {
  std::string message = ":localhost 401 " + client->getNickname() +
      " " + target + " :No such nick/channel\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NoSuchChannel(Client* client, const std::string& target) {
  std::string message = ":localhost 403 " + client->getNickname() +
      " " + target + " :No such channel\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_InviteOnlyChan(Client* client, Channel* channel) {
  std::string message = ":localhost 473 " + client->getNickname() +
      " " + channel->getName() + " :Cannot join channel (+i)\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_ChannelIsFull(Client* client, Channel* channel) {
  std::string message = ":localhost 471 " + client->getNickname() +
      " " + channel->getName() + " :Cannot join channel (+l)\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_BadChannelKey(Client* client, Channel* channel) {
  std::string message = ":localhost 475 " + client->getNickname() +
      " " + channel->getName() + " :Cannot join channel (+k)\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_BadChanMask(Client* client, const std::string& channel) {
  std::string message = ":localhost 476 " + channel + " :Bad Channel Mask\r\n";
  nr::SendMessage(client, message);
}

void nr::rpl_UserNotInChannel(Client* client, Channel* channel, const std::string& target) {
  std::string message = ":localhost 441 " + target + " " +
      channel->getName() + " :They aren't on that channel\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_NoToChannel(Client* client, Channel* channel) {
  std::string message = ":localhost 442 " + client->getNickname() + " " +
      channel->getName() + " :You're not on that channel\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_UserOnChannel(Client* client, Channel *channel, const std::string& nickname) {
  std::string message = ":localhost 443 " + client->getNickname() +
      " " + nickname + " " + channel->getName() + " :is already on channel\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_ChanoPrivsNeeded(Client* client, Channel* channel) {
  std::string message = ":localhost 482 " + client->getNickname() +
      " " + channel->getName() + " :You're not channel operator\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_KickNotify(const Client& client, Channel* channel, const Client& target, const std::string& msg, Client* clientToSend) {
  std::string message =  ":" + client.getNickname() + " KICK " +
      channel->getName() + " " + target.getNickname() + " :" + msg + "\r\n";

  nr::SendMessage(clientToSend, message);
}
void nr::rpl_ErrorNotify(Client* client, const std::string& msg) {
  std::string message =  "ERROR :"+ (msg.empty() ? "Leaving\r\n" : msg + "\r\n");
  nr::SendMessage(client, message);
}

void nr::rpl_OperatorNotify(Client* client, Channel* channel, const std::string& nickname, bool adding, const Client& clientGivingOp) {
  std::string message = ":" + clientGivingOp.getNickname() +" MODE " +
      channel->getName() + (adding ? " +o " :  " -o ") + nickname + "\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_PasswordNotify(const Client& client, Channel* channel, const std::string& password, bool adding, Client* clientToSend) {
  std::string message = ":" + client.getNickname() + " MODE " +
      channel->getName() + (adding ? " +k " :  " -k ") + password + "\r\n";

  nr::SendMessage(clientToSend, message);
}

void nr::rpl_LimitNotify(Client* client, Channel* channel, const std::string& channelLimit, bool adding) {
  std::string message = ":" + client->getNickname() + " MODE " +
      channel->getName() + (adding ? " +l " :  " -l ") + channelLimit + "\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_PartNotify(Client* client, Channel* channel, const Client& clientLeaving, const std::string& msg) {
  std::string message = ":" + clientLeaving.getNickname() +
      " PART " + channel->getName() + (msg.empty() ? " Leaving\r\n" : " " + msg + "\r\n");

  nr::SendMessage(client, message);
}

void nr::rpl_InvalidModeParam(Client* client, const Channel& channel, const std::string& mode, const std::string& param) {
  std::string message = ":localhost 696 " + client->getNickname() + " " +
      channel.getName() + " " + mode + " " + param +
      " :Invalid mode parameter\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_PassWdMismatch(Client* client) {
  std::string message = ":localhost 464 " + client->getNickname() +
      " :Password incorrect\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_ChannelModeIs(Client* client, Channel* channel) {
  std::stringstream ssMsg;

  ssMsg << ":localhost 324 " << client->getNickname() << " " << channel->getName();
  for (int i = 0; i < 4; i++) {
    switch (i) {
      case 0:
        channel->getModeFlag(MODE_INVITE_ONLY) ? ssMsg << " +i" : ssMsg << "";
        break;
      case 1:
        channel->getModeFlag(MODE_PASSWORD) ? ssMsg << " +k" : ssMsg << "";
        break;
      case 2:
        channel->getModeFlag(MODE_TOPIC) ? ssMsg << " +t" : ssMsg << "";
        break;
      case 3:
        channel->getModeFlag(MODE_LIMIT) ? ssMsg << " +l " << channel->getLimitUser() : ssMsg << "";
        break;
      default:
        break;
    }
  }
  ssMsg << "\r\n";
  std::string message = ssMsg.str();
  nr::SendMessage(client, message);
}


void nr::rpl_MotdStart(Client *client) {
  std::string message = ":localhost 375 " + client->getNickname() +
                        + ":- ft_irc@" + client->getNickname() +
                        " Message of the day - \r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_Motd(Client *client, const std::string& motdLine) {
  std::string message = ":localhost 372 " + client->getNickname() + " :"
                        + motdLine + "\r\n";

  nr::SendMessage(client, message);
}

void nr::rpl_EndOfMotd(Client *client) {
  std::string message = ":localhost 376 " + client->getNickname() +
                        " :End of /MOTD command.\r\n";

  nr::SendMessage(client, message);
}


void nr::rpl_NoMotd(Client *client) {
  std::string message = ":localhost 376 " + client->getNickname() +
                        " :MOTD File is missing\r\n";

  nr::SendMessage(client, message);
}
