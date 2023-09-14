#include "parser.hh"
#include "ast.hh"
#include "expression.hh"
#include <string>
#include <fstream>

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
    std::cout << "-d                Print out debug information about the final AST and LLVM IR." << std::endl;
    std::cout << "-df               Print out debug information about the AST at each stage and LLVM IR." << std::endl;
    std::cout << "-da               Print out debug information about the final AST." << std::endl;
    std::cout << "-daf              Print out debug information about the AST at each stage." << std::endl;
    std::cout << "-di               Print out debug information about the LLVM IR." << std::endl;
    std::cout << "-dc               Print out debug information about the compile commands." << std::endl;
    std::cout << "-O                Optimize the generated object / executable." << std::endl;
    std::cout << "-c                Only compile the sources do not link into executable." << std::endl << std::endl;

    std::cout << "-o <file>         Set the name of the outputted object / executable." << std::endl;
    std::cout << "-cc <compiler>    Set the c / c++ compiler to use (default is g++)." << std::endl << std::endl;

    std::cout << "-l<library>       Add library to link to executable at compile time." << std::endl;
    std::cout << "-L<path>          Add path to library link paths." << std::endl;
    std::cout << "-I<path>          Add path to include file paths." << std::endl;
}

inline bool file_exists(std::string path) {
    std::ifstream f(path.c_str());
    return f.good();
}

std::vector<std::string> resolve_paths(AST* ast, std::vector<std::string> load_paths, std::vector<std::string> sources) {
    std::vector<std::string> resolved_paths = std::vector<std::string>();

    for (long unsigned int i = 0; i < sources.size(); i++) {
        if (file_exists(sources[i])) {
            resolved_paths.push_back(sources[i]);
        } else {
            for (long unsigned int j = 0; j < load_paths.size(); j++) {
                std::string tmp_path = load_paths[j];
                tmp_path.append(sources[i]);
                if (file_exists(tmp_path)) {
                    resolved_paths.push_back(tmp_path);
                    break;
                }
            }
        }
    }
    if (sources.size() !=  resolved_paths.size())
        ast->push_err("Could not find all files specified.");

    return resolved_paths;
}

int main(int argc, char** argv) {
#ifdef DEBUGGING
    yydebug = 1;
#endif
    struct {
        std::vector<std::string> sources;
        std::vector<std::string> libs;
        std::vector<std::string> lib_paths;
        std::vector<std::string> include_paths;
        std::string outfile;
        std::string c_compiler;
        bool only_compile;
        bool optimizations;
        bool debug_ast;
        bool debug_full_ast;
        bool debug_ir;
        bool debug_compile_command;
    } compiler_args;
    compiler_args.optimizations = false;
    compiler_args.debug_ast = false;
    compiler_args.debug_full_ast = false;
    compiler_args.debug_ir = false;
    compiler_args.debug_compile_command = false;
    compiler_args.only_compile = false;
    compiler_args.c_compiler = std::string("g++");
    compiler_args.outfile = std::string("out");

    compiler_args.sources.push_back("stdlib.ds");

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
            compiler_args.debug_ast = true;
            compiler_args.debug_ir = true;
            compiler_args.debug_compile_command = true;
            args++;
            continue;
        }
        if (cur_arg.compare("-da") == 0) {
            compiler_args.debug_ast = true;
            args++;
            continue;
        }
        if (cur_arg.compare("-df") == 0) {
            compiler_args.debug_full_ast = true;
            compiler_args.debug_ir = true;
            compiler_args.debug_compile_command = true;
            args++;
            continue;
        }
        if (cur_arg.compare("-daf") == 0) {
            compiler_args.debug_full_ast = true;
            args++;
            continue;
        }
        if (cur_arg.compare("-di") == 0) {
            compiler_args.debug_ir = true;
            args++;
            continue;
        }
        if (cur_arg.compare("-dc") == 0) {
            compiler_args.debug_compile_command = true;
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
        if (cur_arg.substr(0, 2).compare("-I") == 0) {
            std::string path = cur_arg.substr(2, cur_arg.size() - 2);
            if (path.back() != '/')
                path.append("/");
            compiler_args.include_paths.push_back(path);
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
    if (development) {
        compiler_args.include_paths.push_back(std::string("./dusklibs/"));
        compiler_args.lib_paths.push_back("-L./CMake ");
    } else {
        // TODO: Actually install this somewhere and load from there.
        // This will have to be done for all std libraries
        // /usr/local/lib/libstddusk.a
        // /usr/local/include/dusk/stdlib.ds
        compiler_args.include_paths.push_back(std::string("/usr/local/include/dusk"));
        compiler_args.sources.push_back("./dusklibs/stdlib.ds");
    }

    std::vector<std::string> already_parsed;
    int rc = 0;
    while (true) {
        if (compiler_args.sources.size() == 0)
            break;
        compiler_args.sources = resolve_paths(&ast, compiler_args.include_paths, compiler_args.sources);

        for (auto& file : compiler_args.sources) {
            bool parsed = false;
            for (auto& p : already_parsed) {
                if (p.compare(file) == 0)
                    parsed = true;
            }
            if (parsed)
                continue;
            yyin = fopen(file.data(), "r");
            if (!yyin) {
                std::cerr << "Unable to open " << file << ": " << strerror(errno) << '\n';
                return 1;
            }
            yylineno = 1;
            rc = yyparse(ast);
            auto parse_err = std::string("Parse Error in ").append(file.data()).append(": ");
            if (ast.check_error(parse_err))
                return rc;
            already_parsed.push_back(file);
        }
        compiler_args.sources = ast.includes;
        ast.includes.clear();
    }

    if (ast.check_error(std::string("Could not file file: ")))
        return 5;

    for (auto& req : ast.require) {
        std::string lib = std::string("-l");
        lib.append(req);
        compiler_args.libs.push_back(lib);
    }

    ast.finalize_structs();

    if (compiler_args.debug_full_ast) {
        ast.debug();
    }
    ast.static_checking(compiler_args.only_compile);

    if (compiler_args.debug_full_ast) {
        ast.debug();
    }
    if (!compiler_args.only_compile) {
        ast.clean_ast();
    }

    if (compiler_args.debug_ast || compiler_args.debug_full_ast) {
        ast.debug();
    }
    if (ast.check_error(std::string("Logic Error: ")))
        return 2;

    std::string objectfile = "output.o";
    if (compiler_args.only_compile && compiler_args.outfile.compare("out") != 0) {
        objectfile = compiler_args.outfile;
    }
    ast.codegen((char)compiler_args.debug_ir, compiler_args.optimizations, objectfile);

    if (!compiler_args.only_compile) {
        auto compile_str = compiler_args.c_compiler
            .append(" ")
            .append(objectfile)
            .append(" ");
        if (compiler_args.optimizations)
            compile_str.append("-O3 ");
        for (auto& lib_path : compiler_args.lib_paths)
            compile_str.append(lib_path).append(" ");
        for (auto& lib : compiler_args.libs)
            compile_str.append(lib).append(" ");

        compile_str.append("-o ").append(compiler_args.outfile);

        if (compiler_args.debug_compile_command)
            std::cout << compile_str << std::endl;

        // Compile the generated object file into an executable
        rc = system(compile_str.data());

        // Clean object file used for compilation
        auto clean_str = std::string("rm -f ").append(objectfile);
        rc = system(clean_str.data());
    }
    return 0;
}
