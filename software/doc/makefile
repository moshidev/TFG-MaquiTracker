ifeq ($(OS), Windows_NT)
	RM = del /Q
endif

.PHONY: all clean

all: $(patsubst %.md,%.pdf,$(wildcard *.md))

%.pdf: %.md
	pandoc -o $@ $<

clean:
	$(RM) $(patsubst %.md,%.pdf,$(wildcard *.md))
