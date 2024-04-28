DOC := proyecto.tex
PDFLATEX := pdflatex -halt-on-error

# https://stackoverflow.com/questions/33847866/makefile-for-latex-aspell-with-multiple-files
TEXFILES := $(shell find . -name "*.tex")
.PHONY: comprueba-ortografia

all: doc

doc: $(DOC:.tex=.pdf)

all: proyecto.pdf

%.pdf: %.tex FORCE
	$(PDFLATEX) $< && bibtex $* && $(PDFLATEX) $< && $(PDFLATEX) $<

clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.fls *.fdb_latexmk *.lof *.lot *.synctex.gz *.pdf

%.spchk: %.tex
	test `cat $< | aspell --mode=tex --lang=en --list | aspell --mode=tex --lang=es --home-dir=. --personal=dict.txt --list | wc -l` -eq 0

comprueba-ortografia: $(addsuffix .spchk,$(basename $(TEXFILES)))

FORCE: 
