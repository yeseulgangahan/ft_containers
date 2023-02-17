# SIMPLE =======================================================================
NAME			= ft_container_tester
# ==============================================================================

# SRCS =========================================================================
SOURCES_FOLDER	= ./testers/

SOURCES			=	main.cpp \
					tester.cpp \
					tester_vector.cpp \
					tester_stack.cpp \
					tester_map.cpp \
					
# ==============================================================================

# INCLUDES =====================================================================
INCLUDE 		= ./testers/tester.hpp
# ==============================================================================

# FLAGS ========================================================================
FLAGS 			= -Wall -Wextra -Werror --std=c++98
# ==============================================================================

# OBJECTS ======================================================================
OBJECTS_FOLDER 	= ./testers/objects/

OBJECT			= $(SOURCES:.cpp=.o)
OBJECTS		 	= $(addprefix $(OBJECTS_FOLDER), $(OBJECT));
# ==============================================================================

# TESTER =======================================================================

$(OBJECTS_FOLDER)%.o :	$(SOURCES_FOLDER)%.cpp
	@mkdir -p	$(OBJECTS_FOLDER)
	@echo "Compiling: $<"
	@c++ $(FLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	@c++ $(FLAGS) -o $(NAME) $(OBJECTS)

all: $(NAME)

clean:
	@echo "Cleaning: $(OBJECTS_FOLDER)"
	@rm -rf $(OBJECTS_FOLDER)
	@echo "Cleaning: ./tester/vectors_output"
	@rm -rf ./tester/vectors_output
	@echo "Cleaning: ./tester/lists_output"
	@rm -rf ./tester/lists_output
	@echo "Cleaning: ./tester/stacks_output"
	@rm -rf ./tester/stacks_output
	@echo "Cleaning: ./tester/queues_output"
	@rm -rf ./tester/queues_output
	@echo "Cleaning: ./tester/deques_output"
	@rm -rf ./tester/deques_output
	@echo "Cleaning: ./tester/maps_output"
	@rm -rf ./tester/maps_output

fclean: clean
	@echo "Cleaning: $(NAME)"
	@rm -f $(NAME)

re: fclean all
# ==============================================================================

# SUP ==========================================================================

run: $(NAME)
	@./$(NAME)

# ==============================================================================