#!/bin/python3
import sys
import hashlib

if len(sys.argv) != 2:
    print("Usage: python script.py <hash>")
    sys.exit(1)

h = sys.argv[1]

for i in range(10):
    h = hashlib.sha256(h.encode()).hexdigest()
    print(h)
