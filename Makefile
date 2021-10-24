hellomake: db.c
	@clang -O3 -mcpu=apple-m1 --std=c17 -o db db.c -I.

.PHONY: clean

clean:
	@rm db
run:
	@make clean
	@make hellomake
	@./db
