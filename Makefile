all: test


JenkinsTray:
	cd src && qmake && make
	echo build/*/JenkinsTray

test: JenkinsTray
	cd test && qmake && make
	build/test/test

install: JenkinsTray
	cp build/*/JenkinsTray ~/bin/JenkinsTray
