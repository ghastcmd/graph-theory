ifeq ($(OS), Windows_NT)
CC = clang++
name = prim.exe
else
CC = g++
name = prim.out
endif

RUN_ARGS = -f ../input_w.dat -i 1 -s

$(name): main.cpp
	$(CC) $< -o $(name)

run: $(name)
	./$(name) $(RUN_ARGS)

clean:
	del prim.*