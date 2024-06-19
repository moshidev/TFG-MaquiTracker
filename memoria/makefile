PDFLATEX := pdflatex -halt-on-error

# https://stackoverflow.com/questions/33847866/makefile-for-latex-aspell-with-multiple-files
TEXFILES := $(shell find . -name "*.tex")
.PHONY: clean ortografia

all: ortografia proyecto.pdf

%.pdf: %.tex $(TEXFILES)
	$(PDFLATEX) $< && bibtex $* && $(PDFLATEX) $< && $(PDFLATEX) $<

clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.fls *.fdb_latexmk *.lof *.lot *.synctex.gz *.pdf

%.spchk: %.tex
	test `cat $< | aspell --mode=tex --lang=en --list | aspell --mode=tex --lang=es --home-dir=. --personal=dict.txt --list | tee /dev/stderr | wc -l` -eq 0

ortografia: $(addsuffix .spchk,$(basename $(TEXFILES)))
