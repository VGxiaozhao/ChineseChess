import os, random
import signal, time, sys
import threading, move, copy, board
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()
data_path = "./data"

def weight_variable(shape):
	initial = tf.truncated_normal(shape, stddev=0.1)
	return tf.Variable(initial)

def bias_variable(shape):
	initial = tf.constant(0.1, shape=shape)
	return tf.Variable(initial)
	
def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

#占位符
x_ = tf.placeholder("float", shape=[None, 9, 10, 64])

W_conv1 = weight_variable([5, 5, 64, 128])
b_conv1 = bias_variable([128])
W_conv2 = weight_variable([3, 3, 128, 128])
b_conv2 = bias_variable([128])
W_conv3 = weight_variable([3, 3, 128, 128])
b_conv3 = bias_variable([128])
W_conv4 = weight_variable([3, 3, 128, 128])
b_conv4 = bias_variable([128])
W_conv5 = weight_variable([3, 3, 128, 128])
b_conv5 = bias_variable([128])
W_fc1 = weight_variable([2880*4, 1440])
b_fc1 = bias_variable([1440])

#x_image = tf.reshape(x_, [-1,9,10,64])
h_conv1 = tf.nn.relu(conv2d(x_, W_conv1) + b_conv1)
h_conv2 = tf.nn.relu(conv2d(h_conv1, W_conv2) + b_conv2)
h_conv3 = tf.nn.relu(conv2d(h_conv2, W_conv3) + b_conv3)
h_conv4 = tf.nn.relu(conv2d(h_conv3, W_conv4) + b_conv4)
h_conv5 = tf.nn.softmax(conv2d(h_conv4, W_conv5) + b_conv5)
h_flat = tf.reshape(h_conv5, [-1, 2880*4])
h_output = tf.nn.softmax(tf.matmul(h_flat, W_fc1) + b_fc1)

saver = tf.train.Saver()
saver.restore(sess, data_path)

def cast(string, res=None):
	pad = copy.deepcopy(move.initPad)
	i = 0
	while i < len(string):
		x = ord(string[i]) - ord('0')
		y = ord(string[i+1]) - ord('0')
		id = int(i/2)
		pad[id][1] = x
		pad[id][2] = y
		i+=2
	move.board_show(pad)
	vec = move.Matrix(pad)
	ob  = board.CBoard(pad)
	item= ob.setMat(vec)
	ret = sess.run(h_output, feed_dict = {x_:[item]})
	ret = ret[0]
	cnt = 0
	while True:
		tmp = int(np.argmax(ret))
		id = int(tmp/90) + 16
		x = int((tmp%90)/10)
		y = (tmp%90)%10
		if ret[tmp]<0.000001:
			return
		if (x,y) in ob.mov[id]:
			if type(res)==list:
				if ret[tmp]>0.01:
					res.append((tmp, ret[tmp]))
				#if len(res)>6 or ret[tmp]<0.000001:
				if ret[tmp]<0.01:
					return
			else:
				return tmp
		ret[tmp] = 0.0
	
if __name__ == '__main__':
	#function(1234567812345678,1234567812345678,1234567812345678,1234567812345678)
	#print (fun(1234567))
	print ('initial complete')
	st = time.time()
	for i in range(1):
		lst = []
		print(cast('8979695949392919097717866646260600102030405060708012720323436383', lst))
		print(lst)
	sum = 0.0
	for item in lst:
		sum += item[1]
	print('len:%d sum:%g'%(len(lst),sum))
	#print (time.time()-st)
	#move.board_show(move.initPad)
	txtBoard = "d:/txtboard.txt";
	tagBoard = "d:/tagboard.txt";
	tagBoard10 = "d:/tagboard10.txt";
	txtMove = "d:/txtmove.txt";
	tagMove = "d:/tagmove.txt";
	tagMove10 = "d:/tagmove10.txt";
	while True:
		#标识棋盘已写
		if os.path.exists(tagBoard):
			with open(txtBoard, 'r') as cin:
				for line in cin:
					pass
			print (line)
			ret = cast(line)
			print (ret)
			#先输出结果
			f = open(txtMove, 'w')
			f.write(str(ret))
			f.flush()
			f.close()
			#先删除标识以免二次读取
			os.remove(tagBoard)
			#告诉调用方生成好了
			ff = open(tagMove, 'w') 
			ff.close();
			
		if os.path.exists(tagBoard10):
			with open(txtBoard, 'r') as cin:
				for line in cin:
					pass
			print (line)
			lst = []
			ret = cast(line, lst)
			f = open(txtMove, 'w')
			for ret in lst:
				print (ret)
				f.write(str(ret[0])+'\n')
			f.flush()
			f.close()
			os.remove(tagBoard10)
			ff = open(tagMove10, 'w') 
			ff.close();