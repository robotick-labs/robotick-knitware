#!/usr/bin/env python3
import subprocess
import sys
import os

engine_dir = os.path.join(os.path.dirname(__file__), "robotick-engine")
result = subprocess.run(
    [sys.executable, "setup.py"] + sys.argv[1:],
    cwd=engine_dir,
)
sys.exit(result.returncode)
