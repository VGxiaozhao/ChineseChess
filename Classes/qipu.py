#encoding:utf-8
import os,sys,copy
import numpy as np
reload(sys)
sys.setdefaultencoding('utf-8')
init = [
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
['Bing', 8, 3 ],
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
['Bing', 0, 6 ]
]

removlst = [0 for i in range(32)]
print removlst

def chnToNum(c):
	if c=='一':
		return 1
	if c=='二':
		return 2
	if c=='三':
		return 3
	if c=='四':
		return 4
	if c=='五':
		return 5
	if c=='六':
		return 6
	if c=='七':
		return 7
	if c=='八':
		return 8
	if c=='九':
		return 9
	if c=='十':
		return 10
	if c=='前'.decode('utf-8') or c=='后'.decode('utf-8'):
		return c
	return ord(c)-ord('0')
	
def strQ2B(ustring):
    """全角转半角"""
    rstring = ""
    for uchar in ustring:
        inside_code=ord(uchar)
        if inside_code == 12288:                              #全角空格直接转换            
            inside_code = 32 
        elif (inside_code >= 65281 and inside_code <= 65374): #全角字符（除空格）根据关系转化
            inside_code -= 65248

        rstring += unichr(inside_code)
    return rstring

def chnToEng(c):
	if c=='车':
		return 'Ju'
	if c=='马':
		return 'Ma'
	if c=='相':
		return 'Xiang'
	if c=='士':
		return 'Shi'
	if c=='炮':
		return 'Pao'
	if c=='兵':
		return 'Bing'
	if c=='卒':
		return 'Bing'
	if c=='象':
		return 'Xiang'
	if c=='仕':
		return 'Shi'
	if c=='将':
		return 'Jiang'
	if c=='帅':
		return 'Jiang'
	return c

def getStone(c, x, red, lst):
	if c=='前'.decode('utf-8') or c=='后'.decode('utf-8'):
		tmp = c
		c = x
		x = tmp
		c = chnToEng(c)
	else:
		c = chnToEng(c)
		x = chnToNum(x)
		if red:
			x = 10 - x
		x -= 1
	
	st = 0
	ed = 16
	if not red:
		st = 16
		ed = 32
	
	ans = []
	idx = []
	for i in range(st, ed):
		if removlst[i]:
			continue
		if lst[i][0] == c:
			ans.append(lst[i])
			idx.append(i)
			
	if x != '前' and x != '后':
		for i in range(0, len(ans)):
			if x==ans[i][1]:
				return idx[i]
	else:
		if (x=='前' and red) or (x=='后' and not red):
			if ans[1][2] > ans[0][2]:
				return idx[1]
			else:
				return idx[0]
		else:
			if ans[1][2] < ans[0][2]:
				return idx[1]
			else:
				return idx[0]

def movStone(id, w, d, y, red, lst):
	w = chnToEng(w)
	y = chnToNum(y)
	if w=='Ju' or w=='Pao' or w=='Bing' or w=='Jiang':
		movStraight(id,w,d,y,red,lst)
	if w=='Shi':
		movShi(id,w,d,y,red,lst)
	if w=='Xiang':
		movXiang(id,w,d,y,red,lst)
	if w=='Ma':
		movMa(id,w,d,y,red,lst)
	for i in range(len(lst)):
		if i==id or removlst[i]:
			continue
		if lst[i][1]==lst[id][1] and lst[i][2]==lst[id][2]:
			removlst[i] = 1
	
def movStraight(id,w,d,y,red,lst):
	if d == '平':
		if red:
			lst[id][1] = (10-y)-1
		else:
			lst[id][1] = y-1
	elif (d == '进' and red) or (d == '退' and not red):
		lst[id][2] += y
	else:
		lst[id][2] -= y

def movMa(id,w,d,y,red,lst):
	if red: 
		y = 10-y
	if (d == '进' and red) or (d == '退' and not red):
		if abs(lst[id][1] + 1 - y) == 2:
			lst[id][2] += 1
		else:
			lst[id][2] += 2
	else:
		if abs(lst[id][1] + 1 - y) == 2:
			lst[id][2] -= 1
		else:
			lst[id][2] -= 2
	lst[id][1] = y-1
	
def movShi(id,w,d,y,red,lst):
	if (d == '进' and red) or (d == '退' and not red):
		if red:
			lst[id][1] = (10-y)-1
		else:
			lst[id][1] = y-1
		lst[id][2] += 1
	else:
		if red:
			lst[id][1] = (10-y)-1
		else:
			lst[id][1] = y-1
		lst[id][2] -= 1
	
def movXiang(id,w,d,y,red,lst):
	if (d == '进' and red) or (d == '退' and not red):
		if red:
			lst[id][1] = (10-y)-1
		else:
			lst[id][1] = y-1
		lst[id][2] += 2
	else:
		if red:
			lst[id][1] = (10-y)-1
		else:
			lst[id][1] = y-1
		lst[id][2] -= 2

def vectorize(lst):
	ret = np.zeros((90*32,1))
	lei = 0
	for i in range(len(lst)):
		if not removlst[i]:
			tmp = lei + lst[i][1] + lst[i][2] * 9
		lei += 90
	return ret;
	
def readOp(str, Red, lst):
	id = getStone(str[0], str[1], Red, lst)
	if (str[0] == '前' or str[0] == '后'):
		movStone(id, str[1], str[2], str[3], Red, lst)
	else:
		movStone(id, str[0], str[2], str[3], Red, lst)
	
btest = 1
if not btest:
	lst = [['Ju', 3, 1], ['Ma', 5, 6], ['Xiang', 8, 2], ['Shi', 3, 0], ['Jiang', 4, 0], ['Shi', 4, 1], ['Xiang', 6, 0], ['Ma', 4, 4], ['Ju', 4, 6], ['Pao', 7, 6], ['Pao', 7, 5], ['Bing', 0, 3], ['Bing', 2, 3], ['Bing', 4, 5], ['Bing', 6, 5], ['Bing', 8, 3], ['Ju', 4, 6], ['Ma', 6, 7], ['Xiang', 4, 7], ['Shi', 4, 8], ['Jiang', 4, 9], ['Shi', 3, 9], ['Xiang', 2, 9], ['Ma', 4, 4], ['Ju', 3, 1], ['Pao', 5, 7], ['Pao', 0, 5], ['Bing', 8, 6], ['Bing', 6, 6], ['Bing', 5, 3], ['Bing', 3, 3], ['Bing', 0, 6]]
	#print getStone('前'.decode('utf-8'),'炮'.decode('utf-8'),False,lst)
	readOp('马五进四'.decode('utf-8'), True, lst)
	print lst

if __name__ == '__main__' and btest:
	with open('D:/qipu_xqbase/2.txt','r') as f:
		data = []
		lst = copy.copy(init)
		st, ed = np.zeros((3,1)) , np.zeros((3,1))
		for line in f:
			str = strQ2B(line.decode('utf-8')).split('.')[1].strip(' ')
			st = vectorize(lst)
			readOp(str.split(' ')[0], True, lst)
			ed = vectorize(lst)
			data.append( (st, ed) )
			st = ed 
			try:
				readOp(str.split(' ')[1], False, lst)
				ed = vectorize(lst)
				data.append( (st, ed) )
				st = ed 
			except:
				pass
			
		for i in range(32):
			if removlst[i]:
				continue
			print lst[i]
		print data[-1][0] 
		with open('d:/test.txt', 'w') as cout:
			cout.write(data)
