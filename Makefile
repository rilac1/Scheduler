CC = gcc
INC = -I${CURDIR}/include/
CFLAGS = -g $(INC)

OBJS = sched.o sched_test.o 

SRCS = $(OBJS:.o=.c)

TARGET = sched

.SUFFIXES : .c .o

.c.o:
	@echo "Compilingi scheduler simulator $< ..."
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

all : $(TARGET)

dep : 
	gccmaedep $(INC) $(SRCS)

clean :
	@echo "Cleaning scheduler simulator $< ..."
	rm -rf $(OBJS) $(TARGET) 

new :
	$(MAKE) clean
	$(MAKE)
