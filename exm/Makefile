BIN= paje_example paje_example2 gtg_color otf_example

all: $(BIN)
paje_example: paje_example.c
	gcc -Wall -W -o paje_example paje_example.c $(CFLAGS) $(LDFLAGS) -lgtg -lpthread

paje_example2: paje_example2.c
	gcc -Wall -W -o paje_example2 paje_example2.c $(CFLAGS) $(LDFLAGS) -lgtg -lpthread

gtg_color: gtg_color.c
	gcc -Wall -W -o gtg_color gtg_color.c $(CFLAGS) $(LDFLAGS) -lgtg -lpthread

otf_example: otf_example.c
	gcc -Wall -W -o otf_example otf_example.c $(CFLAGS) $(LDFLAGS) -lgtg -lpthread -lotf

clean:
	rm -f $(BIN)

cleantraces:
	rm -f *def *marker *otf *events *trace *ept