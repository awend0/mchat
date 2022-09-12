NAME	=	mchat
SRCS	=	main.cpp \
			common/Network.cpp \
			common/Payload.cpp \
			protocol/Packet.cpp \
			protocol/Generator.cpp \

FLAGS	=	-std=c++17 -Wall -Wextra -Werror -O3 -I. -lpthread -lPocoFoundation -lPocoUtil -lPocoNet -lz
OBJS	=	$(SRCS:cpp=o)

all: $(NAME)

$(NAME): $(OBJS)
		g++ $(SRCS) -o $(NAME) $(FLAGS) 

%.o: %.cpp
		g++ $(FLAGS) -c $< -o $@

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all