DOC := proyecto.tex
PDFLATEX := pdflatex -halt-on-error

all: doc

doc: $(DOC:.tex=.pdf)

all: proyecto.pdf

%.pdf: %.tex FORCE
	$(PDFLATEX) $< && bibtex $* && $(PDFLATEX) $< && $(PDFLATEX) $<

clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.fls *.fdb_latexmk *.lof *.lot *.synctex.gz *.pdf

FORCE: 
