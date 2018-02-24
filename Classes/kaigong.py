import os
import signal, time, sys
import threading,generateMove, copy
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()
data_path = "d:/serialize/kaigong/data"

def weight_variable(shape):
	initial = tf.truncated_normal(shape, stddev=0.1)
	return tf.Variable(initial)

def bias_variable(shape):
	initial = tf.constant(0.1, shape=shape)
	return tf.Variable(initial)
	
def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

#占位符
x_ = tf.placeholder("float", shape=[None, 5760])
y_ = tf.placeholder("float", shape=[None, 1440])

W_conv1 = weight_variable([5, 5, 64, 128])
b_conv1 = bias_variable([128])
W_conv2 = weight_variable([3, 3, 128, 128])
b_conv2 = bias_variable([128])
W_conv3 = weight_variable([3, 3, 128, 128])
b_conv3 = bias_variable([128])
W_conv4 = weight_variable([3, 3, 128, 64])
b_conv4 = bias_variable([64])
W_conv5 = weight_variable([3, 3, 64, 16])
b_conv5 = bias_variable([16])

x_image = tf.reshape(x_, [-1,9,10,64])
h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_conv2 = tf.nn.relu(conv2d(h_conv1, W_conv2) + b_conv2)
h_conv3 = tf.nn.relu(conv2d(h_conv2, W_conv3) + b_conv3)
h_conv4 = tf.nn.relu(conv2d(h_conv3, W_conv4) + b_conv4)
h_conv5 = tf.nn.relu(conv2d(h_conv4, W_conv5) + b_conv5)
h_flat = tf.reshape(h_conv5, [-1, 1440])	#降维打击

W_fc1 = weight_variable([1440, 1440])
b_fc1 = bias_variable([1440])
y_conv = tf.nn.softmax(tf.matmul(h_flat, W_fc1) + b_fc1)

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
	
	'''
def data_process1(data):
	rx = []
	for item in data:
		tx = []
		for xx in item:
			tx.append(float(xx[0]))
		tx = np.array(tx)
		rx.append(tx)
		
	return np.array(rx)
	
if __name__ == '__main__':
	lst = copy.deepcopy(qipu2.initPad)
	dead = [0 for i in range(0,32)]
	a = []
	a.append(qipu2.vectorize(lst, dead))
	yy = np.zeros((1440,1))
	xx = data_process1(a)
	tmp = sess.run(x_image, feed_dict={x_:xx})
	for xxx in tmp[0]:
		print ('[', end = '')
		for yyy in xxx:
			print ('[', end = '')
			for zzz in yyy:
				print (zzz, end = ', ')
			print (']')
		print (']')
	
	tmp = sess.run(h_conv1, feed_dict={x_:xx})
	for xxx in tmp[0]:
		print ('[', end = '')
		for yyy in xxx:
			print ('[', end = '')
			for zzz in yyy:
				print (zzz, end = ', ')
			print (']')
		print (']')

		'''
		
test_data = generateMove.getTrainData('D:/qipu_dp/_%d.txt', 81950,82000)
test_x,test_y = [],[]
ss = 10
slice = int(len(test_data)/ss)
for i in range(0, len(test_data), slice):
	tmp_x, tmp_y = data_process(test_data[i: i+slice])
	test_x.append(tmp_x)
	test_y.append(tmp_y)

def getFuckingAccuracy():
	sum = 0.0
	for i in range(ss):
		sum += accuracy.eval(feed_dict={x_: test_x[i], y_: test_y[i]})
	sum /= ss
	return sum
	
first_step = 1
for _ in range(3):
	print("start fucking train")
	step = 1
	i = 1
	if first_step != -1:
		i = first_step
		first_step = -1
		
	while i<1075000:
		training_data = generateMove.getTrainData('D:/lisan/_%d.txt', i,i+step-1)
		#if i%100==0:
		print ('on training %d (len:%d)...'%(i,len(training_data)))
		if len(training_data)>200 or len(training_data)==0:
			i+=step
			continue
		xs,ys = data_process(training_data)
		
		train_step.run(feed_dict={x_: xs, y_: ys})
		
		if i%100==1:
			print ("test accuracy %g"%getFuckingAccuracy())
		if i%1000==761:	
			print ('write data to hard disk')
			saver.save(sess, data_path)
		i += step
		
saver.save(sess, data_path)