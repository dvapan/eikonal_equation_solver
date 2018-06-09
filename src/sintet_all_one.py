#!/usr/bin/env python

import scipy as sc 
import math

data = []
img = []


r = 9
x0,y0 = 25,25
dimx,dimy = 50,50
for y in range(dimy):
    line = []
    for x in range(dimx):
        l = (1.0*(x-x0)/r)**2 + (1.0*(y-y0)/r)**2
        if l <= 0.8:
            line.append(math.sqrt(1-l))
        else:
            line.append( 0.0)
    img.append(line)
        


print dimx * dimy, 3
data = sc.asarray(img)

max_val = data.max()
lst = []
for i in range(len(data)):
    for j in range(len(data[i])):
        sout=""
        if data[i][j] != 0:
            tst = math.sqrt((1.0*max_val/data[i][j])**2 - 1)
            if abs(tst) > 0.01:
                sout= "{} {} {}".format(i,j,1)#1.0/tst)
            else:
                sout= "{} {} {}".format(i,j,1)#sc.inf)
        else:
            sout= "{} {} {}".format(i,j,0)
        if i == x0 and j==y0:
            sout+= " 0"
        else:
            sout+= " {}".format(sc.inf)
        print sout

print "0 0\n5 1\n2 3\n"
# print len(lst)
# for coords in lst:
#     print " ".join(map(str,coords))

# print 1
# print x0+4, y0+4

