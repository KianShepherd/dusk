#!/usr/bin/env python
"""Gen flex and bison files if needed."""
import os
import subprocess


grammar_time = os.path.getmtime('../src/grammar.y')
grammar_gen_time = 0
tokens_time = os.path.getmtime('../src/tokens.l')
tokens_gen_time = 0

gen_grammar = False
gen_scanner = False

if os.path.exists('../src/parser.cc'):
    grammar_gen_time = os.path.getmtime('../src/parser.cc')
if os.path.exists('../src/scanner.cc'):
    tokens_gen_time = os.path.getmtime('../src/scanner.cc')

if grammar_gen_time == 0 or tokens_gen_time == 0:
    gen_grammar = True
    gen_scanner = True
else:
    if grammar_time > grammar_gen_time:
        gen_grammar = True
    elif tokens_time > tokens_gen_time:
        gen_scanner = True

if gen_grammar:
    subprocess.run(['bison', '../src/grammar.y'])
    subprocess.run(['cp', 'src/parser.cc', '../src/parser.cc'])
    subprocess.run(['cp', 'include/parser.hh', '../include/parser.hh'])

if gen_scanner:
    subprocess.run(['flex', '../src/tokens.l'])
    subprocess.run(['cp', 'src/scanner.cc', '../src/scanner.cc'])
