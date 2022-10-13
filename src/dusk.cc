#include "parser.hh"
#include "ast.hh"
#include "expression.hh"
#include <string>

#define debuging 1

void yyerror(AST& ast, const char* msg) {
    std::string err_msg = std::string(msg);
    err_msg.append(" at line ");
    err_msg.append(std::string(std::to_string(yylineno)));
    ast.push_err(err_msg);
}

int main(int argc, char** argv) {
#ifdef DEBUGGING
    yydebug = 1;
#endif
    AST ast = AST();
    ast.stdlib();
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
    if (ast.check_error(std::string("Parse Error: "))) {
        return rc;
    }
    if (debuging)
        ast.debug();
    //ast.static_checking();
    if (ast.check_error(std::string("Logic Error: "))) {
        return 2;
    }
    if (debuging)
        ast.debug();
    if (debuging)
        std::cout << "==================================" << std::endl;
    ast.codegen((char)debuging);

    rc = system("g++ output.o -lstddusk -o out");
    rc = system("rm -f output.o");
    return 0;
}
