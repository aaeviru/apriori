obj = apriori.o
CFLAGS =
CC = g++

all:apriori

apriori:$(obj)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean
clean :
	rm $(obj) apriori
