all:
	cd src && qmake && make
	cd test && qmake && make
	build/test/test
	echo build/*/JenkinsTray

install:
	cp build/*/JenkinsTray ~/bin/JenkinsTray
