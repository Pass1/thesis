# This script is designed to split up a frame 5440x30720 into 40 subframes 1360x768 to fit zone 2 (AESOP diplsay).
# Written by Chris Knox and Luca Passone.

import sys
import Image
import os

def split_on_nodes(filename):
    'split a file up to fill the nodes of zone 2'
    orig = Image.open(filename)
    z2_0_0 = orig.crop((0, 0, 1360, 3072))
    z2_0_0.load()
    z2_0_0.save("z2-0-0-" + filename)

def split_on_screens(filename, rows, cols, width, height):
    im = Image.open(filename)
    base, ext = os.path.splitext(filename)
    output_dir = "screens/"
    if not os.path.exists(output_dir + os.path.dirname(filename) ):
        os.makedirs(output_dir + os.path.dirname(filename))
    for i in range(rows):
        for j in range(cols):
            im_crop = im.crop ((j * width, i * height, (j + 1) * width, (i + 1) * height))
            im_crop.load()
            im_crop.save(output_dir + base + "_00" + str(j) + "_00" + str(i) + ext)

if __name__ == "__main__":
    print len(sys.argv), "\n"
    if (len(sys.argv)==4):
        split_on_screens(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), 1360, 768)
    else:
        print "You need to specify 3 parameters: filename x_screens y_screens\nExiting\n"
