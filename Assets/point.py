#!/usr/bin/python

#points(x, y) in photoshop.
def point(w, h, ax, ay, hx, hy, fx, fy):
    anchorX = ax / w
    anchorY = (h - ay) / h
    halfOfW = hx - ax
    halfOfH = ay - hy
    fireX = fx - ax
    fireY = ay - fy
    
    print "setGeometry(%d.0f, %d.0f, ccp(%d.0 / %d, %d.0 / %d), CPoint(%d.0f, %d.0f));" % (halfOfW, halfOfH, ax, w, h - ay, h, fireX, fireY)

if __name__ == "__main__":
    import sys
    import string
    point(string.atoi(sys.argv[1]), string.atoi(sys.argv[2]), string.atoi(sys.argv[3]), string.atoi(sys.argv[4]), string.atoi(sys.argv[5]), string.atoi(sys.argv[6]), string.atoi(sys.argv[7]), string.atoi(sys.argv[8]))
    