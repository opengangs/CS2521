#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "textbuffer.h"



typedef struct Line {
    char *text;
    struct Line *prev;
    struct Line *next;
} Line;

typedef struct textbuffer {
    int nitems;
    Line *first;
    Line *last;
    Line *curr;
} textbuffer;


static TB duplicate (TB tb) {

    char *dup = dumpTB (tb, false);
    TB new = newTB (dup);
    free (dup);
    
    return new;
}
/** Acts like the nonstandard function, strdup() --> duplicates string */
/* Originally used: https://stackoverflow.com/a/20712069 
static char *strdup(const char *orig) {
   size_t bytes = strlen(orig) + 1;
   char *copy = malloc(bytes);
   if (copy != 0)
     memcpy(copy, orig, bytes);
   return copy;
}
*/

/** Given an array of text, split the array into chunks and return the string */
static char *stringSplit (char *text) {
    if (text == NULL) return NULL;
    
    static int i = 0;
    int tmp = i;
    int nitems = 0;
    
    char arr = text[i++];
    while (arr != '\0') {
        nitems++;
        if (arr == '\n') break;
        
        arr = text[i++];
    }
    
    // check empty string.
    if (nitems == 0) {
        i = 0;
        return NULL;
    }
    
    // dynamically allocate array.
    char *string = calloc (1, sizeof (char) * nitems);
    if (string == NULL) abort();
    
    for (int j = 0; j < nitems - 1; j++) {
        string[j] = text[tmp++];
    }
    
    string[nitems - 1] = '\0';
    return string;
}

static int countInt (int num) {
    int count = 0;
    while (num != 0) {
        num /= 10;
        ++count;
    }
    
    return count;
}

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB(char *text) {
    if (text == NULL) return NULL;
    
    TB new = calloc (1, sizeof (textbuffer));
    if (new == NULL) abort();
    
    char *tokenise = stringSplit (text);
    Line *prev, *curr;
    prev = curr = NULL;
    
    while (tokenise != NULL) {
        // dynamically allocate new memory for current pointer.
        curr = calloc (1, sizeof (Line));
        if (curr == NULL) abort();
        
        curr->text = tokenise;
        if (prev == NULL) prev = curr;
        else {
            curr->prev = prev;
            prev->next = curr;
            prev = curr;
        }
        
        new->nitems++;
        
        if (new->first == NULL) new->first = curr;
        
        tokenise = stringSplit (text);
    }
    
    new->last = curr;
    
    return new;
}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB(TB tb) {
    if (tb == NULL) return;
    
    Line *curr = tb->first;
    while (curr != NULL) {
        Line *next = curr->next;
        free (curr->text);
        free (curr);
        
        curr = next;
    }
    
    free (tb);
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB(TB tb, bool showLineNumbers) {
	if (tb == NULL) return NULL;
	
	int lineNumber = 1;
	Line *curr;
	int size = 0;
	for (curr = tb->first; curr != NULL; curr = curr->next) {
	    size = size + (strlen (curr->text)) + 1;
	}
	
	char *new = calloc (1, sizeof (textbuffer)*size + 1);
	if (new == NULL) abort();
	
	curr = tb->first;
	int count = 0;

    if (showLineNumbers == true) {
        count = count + 3;
        for (curr = tb->first; curr != NULL; curr = curr->next) {
            char *strInt = calloc (1, sizeof(char)*countInt (lineNumber) + 3);
            sprintf (strInt, "%d. ", lineNumber++);
            strcat (new, strInt);
            free (strInt);
            for (int i = 0; i < strlen (curr->text); i++) {
                new[count++] = curr->text[i];
            }
            new[count++] = '\n';
            count = count + countInt (lineNumber) + 2;
        }
    }
    
    else {
	    for (curr = tb->first; curr != NULL; curr = curr->next) {
	        for (int i = 0; i < strlen (curr->text); i++) {
	          new[count++] = curr->text[i];
	        }
	    
	        new[count++] = '\n';
	    }
    }
	
	return new;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB(TB tb) {
	if (tb == NULL) return 0;
	return tb->nitems;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB (TB tb, int from, int to, char *prefix) {

    // check valid inputs
    if (from > to) abort();
    if (from < 1 || to > tb->nitems) {
        printf ("addPrefixTB: Out of range!\n");
    }
    
    if (tb == NULL) abort();
    
    int count = 1;
    Line *curr = tb->first;
    
    // move to text with line number from.
    while (count < from) {
        curr = curr->next;
        count++;
    }
    
    while (count < to + 1) {
        int newLength = strlen (curr->text) + strlen (prefix) + 1; // returns length of new string.
        char *string = malloc (sizeof (char)*newLength);
        strcpy (string, prefix); // string = prefix
        strcat (string, curr->text); // string = prefix + curr->text.
        
        // free to avoid memory leaks.
        free (curr->text); // set curr->text == NULL;
        curr->text = string;
        
        curr = curr->next;
        count++;
    }
}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB(TB tb1, int pos, TB tb2) {

    // check valid inputs
    if (tb1 == NULL || tb2 == NULL) return; // don't do anything xd
    if (pos > tb1->nitems + 1 || pos <= 0) {
        printf ("mergeTB: Out of range!\n");
        abort();
    }
    
    if (tb2->nitems == 0) {
        return;
    }
    Line *tb1Curr = tb1->first;
    Line *tb2Curr = tb2->first;
    
    int position = 1;
    while (position < pos - 1) {
        tb1Curr = tb1Curr->next;
        position++;
    }
    
    Line *tmp;
    
    if (pos == 1) {
        tmp = tb1->first;
        tb1->first = tb2Curr;
    }
    
    else {
        tmp = tb1Curr->next;
        tb1Curr->next = tb2Curr;
    }
    
    // position is now equal to pos and tb1Curr is located at the line of number pos.
    // start merge.
    while (tb2Curr->next != NULL) {
        // iterate through tb2.
        tb2Curr = tb2Curr->next;
    }
    
    tb2Curr->next = tmp;
    tb1->nitems += tb2->nitems;
    
    free (tb2);
}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB(TB tb1, int pos, TB tb2) {
    mergeTB (tb1, pos, duplicate (tb2));
}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB(TB tb, int from, int to) {
	
	// check valid inputs
	if (from > to) abort();
	if (from > linesTB (tb) || to < 1) {
	    printf ("cutTB: Out of range!\n");
	}
	
	Line *tbFrom = tb->first;
	Line *tbTo = tb->first;
	int pos = 1;
	
	// find text with tbFrom.
	while (pos != from) {
	    tbFrom = tbFrom->next;
	    pos++;
	}
	
	pos = 1;
	
	// find text with tbTo.
	while (pos != to) {
	    tbTo = tbTo->next;
	    pos++;
	}
	
	// tbFrom contains text in line number from.
	// tbTo contains text in line number to.
	
	// join strings.
	if (tbFrom->prev != NULL) {
	    tbFrom->prev->next = tbTo->next;
	}
	
	if (tbTo->next != NULL) {
	    tbTo->next->prev = tbFrom->prev;
	}
	
	// join strings from end.
	if (from == 1) {
	    tb->first = tbTo->next;
	}
	
	if (to == linesTB (tb)) {
	    tb->last = tbFrom->prev;
	}
	
	// create a new TB and return.
	TB new = newTB ("");
	new->first = tbFrom;
	new->last = tbTo;
	
	tbFrom->prev = tbTo->next = NULL;
	
	new->nitems = (to - from + 1);
	tb->nitems = tb->nitems - (to - from + 1);
	
	return new;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search) {
    
    if (search == NULL) abort();
    if (strcmp (search, "") == 0) return NULL;
    if (tb->nitems == 0) return NULL;
    
    Match node = malloc (sizeof (matchNode));
    Match head = node; // pointer to head node.
    node->next = NULL;
    
    Line *curr = tb->first;
    
    int first = 0;
    int row = 1;
    
    int length = strlen (search);
    
    // iterate through each row.
    while (curr != NULL) {
    
        char *index = strstr (curr->text, search);
        
        if (!first) {
            if (index != NULL) {
                first++;
                
                // found a substring.
                node->lineNumber = row;
                
                // initialise first instance.
                node->columnNumber = (index - curr->text);
                node->columnNumber = node->columnNumber + 1; // + 1 since index starts at 1.
                node->next = NULL;
                
                // find next instance.
                index = strstr (curr->text + node->columnNumber + length, search);
            }
        }
        
        // assume we found the head.
        while (index != NULL) {
            // found new instance.
            node->next = malloc (sizeof (matchNode));
            
            // initialise node.
            node->next->lineNumber = row;
            node->next->columnNumber = (index - curr->text);
            node->next->columnNumber = node->next->columnNumber + 1;
            node = node->next;
            
            node->next = NULL;
            
            index = strstr (curr->text + node->columnNumber + length, search);
        }
        row++;
        curr = curr->next;
    }
    
    if (!first) {
        // malloc'd but unused.
        free (head);
        head = NULL;
    }
    
    return head;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB(TB tb, int from, int to) {

    if (from > to) abort();
	if (from > linesTB (tb) || to < 1) {
	    printf ("deleteTB: Out of range!\n");
	}
	
	Line *tbFrom = tb->first;
	Line *tbTo = tb->first;
	int pos = 1;
	
	// find text with tbFrom.
	while (pos != from) {
	    tbFrom = tbFrom->next;
	    pos++;
	}
	
	pos = 1;
	
	// find text with tbTo.
	while (pos != to) {
	    tbTo = tbTo->next;
	    pos++;
	}
	
	// tbFrom contains text in line number from.
	// tbTo contains text in line number to.
	
	// join strings.
	if (tbFrom->prev != NULL) {
	    tbFrom->prev->next = tbTo->next;
	}
	
	if (tbTo->next != NULL) {
	    tbTo->next->prev = tbFrom->prev;
	}
	
	// join strings from end.
	if (from == 1) {
	    tb->first = tbTo->next;
	}
	
	if (to == linesTB (tb)) {
	    tb->last = tbFrom->prev;
	}
	
	Line *curr = tbFrom;
	Line *end = tbTo->next;
	
	while (curr != end) {

        Line *tmp = curr->next;
        free (curr->text);
        free (curr);

        curr = tmp;
	}
	
	tb->nitems = tb->nitems - (to - from + 1);
}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText(TB tb) {

    // i'm really ceebs to do this and the bonus challenges :((
    // pls hELP

}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB(TB tb1, TB tb2) {
	return NULL;
}

void undoTB(TB tb) {

}

void redoTB(TB tb) {

}
