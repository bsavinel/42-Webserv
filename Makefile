NAME = Webserv

################################################################################
#                           COMPILATION SETTING                        	       #
################################################################################

CC			= c++
CPPCFLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 -g3
LDFLAGS		= -Wall -Wextra -Werror -std=c++98 -g3

ifeq (sanitize, $(filter sanitize, $(MAKECMDGOALS)))
	CPPFLAGS += -fsanitize=address
endif

################################################################################
#                               SOURCE FILES                              	   #
################################################################################

SRCS =	main.cpp			\
		Socket.cpp			\
		Server.cpp			\
		exceptWebserv.cpp	\
		Epoll.cpp			\
		HttpRequest.cpp		\
		HttpRespond.cpp		\
		launcher.cpp		\
		ClientEvent.cpp		\
		ServeurEvent.cpp	\
		itoa.cpp			\

################################################################################
#                               INCLUDES                             	       #
################################################################################

INCS			=	-I ./includes/	\

LIBINCS			=	

################################################################################
#                               PATH_TO_FILES                              	   #
################################################################################

SRC_PATH		=	./srcs/
OBJ_PATH		=	./objs/

################################################################################
#                               OUTPUT_FILES                              	   #
################################################################################

OBJS			=	$(addprefix $(OBJ_PATH), ${SRCS:.cpp=.o})
DEPS			=	$(addprefix $(OBJ_PATH), ${SRCS:.cpp=.d})

################################################################################
#                                   COLORS                                     #
################################################################################

BLUE			=	\033[0;34m
RED				=	\033[0;31m
GREEN			=	\033[0;32m
NO_COLOR		=	\033[m

################################################################################
#                                   RULES                                      #
################################################################################

all	: 
	echo "$(BLUE)" Compilation in progress... "$(NO_COLOR)"
	make $(NAME) && echo "$(GREEN)" $(NAME) -- Compilation complete ! "$(NO_COLOR)" || echo "$(RED)" $(NAME) -- Compilation Failed "$(NO_COLOR)"

$(NAME) : $(OBJS)
	echo "Linking $(NAME)"
	$(CC) $(LDFLAGS) $(LIBINCS) -o $@ $(OBJS) $(INCS) 

$(OBJ_PATH)%.o : $(SRC_PATH)%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPCFLAGS) $(LIBINCS) -c $< -o $@  $(INCS)

clean :
	echo "Cleaning objects and dependence files"
	rm -rf $(OBJ_PATH)

fclean : clean
	rm -rf $(NAME)

re : fclean 
	echo "Cleaning executable"
	make all

test : all
	./$(NAME)

vtest : all
	valgrind --leak-check=full ./$(NAME)

################################################################################
#####                              Flags                                   #####
################################################################################

sanitize:
	echo -n ""

-include $(DEPS)

.PHONY : all clean fclean re vtest
#.SILENT: