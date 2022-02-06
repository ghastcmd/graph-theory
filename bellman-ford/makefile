ifeq ($(OS), Windows_NT)
CC = clang++
name = bellman-ford.exe
else
CC = g++
name = bellman-ford.out
endif

RUN_ARGS = -f ../input_w.dat -i 1 -l 2

$(name): main.cpp
	$(CC) $< -o $(name)

run: $(name)
	./$(name) $(RUN_ARGS)

clean:
	del bellman-ford.*