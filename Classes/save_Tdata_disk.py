import generateMove
import pickle
import numpy as np
	
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
	
if __name__ == '__main__':
	for i in range(1, 1078000):
		data = generateMove.getTrainData('D:/lisan/_%d.txt', i, i)
		for item in data:
			x, y = data_process([item])
			cp = (x,y)
			with open('d:/dumptest', 'wb') as f:
				pickle.dump(cp, f)
				