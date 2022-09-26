#include "parser.hh"
#include "ast.hh"
#include "expression.hh"

void yyerror(AST& unused, const char* msg) {
    std::cerr << msg << " at line " << yylineno << '\n';
}

int main(int argc, char** argv) {
#ifdef DEBUGGING
    yydebug = 1;
#endif
    AST ast = AST();
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "Unable to open " << argv[1] << ": " << strerror(errno) << '\n';
            return 1;
        }
    } else {
        yyin = stdin;
    }
    int rc = yyparse(ast);
    if (ast.check_error()) {
        return 2;
    }
    ast.debug();
    return rc;
}
