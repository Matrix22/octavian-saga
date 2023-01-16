CC = g++
CFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lm

build: trial rise redemption

run_trial: trial
	./trial

run_rise: rise
	./rise

run_redemption: redemption
	./redemption

trial: trial.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

rise: rise.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

redemption: redemption.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

pack:
	zip -FSr 323CD_NegruMihai.zip *.cpp *.hpp *.h Makefile README

clean:
	rm -f trial rise redemption

.PHONY: pack build clean
