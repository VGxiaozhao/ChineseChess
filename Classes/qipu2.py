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

def vectorize(lst, dead, red=False):
	if red:
		rlst, rdead = fanzhuan(lst, dead)
	else:
		rlst = copy.deepcopy(lst)
		rdead = copy.deepcopy(dead)
	ret = np.zeros((90*32,1))
	lei = 0
	for i in range(len(rlst)):
		if not dead[i]:
			if rlst[i][1]!=9 or rlst[i][2]!=9:
				tmp = lei + rlst[i][1]*10 + rlst[i][2]
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
				
def deltavec(st, ed):
	ret = np.zeros(st.shape)
	for i in range(90*32):
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
	try:
		lst[id][1] = nx
		lst[id][2] = ny
	except:
		print("error at %d"%id)
	return getScore(flag)
	
def deltavec1440(st, ed):
	ret = np.zeros((1440,1))
	for i in range(1440, 2880):
		if ed[i]>0.5 and st[i]<0.5:
			ret[i-1440]=1.0
			return ret
	return ret
	
#主函数用于测试某些棋谱文件是否正常
if __name__ == '__main__':
	outfile = 'D:/qipu_dp/_%d.txt'
	for i in range(81950,82000):
		last = i
		if last:
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

def fanzhuan(lst, dead):
	rlst, rdead = [], []
	for i in range(16,32):
		rdead.append(dead[i])
		tmp = [] 
		tmp.append(lst[i][0])
		tmp.append(8-lst[i][1])
		tmp.append(9-lst[i][2])
		rlst.append(tmp)
	for i in range(16):
		rdead.append(dead[i])
		tmp = [] 
		tmp.append(lst[i][0])
		tmp.append(8-lst[i][1])
		tmp.append(9-lst[i][2])
		rlst.append(tmp)
	return rlst, rdead
			
def getTrainData(file, sstag, edtag):
	ret = []
	outfile = file
	for i in range(sstag, edtag+1):
		with open(outfile%i, 'r') as cin:
			lst = copy.deepcopy(initPad)
			dead = [0 for i in range(32)]
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
							if x==9 and y==9:
								dead[int(j/2)] = 1
				#棋谱行
				if row!=1:
					#红方先行
					red = 1
					for j in range(0, len(line), 4):
						#初始向量
						stvec = vectorize(lst, dead, red)
						#移动棋子
						if j+3<len(line):
							movStone(line[j], line[j+1], line[j+2], line[j+3], lst, dead)
						#结束向量
						edvec = vectorize(lst, dead, red)
						ret.append((stvec, deltavec1440(stvec, edvec)))
						#红黑交替
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