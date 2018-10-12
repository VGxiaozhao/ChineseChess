# encoding:utf-8
import copy
import numpy as np
import board

initPad = [
    ['Ju', 8, 9],
    ['Ma', 7, 9],
    ['Xiang', 6, 9],
    ['Shi', 5, 9],
    ['Jiang', 4, 9],
    ['Shi', 3, 9],
    ['Xiang', 2, 9],
    ['Ma', 1, 9],
    ['Ju', 0, 9],
    ['Pao', 7, 7],
    ['Pao', 1, 7],
    ['Bing', 8, 6],
    ['Bing', 6, 6],
    ['Bing', 4, 6],
    ['Bing', 2, 6],
    ['Bing', 0, 6],
    ['Ju', 0, 0],
    ['Ma', 1, 0],
    ['Xiang', 2, 0],
    ['Shi', 3, 0],
    ['Jiang', 4, 0],
    ['Shi', 5, 0],
    ['Xiang', 6, 0],
    ['Ma', 7, 0],
    ['Ju', 8, 0],
    ['Pao', 1, 2],
    ['Pao', 7, 2],
    ['Bing', 0, 3],
    ['Bing', 2, 3],
    ['Bing', 4, 3],
    ['Bing', 6, 3],
    ['Bing', 8, 3]
]


def fanzhuan(lst):
    rlst = []
    for i in range(16, 32):
        tmp = []
        tmp.append(lst[i][0])
        if lst[i][1] != 9 or lst[i][2] != 9:
            tmp.append(8 - lst[i][1])
            tmp.append(9 - lst[i][2])
        else:
            tmp.append(9)
            tmp.append(9)
        rlst.append(tmp)
    for i in range(16):
        tmp = []
        tmp.append(lst[i][0])
        if lst[i][1] != 9 or lst[i][2] != 9:
            tmp.append(8 - lst[i][1])
            tmp.append(9 - lst[i][2])
        else:
            tmp.append(9)
            tmp.append(9)
        rlst.append(tmp)
    return rlst


def getTurn(lst, x, y):
    for i in range(32):
        if lst[i][1] == ord(x) - ord('0') and lst[i][2] == ord(y) - ord('0'):
            if i < 16:
                return 1
            else:
                return 0
    return 1


# 返回一个9*10*64的数组
def Matrix(lst):
    ret = np.zeros((9, 10, 64))
    id = 0
    for i in range(len(lst)):
        if lst[i][1] != 9 or lst[i][2] != 9:
            ret[lst[i][1]][lst[i][2]][id] = 1.0
        id += 1
    return ret


def delvec(st, ed):
    for id in range(16, 32):
        if st[id][1] != 9 or st[id][2] != 9:
            if ed[id][1] != 9 or ed[id][2] != 9:
                if st[id][1] != ed[id][1] or st[id][2] != ed[id][2]:
                    a = [0.0 for ij in range(1440)]
                    lei = (id - 16) * 90 + ed[id][1] * 10 + ed[id][2]
                    a[lei] = 1.0
                    return np.array(a)


def movStone(x, y, nx, ny, lst):
    t = ord('0')
    x, y, nx, ny, id = ord(x) - t, ord(y) - t, ord(nx) - t, ord(ny) - t, 0
    for item in lst:
        if item[1] == x and item[2] == y:
            break
        id += 1
    j = 0
    for item in lst:
        if item[1] == nx and item[2] == ny and j != id:
            lst[j][1], lst[j][2] = 9, 9
            break
        j += 1
    lst[id][1] = nx
    lst[id][2] = ny


def getTrainData(file, sstag, edtag):
    ret = []
    outfile = file
    for i in range(sstag, edtag + 1):
        with open(outfile % i, 'r') as cin:
            lst = copy.deepcopy(initPad)
            row = 1
            # 每一行
            for line in cin:
                # 有初始行
                if row == 1 and line != '\n':
                    for j in range(0, len(line), 2):
                        if j + 1 < len(line):
                            x = ord(line[j]) - ord('0')
                            y = ord(line[j + 1]) - ord('0')
                            lst[int(j / 2)][1] = x
                            lst[int(j / 2)][2] = y
                # 棋谱行
                if row != 1 and line and len(line) > 2:
                    # 红方先行
                    red = getTurn(lst, line[0], line[1])
                    for j in range(0, len(line), 4):
                        # 初始向量
                        rlst = lst
                        if red:
                            rlst = fanzhuan(lst)
                        flst = copy.deepcopy(rlst)
                        stvec = Matrix(rlst)
                        ob = board.CBoard(rlst)
                        item1 = ob.setMat(stvec)
                        # 移动棋子
                        if j + 3 < len(line):
                            movStone(line[j], line[j + 1], line[j + 2], line[j + 3], lst)
                        # 结束向量
                        elst = lst
                        if red:
                            elst = fanzhuan(lst)
                        item2 = delvec(flst, elst)
                        if item2 is None:
                            break
                        # 红黑交替
                        red = 1 - red
                        ret.append((item1, item2))
                row += 1
    return ret


def getLastTD(file, sstag, edtag, index):
    ret = []
    for i in range(sstag, edtag + 1):
        tmp = getTrainData(file, i, i)
        for j in range(index, len(tmp)):
            ret.append(tmp[j])
    return ret


def board_show(lst):
    board = [['0' for i in range(9)] for i in range(10)]
    for item in lst:
        if item[1] != 9 or item[2] != 9:
            x = 8 - item[1]
            y = item[2]
            board[y][x] = item[0][0]
    for line in board:
        for x in line:
            print(x, end=' ')
        print('')


def Matrix_show(mat):
    id = 0
    for i1 in mat:
        for i2 in i1:
            for x in i2:
                if id % 10 == 0:
                    print("")
                if id % 90 == 0:
                    if id < 90 * 32:
                        print(initPad[int(id / 90) % 32][0])
                    else:
                        print(initPad[int(id / 90) % 32][0], "'s move")
                print(x, end=', ')
                id += 1


def show(mat):
    id = 0
    for i in range(64):
        if id < 32:
            print(initPad[int(id) % 32][0])
        else:
            print(initPad[int(id) % 32][0], "'s move")
        for j in range(9):
            for k in range(10):
                print(mat[j][k][i], end=', ')
            print("")
        id += 1


def strToMat(string):
    i, id = 0, 0
    lst = copy.deepcopy(initPad)
    while i < len(string):
        x = ord(string[i]) - ord('0')
        y = ord(string[i + 1]) - ord('0')
        lst[id][1] = x
        lst[id][2] = y
        id += 1
        i += 2
    ob = board.CBoard(lst)
    ret = np.vstack((Matrix(lst), ob.getMat()))
    return ret


def test():
    # line = '5499475749392599237343999999999968354230404160829978886383999999'
    line = '8979695949392919097717866646260600102030405060708012720323436383'
    ret = getTrainData('D:/qipu_dp/_%d.txt', 1, 1)
    print(ret[0][0].shape)
    show(ret[0][0])
    tmp = np.argmax(ret[0][1])
    print(initPad[int(tmp / 90)][0], int((tmp % 90) / 10), int(tmp % 90) % 10)
# test();
