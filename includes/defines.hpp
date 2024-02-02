#pragma once

#define SERVER_PREFIX std::string(":ft_irc")
#define EOM std::string("\015\n")
#define MAX_MSG_SIZE 512
#define HOST "0.0.0.0"
#define SERV_MAX_CLIENTS 100

#define RPL_WELCOME std::string("001")
#define RPL_WELCOME_STR std::string("Welcome to our IRC")
#define RPL_CONFIRM_NICK_CHANGE std::string("Your nickname was changed")
#define RPL_YOURHOST std::string("002")
#define RPL_CREATED std::string("003")
#define RPL_MYINFO std::string("004")
#define RPL_BOUNCE std::string("005")

#define RPL_USERHOST std::string("302")
#define RPL_ISON std::string("303")
#define RPL_AWAY std::string("301")
#define RPL_UNAWAY std::string("305")
#define RPL_NOWAWAY std::string("306")

#define RPL_WHOISUSER std::string("311")
#define RPL_WHOISSERVER std::string("312")
#define RPL_WHOISOPERATOR std::string("313")
#define RPL_WHOISIDLE std::string("317")
#define RPL_ENDOFWHOIS std::string("318")
#define RPL_WHOISCHANNELS std::string("319")

#define RPL_LISTSTART std::string("321")
#define RPL_LIST std::string("322")
#define RPL_LISTEND std::string("323")

#define RPL_CHANNELMODEIS std::string("324")
#define RPL_NOTOPIC std::string("331")
#define RPL_TOPIC std::string("332")
#define RPL_INVITING std::string("341")
#define RPL_SUMMONING std::string("342")

#define RPL_VERSION std::string("351")
#define RPL_WHOREPLY std::string("352")
#define RPL_ENDOFWHO std::string("315")
#define RPL_NAMREPLY std::string("353")
#define RPL_ENDOFNAMES std::string("366")
#define RPL_ENDOFNAMES_STR std::string("End of /NAMES list")

#define RPL_LINKS std::string("364")
#define RPL_ENDOFLINKS std::string("365")
#define RPL_BANLIST std::string("367")
#define RPL_ENDOFBANLIST std::string("368")
#define RPL_INFO std::string("371")
#define RPL_ENDOFINFO std::string("374")
#define RPL_MOTDSTART std::string("375")
#define RPL_MOTD std::string("372")
#define RPL_ENDOFMOTD std::string("376")

#define ERR_NOSUCHNICK std::string("401")
#define ERR_NOSUCHNICK_STR std::string("No such nick/channel")
#define ERR_NOSUCHSERVER std::string("402")
#define ERR_NOSUCHCHANNEL std::string("403")
#define ERR_NOSUCHCHANNEL_STR std::string("No such channel")
#define ERR_CANNOTSENDTOCHAN std::string("404")
#define ERR_TOOMANYCHANNELS std::string("405")
#define ERR_WASNOSUCHNICK std::string("406")
#define ERR_WASNOSUCHNICK_STR std::string("There was no such nickname")

#define ERR_TOOMANYTARGETS std::string("407")
#define ERR_NOORIGIN std::string("409")
#define ERR_NORECIPIENT std::string("411")
#define ERR_NOTEXTTOSEND std::string("412")
#define ERR_NOTOPLEVEL std::string("413")
#define ERR_WILDTOPLEVEL std::string("414")

#define ERR_UNKNOWNCOMMAND std::string("421")
#define ERR_NOMOTD std::string("422")
#define ERR_NOADMININFO std::string("423")
#define ERR_FILEERROR std::string("424")
#define ERR_INVALIDCHAR std::string("425") //we made that up
#define ERR_INVALIDCHAR_STR std::string("Invalid char detected")

#define ERR_NONICKNAMEGIVEN std::string("431")
#define ERR_NONICKNAMEGIVEN_STR std::string("No nickname given")
#define ERR_ERRONEUSNICKNAME std::string("432")
#define ERR_ERRONEUSNICKNAME_STR std::string("Erroneous nickname")
#define ERR_NICKNAMEINUSE std::string("433")
#define ERR_NICKNAMEINUSE_STR std::string("Nickname is already in use") 
#define ERR_NICKCOLLISION std::string("436")

#define ERR_USERNOTINCHANNEL std::string("441")
#define ERR_NOTONCHANNEL std::string("442")
#define ERR_NOTONCHANNEL_STR std::string("You're not on that channel")
#define ERR_USERONCHANNEL std::string("443")
#define ERR_USERONCHANNEL_STR std::string("is already on channel")
#define ERR_NOLOGIN std::string("444")
#define ERR_SUMMONDISABLED std::string("445")
#define ERR_USERSDISABLED std::string("446")

#define ERR_NOTREGISTERED std::string("451")
#define ERR_NOTREGISTERED_STR std::string("You have not registered") 
#define ERR_NEEDMOREPARAMS std::string("461")
#define ERR_NEEDMOREPARAMS_STR std::string("Not enough parameters")
#define ERR_ALREADYREGISTRED std::string("462")
#define ERR_ALREADYREGISTRED_STR std::string("You may not reregister")
#define ERR_NOPERMFORHOST std::string("463")
#define ERR_PASSWDMISMATCH std::string("464")
#define ERR_PASSWDMISMATCH_STR std::string("Password incorrect")
#define ERR_YOUREBANNEDCREEP std::string("465")
#define ERR_YOUWILLBEBANNED std::string("466")
#define ERR_KEYSET std::string("467")

#define ERR_CHANNELISFULL std::string("471")
#define ERR_CHANNELISFULL_STR std::string("Cannot join channel (+l)")
#define ERR_UNKNOWNMODE std::string("472")
#define ERR_INVITEONLYCHAN std::string("473")
#define ERR_INVITEONLYCHAN_STR std::string("Cannot join channel (+i)")
#define ERR_BANNEDFROMCHAN std::string("474")
#define ERR_BADCHANNELKEY std::string("475")
#define ERR_BADCHANMASK std::string("476")
#define ERR_NOCHANMODES std::string("477")
#define ERR_BANLISTFULL std::string("478")

#define ERR_NOPRIVILEGES std::string("481")
#define ERR_CHANOPRIVSNEEDED std::string("482")
#define ERR_CHANOPRIVSNEEDED_STR std::string("You're not channel operator")
#define ERR_CANTKILLSERVER std::string("483")

#define ERR_RESTRICTED std::string("484")
#define ERR_UNIQOPPRIVSNEEDED std::string("485")

#define ERR_NOOPERHOST std::string("491")
#define ERR_UMODEUNKNOWNFLAG std::string("501")
#define ERR_USERSDONTMATCH std::string("502")
