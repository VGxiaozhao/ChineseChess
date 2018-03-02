import qipu2
import copy

x = qipu2.getTestData('D:/qipu_dp/_%d.txt', 81950,81950)
print (x)
def see(idx):
	idx = int(idx)
	w = int(idx / 90)
	x = (idx%90) % 9
	y = int ((idx%90) / 9)
	print (qipu2.initPad[w][0], end=',')
	print (x,y)
	
for item in x:
	#print (item[1])
	see(item[1])

def pve(data):
	id = 1
	for i in data:
		if i[0]>0.1:
			print ('id=%d'%id, end=',')
			print(i[0])
		id+=1

def getTrainData(file, sstag, edtag):
	ret = []
	outfile = file
	for i in range(sstag, edtag+1):
		with open(outfile%i, 'r') as cin:
			lst = copy.deepcopy(qipu2.initPad)
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
						stvec = qipu2.vectorize(lst, dead, red)
						#移动棋子
						if j+3<len(line):
							qipu2.movStone(line[j], line[j+1], line[j+2], line[j+3], lst, dead)
						#结束向量
						edvec = qipu2.vectorize(lst, dead, red)
						ret.append((stvec, qipu2.deltavec1440(stvec, edvec)))
						pve(qipu2.deltavec1440(stvec, edvec))
						#红黑交替
						red = 1-red
						break
				row += 1
	return ret
	
x = getTrainData('D:/qipu_dp/_%d.txt', 81950,81950)

j = 0
for i in x[0]:
	print (i, end=',')
	j+=1
	if j>5:
		break
print (x[0])