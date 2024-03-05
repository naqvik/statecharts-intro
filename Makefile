# build 3-speed fan controller

CFLAGS := -Wall -Wextra -pedantic

main : main.c
	$(CC) $(CFLAGS) -o $@ $<

mostlyclean :
	rm -f main
clean : mostlyclean
	@echo "Cleaning build"
