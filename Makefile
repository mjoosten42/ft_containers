NAME = ft_containers
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra -Wpedantic -MMD -MP

#ifdef DEBUG
	CXXFLAGS += -g
	LDFLAGS += -g
#endif

INC = -I include

SRCDIR = src
SRC = $(shell find $(SRCDIR) -name '*.cpp' -exec basename {} \;)

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

std: fclean
std: CXXFLAGS += -D STD
std: all

.PHONY = clean fclean re std

-include $(DEP)