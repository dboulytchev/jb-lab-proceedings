all:
	lualatex -shell-escape tex/main.tex
	lualatex -shell-escape tex/main.tex
#	xelatex -shell-escape tex/main.tex
#	xelatex -shell-escape tex/main.tex

clean:
	rm -rf *.aux *.toc *.out *.log
	rm -rf tex/*.aux

cleanPDF: clean
	rm -rf *.pdf