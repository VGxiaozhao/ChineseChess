import os
import signal, time, sys
import threading,qipu2
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()
data_path = "d:/serialize/duli/data"

def weight_variable(shape):
	initial = tf.truncated_normal(shape, stddev=0.1)
	return tf.Variable(initial)

def bias_variable(shape):
	initial = tf.constant(0.1, shape=shape)
	return tf.Variable(initial)
	
def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

#占位符
x_ = tf.placeholder("float", shape=[None, 2880])
y_ = tf.placeholder("float", shape=[None, 1440])

W_conv1 = weight_variable([5, 5, 32, 64])
b_conv1 = bias_variable([64])
W_conv2 = weight_variable([3, 3, 64, 64])
b_conv2 = bias_variable([64])
W_conv3 = weight_variable([3, 3, 64, 64])
b_conv3 = bias_variable([64])
W_conv4 = weight_variable([3, 3, 64, 64])
b_conv4 = bias_variable([64])
W_conv5 = weight_variable([3, 3, 64, 64])
b_conv5 = bias_variable([64])
W_conv6 = weight_variable([3, 3, 64, 32])
b_conv6 = bias_variable([32])
W_conv7 = weight_variable([3, 3, 32, 16])
b_conv7 = bias_variable([16])

x_image = tf.reshape(x_, [-1,9,10,32])
h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_conv2 = tf.nn.relu(conv2d(h_conv1, W_conv2) + b_conv2)
h_conv3 = tf.nn.relu(conv2d(h_conv2, W_conv3) + b_conv3)
h_conv4 = tf.nn.relu(conv2d(h_conv3, W_conv4) + b_conv4)
h_conv5 = tf.nn.relu(conv2d(h_conv4, W_conv5) + b_conv5)
h_conv6 = tf.nn.relu(conv2d(h_conv5, W_conv6) + b_conv6)
h_conv7 = tf.nn.relu(conv2d(h_conv6, W_conv7) + b_conv7)
h_pool2_flat = tf.reshape(h_conv7, [-1, 1440])	#降维打击

W_fc1 = weight_variable([1440, 1440])
b_fc1 = bias_variable([1440])
y_conv = tf.nn.softmax(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

cross_entropy = -tf.reduce_sum(y_*tf.log(y_conv))
loss = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(y_conv), 1) )
train_step = tf.train.AdadeltaOptimizer(0.1).minimize(loss)
correct_prediction = tf.equal(tf.argmax(y_conv,1), tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

saver = tf.train.Saver()
if os.path.exists(data_path+".meta"):
	print ('restore')
	saver.restore(sess, data_path)
else:
	sess.run(tf.global_variables_initializer())

rtest_x,rtest_y = qipu2.getTrainData('D:/qipu_dp/_%d.txt', 81000,81050)
test_x, test_y = [], []
ss = 10
slice = int(len(rtest_x )/ss)
for i in range(0, len(rtest_x), slice):
	tmp_x, tmp_y = rtest_x[i: i+slice], rtest_y[i:i+slice]
	test_x.append(tmp_x)
	test_y.append(tmp_y)

def getFuckingAccuracy():
	sum = 0.0
	for i in range(ss):
		sum += accuracy.eval(feed_dict={x_: test_x[i], y_: test_y[i]})
		'''
		if i==0:
			ret = sess.run(y_conv, feed_dict = {x_: test_x[i], y_: test_y[i]})
			tmp = []
			for intt, outt in zip(ret, test_y[i]):
				x = np.argmax(intt)
				y = np.argmax(outt)
				print(x, '=', y, end=', ')
		'''
	sum /= ss
	return sum
	
def cast(string):
	vec = np.array([[0.0 for i in range(2880)]])
	yyy = np.array([[0.0 for i in range(1440)]])
	lei = 0
	for i in range(len(string), 2):
		x = ord(string[i]) - ord('0')
		y = ord(string[i+1]) - ord('0')
		if x!=9 or y!=9:
			tmp = lei + x*10 + y
			vec[tmp] = 1.0
		lei += 90
	ret = sess.run(y_conv, feed_dict = {x_:vec, y_:yyy})
	return (np.argmax(ret))
	
first_step = 1
for _ in range(3):
	step = 1
	i = 1
	if first_step != -1:
		i = first_step
		first_step = -1
		
	while i<1075000:
		tx, ty = qipu2.getTrainData('D:/lisan/_%d.txt', i,i+step-1)
		if i%100==0:
			print ('on training %d...'%i)
		if len(tx)>300 or len(tx)==0:
			i+=step
			continue
		st = time.time()
		train_step.run(feed_dict={x_: tx, y_: ty})
		ed = time.time()
		'''
		print(i,end=', ')
		print(ed-st, end=', ')
		print('len: %d'%len(tx))
		'''
		if i%100==1:
			print ("test accuracy %g"%getFuckingAccuracy())
			#print ("test accuracy %g(%d / %d)"%getAccuracy())
		if i%1000==761:	
			print ('write data to hard disk')
			saver.save(sess, data_path)
		i += step
		
saver.save(sess, data_path)