CC		= g++
CFLAGS  = -g -Wall
TARGET  = main
SRCDIR  = ./src
SRCS    = $(SRCDIR)/main.cpp $(SRCDIR)/proc.cpp
OBJS    = $(SRCS:.cpp=.o)
BUILDDIR= ./obj
INCDIR  = -I./inc
LIBDIR  = 
LIBS    = 

# ターゲットファイル生成
$(BUILDDIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)
	mv $(SRCDIR)/*.o $(BUILDDIR)
	
# オブジェクトファイル生成
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(SRCS)
	mv *.o $(SRCDIR)

all: clean mkdir $(BUILDDIR)/$(TARGET)

clean:
	-rm -f $(BUILDDIR)/* $(BUILDDIR)/*.d $(SRCDIR)/%.o

mkdir:
	mkdir -p $(BUILDDIR)
	mkdir -p ./data

.PHONY: all clean mkdir