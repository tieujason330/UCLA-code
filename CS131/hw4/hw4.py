
# Name: Jason Tieu
#
# UID: 304047667
#
# People I interacted with:
#
# Resources I used:
#
"""
https://docs.python.org/2/library/struct.html

http://netpbm.sourceforge.net/doc/ppm.html

http://stackoverflow.com/questions/1712227/how-to-get-the-size-of-a-list

http://stackoverflow.com/questions/3204245/how-do-i-convert-a-tuple-of-tuples-to-a-one-dimensional-list-using-list-comprehe

http://stackoverflow.com/questions/961632/converting-integer-to-string-in-python

http://stackoverflow.com/questions/10411085/converting-integer-to-binary-in-python

http://stackoverflow.com/questions/9184107/python-file-write-new-line

http://askubuntu.com/questions/417757/create-a-file-before-writing-into-it-using-python

https://docs.python.org/2/tutorial/datastructures.html

http://stackoverflow.com/questions/23286254/convert-list-to-a-list-of-tuples-python

https://docs.python.org/2/library/functions.html#reduce

http://stackoverflow.com/questions/3277503/python-read-file-line-by-line-into-array

https://piazza.com/class/i10rs0q03qt7lj

http://stackoverflow.com/questions/642154/how-to-convert-strings-into-integers-in-python

http://stackoverflow.com/questions/1163459/reading-integers-from-binary-file-in-python

http://stackoverflow.com/questions/12377473/python-write-versus-writelines-and-concatenated-strings

http://stackoverflow.com/questions/9644110/difference-between-parsing-a-text-file-in-r-and-rb-mode
"""

import math
import struct

# PROBLEM 1

# parse the file named fname into a dictionary of the form 
# {'width': int, 'height' : int, 'max' : int, 'pixels' : (int * int * int) list}
def parsePPM(fname):
    ppm_dict = {}
    f = open(fname, 'rb')
    
    magicnum = f.readline().split() #1st line
    width_height = f.readline().split() #2nd line
    max_val = f.readline().split() #3rd line

    #add to dict
    ppm_dict['width'] = int(width_height[0])
    ppm_dict['height'] = int(width_height[1])
    ppm_dict['max'] = int(max_val[0])
 
    #rest of the lines are RGB values, so read in 3 bytes at a time
    pixels_list = []
    
    rgbBytes = f.read(3)

    while rgbBytes != '':
    #use pack to parse binary bytes into ints
        n = struct.unpack('BBB', rgbBytes) #unpacks 3 bytes at a time gives (a,b,c) tuple
        pixels_list.append(n)
        rgbBytes = f.read(3) #read next 3 bytes

    #add 'pixels' to dictionary
    ppm_dict['pixels'] = pixels_list
    f.close()
    #print 'test5'
    return ppm_dict

# a test to make sure you have the right format for your dictionaries
def testParsePPM():
    return parsePPM("example.ppm") == {'width': 2, 'max': 255, 'pixels': [(10, 23, 52), (82, 3, 215), (30, 181, 101), (33, 45, 205), (40, 68, 92), (111, 76, 1)], 'height': 3}

# write the given ppm dictionary as a PPM image file named fname
# the function should not return anything
def unparsePPM(ppm, fname):
    f = open(fname, 'wb') #wb for binary mode so end of line characters aren't auto-converted to OS specific ones
    #must be strings
    f.write('P6') #1
    f.write('\n') #2
    f.write(str(ppm['width'])) #3
    f.write(' ') #4
    f.write(str(ppm['height'])) #5
    f.write('\n') #6
    f.write(str(ppm['max'])) #7
    f.write('\n') #8
    #9 need to convert ints back to binary
    listoftups = ppm['pixels']
    
    #must use pack to convert int into bytes
    bytesline = [struct.pack('BBB', R, G, B) for (R,G,B) in listoftups]
        
    #print '-------------------------------'
    #print bytestring
    #print '--------------------------------'
    f.writelines(bytesline) #writelines for list of strings

    f.close()


