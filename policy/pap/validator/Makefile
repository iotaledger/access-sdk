ifdef V
AT_0 := @
AT_1 :=
AT = $(AT_$(V))
else
AT = @
endif

SOURCES =	validate.c \
			validator.c \
			optimizer.c \
			parser.c \
			jsmn.c

validate: validate.o validator.o optimizer.o parser.o jsmn.o
	@echo LD validate
	$(AT)$(CC) -o validate $^ $(LDFLAGS)


%.o: %.c
	@echo CC $@
	$(AT)$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o validate

.PHONY: clean
