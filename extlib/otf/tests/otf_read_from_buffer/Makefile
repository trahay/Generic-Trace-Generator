
.PHONY: all
all: otf_read_from_buffer

otf_read_from_buffer: otf_read_from_buffer.c
	gcc -o otf_read_from_buffer otf_read_from_buffer.c `otfconfig --includes` `otfconfig --libs` 

clean:
	rm -f otf_read_from_buffer 
