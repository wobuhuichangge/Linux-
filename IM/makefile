cc=g++
include=-Imongoose
IMserver:IMserver.cc mongoose.c
	$(cc) -o $@ $^  $(include) 
.PHONY:clean
clean:
	rm -f IMserver
