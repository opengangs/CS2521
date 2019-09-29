// myed.c - a very simple, line-based text editor
// Written by John Shepherd, March 2013
// Last modified, August 2014
//
// Documentation:
//
// Usage: myed FileName
// Opens FileName and reads it into internal doubly-linked list
// Gives error message if no such file or not readable
//
// Editor commands:
// . = show current line
// % = show all lines
// p = move to previous line and show it
// n = move to next line and show it
// NN = move to line number NN
// +NN = move forward by NN lines and show line
// -NN = move backward by NN lines and show line
// i = read new line and insert in front of current
// a = read new line and insert after current
// d = delete current line
// w = write out contents of file to file FileName.new
// q = quit from the editor

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "DLList.h"

// size of line buffers
#define MAX 8192

static int getCommand (char *s);
static void showCurrLine (DLList lines);
static void showHelp (void);

int main (int argc, char *argv[])
{
	// Check command-line args
	if (argc < 2)
		errx (EX_USAGE, "usage: %s <filename>", argv[0]);

	char fname[MAX];
	strcpy (fname, argv[1]);

	FILE *f;
	if ((f = fopen (fname, "r")) == NULL)
		err (EX_IOERR, "couldn't open '%s'", fname);
	DLList lines = getDLList (f);
	fclose (f);

	// Main loop
	bool done = false;
	char cmd[MAX];   // command typed by user
	while (!done && getCommand (cmd)) {
		switch (cmd[0]) {
		case '.':
			// show current line
			showCurrLine (lines);
			break;

		case '%':
			// show all lines
			putDLList (stdout, lines);
			break;

		case 'n':
			// move to next line
			DLListMove (lines, 1);
			showCurrLine (lines);
			break;

		case 'p':
			// move to previous line
			DLListMove (lines, -1);
			showCurrLine (lines);
			break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9': {
			// move to line NN
			int n = 0; // just in case ...
			sscanf (cmd, "%d", &n);
			DLListMoveTo (lines, n);
			showCurrLine (lines);
		} break;

		case '+': case '-': {
			// move forward/backward NN lines
			int n = 0;
			sscanf (cmd, "%d", &n);
			DLListMove (lines, n);
			showCurrLine (lines);
		} break;

		case 'i': {
			// read new line and insert in front of current
			char new[MAX] = {};
			fgets (new, MAX, stdin);
			new[strlen (new) - 1] = '\0';
			DLListBefore (lines, new);
		} break;

		case 'a': {
			// read new line and insert after current
			char new[MAX] = {};
			fgets (new, MAX, stdin);
			new[strlen (new) - 1] = '\0';
			DLListAfter (lines, new);
		} break;

		case 'd':
			// delete current line
			DLListDelete (lines);
			break;

		case 'w':
			// write lines to FileName.new
			strcat (fname, ".new");
			if ((f = fopen (fname, "w")) == NULL)
				fprintf (stderr, "Can't write %s\n", fname);
			else {
				putDLList (f, lines);
				fclose (f);
			}
			break;

		case '?':
			showHelp ();
			break;

		case 'q':
			done = true;
			break;
		}
	}

	// Finish up cleanly
	freeDLList (lines);

	return EXIT_SUCCESS;
}

// getCommand(buf)
// prompt for and read next user command
// store it in buf
// return 1 if got a command, 0 if EOF
static int getCommand (char *buf)
{
	printf ("> ");
	return (fgets (buf, MAX, stdin) != NULL);
}

// showCurrLine(lines)
static void showCurrLine (DLList lines)
{
	printf ("%s", DLListCurrent (lines));
	printf ("\n");
}

// giveHelp()
// show help message
static void showHelp (void)
{
	puts (
		"Editor commands:\n"
		"    . = show current line\n"
		"    % = show all lines\n"
		"    p = move to previous line and show it\n"
		"    n = move to next line and show it\n"
		"    NN = move to line number NN\n"
		"    +NN = move forward by NN lines and show line\n"
		"    -NN = move backward by NN lines and show line\n"
		"    i = read new line and insert in front of current\n"
		"    a = read new line and insert after current\n"
		"    d = delete current line\n"
		"    w = write out contents of file to file FileName.new\n"
		"    ? = show this help message\n"
		"    q = quit from the editor\n"
	);
}
