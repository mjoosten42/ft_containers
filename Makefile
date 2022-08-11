NAME = ft_containers
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra -Wpedantic -MMD -MP

CXXFLAGS += -I/Users/mjoosten/.brew/opt/llvm/include
LDFLAGS = -L/Users/mjoosten/.brew/opt/llvm/lib

#ifdef DEBUG
	CXXFLAGS += -g
	LDFLAGS += -g
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

std: fclean
std: CXXFLAGS += -D STD
std: all

.PHONY = clean fclean re std

-include $(DEP)

#LDFLAGS="-L/Users/mjoosten/.brew/opt/llvm/lib -Wl,-rpath,/Users/mjoosten/.brew/opt/llvm/lib"
# echo 'export PATH="/Users/mjoosten/.brew/opt/llvm/bin:$PATH"' >> ~/.zshrc