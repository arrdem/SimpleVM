#!/usr/bin/python

"""SimpleVMKlasses.py

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
       As part of my compiler (described in greater detail in SimpleVM)
       I found myself in need of a data structure to store the code which was
       being built at any given time.

       On any ordinary project, I will (out of pure bad habit) use nested lists
       or hashmaps (dicts) to store my data. As I had been writing Java for some
       time prior to beginning this journey, I decided to make the "right"
       choice from an OO design standard and actually leverage classes and some
       class properties in this project instead of mucking with lists.

       I also found that I needed (or at any rate wanted) two DIY Exceptions
       which were specific to having recursive function definitions which I
       could toss back and forth to make obvious various what was going on.
"""

################################################################################

import re
import sys

################################################################################

class RecursiveFunction(Exception):
    """
    A very basic Exception which is used to signal the presence of a recursive
    function definition, a construct which this compiler does not allow for the
    time being.
    """
    def __init__(self, value):
        Exception.__init__(self)
        self.parameter = value

    def __str__(self):
        return repr(self.parameter)

class RecursiveDepencencyException(Exception):
    """
    @DEPRECATED
    This Exception was envisioned as a way to signal that a file's #INCLUDE tree
    included itself or some other weirdness. However, it became used as more
    general flag for stating that a function could __eventially__ recurse
    without dropping back up the execution tree at all.
    """
    def __init__(self, value):
        Exception.__init__(self)
        self.parameter = value

    def __str__(self):
        return repr(self.parameter)

