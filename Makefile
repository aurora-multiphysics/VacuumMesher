include Make.common

hello:
	@echo "${libmesh_LIBS}"

app:
	@mpic++ main.cpp -o lol ${libmesh_INCLUDE} ${libmesh_LIBS}