#!/usr/bin/env python
# This is a personal first... a formal test suite for one of my Python
# creations. Ah well... I figure that when I'm creating an entire VM
# from scratch I should at least be careful about it.

import sys
from VMObject import *

def typeof(o):
    if(type(o) == type(VMObject())):

