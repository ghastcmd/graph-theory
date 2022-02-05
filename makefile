dijkstra = dijkstra
ARGS = 

d_dijkstra:
	$(MAKE) -C dijkstra $(ARGS)

run_dijkstra:
	$(MAKE) -C dijkstra run

# dijkstra run:
# 	$(MAKE) -C dijkstra run