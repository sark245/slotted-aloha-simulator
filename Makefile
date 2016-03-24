# Compiler to use.
CC=gcc

# Flags to use for compilation.
CFLAGS=-c -Wall -Wextra -O2

# Flags to use for linking.
LDFLAGS=

# Directory for files generated during building.
BUILD_DIR = ./build

# Name of executable target.
EXECUTABLE=aloha

# Sources to be compiled.
SOURCES=aloha.c file_output.c


OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJECTS:%.o=%.d)

all: $(EXECUTABLE)


# Creates build directories with same strucuture as sources.
# Links all the object files.
$(EXECUTABLE): $(OBJECTS) 
	@mkdir -p $(@D)
	@echo ":: Linking executable: " $@
	$(CC) $(LDFLAGS) $^ -o $@


# Includes all .d files.
# Note: these are not available during first build,
# but it is not necessary since everything is built anyway.
-include $(DEPS)



# The -MMD flags additionaly creates a .d file with
# the same name as the .o file.
$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(@D)
	@echo ":: Generating" $@
	$(CC) $(CFLAGS) -MMD $< -o $@



.PHONY: clean experiment
clean:
	-rm -rf $(DEPS) $(OBJECTS) $(EXECUTABLE)

experiment: $(EXECUTABLE)
	bash experiments/p_vs_delays.sh
	gnuplot experiments/plot_p_vs_delays.gnuplot -p
