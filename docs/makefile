# https://stackoverflow.com/questions/33847866/makefile-for-latex-aspell-with-multiple-files
MDFILES := $(shell find . -name "*.md")
.PHONY: ortografia formato clean

all: ortografia formato

%.spchk: %.md
	test `cat $< | aspell --mode=markdown --lang=en --list | aspell --mode=markdown --lang=es --home-dir=. --personal=dict.txt --list | tee /dev/stderr | wc -l` -eq 0

ortografia: $(addsuffix .spchk,$(basename $(MDFILES)))

formato:
	mdl $(MDFILES)

clean:
	rm -f *.aspell