#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""SimpleVMCompiler.py

   Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>

[LICENSE]
       This program is free software; you can redistribute it and/or modify
       it under the terms of the GNU General Public License as published by
       the Free Software Foundation; either version 2 of the License, or
       (at your option) any later version.

       This program is distributed in the hope that it will be useful,
       but WITHOUT ANY WARRANTY; without even the implied warranty of
       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
       GNU General Public License for more details.

       You should have received a copy of the GNU General Public License
       along with this program; if not, write to the Free Software
       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
       MA 02110-1301, USA.

[ABOUT]
       This is a program I hacked together in '11 as a compiler for my CS 314H
       class. Dr. Lin suggested that as an extra credit component we could
       construct a compiler of some sort which could build a higher level
       language like Java or C into an "assembler" of sorts provided to us in
       the assignment.

       This is a single-pass compiler which makes no attempt to achieve any
       sort of optimizations. In fact if it works as designed I will be
       positively ecstatic.

       This program reads in raw text from .vm files, and builds them to .cri
       code as used by the Assignment 3 interpreter.

       As I was hacking my compiler together, I began to encounter problems
       due in part to the style in which the original code was written.
       In an effort to clean up my mess, I began to re-name and sbstract many
       operations previously done in-line or otherwise using Python hacks to
       achieve greater ease of maintinance and bug-hunting.

       This file serves to contain the actual compile() function, as well as a
       number of helper and other BS methods extracted from various functions
       and one-liners.

[STYLE]
       One of my goals in writing this file is to get away from and remove many
       of the really crazy hacks which I am used to writing in python in favor
       of a more clearly named, more functional approach to this problem.

       As a result, the following things should happen:
           1)  In-line hacks should go away except where appropriate or
               pythonic.
           2)  Single character variable names are a thing of the past.
           3)  __EVERYTHING__ gets a docstring.

[STRUCTURE - execution]
        This program is designed to be invoked via the kk file, __but__ the same
        code also exists in KritterKimpiler as __main__
"""
################################################################################

import sys
import re
import os

from SimpleVMCompilerClasses import CodeBlock, RecursiveFunction

################################################################################

__usage__ = \
"""SimpleVM - a tool for making UT@A CS313 Critters in almost C
        Cpyrght. Reid McKenzie, 2011

Usage: kk [file | dir] ] [file | dir]+ [opts]]
Options:
    cli  -  causes the compiler to go into a user interface mode for debugging
            and code inspection at compile time
    dbg  -  enables a number of debugging statements
    cmnt -  causes the compiler to print the output code before exiting
 comment -  causes the compiler to automatically comment the output describing
            compilation and execution flow
  nosave -  causes the compiler to __not__ save the output to a file
"""

__cli_help__ = """\nSimpleVM CLI commands ......
HELP          - you're looking at it.
LS            - lists defined variables, vaid targets of more.
MORE OBJ      - allows scrolling through large objects just like the Unix more.
COMPILE [OBJ] - compiles some function object, defaulting to 'main0'
LOAD FNAME    - forces the compiler to load a target file
SAVE [FNAME]  - writes the last built code out to a file.
                defaults to the specified source file as a '.cri'
