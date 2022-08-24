NAME = ft_containers
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra -Wpedantic -MMD

CXXFLAGS += -I/Users/mjoosten/.brew/opt/llvm/include
LDFLAGS = -L/Users/mjoosten/.brew/opt/llvm/lib

#ifdef DEBUG
	CXXFLAGS += -g
	LDFLAGS += -g
#endif

INC = -I include

SRCDIR = src
SRC = vectorTest.cpp

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

std: fclean
std: CXXFLAGS += -D STD
std: all

diff:
	make std
	./$(NAME) > std 2>&1
	make re
	./$(NAME) > ft 2>&1
	diff ft std > diff.log || true
	$(RM) ft std

.PHONY = clean fclean re run std diff

-include $(DEP)