class CodeBlock:
    """
    This is the primary reason this file exists. It serves to represent a
    function definition in the SimpleVM's internals, describing things
    like function dependencies, the method signature and the base method code.
    """
    def __init__(self, name, args, code):
        """Builds the CodeBlock object, checks depends and makes .code"""
        self.name       = name
        self.args       = [b for b in args.split(",") if b]
        self.signature  = self.make_call_signature(name, args)

        self.code = ""

        for line in code:
            if line:
                if line[-1] != "\n":
                    self.code  +=  line+"\n"
                else:
                    self.code  +=  line

        for regex in [("$ +", ""), (" +", " "), ("#", "")]:
            self.code = re.sub(re.compile(regex[0]), regex[1], self.code)

        self.depends = [self.make_call_signature(m[0], m[1]) for m in
                            re.findall(re.compile("(\w+)\((.+)\);"),
                                  self.code)
                        ]

        if self.signature in self.depends:
            raise RecursiveDepencencyException(self.signature)

        self.code = re.split(re.compile("\n|;"), self.code)

        if self.depends != []:
            self.depends = [self.make_call_signature(a[0], a[1])
                                for a in self.depends]

    def __str__(self):
        """
        Overloads the Class.__str__ method to return the CodeBlock's invocation
        signature. This way printing a CodeBlock returns something useful and
        allows easy IDing of the printed CodeBlock.
        """
        return self.signature

    def make_args_list(self, argstr):
        """
        Processes an input string containing comma-deleminated arguments (which
        may be nested function calls, string litterals or other weirdnesses)
        into a single list of items.

        This manual implementation is required over string.split() because there
        are many cases such as "and(a(), b()), c()" where string.split(",")
        would naively return a split list without any regard to the contents of
        each string in the list.
        """
        args, parens, bufferstr = [], 0, ""

        for char in argstr:
            if char == "(":
                parens += 1
                bufferstr += char

            elif char == ")":
                parens -= 1
                bufferstr += char

            elif (char == ",") and (parens == 0):
                args += [bufferstr]
                bufferstr = ""

            elif char == " ":
                continue

            else:
                bufferstr += char

        if bufferstr != "":
            args += [bufferstr]

        return args

    def make_call_signature(self, name, args):
        """
        Calculates a function's signature given a name and a parsed list of args
        """
        return (name + str(len([a for a in self.make_args_list(args) if a])))

    def make_call_sig_line(self, line):
        """
        A more advanced make_call_signature() which takes a raw string and is
        more flexible as a result. Does __not__ require any pre-parsing of the
        calling function. That's the point!
        """
        call = re.match(re.compile("(\w+)\((.*?)\);"), line).groups()
        return self.make_call_signature(call[0], call[1])

    def compile(self, functions, argv,
                dbg,
                comment,
                spacepad = 25):

        """
        This function is the reason that this file exists.
        Leveraging a number of regular expressions, this function searches the
        CodeBlock object's source code for calls to other functions, then
        inserts the compilation of those functions inline where appropriate.

        functions - the global dict of signatures : CodeBlock objects
        argv      - a list of strings which represents the "arguments" (really
                    just substitution strings) with which this code is envoked.

        dbg       - controls the extent to which the compiler prints to stdout
                    explaining what it is doing while it works.
        comment   - causes the compiler to insert comments into the output code
                    detailing the composition of functions and origin of any non
                    -macro line.
        spacepad  - the column number where it's OK for the compiler to start
                    adding comments.
        """

        # pass no. 0 - replace arguments
        temp_lines = ''.join([a+'\n' for a in self.code])
        if argv != []:
            for argv_ind in range(len(argv)):
                temp_lines = re.sub(
                                    re.compile("\$"+str(argv_ind)),
                                    argv[argv_ind],
                                    temp_lines)
        temp_lines =  re.split(re.compile("\n"), temp_lines)

        output = ""

        # pass no. 1 - function expansion
        for i in range(len(temp_lines)):
            line = temp_lines[i]
            if dbg:
                sys.stdout.write("[{0} - {1}]\t{2}\n".format(
                                            self.signature.center(20, " "),
                                            str(i).center(4, " "),
                                            line)
                                )
                sys.stdout.flush()

            match = re.search(re.compile("(\w+)( ?)\((.+)?\)(;?)"), line)

            if not match:
                output += line.strip("\n") + "\n"
                continue

            if match:
                match = match.groups()
                match = list(match)

                if not match[2]:
                    match[2] = ""

                if dbg:
                    sys.stdout.write("[{0} - {1}]\t{2}\n".format(
                                            self.signature.center(20, " "),
                                            str(i).center(4, " "),
                                            match)
                                    )
                    sys.stdout.flush()

                call_sig    = self.make_call_signature(match[0], match[2])
                call_args   = self.make_args_list(match[2])

                if dbg:
                    print "[RECURSING...]"

                output += functions[call_sig].compile(functions,
                                                      call_args,
                                                      dbg,
                                                      comment)
        output = [o for o in output.split("\n") if o]

        output = complete_tags(output,
                               bool(comment),
                               int(spacepad))

        if bool(comment):
            output[0]  +=  ((" "*(spacepad-len(output[0]))) +
                            "// begin " + self.signature)

            if output[0] != output[-1]:
                output[-1]  +=  ((" "*(spacepad-len(output[-1]))) +
                                "// end    " + self.signature)

        if dbg:
            print "[BREAKING....]"

        return ''.join([a+'\n' for a in output])

def complete_tags(code, comment, spacepad=25):
    """
    serves to compartmentalize the line/tag completion process.
    """
    labels = dict()

    # pass no. 2 - find @ tags...
    for i in range(len(code)):
        match = re.findall(re.compile("@(\w+);?"), code[i])

        if match:
            match = match[0]
            labels[match] = i
            code[i] = "go +1"

            if comment:
                code[i] += (" "*(spacepad-len(code[i])))+ \
                              "// mark {0}".format(match)

    # pass no. 3 - complete % gotos
    for i in range(len(code)):
        for match in re.findall(re.compile("%(\w+)"), code[i]):

            if match in labels:
                delta = (labels[match]-i)

                if delta > 0:
                    delta = "+" + str(delta)
                else:
                    delta = str(delta)

                code[i] = re.sub(re.compile("(%{0})(;?)".format(match)),
                              delta,
                              code[i])

                if comment:
                    code[i] += (" "*(spacepad-len(code[i]))) + "// goto "+match

    return code
