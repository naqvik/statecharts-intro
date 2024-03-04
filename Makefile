# build 3-speed fan controller

main : main.c
	$(CC) $(CFLAGS) -o $@ $<

mostlyclean :
	rm -f main
clean : mostlyclean
	@echo "Cleaning build"
