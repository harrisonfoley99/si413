def: pat1 pat2

pat1:
	bison -v -d pat.ypp
	flex -o pat.yy.c pat.lpp
	g++ -g -o pat pat.yy.c pat.tab.cpp sym.cpp

pat2:
	bison -v -d pati.ypp
	flex -o pati.yy.c pati.lpp
	g++ -g -o pati pati.yy.c pati.tab.cpp helper.cpp

clean:
	rm -f *.yy.c pat pati pat*.output pat*.tab.* *~ tmp.txt tmp.ps



