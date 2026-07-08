compile:
	g++ solver.cpp

run: compile
	./a.exe

clean:
	del a.exe