NAME	=	ircserv

NAME_BONUS	= bugshot

CC 		=	c++
CXX_FLAGS_OBJS	=	-g -std=c++98 -Wall -Wextra -Werror -MMD -MP
CXX_FLAGS_EXEC	=	-g -std=c++98 -Wall -Wextra -Werror
F_INC 	=	includes/
F_INC_BONUS	=	includes_bonus/
F_OBJS	=	.objects/
F_OBJS_BONUS	=	.objects_bonus/
F_SRC	=	srcs/
F_CMD	=	srcs/commands/
F_SRC_BONUS	=	srcs_bonus/

SRCS 	=	$(F_SRC)main.cpp \
			$(F_SRC)Server.cpp \
			$(F_SRC)Client.cpp \
			$(F_SRC)Channel.cpp \
			$(F_SRC)NumericReply.cpp \
			$(F_CMD)Join.cpp \
			$(F_CMD)Privmsg.cpp \
			$(F_CMD)Mode.cpp \
			$(F_CMD)Topic.cpp \
			$(F_CMD)Kick.cpp \
			$(F_CMD)Invite.cpp \
			$(F_CMD)Part.cpp \
			$(F_CMD)Quit.cpp \
			$(F_CMD)User.cpp \
			$(F_CMD)Nick.cpp \
			$(F_CMD)Who.cpp \
			$(F_CMD)Pass.cpp \
			$(F_CMD)Motd.cpp \
			$(F_CMD)Ping.cpp \
			$(F_CMD)Pong.cpp
SRCS_BONUS = $(F_SRC_BONUS)Bot.cpp \
             $(F_SRC_BONUS)main.cpp
O_SRCS    =   $(SRCS:%.cpp=$(F_OBJS)%.o)
O_SRCS_BONUS	=	$(SRCS_BONUS:%.cpp=$(F_OBJS_BONUS)%.o)
D_SRCS    =   $(O_SRCS:$(F_OBJS)%.o=$(F_OBJS)%.d)
D_SRCS_BONUS    =   $(O_SRCS_BONUS:$(F_OBJS_BONUS)%.o=(F_OBJS_BONUS)%.d)

$(F_OBJS)%.o : %.cpp
		mkdir -p $(F_OBJS)$(F_SRC) $(F_OBJS)$(F_CMD)
		$(CC) $(CXX_FLAGS_OBJS) -c $< -o $@

$(F_OBJS_BONUS)%.o : %.cpp
		mkdir -p $(dir $@)
		$(CC) $(CXX_FLAGS_OBJS) -c $< -o $@

all:
	$(MAKE) $(O_SRCS) $(NAME)

$(NAME):  $(SRCS)
	$(CC) $(CXX_FLAGS_EXEC) -I $(F_INC) $(SRCS) -o $(NAME)

$(NAME_BONUS):	$(SRCS_BONUS)
	$(CC) $(CXX_FLAGS_EXEC) -I $(F_INC_BONUS) $(SRCS_BONUS) -o $(NAME_BONUS)

clean:
	rm -rf $(F_OBJS) $(F_OBJS_BONUS)

fclean:
	$(MAKE) clean
	rm -rf $(NAME) $(NAME_BONUS)

bonus:
	$(MAKE) $(O_SRCS_BONUS) $(NAME_BONUS)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re bonus

-include $(D_SRCS) $(D_SRCS_BONUS)
