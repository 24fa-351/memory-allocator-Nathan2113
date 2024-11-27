# Need to be able to make without "-DSYSTEM_MALLOC" and it works for both
all: memtest_sys memtest_dev

memtest_sys: main.c
	gcc -o memtest_sys -DSYSTEM_MALLOC main.c

memtest_dev: main.c malloc.c malloc.h
	gcc -o memtest_dev main.c malloc.c

clean:
	rm -f memtest_sys memtest_dev