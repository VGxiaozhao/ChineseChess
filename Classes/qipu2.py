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
	
def getScore(id):
	if id==-1:
		return 0
	if initPad[id][0] == 'Ju':
		return 18
	if initPad[id][0] == 'Ma':
		return 8
	if initPad[id][0] == 'Pao':
		return 9
	if initPad[id][0] == 'Bing':
		return 2
	if initPad[id][0] == 'Xiang':
		return 4
	if initPad[id][0] == 'Shi':
		return 4
	if initPad[id][0] == 'Jiang':
		return 50
	return 0

def vectorize(lst, dead):
	ret = np.zeros((90*32,1))
	lei = 0
	for i in range(len(lst)):
		if not dead[i]:
			tmp = lei + lst[i][1] + lst[i][2] * 9
			ret[tmp] = 1.0
		lei += 90
	return ret;

def outPut(data):
	with open("d:/test.txt", 'w' ) as cout:
		for item in data:
			for cc in item:
				for i in cc:
					cout.write('%f'%i)
					cout.write(', ')
				cout.write('\n')
				
def vec2pad(vec):
	ret = []
	idx = 0
	for i in vec:
		if i>0.5:
			tmp = []
			w = idx / 90
			x = (idx%90) % 9
			y = (idx%90) / 9
			'''
			tmp.append[initPad[w][0]]
			tmp.append[x]
			tmp.append[y]
			ret.append(tmp)
			'''
		idx += 1
	return ret

def deltavec(st, ed):
	ret = np.zeros(st.shape)
	for i in range(0, 90*32):
		if ed[i]>0.5 and st[i]<0.5:
			ret[i]=1.0
			return ret
	return ret

def movStone(x,y,nx,ny,lst,dead):
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
	flag = -1
	for item in lst:
		if item[1]==nx and item[2]==ny and j!=id:
			dead[j] = 1
			flag = j
			lst[j][1], lst[j][2] = 9,9
			break
		j += 1
	lst[id][1] = nx
	lst[id][2] = ny
	return getScore(flag)
	
def deltavec1440(st, ed):
	ret = np.zeros((1440,1))
	for i in range(1440, 90*32):
		if ed[i]>0.5 and st[i]<0.5:
			ret[i-1440]=1.0
			return ret
	return ret
	
#主函数用于测试某些棋谱文件是否正常
if __name__ == '__main__':
	outfile = 'D:/lisan/_%d.txt'
	for i in range(1,1078000):
		last = i
		if last%2000==0:
			print  ('ok in %d'%last)
			
		try:
			with open(outfile%i, 'r') as cin:
				lst = copy.deepcopy(initPad)
				dead = [0 for i in range(32)]
				row = 1
				for line in cin:
					if row==1 and line != '\n':
						for j in range(0, len(line), 2):
							if j+1<len(line):
								x = ord(line[j]) - ord('0')
								y = ord(line[j+1]) - ord('0')
								lst[int(j/2)][1] = x
								lst[int(j/2)][2] = y
								if x==9 and y==9:
									dead[int(j/2)] = 1
					if row!=1:
						red = 1
						for j in range(0, len(line), 4):
							if j+3<len(line):
								movStone(line[j], line[j+1], line[j+2], line[j+3], lst, dead)
							red = 1-red
					row += 1
		except:
			print( last)
			r = random.randint(1, 78000)
			os.system('cp D:/qipu_xqd/_%d.txt D:/lisan/_%d.txt'%(r, last))
		
def getTrainData(file, sstag, edtag):
	ret = []
	outfile = file
	for i in range(sstag, edtag+1):
		with open(outfile%i, 'r') as cin:
			lst = copy.deepcopy(initPad)
			dead = [0 for i in range(32)]
			row = 1
			for line in cin:
				if row==1 and line != '\n':
					for j in range(0, len(line), 2):
						if j+1<len(line):
							x = ord(line[j]) - ord('0')
							y = ord(line[j+1]) - ord('0')
							lst[int(j/2)][1] = x
							lst[int(j/2)][2] = y
							if x==9 and y==9:
								dead[int(j/2)] = 1
				if row!=1:
					red = 1
					for j in range(0, len(line), 4):
						stvec = vectorize(lst, dead)
						if j+3<len(line):
							movStone(line[j], line[j+1], line[j+2], line[j+3], lst, dead)
						if not red:
							edvec = vectorize(lst, dead)
							ret.append((stvec, deltavec1440(stvec, edvec)))
						red = 1-red
				row += 1
	return ret
				
def getTestData(file, sstag, edtag):
	tmp = getTrainData(file, sstag, edtag)
	ret = []
	for item in tmp:
		y = np.argmax(item[1])
		ret.append( (item[0], y) )
	return ret