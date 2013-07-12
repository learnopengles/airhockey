CFLAGS = -I. -I../common -Wall -Wextra

SOURCES = main.c ../common/game.c
OBJECTS = main.o ../common/game.o
TARGET = airhockey.html

# Targets start here.
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $(OBJECTS)

clean:
	$(RM) $(TARGET) $(OBJECTS)

depend:
	@$(CC) $(CFLAGS) -MM $(SOURCES)
	
# list targets that do not create files (but not all makes understand .PHONY)
.PHONY:	all clean depend

# Dependences (call 'make depend' to generate); do not delete:
# Build for these is implicit, no need to specify compiler command lines.

main.o: main.c ../common/game.h
../common/game.o: ../common/game.c ../common/game.h glwrapper.h