########################################

CHECKPOINT=Checkpoint
REPORT=Report
LATEXMK=latexmk
OUTDIR=\

LATEXMKRC=.latexmkrc

LATEXMKFLAGS=\
	--pdf\
	--shell-escape\
	-synctex=1\
	-interaction=nonstopmode\
	-file-line-error\
	-outdir=$(OUTDIR)

LATEXMKCLEANFLAGS=\
	-outdir=$(OUTDIR)\
	-c

.PHONY: all

all: report checkpoint 

report: $(REPORT).pdf
checkpoint: $(CHECKPOINT).pdf

# Starts a server that watches for changes in the tex files and recompiles
pvc:
	$(LATEXMK) $(LATEXMKFLAGS) -pvc $(ROOT_FILE).tex

%.pdf: %.tex
	$(LATEXMK) $(LATEXMKFLAGS) $<

# filters out pdf from the clean (in case you want them there ☺ )
TO_CLEAN = $(filter-out %.pdf, $(wildcard $(OUTDIR)/*))
# clean:
# 	$(RM) -rf $(TO_CLEAN)

cleanpdf:
	$(RM) *.pdf
