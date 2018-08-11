#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include <mrsh/parser.h>

enum symbol_name {
	EOF_TOKEN,
	TOKEN,

	NEWLINE,

	/* The following are the operators (see XBD Operator)
	   containing more than one character. */

	AND_IF,
	OR_IF,
	DSEMI,

	DLESS,
	DGREAT,
	LESSAND,
	GREATAND,
	LESSGREAT,
	DLESSDASH,

	CLOBBER,
};

struct symbol {
	enum symbol_name name;
	char *str;
};

// Keep sorted from the longest to the shortest
static const struct symbol operators[] = {
	{ DLESSDASH, "<<-" },
	{ AND_IF, "&&" },
	{ OR_IF, "||" },
	{ DSEMI, ";;" },
	{ DLESS, "<<" },
	{ DGREAT, ">>" },
	{ LESSAND, "<&" },
	{ GREATAND, ">&" },
	{ LESSGREAT, "<>" },
	{ CLOBBER, ">|" },
};

#define OPERATOR_MAX_LEN 3

static const char *keywords[] = {
	"if",
	"then",
	"else",
	"elif",
	"fi",
	"do",
	"done",

	"case",
	"esac",
	"while",
	"until",
	"for",

	"{",
	"}",
	"!",

	"in",
};

struct mrsh_parser {
	FILE *f; // can be NULL

	struct buffer buf;

	bool has_sym;
	enum symbol_name sym;
	int lineno;

	struct mrsh_array here_documents;
};

size_t parser_peek(struct mrsh_parser *state, char *buf, size_t size);
char parser_peek_char(struct mrsh_parser *state);
size_t parser_read(struct mrsh_parser *state, char *buf, size_t size);
char parser_read_char(struct mrsh_parser *state);
void parser_set_error(struct mrsh_parser *state, const char *msg);
bool is_operator_start(char c);
enum symbol_name get_symbol(struct mrsh_parser *state);
void consume_symbol(struct mrsh_parser *state);
bool symbol(struct mrsh_parser *state, enum symbol_name sym);
bool eof(struct mrsh_parser *state);
bool newline(struct mrsh_parser *state);
void linebreak(struct mrsh_parser *state);
bool newline_list(struct mrsh_parser *state);

size_t peek_name(struct mrsh_parser *state);
size_t peek_word(struct mrsh_parser *state, char end);
struct mrsh_word *expect_parameter(struct mrsh_parser *state);
struct mrsh_word *back_quotes(struct mrsh_parser *state);
struct mrsh_word *word(struct mrsh_parser *state, char end);

#endif
