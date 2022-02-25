dijkstra = dijkstra
ARGS = 

d_dijkstra:
	$(MAKE) -C dijkstra $(ARGS)

d_prim:
	$(MAKE) -C prim $(ARGS)

d_bellman-ford:
	$(MAKE) -C bellman-ford $(ARGS)

d_ford-fulkerson:
	$(MAKE) -C ford-fulkerson $(ARGS)

clean:
	$(MAKE) d_dijkstra ARGS=clean
	$(MAKE) d_prim ARGS=clean
	$(MAKE) d_bellman-ford ARGS=clean
	$(MAKE) d_ford-fulkerson ARGS=clean