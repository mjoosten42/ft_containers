NAME := ft_containers
CXX := c++
CXXFLAGS = -Wall -Werror -Wextra -Wpedantic -MMD #-std=c++98

CXXFLAGS += -I/Users/mjoosten/.brew/opt/llvm/include
LDFLAGS = -L/Users/mjoosten/.brew/opt/llvm/lib

#ifdef DEBUG
	CXXFLAGS += -g -fsanitize=undefined
	LDFLAGS += -g -fsanitize=undefined
#endif

INC = -I include

SRCDIR = src
SRC = main.cpp

OBJDIR = obj
OBJ = $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))

DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(LDFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< $(INC) -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

run: all
	./$(NAME)

TESTFILES = map.cpp

test_build:
	$(CXX) -Wall -Werror -Wextra -Wpedantic tests/$(TESTFILES) $(INC) -o catch2

test_run:
	./catch2

test:
	$(CXX) -Wall -Werror -Wextra -Wpedantic tests/$(TESTFILES) $(INC) -o catch2 && ./catch2


.PHONY = clean fclean re run test

-include $(DEP)
