"""
Adopted from the code by: Ricardo Bonna
Module description: This module presents an example of an MPEG4 decoder
modeled with SADF
"""


from typing import List, Tuple
import numpy as np
import sys


################### Macro Block classes ####################


class MacroBlock(object):
    def __init__(self, block, pos):
        self.block = block
        self.pos = pos

    def __repr__(self):
        return (
            "PosB {block = fromLists "
            + np.array2string(self.block, separator=", ")
            + ", pos = "
            + str(tuple(self.pos))
            + "}"
        )

    def __str__(self):
        return (
            "PosB {block = fromLists "
            + np.array2string(self.block, separator=", ")
            + ", pos = "
            + str(tuple(self.pos))
            + "}"
        )


class FullB(MacroBlock):
    def __init__(self, block, pos, motionV):
        MacroBlock.__init__(self, block, pos)
        self.motionV = motionV

    def __repr__(self):
        return (
            "FullB {block = fromLists "
            + np.array2string(self.block, separator=", ")
            + ", pos = "
            + str(tuple(self.pos))
            + ", motionV = "
            + str(tuple(self.motionV))
            + "}"
        )

    def __str__(self):
        return (
            "FullB {block = fromLists "
            + np.array2string(self.block, separator=", ")
            + ", pos = "
            + str(tuple(self.pos))
            + ", motionV = "
            + str(tuple(self.motionV))
            + "}"
        )


# Split a large block into a listo of macro blocks of size d = (dr,dc) or smaller
def frame2mblocks(d: Tuple[int, int], frame: np.ndarray) -> List[MacroBlock]:
    (dr, dc) = d
    result = []
    i = 0
    while i < frame.shape[0]:
        j = 0
        while j < frame.shape[1]:
            result.append(
                MacroBlock(frame[i : i + dr, j : j + dc], np.array([i + 1, j + 1]))
            )
            j += dc
        i += dr
    return result


################### Input creation functions ####################

# uncomment the line below if you want to put these functions on a different file
# import numpy as np


# Generates a random input stream of frame types
# arguments: size = length of the output stream
#            nb = number of blocks per frame
def genFtStream(size: int, nb: int) -> List[str]:
    #    ft_table = ['I','P0','P30','P40','P50','P60','P70','P80','P99']
    ft_table = ["I", "P0", "P30", "P40", "P50", "P60"]
    ft = ["I"]
    for i in range(size - 1):
        a = np.random.randint(1, len(ft_table))
        ft.append(ft_table[a])
    #        if a == 0:
    #            ft.append('I')
    #        else:
    #            ft.append('P'+str(a))
    return ft


# Generates a random input stream of macro blocs based on a list of frame types
# arguments: frameTypeList = list outputted by genFtStream
#            fs = tuple with the frame size
#            bs = block size
def genInpStream(frameTypeList: List[str], fs: Tuple[int, int], bs: int) -> List[Tuple]:
    output = []
    for i in frameTypeList:
        if i == "I":
            output += frame2mblocks(
                (bs, bs), (256 * np.random.rand(fs[0], fs[1])).astype(int)
            )
        elif i[0] == "P":
            a = int(i[1:])
            posList = [
                np.array([a, b])
                for a in range(1, fs[0] - bs + 2, bs)
                for b in range(1, fs[0] - bs + 2, bs)
            ]
            output += [
                FullB(
                    (256 * np.random.rand(bs, bs)).astype(int),
                    posList.pop(np.random.randint(0, len(posList))),
                    (np.random.randint(2 * bs + 1, size=2) - bs).astype(int),
                )
                for j in range(a)
                if posList
            ]
    return output


################### Save to file ####################


def saveInpsToFile(ft, mb, ftFile="ft.inp", mbFile="mbInputs.inp"):
    ft = str(ft).replace("'", '"')
    mb = str(mb).replace("\n", "")
    f = open(ftFile, "w")
    f.write(ft)
    f.close()
    f = open(mbFile, "w")
    f.write(mb)
    f.close()
    return


################### Execution as main ####################

if __name__ == "__main__":
    args = sys.argv[1:]
    np.set_printoptions(threshold=sys.maxsize)
    fs = (int(args[0]), int(args[1]))
    bs = int(args[2])
    nb = int(fs[0] * fs[1] / (bs**2))
    ft = genFtStream(int(args[3]), nb)
    mb = genInpStream(ft, fs, bs)
    saveInpsToFile(ft, mb)
