#!/usr/bin/env python3
import sys
import os

engine_setup = os.path.join(os.path.dirname(__file__), "robotick-engine", "setup.py")
os.execv(sys.executable, [sys.executable, engine_setup] + sys.argv[1:])