EXIT          - kills this CLI, exits"""

################################################################################

def load(fname, included_files, loaded_text):
    """This function is responsible for recursively loading dependency files,
    catching problems like recursive file inclusions and missilg library files.

    filename        - the full path of the target file as a string.
    included_files  - a set of strings representing every loaded file.
    loaded_text     - a list of lines, representing all the loaded code.
    """

    if not "./libvm/" in fname:
        fname = "./libvm/"+fname

    if not ".vm" in fname:
        fname += ".vm"

    included_files = set(included_files)
    included_files.add(fname)

    include = re.compile(" *?#include ([\'\"\w_\.\*]+)")

    with open(fname) as file_text: # deal with potential errors in opening..
        for line in file_text:
            loaded_text.append(line)
            try:
                match = (re.match(include, line).groups())[0]
            except IndexError:
                continue

            except AttributeError:
                continue

            match = str(match)

            if match is None:
                continue

            if match not in included_files:
                print "[LOADING]", match

                if match not in included_files:
                    included_files, loaded_text = load(match,
                                                       included_files,
                                                       loaded_text)
                    continue
            else:
                continue

    return included_files, loaded_text

def function_depend_check(function, encountered, all_functions, depth=0):
    """
    A tail recursive function which checks to ensure that there are no
    recursive dependencies or loops which will cause infinite output if
    the compiler attempts to expand them.
    """

    if encountered is not set:
        encountered = set(encountered)

    encountered.add(function)

    for depend in function.depends:
        try:
            if all_functions[depend] in encountered:
                raise RecursiveFunction("{0}:{1}   {2}".format(function,
                                                               depend,
                                                               depth
                                                               )
                                        )

            else:
                print (" "*depth*4) + depend
                function_depend_check(all_functions[depend],
                                          encountered,
                                          all_functions,
                                          depth=(depth+1))
        except RecursiveFunction as exe:
            print exe
            sys.exit(1)

        except NameError or KeyError:
            pass

def parse(code, all_functions):
    """
    Contains all the regex parsing of input lines, and the generation of the
    "abstract syntax tree" to the extent that SimpleVM in fact uses one.

    code    - a list of lines as returned by the load() function.
    """

    for j in range(len(code)):
        line = code[j]
        for regex in [("\/\/.*?\n", ""), ("^[ \t]*", ""), ("[ \t]+", " ")]:
            line = re.sub(re.compile(regex[0]), regex[1], line)
        code[j] = line

    function_matches = re.findall(
        re.compile("(\w+)( ?)\(([\w, \.\$# ]+)?\)( )?{(.*?)}", re.DOTALL),
        ''.join(code))

    for match in function_matches:
        match = list(match)

        match[4] = re.sub(re.compile("\n|\r"), "", match[4])
        match[4] = re.split(re.compile(";\n?"), match[4])

        temp_block = CodeBlock(match[0], match[2], match[4])

        if temp_block.signature not in all_functions:
            all_functions[temp_block.signature] = temp_block

        else:
            print 'Warning: function '+temp_block.signature+' is defined twice'

    return code, all_functions

def compile_block(target_block, blocks, dbg, comment, join):
    """
    A belated addition to the SimpleVM design, it provides a more unit
    -testable interface to the actual compilation process.

    string_of_code - multiline input string_of_code
    CodeBlock          - the base/root CodeBlock to compile
    CodeBlocks         - the dict. of all CodeBlocks
    __dbg__            - a boolean flag which controls debugging printing.
    """

    name = re.sub(re.compile("(.)?(\/)+.+(\/)+"),
                  "",
                  os.path.splitext(sys.argv[1])[0]) + " - KritterKompiled"

    compiled_block = target_block.compile(blocks,
                                          [],
                                          dbg,
                                          comment)

    if join:
        compiled_block = name + "\n" + compiled_block + "\ngo 1\n"

        for regex in [(";|#", ""), ("\n +", "\n"), ("\n+", "\n")]:
            compiled_block = re.sub(re.compile(regex[0]),
                                               regex[1],
                                               compiled_block)

        compiled_block += "\nGeneric comment to keep the loader happy.\n"

    return compiled_block

if __name__ == "__main__":
    __files__ = [f for f in sys.argv[1:] if os.path.isfile(f)]

    if __files__ in [[], None]:
        for d in [d for d in sys.argv[1:] if os.path.isdir(d)]:
            for f in os.path.listdir(d):
                if re.match(re.compile(".*?(\.kr)(it)?"), f):
                    __files__.append(f)

    __opts__ = [o.lower() for o in sys.argv[1:] if not o in __files__]

    __cli__         = 'cli'         in __opts__
    __dbg__         = 'dbg'         in __opts__
    __prnt__        = 'print'       in __opts__
    __cmnt__        = 'comment'     in __opts__
    __fout__        = not ('nosave' in __opts__)
    __hlp__         = 'help'        in __opts__
    __includes__    = set()
    __text__        = []
    __functions__   = {}


    if __hlp__:
        print __usage__
        sys.exit(0)

    if __files__:
        for __filename__ in __files__:
            # now do file #include etc...
            __includes__, __text__  = load  (__filename__, set(), [])
            __text__, __functions__ = parse (__text__, {})

            # do depend. checking....
            for __func__ in __functions__:
                function_depend_check(__functions__[__func__],
                                      [],
                                      __functions__)

            if not __cli__:
                __compiledcode__ = compile_block(__functions__['main0'],
                                           __functions__,
                                           __dbg__,
                                           __cmnt__,
                                           True)

                if __prnt__:
                    print "_" * 80 + "\n" + __compiledcode__

                if __fout__:
                    open(
                            os.path.splitext(__filename__)[0]+".cri", 'w'
                        ).write(__compiledcode__)

                print __filename__+(" "*(80-len(__filename__)-6))+"[DONE]"

    if __cli__:
        # just do the CLI here and save a million namespace problems
        from mckenzieLib import IO
        while True:
            try:
                COMMAND = raw_input(" > ").strip("\n").lower().split(" ")

                if (COMMAND[0] == 'more'):
                    IO.more(dir()[COMMAND[1]])

                elif (COMMAND[0] == 'compile'):
                    IO.more(compile_block(__functions__["main0"],
                                     __functions__,
                                     dbg=__dbg__))

                    continue

                elif (COMMAND[0] == "ls"):
                    for i in ['__functions__',
                              'includes',
                              'text',
                              '__dbg__',
                              '__cmnt__',
                              'code']:
                        try:
                            if(eval("{0} != None".format(i))):
                                print i
                        except NameError or Exception:
                            continue

                elif (COMMAND[0] == "help"):
                    print __cli_help__

                elif COMMAND[0] in ('exit', 'quit'):
                    raise EOFError()

                elif (COMMAND[0] == "eval"):
                    print eval(''.join(COMMAND[1:]))
                    continue

                elif (COMMAND[0] == 'load'):
                    s = COMMAND[1]
                    try:
                        __includes__, __text__  = load  (COMMAND[1],
                                                         __includes__,
                                                         __text__)
                        __text__, __functions__ = parse (__text__,
                                                         __functions__)

                        print s + " "*(80-len(s)-6)+"[OKAY]"
                    except Exception as e:
                        print e
                        print s + " "*(80-len(s)-6)+"[FAIL]"
                        continue

                else:
                    print "Sorry, you may have used a valid command but it" +\
                            " isn't implemented yet."

            except EOFError as ex:
                print "\nBye!\n"
                exit(1)

            except Exception as ex:
                print ex

    elif __files__ in [[], None]:
        sys.exit("Failed to find any input files. "+\
                    "Use `SimpleVM help` for help.")
