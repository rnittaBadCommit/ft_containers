NAME_FT=my_container
NAME_STD=std_container
SRCS=main.cpp
OBJDIR=./obj/
OBJS=$(SRCS:%.cpp=$(OBJDIR)%.o)
DEPS=$(SRCS:%.cpp=$(OBJDIR)%.d)
FT=1
CXX=c++
DEFAULT_CXXFLAGS=-g -D FT=$(FT) -Wall -Wextra -Werror -std=c++98 -pedantic -MMD
CXXFLAGS=$(DEFAULT_CXXFLAGS)

all: $(OBJDIR) $(NAME_FT)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME_FT): $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME_FT)

$(NAME_STD): $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME_STD)

clean:
	$(RM) $(OBJS)
	$(RM) $(SRCS:.cpp=.d)

fclean: clean
	$(RM)  $(NAME_FT) $(NAME_STD)

re: fclean $(NAME_FT)

ft: fclean $(NAME_FT)
	@echo
	@echo
	@echo "ft START"
	@echo
	@echo
	@./$(NAME_FT)

std: fclean set_std $(NAME_STD)
	@echo
	@echo
	@echo "ft START"
	@echo
	@echo
	@./$(NAME_STD)

test: set_sanitize_address

exec_ft: $(NAME_FT)
	./$(NAME_FT)

set_std:
	$(eval FT := 0)

set_sanitize_address:
	$(eval CXXFLAGS := $(DEFAULT_CXXFLAGS) -fsanitize=address)

set_sanitize_leaks:
	$(eval CXXFLAGS :== $(DEFAULT_CXXFLAGS) -fsanitize=leak)

# -include $(DEPS)

.PHONY: clean fclean re set_test ft std test set_std set_sanitize_address set_sanitize_leaks

