# build 3-speed fan controller

CFLAGS := -Wall -Wextra -pedantic

main : main.c
	$(CC) $(CFLAGS) -o $@ $<
test-main : main.c
	$(CC) $(CFLAGS) -DTEST -o $@ $<  && ./$@

mostlyclean :
	rm -f main test-main
clean : mostlyclean
	@echo "Cleaning build"
