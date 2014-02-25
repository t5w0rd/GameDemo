#!/usr/bin/python

import os
import sys

def renamedImageFiles(dir, test):
    try:
        fs = os.listdir(dir)
    except:
        return 0
        
    dirname = dir[dir.rfind('/') + 1:]
    fs.sort()
    c = 0
    for f in fs:
        old = '%s/%s' % (dir, f)
        #new = '%s/%s_%02d.png' % (dir, dirname, c)
        new = '%s/%02d.png' % (dir, c)
        if test:
            print '%s -> %s' % (old, new)
        else:
            os.rename(old, new)

        c += 1
        
    return len(fs)

if __name__ == "__main__":
    for argv in sys.argv[1:]:
        dir = os.path.abspath(argv)
        res = renamedImageFiles(dir, True)
        if res == 0:
            continue
            
        res = raw_input('Are your sure to rename the files?(y/N):')
        
        if len(res) > 0 and res[0].lower() == 'y':
            count = renamedImageFiles(dir, False)
            print '%s file(s) renamed.' % (count)
        else:
            print 'Operation canceled.'
