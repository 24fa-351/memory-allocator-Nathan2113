# Need to be able to make without "-DSYSTEM_MALLOC" and it works for both
all: memtest_sys memtest_dev

memtest_sys: main.c
	gcc -o memtest_sys -DSYSTEM_MALLOC main.c tests.c

memtest_dev: main.c malloc.c malloc.h
	gcc -o memtest_dev main.c malloc.c tests.c custom_malloc.c

clean:
	rm -f memtest_sys memtest_dev