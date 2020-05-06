import os

lines = open('2018_HLT_IsoMu24_v13.txt').readlines()

for line in lines:
  words = line.split()

  print 'print "#### '+words[0]+'"'
  print 'print '+words[0]
