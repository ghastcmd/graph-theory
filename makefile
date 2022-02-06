dijkstra = dijkstra
ARGS = 

d_dijkstra:
	$(MAKE) -C dijkstra $(ARGS)

d_prim:
	$(MAKE) -C prim $(ARGS)

d_bellman-ford:
	$(MAKE) -C bellman-ford $(ARGS)

d_ford-fulkerson:
	$(MAKE) -C bellman-ford $(ARGS)