ifeq ($(OS), Windows_NT)
CC = clang++
name = dijkstra.exe
else
CC = g++
name = dijkstra.out
endif

$(name): main.cpp
	$(CC) $< -o $(name)

run: $(name)
	./$(name)

clean:
	del dijkstra.exe
	del dijkstra

pipe: $(name)
	./dijkstra.out < input_w.dat