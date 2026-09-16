#include <stdio.h>  
#include <ctype.h>


enum Statetype{
	STATE_TEXT,
	STATE_MAYBE_COMMENT,
	STATE_COMMENT,
	STATE_MAYBE_END_COMMENT,
	STATE_STRING,
	STATE_STRING_ESCAPE,
	STATE_CHAR,
	STATE_CHAR_ESCAPE

};
enum Statetype handleInText(int c)
{
        enum Statetype state;
        if(c == '/') {
                state = STATE_MAYBE_COMMENT;

        }else if(c == '\'') {
                putchar(c);
                state = STATE_CHAR;

        }else if(c == '"') {
                putchar(c);
                state = STATE_STRING;
        }else{
                putchar(c);
                state = STATE_TEXT;
        }
        return state;
}

enum Statetype maybeInComment(int c){
        enum Statetype state;
        if(c=='*'){
                state = STATE_COMMENT;
        }else{
                putchar('/');
                putchar(c);
                state = STATE_TEXT;
        }
        return state;
}

enum Statetype handleComment(int c)
{
        enum Statetype state;
        if(c=='*'){
                state = STATE_MAYBE_END_COMMENT;
        }else if(c == '\n'){
                putchar(c);
                state = STATE_COMMENT;
        }else{
                state = STATE_COMMENT;
        }
        return state;
}

enum Statetype handleMaybeEndComment(int c)
{
        enum Statetype state;
        if(c == '/'){
                putchar(' ');
                state = STATE_TEXT;
        }else if(c == '\n') {
                putchar('\n');
                state = STATE_COMMENT;
        }else if(c == '*'){
		state = STATE_MAYBE_END_COMMENT;
	}
	else{
                state = STATE_COMMENT;   
        }
        return state;
}

enum Statetype handleInString(int c)
{
	enum Statetype state;
	if(c == '\\'){
		putchar('\\');
		state = STATE_STRING_ESCAPE;
	}else if(c == '"'){
		putchar('"');
		state = STATE_TEXT;
	}else{
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
	     if(c == '\\'){
                putchar('\
				');
                state = STATE_CHAR_ESCAPE;
        }else if(c == '\''){
                putchar('\'');
                state = STATE_CHAR;
        }else{
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
	enum Statetype prevState;
	int lane = 1; 
	int commentStartLine = 0;
	enum Statetype state = STATE_TEXT;
	while ((c = getchar())!= EOF)
		prevState = state;
	{
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
		}
		if(prevState == STATE_MAYBE_COMMENT && state == STATE_COMMENT){
			commentStartline = line;
		}
		if(c=='\n'){line ++}
		if (state == STATE_COMMENT || state == STATE_MAYBE_END_COMMENT) {
        		fprintf(stderr, "Error: line %d: unterminated comment\n", commentStartLine);
        		return EXIT_FAILURE;
		} else {
       		 return EXIT_SUCCESS;
		}	
	}

}
