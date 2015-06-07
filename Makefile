FLAGS = "-Wall"

rtok: converter.o converter_tree.o gui.o
	gcc $(FLAGS) converter.o converter_tree.o gui.o -o rtok -lncursesw

converter.o: converter.c converter.h
	gcc $(FLAGS) -c converter.c
converter_tree.o: converter.h converter_tree.c converter_tree.h
	gcc $(FLAGS) -c converter_tree.c
gui.o: converter_tree.o converter.o gui.c
	gcc $(FLAGS) -c gui.c


