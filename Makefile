CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS =
OBJFILES = aes.o attack.o usage.o
TARGET = aestest

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES)
