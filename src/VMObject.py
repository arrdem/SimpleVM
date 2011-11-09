#!/usr/bin/env python
# defines the VMObject type
class VMObject():
    """Basic type for all the memory the VM has.."""
    def __init__(self, *args):
        if(args):
            if 1 >= len(args):
                self.__addr = args[0]
            if 2 >= len(args):
                self.__data = args[1];

    def addr(this):
        return this.__addr;

    def set_addr(this, addr):
        this.__addr = addr;

    def data(this):
        return this.__data;

    def set_data(this, data):
        this.__data = data;

class VMInteger(int, VMObject):
    def __init__(self, *args):
        args = [a for a in args if a]
        if(len(args) >= 1):
            super(VMInteger, self).__init__(int(args[0]))
            self.__data = int(args[0])
        else:
            super(VMInteger, self).__init__()

    def __str__(self) :
        return repr(self)

    def __repr__(self) :
        if(self.denominator) == 1:
            return str(self.numerator)
        else:
            return "%s/%s" % (self.numerator, self.denominator)
