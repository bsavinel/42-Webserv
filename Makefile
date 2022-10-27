NAME = Webserv

################################################################################
#                           COMPILATION SETTING                        	       #
################################################################################

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address
CPPFLAGS	= -MMD

ifeq (sanitize, $(filter sanitize, $(MAKECMDGOALS)))
	CPPFLAGS += -fsanitize=address
endif

################################################################################
#                               SOURCE FILES                              	   #
################################################################################

SRCS =	Config/Config.cpp					\
		Config/Location.cpp					\
		Config/Server.cpp					\
		Event/ClientEvent.cpp				\
		Event/ServeurEvent.cpp				\
		HTTP/HttpManager/HttpM_Delete.cpp	\
		HTTP/HttpManager/HttpM_Get.cpp		\
		HTTP/HttpManager/HttpM_GetSet.cpp	\
		HTTP/HttpManager/HttpM_Post.cpp		\
		HTTP/HttpManager/HttpManager.cpp	\
		HTTP/HttpManager/autoIndex.cpp		\
		HTTP/HttpRequest.cpp				\
		Utils/builder.cpp					\
		Utils/Error.cpp						\
		Utils/exceptWebserv.cpp				\
		Utils/utils.cpp						\
		Utils/headerRespond.cpp				\
		Utils/ft_itoa.cpp					\
		Epoll.cpp							\
		launcher.cpp						\
		Cgi.cpp								\
		main.cpp

################################################################################
#                               INCLUDES                             	       #
################################################################################

INCS			=	-I ./includes/				\
					-I ./includes/Config		\
					-I ./includes/HTTP			\
					-I ./includes/LoopManage	\
					-I ./includes/Utils

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
DEPS			=	${OBJS:.o=.d}

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
	$(CXX) $(CXXFLAGS) $(LIBINCS) -o $@ $(OBJS) $(INCS) 

$(OBJ_PATH)%.o : $(SRC_PATH)%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LIBINCS) -c $< -o $@  $(INCS)

clean :
	echo "Cleaning objects and dependence files"
	rm -rf $(OBJ_PATH)

fclean : clean
	rm -rf $(NAME)

re : fclean 
	echo "Cleaning executable"
	make all

################################################################################
#####                              Flags                                   #####
################################################################################

sanitize:
	echo -n ""

-include $(DEPS)

.PHONY : all clean fclean re

.SILENT: