#include <stdio.h>
#include <stdlib.h>
 
enum Statetype {
    STATE_TEXT,
    STATE_MAYBE_COMMENT,     /* just saw '/' */
    STATE_COMMENT,
    STATE_MAYBE_END_COMMENT, /* in a comment, just saw '*' */
    STATE_STRING,
    STATE_STRING_ESCAPE,     /* in a string, just saw '\' */
    STATE_CHAR,
    STATE_CHAR_ESCAPE        /* in a char literal, just saw '\' */
};
 
enum Statetype handleInText(int c)
{
    enum Statetype state;
 
    if (c == '/') {
        state = STATE_MAYBE_COMMENT; /* don't print yet - might be a comment */
    } else if (c == '\'') {
        putchar(c);
        state = STATE_CHAR;
    } else if (c == '"') {
        putchar(c);
        state = STATE_STRING;
    } else {
        putchar(c);
        state = STATE_TEXT;
    }
    return state;
}
 
enum Statetype maybeInComment(int c)
{
    enum Statetype state;
 
    if (c == '*') {
	putchar(' '); /* close comment as a single space */
	state = STATE_COMMENT; /* the withheld '/' is dropped - it's a comment */
    } else if(c == '/'){
    	putchar('/');
	state = STATE_MAYBE_COMMENT;
    }else {
        putchar('/'); /* not a comment - emit the '/' we withheld, plus this char */
        state = handleInText(c);
    }
    return state;
}
 
enum Statetype handleComment(int c)
{
    enum Statetype state;
 
    if (c == '*') {
        state = STATE_MAYBE_END_COMMENT;
    } else if (c == '\n') {
        putchar(c); /* keep newlines so output line numbers still match */
        state = STATE_COMMENT;
    } else {
        state = STATE_COMMENT;
    }
    return state;
}
 
enum Statetype handleMaybeEndComment(int c)
{
    enum Statetype state;
 
    if (c == '/') {
        state = STATE_TEXT;
    } else if (c == '\n') {
        putchar('\n');
        state = STATE_COMMENT;
    } else if (c == '*') {
        state = STATE_MAYBE_END_COMMENT;
    } else {
        state = STATE_COMMENT;
    }
    return state;
}
 
enum Statetype handleInString(int c)
{
    enum Statetype state;
 
    if (c == '\\') {
        putchar('\\');
        state = STATE_STRING_ESCAPE;
    } else if (c == '"') {
        putchar('"');
        state = STATE_TEXT;
    } else {
        putchar(c);
        state = STATE_STRING;
    }
    return state;
}
 
enum Statetype handleInStringEscape(int c)
{
    enum Statetype state;
 
    putchar(c);
    state = STATE_STRING;
    return state;
}
 
enum Statetype handleInChar(int c)
{
    enum Statetype state;
 
    if (c == '\\') {
        putchar('\\');
        state = STATE_CHAR_ESCAPE;
    } else if (c == '\'') {
        putchar('\'');
        state = STATE_TEXT;
    } else {
        putchar(c);
        state = STATE_CHAR;
    }
    return state;
}
 
enum Statetype handleInCharEscape(int c)
{
    enum Statetype state;
 
    putchar(c);
    state = STATE_CHAR;
    return state;
}
 
int main(void)
{
    int c;
    enum Statetype state = STATE_TEXT;
    enum Statetype prevState = STATE_TEXT;
    int line = 1;
    int commentStartLine = 0;
 
    while ((c = getchar()) != EOF) {
        prevState = state;
 
        switch (state) {
        case STATE_TEXT:
            state = handleInText(c);
            break;
        case STATE_MAYBE_COMMENT:
            state = maybeInComment(c);
            break;
        case STATE_COMMENT:
            state = handleComment(c);
            break;
        case STATE_MAYBE_END_COMMENT:
            state = handleMaybeEndComment(c);
            break;
        case STATE_STRING:
            state = handleInString(c);
            break;
        case STATE_STRING_ESCAPE:
            state = handleInStringEscape(c);
            break;
        case STATE_CHAR:
            state = handleInChar(c);
            break;
        case STATE_CHAR_ESCAPE:
            state = handleInCharEscape(c);
            break;
        }
 
        /* just entered a comment - remember where, for the error message */
        if (prevState == STATE_MAYBE_COMMENT && state == STATE_COMMENT) {
            commentStartLine = line;
        }

        if (c == '\n') {
            line++;
        }

    }
        if (state == STATE_MAYBE_COMMENT) {
        putchar('/');
        }


    /* EOF while still inside a comment => it was never closed */
    if (state == STATE_COMMENT || state == STATE_MAYBE_END_COMMENT) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", commentStartLine);
        return EXIT_FAILURE;
    }
 
    return EXIT_SUCCESS;
}
