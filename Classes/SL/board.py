import numpy as np
import copy
class CBoard:
	def say(self):
		for i in self.board:
			print(i)
	
	#构造函数传入一个list，结构在ms.py的initPad中体现
	def __init__(self, lst):
		self.board = [[-1 for i in range(10)] for i in range(9)]
		self.pad = copy.deepcopy(lst)
		id = 0
		for item in self.pad:
			if item[1]!=9 or item[2]!=9:
				self.board[item[1]][item[2]] = id
			id += 1
		self.eat = [[] for i in range(32)]
		self.pro = [[] for i in range(32)]
		self.mov = [[] for i in range(32)]
	
	def getMat(self):
		for i in range(32):
			self.listMove(i)
		ret = []
		flag_check1, flag_check2 = 0, 0
		for it in self.mov:
			tmp = [[0.0 for tmp_i1 in range(10)] for tmp_i2 in range(9)]
			if it:
				for mv in it:
					tmp[mv[0]][mv[1]] = 1.0
			ret.append(tmp)
		'''
		#保护层
		for it in self.pro:
			tmp = [[0.0 for i in range(10)] for i in range(9)]
			for mv in it:
				tmp[mv[0]][mv[1]] = 1.0
			ret.append(tmp)
		#攻击层
		for it in self.eat:
			tmp = [[0.0 for i in range(10)] for i in range(9)]
			for mv in it:
				tmp[mv[0]][mv[1]] = 1.0
				if self.board[mv[0]][mv[1]] == 4:
					flag_check1 = 1
				if self.board[mv[0]][mv[1]] == 20:
					flag_check2 = 1
			ret.append(tmp)
		#下面是被将军层的添加
		yao = [[1.0 for i in range(10)] for i in range(9)]
		lin = [[0.0 for i in range(10)] for i in range(9)]
		if flag_check1:
			ret.append(copy.deepcopy(yao))
		else:
			ret.append(copy.deepcopy(lin))
		if flag_check2:
			ret.append(copy.deepcopy(yao))
		else:
			ret.append(copy.deepcopy(lin))
		'''
		return ret
	
	def setMat(self, mat):
		self.mov = [[] for i in range(32)]
		for i in range(32):
			self.listMove(i)
		for i in range(32):
			for item in self.mov[i]:
				x = item[0]
				y = item[1]
				mat[x][y][i+32] = 1.0
		return np.array(mat)
	
	def getVector(self, red):
		id, tmp = 0, []
		ret = self.getMat()
		for ceng in ret:
			for x in ceng:
				for y in x:
					tmp.append([y])
		t = 1.0
		if red:	t=0.0
		for i in range(90):
			tmp.append([t])
		return np.mat(tmp)
	
	def isEnemy(self, a, b):
		if a==-1 or b==-1:	return False
		if a<=15 and b>15:	return True
		if b<=15 and a>15:	return True
		return False
		
	def isPartner(self, a, b):
		if a==-1 or b==-1:	return False
		if a<=15 and b<=15: return True
		if a>15  and b>15:	return True
		return False
	
	def listMove(self, id):
		ret = [ [0.0 for i in range(10)] for i in range(9) ]
		#已死判断，不搜索
		if self.pad[id][1] == self.pad[id][2] and self.pad[id][1] == 9:
			return []
		if self.pad[id][0] == 'Ju':
			self.listMoveJu(id)
		if self.pad[id][0] == 'Ma':
			self.listMoveMa(id)
		if self.pad[id][0] == 'Xiang':
			self.listMoveXiang(id)
		if self.pad[id][0] == 'Shi':
			self.listMoveShi(id)
		if self.pad[id][0] == 'Jiang':
			self.listMoveJiang(id)
		if self.pad[id][0] == 'Bing':
			self.listMoveBing(id)
		if self.pad[id][0] == 'Pao':
			self.listMovePao(id)
			
	def pushList(self, id, nx, ny):
		kid = self.board[nx][ny]
		if kid == -1:
			self.mov[id].append((nx, ny))
		elif self.isEnemy(id, kid):
			self.mov[id].append((nx, ny))
			self.eat[id].append((nx, ny))
		else:
			self.pro[id].append((nx, ny))
			
	def listMoveJu(self, id):
		x, y = self.pad[id][1], self.pad[id][2]
		self.listMoveJuDfs(id, x, y, -1, 0) 
		self.listMoveJuDfs(id, x, y, 1, 0)
		self.listMoveJuDfs(id, x, y, 0, -1)
		self.listMoveJuDfs(id, x, y, 0, 1)
		
	def listMoveJuDfs(self, id, x, y, dx, dy):
		nx = x + dx
		ny = y + dy
		if nx<0 or nx>8 or ny<0 or ny>9:
			return;
		self.pushList(id, nx, ny)
		if self.board[nx][ny]==-1:
			self.listMoveJuDfs(id, nx, ny, dx, dy)
			
	def listMovePao(self, id):
		x, y = self.pad[id][1], self.pad[id][2]
		self.listMovePaoDfs(id, x, y, -1, 0, False) 
		self.listMovePaoDfs(id, x, y, 1, 0, False)
		self.listMovePaoDfs(id, x, y, 0, -1, False)
		self.listMovePaoDfs(id, x, y, 0, 1, False)
		
	def listMovePaoDfs(self, id, x, y, dx, dy, yue):
		nx = x+dx
		ny = y+dy
		if nx<0 or nx>8 or ny<0 or ny>9:
			return
		if self.board[nx][ny]==-1:
			if not yue:
				self.pushList(id, nx, ny)
			self.listMovePaoDfs(id, nx, ny, dx, dy, yue)
		elif self.board[nx][ny]!=-1 and not yue:
			self.listMovePaoDfs(id, nx, ny, dx, dy, True)
		elif yue and self.board[nx][ny]!=-1:
			self.pushList(id, nx, ny)
	
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
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [-2, -2, -1, -1, 1, 1, 2, 2]
		dy = [-1, 1, -2, 2, -2, 2, -1, 1]
		ax = [-1, -1, 0, 0, 0, 0, 1, 1]
		ay = [0, 0, -1, 1, -1, 1, 0, 0]
		for d in range(8):
			nx = x + dx[d]
			ny = y + dy[d]
			xx = x + ax[d]
			yy = y + ay[d]
			if xx>=0 and xx<=8 and yy>=0 and yy<=9 and self.board[xx][yy]==-1:
				if nx>=0 and nx<=8 and ny>=0 and ny<=9:
					self.pushList(id, nx, ny)
	
	def listMoveXiang(self, id):
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [ -2, -2, 2, 2 ]
		dy = [ -2, 2, -2, 2 ]
		ax = [ -1, -1, 1, 1 ]
		ay = [ -1, 1, -1, 1 ]
		for d in range(4):
			nx = x+dx[d]
			ny = y+dy[d]
			xx = x+ax[d]
			yy = y+ay[d]
			if xx>=0 and xx<=8 and yy>=0 and yy<=9 and self.board[xx][yy]==-1:
				if nx>=0 and nx<=8 and ny>=0 and ny<=9 and not self.isOutRiver(id, ny):
					self.pushList(id, nx, ny)
		
	def listMoveJiang(self, id):
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [ -1, 1, 0, 0 ]
		dy = [ 0, 0, -1, 1 ]
		for d in range(4):
			nx = x+dx[d]
			ny = y+dy[d]
			if nx>=3 and nx<=5 and ny>=0 and ny<=9:
				if self.isDown(id) and ny<=2:
					self.pushList(id, nx, ny)
				if not self.isDown(id) and ny>=7:
					self.pushList(id, nx, ny)
					
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
					self.pushList(id, nx, ny)
				else:
					break
			
	def listMoveShi(self, id):
		x = self.pad[id][1]
		y = self.pad[id][2]
		dx = [ -1, -1, 1, 1 ]
		dy = [ -1, 1, -1, 1 ]
		for d in range(4):
			nx = x+dx[d]
			ny = y+dy[d]
			if nx>=3 and nx<=5 and ny>=0 and ny<=9:
				if self.isDown(id) and ny<=2:
					self.pushList(id, nx, ny)
				if not self.isDown(id) and ny>=7:
					self.pushList(id, nx, ny)
		
	def listMoveBing(self, id):
		x = self.pad[id][1]
		y = self.pad[id][2]
		if self.isOutRiver(id, self.pad[id][2]) and x-1>=0:
			self.pushList(id, x-1, y)
		if self.isOutRiver(id, self.pad[id][2]) and x+1<=8:
			self.pushList(id, x+1, y)
		d = -1
		if self.isDown(id): d=1
		if y+d>=0 and y+d<=9:
			self.pushList(id, x, y+d)