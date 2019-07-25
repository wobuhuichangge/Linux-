INCLUDE= -I. -I./speech
LIB= -ljsoncpp -lcurl -lcrypto -lpthread
Eva:Eva.cc
	g++ -o $@ $^ $(LIB) -std=c++11 $(INCLUDE) #-static
.PHONY:clean
clean:
	rm -f Eva
