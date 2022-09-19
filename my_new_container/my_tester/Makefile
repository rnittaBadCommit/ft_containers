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

$(NAME_STD): fclean set_std $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME_STD)

clean:
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(SRCS:.cpp=.d)

fclean: clean
	$(RM) $(NAME_FT) $(NAME_STD)
	rm -rf $(OBJDIR)
	$(RM) ft_result std_result result_diff fsanitize_result

re: fclean $(NAME_FT)

ft: fclean $(NAME_FT)
	@rm -f ft_result
	@echo
	@echo
	@echo "=====================  ft START  ====================="
	@echo
	./$(NAME_FT) 42 | tee -a ft_result
	./$(NAME_FT) 43 | tee -a ft_result
	./$(NAME_FT) 44 | tee -a ft_result
	./$(NAME_FT) 45 | tee -a ft_result
	@echo
	@echo "=====================  ft END  ====================="
	@echo
	@echo


std: fclean $(NAME_STD)
	@rm -f std_result
	@echo
	@echo
	@echo "=====================  std START  ====================="
	@echo
	./$(NAME_STD) 42 | tee -a std_result
	./$(NAME_STD) 43 | tee -a std_result
	./$(NAME_STD) 44 | tee -a std_result
	./$(NAME_STD) 45 | tee -a std_result
	@echo
	@echo "=====================  std END  ====================="
	@echo
	@echo

test: ft

test: std

test:
	@echo
	@echo
	@echo "=====================  diff ft_result std_result ====================="
	@echo
	@diff ft_result std_result | tee result_diff
	@echo
	@echo "=====================  diff END  ====================="
	@echo
	@echo

test: set_sanitize_address re
	@echo
	@echo
	@echo "=====================  test -fsanitize=address START  ====================="
	@echo
	./$(NAME_FT) 42 2>&1 >/dev/null  | tee -a fsanitize_result
	./$(NAME_FT) 43 2>&1 >/dev/null  | tee -a fsanitize_result
	./$(NAME_FT) 44 2>&1 >/dev/null  | tee -a fsanitize_result
	./$(NAME_FT) 45 2>&1 >/dev/null  | tee -a fsanitize_result
	@echo
	@echo "=====================  test -fsanitize=address END  ====================="
	@echo
	@echo

set_sanitize_address:
	$(eval CXXFLAGS := $(DEFAULT_CXXFLAGS) -fsanitize=address)

ft_err: set_err $(NAME_FT)

std_err: set_err $(NAME_STD)

set_err:
	$(eval CXXFLAGS := $(DEFAULT_CXXFLAGS) -DERR=1)

set_std:
	$(eval FT := 0)

# -include $(DEPS)

.PHONY: all clean fclean re FORCE set_test ft std test set_std set_sanitize_address