# PROBLEM 2
def negate(ppm):
    nppm = ppm.copy()
    Max = int(ppm['max'])
    #convert list of tuples into a list for easy manipulation
    listoftups = ppm['pixels']
    listofints = [i for tup1 in listoftups for i in tup1]

    negatelist = map((lambda v: Max-v), listofints)

    #convert list into list of tuples (3 elements)
    it = iter(negatelist)
    negatelist = zip(it,it,it)

    nppm['pixels'] = negatelist

    return nppm


# PROBLEM 3
def mirrorImage(ppm):
    mirr_ppm = ppm.copy()
    #want to flip the tuples only for the widths
    width = int(ppm['width'])
    listoftups = ppm['pixels']

    for i in range(0,len(listoftups), width):
        listoftups[i:i+width] = reversed(listoftups[i:i+width])

    mirr_ppm['pixels'] = listoftups
    return mirr_ppm

# PROBLEM 4

# produce a greyscale version of the given ppm dictionary.
# the resulting dictionary should have the same format, 
# except it will only have a single value for each pixel, 
# rather than an RGB triple.
def greyscale(ppm):
    gppm = ppm.copy()
    listoftups = ppm['pixels']
    listofavg = [int(round(.299*R + .587*G + .114*B)) for (R,G,B) in listoftups]
    gppm['pixels'] = listofavg
    return gppm

# take a dictionary produced by the greyscale function and write it as a PGM image file named fname
# the function should not return anything
def unparsePGM(pgm, fname):
    f = open(fname, 'wb') #wb for binary mode so end of line characters aren't auto-converted to OS specific ones
    #must be strings
    f.write('P5') #1
    f.write('\n') #2
    f.write(str(pgm['width'])) #3
    f.write(' ') #4
    f.write(str(pgm['height'])) #5
    f.write('\n') #6
    f.write(str(pgm['max'])) #7
    f.write('\n') #8
    #9 need to convert ints back to binary
    listofints = pgm['pixels']

    #must use pack to convert int into bytes
    bytesline = [struct.pack('B', x) for x in listofints]
        
    #print '-------------------------------'
    #print bytestring
    #print '--------------------------------'
    f.writelines(bytesline)

    f.close()


# PROBLEM 5

# gaussian blur code adapted from:
# http://stackoverflow.com/questions/8204645/implementing-gaussian-blur-how-to-calculate-convolution-matrix-kernel
def gaussian(x, mu, sigma):
  return math.exp( -(((x-mu)/(sigma))**2)/2.0 )

def gaussianFilter(radius, sigma):
    # compute the actual kernel elements
    hkernel = [gaussian(x, radius, sigma) for x in range(2*radius+1)]
    vkernel = [x for x in hkernel]
    kernel2d = [[xh*xv for xh in hkernel] for xv in vkernel]

    # normalize the kernel elements
    kernelsum = sum([sum(row) for row in kernel2d])
    kernel2d = [[x/kernelsum for x in row] for row in kernel2d]
    return kernel2d

# blur a given ppm dictionary, returning a new dictionary  
# the blurring uses a gaussian filter produced by the above function
def gaussianBlur(ppm, radius, sigma):
    # obtain the filter
    gausppm = ppm.copy()
    gfilter = gaussianFilter(radius, sigma)

    filterLen = len(gfilter)
    pixels = ppm['pixels']
    w  = ppm['width']
    h = ppm['height']
    newpixs = list(pixels)
    
    for x in range(radius, h-radius):
        for y in range(radius, w-radius):
            newRGB = [0,0,0]
            for i in range(-radius, radius+1):
                n1 = radius+i
                for j in range(-radius, radius+1):
                    index = (x+i)*w+y+j #can't be < 0 and > len(pixels)
                    n2 = radius+j
                    newRGB[0] += pixels[index][0] * gfilter[n1][n2] #R
                    newRGB[1] += pixels[index][1] * gfilter[n1][n2] #G
                    newRGB[2] += pixels[index][2] * gfilter[n1][n2] #B
            newpixs[x*w+y] = (int(round(newRGB[0])), int(round(newRGB[1])), int(round(newRGB[2])))
    gausppm['pixels'] = newpixs
    return gausppm

