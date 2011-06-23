# This is a script designed to take two images and interlace them. For a usefullness
# point of view the images should be left and right eye images which one wishes to
# display on a JVC micro-polarized display (or any other display using interlaced
# stereo)

import Image
import sys
import numpy

def interlace(left, right, stereo):
    'simple function to interlace 2 images'
    lefta = numpy.asarray(Image.open(left))
    righta = numpy.asarray(Image.open(right))
    stereoa = numpy.copy(lefta)
    count = 0

    for row in righta:
        if count % 2 == 0:
            stereoa[count,:,:] = row
        count += 1
    stereoi = Image.fromarray(stereoa)
    stereoi.save(stereo)
    
if __name__ == "__main__":
    'assumes the arguments are of the form left right stereo'
    interlace(sys.argv[1], sys.argv[2], sys.argv[3])
