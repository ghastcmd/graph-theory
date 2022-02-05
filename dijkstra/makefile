ifeq ($(OS), Windows_NT)
CC = clang++
name = dijkstra.exe
else
CC = g++
name = dijkstra.out
endif

RUN_ARGS = -f ../input_w.dat -s

$(name): main.cpp
	$(CC) $< -o $(name)

run: $(name)
	./$(name) $(RUN_ARGS)

clean:
	del dijkstra.exe
	del dijkstra

pipe: $(name)
	./dijkstra.out < input_w.dat