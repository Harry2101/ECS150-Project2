# Target library
lib := libuthread.a
objects := queue.o

CC := gcc
CFLAGS := -Wall -MMD -Wextra #-Werror

Stat_lib := ar 
Flags := rcs
ifneq ($(V),1)
Q = @
endif

all: $(lib)

deps := $(patsubst %.o,%.d,$(objects))
-include $(deps)

%.o: %.c 
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(lib): $(objects)
	$(Q)$(Stat_lib) $(Flags) $@ $^

clean:
	$(Q)rm -rf $(lib) $(objects) $(deps)