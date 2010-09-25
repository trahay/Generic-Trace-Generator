BIN= paje_example gtg_color

all: $(BIN)
paje_example: paje_example.c
	gcc -o paje_example paje_example.c $(CFLAGS) $(LDFLAGS) -lgtg

gtg_color: gtg_color.c
	gcc -o gtg_color gtg_color.c $(CFLAGS) $(LDFLAGS) -lgtg

clean:
	rm -f $(BIN)