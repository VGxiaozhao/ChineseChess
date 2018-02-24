import qipu2
import numpy as np
import copy
class CBoard:
	board = [[-1 for i in range(10)] for i in range(9)]
	pad = copy.deepcopy(qipu2.initPad)
	def say(self):
		for i in self.board:
			print(i)
	
	def __init__(self, lst):
		id = 0
		for item in lst:
			self.pad[id][1] = item[1]
			self.pad[id][2] = item[2]
			if item[1]!=9 or item[2]!=9:
				self.board[item[1]][item[2]] = id
			id += 1
	
	def get32(self):
		ret = []
		for i in range(32):
			ret.append(self.listMove(i))
		return ret
	
	def getVector(self):
		id = 0
		map = self.get32()
		tmp = []
		for ceng in map:
			for x in ceng:
				for y in x:
					tmp.append([y])
		return np.mat(tmp)
	
	def isEnemy(self, a, b):
		if a==-1 or b==-1:	return True
		if a<=15 and b>15:	return True
		if b<=15 and a>15:	return True
		return False
	
	def listMove(self, id):
		ret = [ [0.0 for i in range(10)] for i in range(9) ]
		if self.pad[id][1] == self.pad[id][2] and self.pad[id][1] == 9:
			return ret
		if self.pad[id][0] == 'Ju':
			lst = self.listMoveJu(id)
		if self.pad[id][0] == 'Ma':
			lst = self.listMoveMa(id)
		if self.pad[id][0] == 'Xiang':
			lst = self.listMoveXiang(id)
		if self.pad[id][0] == 'Shi':
			lst = self.listMoveShi(id)
		if self.pad[id][0] == 'Jiang':
			lst = self.listMoveJiang(id)
		if self.pad[id][0] == 'Bing':
			lst = self.listMoveBing(id)
		if self.pad[id][0] == 'Pao':
			lst = self.listMovePao(id)
		for i in lst:
			ret[i[0]][i[1]] = 1.0
		return ret;
			
	def listMoveJu(self, id):
		lst = []
		x, y = self.pad[id][1], self.pad[id][2]
		self.listMoveJuDfs(id, x, y, -1, 0, lst) 
		self.listMoveJuDfs(id, x, y, 1, 0, lst)
		self.listMoveJuDfs(id, x, y, 0, -1, lst)
		self.listMoveJuDfs(id, x, y, 0, 1, lst)
		return lst
		
	def listMoveJuDfs(self, id, x, y, dx, dy, lst):
		nx = x + dx
		ny = y + dy
		if nx<0 or nx>8 or ny<0 or ny>9:
			return
		if self.isEnemy(id, self.board[nx][ny]):
			lst.append((nx, ny))
		if self.board[nx][ny]==-1:
			self.listMoveJuDfs(id, nx, ny, dx, dy, lst)
			
	def listMovePao(self, id):
		lst = []
		x, y = self.pad[id][1], self.pad[id][2]
		self.listMovePaoDfs(id, x, y, -1, 0, False, lst) 
		self.listMovePaoDfs(id, x, y, 1, 0, False, lst)
		self.listMovePaoDfs(id, x, y, 0, -1, False, lst)
		self.listMovePaoDfs(id, x, y, 0, 1, False, lst)
		return lst
		
	def listMovePaoDfs(self, id, x, y, dx, dy, yue, lst):
		nx = x+dx
		ny = y+dy
		if nx<0 or nx>8 or ny<0 or ny>9:
			return
		if self.board[nx][ny]==-1:
			if not yue:
				lst.append( (nx,ny) )
			self.listMovePaoDfs(id, nx, ny, dx, dy, yue, lst)
		elif self.board[nx][ny]!=-1 and not yue:
			self.listMovePaoDfs(id, nx, ny, dx, dy, True, lst)
		elif yue and self.board[nx][ny]!=-1 and self.isEnemy(id, self.board[nx][ny]):
			lst.append((nx, ny))
	
	def isDown(self, id):
		J = 4
		if id>=16:	J=20
		down = False
		if self.pad[J][2] <= 2:	down = True
		return down
	
	def isOutRiver(self, id, y):
		down = self.isDown(id)
		if down and y>=5: return True
		elif not down and y<5:	return True
		return False
	
	def listMoveMa(self, id):
		lst = []
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [-2, -2, -1, -1, 1, 1, 2, 2]
		dy = [-1, 1, -2, 2, -2, 2, -1, 1]
		ax = [-1, -1, 0, 0, 0, 0, 1, 1]
		ay = [0, 0, -1, 1, -1, 1, 0, 0]
		for d in range(8):
			nx = x + dx[d]
			ny = y + dy[d]
			if nx>=0 and nx<=8 and ny>=0 and ny<=9 and self.isEnemy(id, self.board[nx][ny]):
				xx = x + ax[d]
				yy = y + ay[d]
				if xx>=0 and xx<=8 and yy>=0 and yy<=9 and self.board[xx][yy]==-1:
					lst.append( (nx, ny) )
		return lst
	
	def listMoveXiang(self, id):
		lst = []
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [ -2, -2, 2, 2 ]
		dy = [ -2, 2, -2, 2 ]
		ax = [ -1, -1, 1, 1 ]
		ay = [ -1, 1, -1, 1 ]
		for d in range(4):
			nx = x+dx[d]
			ny = y+dy[d]
			if nx>=0 and nx<=8 and ny>=0 and ny<=9 and not self.isOutRiver(id, ny) and self.isEnemy(id, self.board[nx][ny]):
				xx = x+ax[d]
				yy = y+ay[d]
				if xx>=0 and xx<=8 and yy>=0 and yy<=9 and self.board[xx][yy]==-1:
					lst.append((nx, ny))
		return lst
		
	def listMoveJiang(self, id):
		lst = []
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [ -1, 1, 0, 0 ]
		dy = [ 0, 0, -1, 1 ]
		for d in range(4):
			nx = x+dx[d]
			ny = y+dy[d]
			if nx>=3 and nx<=5 and ny>=0 and ny<=9 and self.isEnemy(id, self.board[nx][ny]):
				if self.isDown(id) and ny<=2:
					lst.append((nx, ny))
				if not self.isDown(id) and ny>=7:
					lst.append((nx, ny))
					
		#对面笑杀
		down = self.isDown(id)
		st, ed, step = 1,10,1
		if not down:
			st = -1
			ed = -10
			step = -1
		for d in range(st, ed+1, step):
			ny = y+d
			if ny>=0 and ny<=9 and self.board[x][ny]!=-1:
				if self.pad[self.board[x][ny]][0] == 'Jiang':
					lst.append((x, ny))
				else:
					break
		return lst
			
	def listMoveShi(self, id):
		lst = []
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [ -1, -1, 1, 1 ]
		dy = [ -1, 1, -1, 1 ]
		for d in range(4):
			nx = x+dx[d]
			ny = y+dy[d]
			if nx>=3 and nx<=5 and ny>=0 and ny<=9 and self.isEnemy(id, self.board[nx][ny]):
				if self.isDown(id) and ny<=2:
					lst.append((nx, ny))
				if not self.isDown(id) and ny>=7:
					lst.append((nx, ny))
		return lst
		
	def listMoveBing(self, id):
		lst = []
		x = self.pad[id][1]
		y = self.pad[id][2]
		if self.isOutRiver(id, self.pad[id][2]) and x-1>=0 and self.isEnemy(id, self.board[x-1][y]):
			lst.append((x-1,y))
		if self.isOutRiver(id, self.pad[id][2]) and x+1<=8 and self.isEnemy(id, self.board[x+1][y]):
			lst.append((x+1,y))
		d = -1
		if self.isDown(id): d=1
		if y+d>=0 and y+d<=9 and self.isEnemy(id, self.board[x][y+d]):
			lst.append((x,y+d))
		return lst

		
