build: hangman

hangman: hangman.c
	gcc -Wall hangman.c -o hangman -lncurses

clean:
	rm hangman
