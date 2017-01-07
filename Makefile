.PHONY: all clean install
default:    all
all:
	scons debug=1
clean:
	scons -c
install:
	scons install
