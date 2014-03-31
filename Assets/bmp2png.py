#!/usr/bin/python

import os
import sys

def bmp2png(dir, test):
    fs = os.listdir(dir)
    dirname = dir[dir.rfind('/') + 1:]
    fs.sort()
    c = 0
    for f in fs:
        if f[-4:].lower() != '.bmp':
            continue
    
        old = '%s/%s' % (dir, f)
        new = old[0:-4] + '.png'
        
        if test:
            print '%s -> %s' % (old, new)
        else:
            os.system('./bmp2png.exe %s %s' % (old, new))

        c += 1
        
    return len(fs)

if __name__ == "__main__":
    dir = os.path.normpath(sys.argv[1])
    dir = sys.argv[1]
    bmp2png(dir, True)
    res = raw_input('Are your sure to convert the files?(y/N):')
    
    if len(res) > 0 and res[0].lower() == 'y':
        count = bmp2png(dir, False)
        print '%s file(s) converted.' % (count)
    else:
        print 'Operation canceled.'
