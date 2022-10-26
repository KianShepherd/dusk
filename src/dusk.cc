#include "parser.hh"
#include "ast.hh"
#include "expression.hh"
#include <string>

#define development 1

void yyerror(AST& ast, const char* msg) {
    std::string err_msg = std::string(msg);
    err_msg.append(" at line ");
    err_msg.append(std::string(std::to_string(yylineno)));
    ast.push_err(err_msg);
}

void print_help() {
    std::cout << "Usage: dusk [options] file..." << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h                Display this information." << std::endl;
    std::cout << "-d                Print out debug information about the AST and LLVM IR." << std::endl;
    std::cout << "-O                Optimize the generated object / executable." << std::endl;
    std::cout << "-c                Only compile the sources do not link into executable." << std::endl << std::endl;

    std::cout << "-o <file>         Set the name of the outputted object / executable." << std::endl;
    std::cout << "-cc <compiler>    Set the c / c++ compiler to use (default is g++)." << std::endl << std::endl;

    std::cout << "-l<library>       Add library to link to executable at compile time." << std::endl;
    std::cout << "-L<path>          Add path to library link paths." << std::endl;
}

int main(int argc, char** argv) {
#ifdef DEBUGGING
    yydebug = 1;
#endif
    struct {
        std::vector<std::string> sources;
        std::vector<std::string> libs;
        std::vector<std::string> lib_paths;
        std::string outfile;
        std::string c_compiler;
        bool only_compile;
        bool optimizations;
        bool debug_info;
    } compiler_args;
    compiler_args.optimizations = false;
    compiler_args.debug_info = false;
    compiler_args.only_compile = false;
    compiler_args.c_compiler = std::string("g++");
    compiler_args.outfile = std::string("out");

    if (development) {
        compiler_args.sources.push_back("./dusklibs/stdlib.ds");
        compiler_args.lib_paths.push_back("-L./CMake ");
    } else {
        // TODO: Actually install this somewhere and load from there.
        compiler_args.sources.push_back("./dusklibs/stdlib.ds");
    }

    AST ast = AST();
    if (argc == 1) {
        std::cout << "No input files given." << std::endl;
        return 2;
    }
    int args = 1;
    while (true) {
        if (args == argc)
            break;
        auto cur_arg = std::string(argv[args]);
        if (cur_arg.compare("-h") == 0) {
            print_help();
            return 0;
        }
        if (cur_arg.compare("-O") == 0) {
            compiler_args.optimizations = true;
            args++;
            continue;
        }
        if (cur_arg.compare("-d") == 0) {
            compiler_args.debug_info = true;
            args++;
            continue;
        }
        if (cur_arg.compare("-o") == 0) {
            args++;
            if (args == argc) {
                std::cout << "No output file given." << std::endl;
                return 2;
            }
            compiler_args.outfile = std::string(argv[args]);
            args++;
            continue;
        }
        if (cur_arg.substr(0, 2).compare("-l") == 0) {
            compiler_args.libs.push_back(cur_arg);
            args++;
            continue;
        }
        if (cur_arg.substr(0, 2).compare("-L") == 0) {
            compiler_args.lib_paths.push_back(cur_arg);
            args++;
            continue;
        }
        if (cur_arg.compare("-cc") == 0) {
            args++;
            if (args == argc) {
                std::cout << "No C compiler given." << std::endl;
                return 2;
            }
            compiler_args.c_compiler = std::string(argv[args]);
            args++;
            continue;
        }
        if (cur_arg.compare("-c") == 0) {
            compiler_args.only_compile = true;
            args++;
            continue;
        }
        compiler_args.sources.push_back(cur_arg);
        args++;
    }

    int rc = 0;
    for (auto& file : compiler_args.sources) {
        yyin = fopen(file.data(), "r");
        if (!yyin) {
                std::cerr << "Unable to open " << file << ": " << strerror(errno) << '\n';
            return 1;
        }
        rc = yyparse(ast);
        if (ast.check_error(std::string("Parse Error: ")))
            return rc;
    }
    ast.static_checking();
    if (ast.check_error(std::string("Logic Error: ")))
        return 2;
    
    if (compiler_args.debug_info) {
        ast.debug();
    }

    std::string objectfile = "output.o";
    if (compiler_args.only_compile && compiler_args.outfile.compare("out") != 0) {
        objectfile = compiler_args.outfile;
    }
    ast.codegen((char)compiler_args.debug_info, compiler_args.optimizations, objectfile);

    if (!compiler_args.only_compile) {
        auto compile_str = compiler_args.c_compiler
            .append(" ")
            .append(objectfile)
            .append(" ");
        for (auto& lib_path : compiler_args.lib_paths)
            compile_str.append(lib_path).append(" ");
        compile_str.append("-lstddusk ");
        for (auto& lib : compiler_args.libs)
            compile_str.append(lib).append(" ");

        compile_str.append("-o ").append(compiler_args.outfile);

        std::cout << compile_str << std::endl;

        // Compile the generated object file into an executable
        rc = system(compile_str.data());

        // Clean object file used for compilation
        auto clean_str = std::string("rm -f ").append(objectfile);
        rc = system(clean_str.data());
    }
    return 0;
}
