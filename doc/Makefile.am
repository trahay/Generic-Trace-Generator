# Copyright © CNRS, INRIA, Université Bordeaux 1
# See COPYING in top-level directory.

if USE_DOXYGEN

DOX_DIR = .
DOC_DIR = $(builddir)
DOX_CONFIG = $(DOX_DIR)/Doxyfile

DOX_MAN_DIR = $(DOC_DIR)/man
DOX_LATEX_DIR = $(DOC_DIR)/latex
DOX_PDF = $(DOC_DIR)/gtg_refmanual.pdf
DOX_HTML_DIR = $(DOC_DIR)/html/
DOX_TAG = $(DOC_DIR)/gtg.tag

# TODO: we should add source files so that the documentation is
# regenerated when a file changes
docx_inputs = $(DOX_CONFIG)

$(DOX_TAG): $(docx_inputs)
	doxygen $(DOX_CONFIG)
	touch $(DOX_TAG)

$(DOX_HTML_DIR): $(DOX_TAG)

$(DOX_PDF): $(DOX_TAG)
	cd $(DOX_LATEX_DIR) &&\
        rm -f *.aux *.toc *.idx *.ind *.ilg *.log *.out; \
        $(PDFLATEX) refman.tex; \
        $(MAKEINDEX) refman.idx; \
        $(PDFLATEX) refman.tex; \
        done=0; repeat=5; \
        while test $$done = 0 -a $$repeat -gt 0; do \
           if $(EGREP) 'Rerun (LaTeX|to get cross-references right)' refman.log > /dev/null 2>&1; then \
               $(PDFLATEX) refman.tex; \
               repeat=`expr $$repeat - 1`; \
           else \
               done=1; \
           fi; \
        done; \
	cd -; \
        mv $(DOX_LATEX_DIR)/refman.pdf $(DOX_PDF)

EXTRA_DIST = $(DOX_HTML_DIR)

if USE_PDFLATEX
EXTRA_DIST += $(DOX_PDF)
endif

# Crappy hack: automake cannot copy a directory, so we have to do it by hand.
install_html: $(DOX_TAG) $(DOX_HTML_DIR)/index.html
	mkdir -p $(DESTDIR)$(htmldir)
	cp -r $(DOX_HTML_DIR) $(DESTDIR)$(htmldir)

install_pdf: $(DOX_PDF)
	mkdir -p $(DESTDIR)$(pdfdir)
	cp -r $(DOX_PDF) $(DESTDIR)$(pdfdir)

if USE_PDFLATEX
install-data-am: install_pdf install_html
else
install-data-am: install_html
endif

include_HEADERS = Doxyfile
endif
