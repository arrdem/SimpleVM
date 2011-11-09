#!/usr/bin/python
# Test implementation of the SimpleVM. Final deployed version will be in C.
#

import sys
from VMMemory import *

# define functions
FUNCTIONS = {
    # keyword: [num_args, [arg_types...], lambda]
    "ADD" : [3, [INTEGER, INTEGER, POINTER], lambda x,y,z: z.set_data(x+y)],
    "SUB" : [3, [INTEGER, INTEGER, POINTER], lambda x,y,z: z.set_data(x-y)],
    "MUL" : [3, [INTEGER, INTEGER, POINTER], lambda x,y,z: z.set_data(x*y)],
    "DIV" : [3, [INTEGER, INTEGER, POINTER], lambda x,y,z: z.set_data(x/y)],
    "MOD" : [3, [INTEGER, INTEGER, POINTER], lambda x,y,z: z.set_data(x%y)]
}


def load(f):
    """loads a file of some type into program memory for parsing"""
    if f is str:
        return open(file).readlines()
    if f is file:
        return f.readlines();

def parse(l):
    """parses loaded code into a parse tree for eval()"""
    return [i.upper().split(" ") for i in l]

def eval(env, line, code):
    """executes loaded code"""
    # env is a string:VMObject map representing the VM's memory
    # line is an integer representing the line under hte cursor
    # code is a list of lists of strings representing the parse tree of each
    #   line of "assembler" code
    l = code[line]
    f = FUNCTIONS[l[0]]

    if not (f[0] == len(line) - 1):
        raise SystemError("[EVAL] BAD ARGS")

    elif not typechecker([(env[

def env():
    env = {}
    env["NULL"] = VMObject(None, None, None)
    env["EXEC"] = VMObject(STACK, [])
    env[""] = env["NULL"]

if __name__ == "__main__" or 1:
    code = load(sys.argv[1])
    env = env()
    while(1):

