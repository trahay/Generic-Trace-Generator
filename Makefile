include config.in

OBJ	= $(OBJDIR)/GTGBasic1_c.o     \
	  $(OBJDIR)/GTGBasic1_f.o     \
	  $(OBJDIR)/paje_GTGBasic1_c.o \
	  $(OBJDIR)/interfaceCFortranBasic1.o    \
	  $(OBJDIR)/OTF_GTGBasic1_c.o

.PHONY	: doc lib

all	: dir doc examples

examples	: dir lib c_example f_example

dir	:
		mkdir -p $(DIR)

clean	:
		rm -rf *~ $(DIR) $(DOCDIR)	

lib	: dir $(OBJ)
		$(AR) $(AROPT) $(LIBDIR)/lib$(LIBNAME).a $(OBJ)

$(OBJDIR)/%.o	: $(CDIR)/%.c
		$(CC) $(CFLAGS) $(TRACE_FLAG) -I$(INCDIR)/ -c $<  -o $@

$(OBJDIR)/%.o	: $(FDIR)/%.f90
		$(FC) $(FFLAGS) $(MODDIR) -c $<  -o $@

doc	:
		mkdir -p $(DOCDIR)
		$(DOC_GEN) $(DOCFILE)

clean_exm	: 
		rm -rf $(BINDIR) *.trace *.ept

f_example	: fpaje

c_example	: cpaje cpaje2 cvite


cpaje	: $(EXMDIR)/paje_example.c
		@echo ''
		@echo '<--------------------------->'
		$(CC) $(CFLAGS) -I$(INCDIR) $< -o $(BINDIR)/$@ -L$(LIBDIR) $(LIBLINK)
		@echo 'Example paje C done.'
		@echo 'You can generate cpaje.trace with :'
		@echo './bin/cpaje'
cpaje2	: $(EXMDIR)/paje_example2.c
		@echo ''
		@echo '<--------------------------->'
		$(CC) $(CFLAGS) -I$(INCDIR) $< -o $(BINDIR)/$@ -L$(LIBDIR) $(LIBLINK)
		@echo 'Example paje2 C done.'
		@echo 'You can generate cpaje2.trace with :'
		@echo './bin/cpaje2'

fpaje	: $(EXMDIR)/paje_example.F90
		@echo ''
		@echo '<--------------------------->'
		$(FC) $(FFLAGS) -I$(INCDIR) $< -o $(BINDIR)/$@ -L$(LIBDIR) $(LIBLINK)
		@echo 'Example paje Fortran done.'
		@echo 'You can generate fpaje.trace with :'
		@echo './bin/fpaje'


cvite	: $(EXMDIR)/vite_example.c
		@echo ''
		@echo '<--------------------------->'
		$(CC) $(CFLAGS) -I$(INCDIR) $< -o $(BINDIR)/$@  -L$(LIBDIR) $(LIBLINK)
		@echo 'Example vite done.'
		@echo 'You can generate cvite_root.ept with :'
		@echo 'mpirun -n 6 ./bin/cvite'
