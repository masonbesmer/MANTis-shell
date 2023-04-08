#=================================================|
#== A C Makefile using wildcard substitution      |
#== Project should be segregated: .h in ./        |
#== and .c in ./  , then make does the rest.      |
#== by Nathan Jodoin || @corigne                  |
#=================================================|
EXE = newshell
CC = gcc
FLAGS =  -c -Wall
LNKFLAGS = -w
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
#=======================

$(EXE): $(OBJS)
	@echo Linking primary executable...
	$(CC) $(LNKFLAGS) $^ -o $@
	@echo The make completed successfully. Execute command ./$(EXE) to run!

$(OBJS): %.o : %.c
	$(CC) $(FLAGS) $< -o $@
	@echo Compiling object $@...

###############################
.PHONY: clean all prep
clean:
	@rm -df $(EXE) *.o; echo All cleaned up!
