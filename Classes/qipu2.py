#encoding:utf-8
import os,sys,copy
import numpy as np
import random
initPad = [
['Ju', 8, 9 ],
['Ma', 7, 9 ],
['Xiang', 6, 9 ],
['Shi', 5, 9 ],
['Jiang', 4, 9 ],
['Shi', 3, 9 ],
['Xiang', 2, 9 ],
['Ma', 1, 9 ],
['Ju', 0, 9 ],
['Pao', 7, 7 ],
['Pao', 1, 7 ],
['Bing', 8, 6 ],
['Bing', 6, 6 ],
['Bing', 4, 6 ],
['Bing', 2, 6 ],
['Bing', 0, 6 ],
['Ju', 0, 0 ],
['Ma', 1, 0 ],
['Xiang', 2, 0 ],
['Shi', 3, 0 ],
['Jiang', 4, 0 ],
['Shi', 5, 0 ],
['Xiang', 6, 0 ],
['Ma', 7, 0 ],
['Ju', 8, 0 ],
['Pao', 1, 2 ],
['Pao', 7, 2 ],
['Bing', 0, 3 ],
['Bing', 2, 3 ],
['Bing', 4, 3 ],
['Bing', 6, 3 ],
['Bing', 8, 3 ]
]
	
def fanzhuan(lst):
	rlst= []
	for i in range(16,32):
		tmp = [] 
		tmp.append(lst[i][0])
		if lst[i][1]!=9 or lst[i][2]!=9:
			tmp.append(8-lst[i][1])
			tmp.append(9-lst[i][2])
		else:
			tmp.append(9)
			tmp.append(9)
		rlst.append(tmp)
	for i in range(16):
		tmp = [] 
		tmp.append(lst[i][0])
		if lst[i][1]!=9 or lst[i][2]!=9:
			tmp.append(8-lst[i][1])
			tmp.append(9-lst[i][2])
		else:
			tmp.append(9)
			tmp.append(9)
		rlst.append(tmp)
	return rlst
			
def vectorize(lst, red=False):
	if red:
		rlst = fanzhuan(lst)
	else:
		rlst = copy.deepcopy(lst)
	ret = np.array([0.0 for i in range(2880)])
	lei = 0
	for i in range(len(rlst)):
		if rlst[i][1]!=9 or rlst[i][2]!=9:
			tmp = lei + rlst[i][1]*10 + rlst[i][2]
			ret[tmp] = 1.0
		lei += 90
	return ret;
	
def movStone(x,y,nx,ny,lst):
	t = ord('0')
	x = ord(x) - t
	y = ord(y) - t
	nx = ord(nx) - t
	ny = ord(ny) - t
	id = 0
	for item in lst:
		if item[1]==x and item[2]==y:
			break
		id+=1
	j = 0
	for item in lst:
		if item[1]==nx and item[2]==ny and j!=id:
			lst[j][1], lst[j][2] = 9,9
			break
		j += 1
	lst[id][1] = nx
	lst[id][2] = ny
	
def deltavec1440(st, ed):
	ret = np.array([0.0 for i in range(1440)])
	for i in range(1440, 2880):
		if ed[i]>0.5 and st[i]<0.5:
			ret[i-1440]=1.0
			return ret

def getTurn(lst, x, y):
	for i in range(32):
		if lst[i][1] == ord(x) - ord('0') and lst[i][2] == ord(y) - ord('0'):
			if i<16:
				return 1
			else:
				return 0
	return 1
			
def getTrainData(file, sstag, edtag):
	retx, rety = [], []
	outfile = file
	for i in range(sstag, edtag+1):
		with open(outfile%i, 'r') as cin:
			lst = copy.deepcopy(initPad)
			row = 1
			#每一行
			for line in cin:
				#有初始行
				if row==1 and line != '\n':
					for j in range(0, len(line), 2):
						if j+1<len(line):
							x = ord(line[j]) - ord('0')
							y = ord(line[j+1]) - ord('0')
							lst[int(j/2)][1] = x
							lst[int(j/2)][2] = y
				#棋谱行
				if row!=1 and len(line)>2:
					#红方先行
					red = getTurn(lst, line[0], line[1])
					index = 1
					for j in range(0, len(line), 4):
						#print('??????')
						#初始向量
						stvec = vectorize(lst, red)
						#移动棋子
						if j+3<len(line):
							movStone(line[j], line[j+1], line[j+2], line[j+3], lst)
						#结束向量
						edvec = vectorize(lst, red)
						tmp = deltavec1440(stvec, edvec)
						if tmp is None:
							break;
						rety.append(tmp)
						retx.append(copy.deepcopy(stvec))
						#ret.append((stvec, deltavec1440(stvec, edvec)))
						#红黑交替
						red = 1-red
						index += 1
				row += 1
	return np.array(retx), np.array(rety)

def show(data):
	index = 0
	for i in data:
		if index%10==0:
			print('')
		if index%90==0:
			print(initPad[int(index/90)%32][0])
		print (i, end=', ')
		index+=1
	
if __name__ == '__main__':
	x, y = getTrainData('D:/lisan/_%d.txt', 397,397)
	index = 1
	print (x.shape)
	for i in y:
		try:
			print(index, i.shape)
			index+= 1
		except:
			print('err:%d'%index)
			index+= 1