ifeq ($(OS), Windows_NT)
CC = clang++
name = ford-fulkerson.exe
else
CC = g++
name = ford-fulkerson.out
endif

RUN_ARGS = -f ../input_w.dat -i 1 -l 6 -s

$(name): main.cpp
	$(CC) $< -o $(name)

run: $(name)
	./$(name) $(RUN_ARGS)

clean:
	del ford-fulkerson.*