import os,sys,copy
import numpy as np
import random
import qipu2

depth = 12
lamb = 0.8

for index in range(1,2):
	outfile = 'D:/qipu/_%d.txt'
	
	with open(outfile%index, 'r') as cin:
		ret = []
		lst = copy.deepcopy(qipu2.initPad)
		dead = [0 for i in range(32)]
		row = 1
		lens = 0
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
				dp = [[] for i in range(int(len(line)/4))]
				ju = []
				for j in range(0, len(line), 4):
					if j+3<len(line):
						ju.append(qipu2.vectorize(lst, dead))
						score = qipu2.movStone(line[j], line[j+1], line[j+2], line[j+3], lst, dead)
						sum = 1.0
						for k in range(depth):
							id = int(j/4)
							dp[id].append(1.0*score*sum)
							sum *= -lamb
					red = 1-red
				lens = int(len(line)/4)
			row += 1
		i = lens-1
		while i>1:
			i -= 1
			j = 11
			while j>=1:
				#print (i,j)
				dp[i-1][j-1] += dp[i][j]
				j-=1
		for i in range(len(dp)):
			ret.append((ju[i], np.tanh(dp[i][0])))
		
		for item in ret:
			print (item[0], item[1])

