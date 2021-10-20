hellomake: db.c
	@gcc -std=c17 -O3 -march=native -o db db.c -I. -ggdb

.PHONY: clean

clean:
	@rm db
