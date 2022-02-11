CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = aes.o usage.o
TARGET = aestest

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES)