def test():
	x = CBoard(qipu2.initPad)
	x.say()
	mat = x.get32()
	id = 0
	vec = x.getVector()
	print(vec.shape)
	'''
	for ceng in mat:
		print (qipu2.initPad[id][0])
		id += 1
		for y in ceng:
			print(y)
	'''
			
def data_process(data):
	x = []
	y = []
	for item in data:
		tx = []
		for xx in item[0]:
			tx.append(float(xx[0]))
		tx = np.array(tx)
		x.append(tx)
		
		ty = []
		for yy in item[1]:
			ty.append(float(yy[0]))
		ty = np.array(ty)
		y.append(ty)
	return np.array(x), np.array(y)

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
						rls = copy.deepcopy(lst)
						if red:
							rls, rd = qipu2.fanzhuan(lst, dead)
						moveGen = CBoard(rls)
						moveVec = moveGen.getVector();
						stvec = np.vstack((stvec, moveVec))
						#移动棋子
						if j+3<len(line):
							qipu2.movStone(line[j], line[j+1], line[j+2], line[j+3], lst, dead)
						#结束向量
						edvec = qipu2.vectorize(lst, dead, red)
						ret.append((stvec, qipu2.deltavec1440(stvec, edvec)))
						#红黑交替
						red = 1-red
				row += 1
	return ret
	
if __name__ == '__main__':
	x = getTrainData('D:/qipu_dp/_%d.txt', 81950,81952)
	#test()
	print (x[0][0].shape)
	id = 0
	for i in x[0][0]:
		if id%10==0:
			print ('')
		if id%90==0:
			if id/90<32:
				print (qipu2.initPad[int(id/90)][0])
			else:
				print (qipu2.initPad[int(id/90)-32][0] + "'s move: ")
		print(i, end='')
		id += 1
	print("")
	id = 0
	for i in x[0][1]:
		if i>0.5:
			print (id)
			print (int(id/90), qipu2.initPad[int(id/90)][0])
			x = int((int(id)%90)/10)
			y = (int(id)%90)%10
			print (x,y)
		id += 1