NAME := ft_containers
CXX := c++
CXXFLAGS = -Wall -Werror -Wextra -Wpedantic

CXXFLAGS += -I/Users/mjoosten/.brew/opt/llvm/include
LDFLAGS = -L/Users/mjoosten/.brew/opt/llvm/lib

ifdef DEBUG
	CXXFLAGS += -g -fsanitize=address
	LDFLAGS += -g -fsanitize=address
endif

ifdef STD
	CXXFLAGS += -D STD
endif

INC = -I include

SRCDIR = src
SRC = subject.cpp

OBJDIR = obj
OBJ = $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))

DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(LDFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -std=c++98 -MMD -c $< $(INC) -o $@

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

CASE := map

test:
	$(CXX) $(CXXFLAGS) tests/$(CASE).cpp $(INC) -o catch2 && ./catch2

LIST = vector stack map set

testall:
	@for case in $(LIST) ; do make test CASE=$$case; done

bm:
	$(CXX) $(CXXFLAGS) benchmark/*.cpp $(INC) -D STD && ./a.out
	$(CXX) $(CXXFLAGS) benchmark/*.cpp $(INC) && ./a.out

.PHONY = clean fclean re run test testall bm

-include $(DEP)
