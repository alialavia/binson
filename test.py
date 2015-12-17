from subprocess import Popen, PIPE
import random
testnum = 1000
MAX = 1e60
for _ in range(testnum):        
    bound = random.uniform(1, MAX)
    n = random.uniform(-1.0, +1.0) * bound
    s = "%f" % n
    l = len(s)
    proc = Popen(["bin/Debug/binson", "n", s], stdout=PIPE)
    out = int(proc.stdout.readline())
    #print l, out, s
    if l != out:
        print l, out, s
        raw_input()

print "%d TESTS DONE" % testnum
