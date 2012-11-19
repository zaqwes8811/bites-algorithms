#!/usr/bin/python
#-*- coding: utf-8 -*-
import math
y = 120
ds = 0
for i in range(20) :
  ds = ds+y*math.pow(2/10.0, i)*(12/10.0)
  print 'ds : '+str(ds)
print 1440/8
