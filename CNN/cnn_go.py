import os
import copy
import move
import board
import numpy as np
from model import *
from config import *


def cast(string, res=None):
    pad = copy.deepcopy(move.initPad)
    i = 0
    while i < len(string):
        x = ord(string[i]) - ord('0')
        y = ord(string[i + 1]) - ord('0')
        id = int(i / 2)
        pad[id][1] = x
        pad[id][2] = y
        i += 2
    move.board_show(pad)
    vec = move.Matrix(pad)
    ob = board.CBoard(pad)
    item = ob.setMat(vec)
    ret = sess.run(h_output, feed_dict={x_: [item]})
    ret = ret[0]
    cnt = 0
    while True:
        tmp = int(np.argmax(ret))
        id = int(tmp / 90) + 16
        x = int((tmp % 90) / 10)
        y = (tmp % 90) % 10
        if ret[tmp] < 0.000001:
            return
        if (x, y) in ob.mov[id]:
            if type(res) == list:
                if ret[tmp] > MOVE_THRESHOLD:
                    res.append((tmp, ret[tmp]))
                # if len(res)>6 or ret[tmp]<0.000001:
                if ret[tmp] < MOVE_THRESHOLD:
                    return
            else:
                return tmp
        ret[tmp] = 0.0


def main():
    print('initial complete')
    for i in range(1):
        lst = []
        print(cast('8979695949392919097717866646260600102030405060708012720323436383', lst))
        print(lst)
    sum = 0.0
    for item in lst:
        sum += item[1]
    print('len:%d sum:%g' % (len(lst), sum))
    txtBoard = "d:/txtboard.txt"
    tagBoard = "d:/tagboard.txt"
    txtMove = "d:/txtmove.txt"
    tagMove = "d:/tagmove.txt"
    while True:
        # 标识棋盘已写
        if os.path.exists(tagBoard):
            with open(txtBoard, 'r') as cin:
                for line in cin:
                    pass
            print(line)
            lst = []
            ret = cast(line, lst)
            f = open(txtMove, 'w')
            for ret in lst:
                print(ret)
                f.write(str(ret[0]) + '\n')
            f.flush()
            f.close()
            os.remove(tagBoard)
            ff = open(tagMove, 'w')
            ff.close()


if __name__ == '__main__':
    main()
