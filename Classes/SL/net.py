import os, random
import signal, time, sys
import threading, move, copy
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
y_ = tf.placeholder("float", shape=[None, 1440])

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

loss = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(h_output), 1))
train_step = tf.train.AdadeltaOptimizer(0.01).minimize(loss)
#train_step = tf.train.GradientDescentOptimizer(100).minimize(loss)
correct_prediction = tf.equal(tf.argmax(h_output,1), tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

i = 1
saver = tf.train.Saver()
if os.path.exists(data_path+".meta"):
	print ('restore')
	saver.restore(sess, data_path)
	with open(data_path+".txt", "r") as cin:
		i = cin.readline()
	i = int(i)
else:
	sess.run(tf.global_variables_initializer())
	
def getAccuracy(tdata):
	sum, cnt = 0.0, 0.0
	for x, y in tdata:
		ret = sess.run(h_output, feed_dict={x_: [x], y_: [y]})
		if np.argmax(y) == np.argmax(ret):
			sum += 1.0
		cnt += 1.0
	return sum/cnt, sum, cnt

#g_testData = move.getLastTD('D:/qipu_dp/_%d.txt', 1,50, 16)
g_testData = move.getTrainData('D:/qipu_dp/_%d.txt', 1, 50)
step, epoch, batch = 25,1,40
cnt = 1
while i<1075000:
	#tdata = move.getTrainData('D:/lisan/_%d.txt', i,i+step-1)
	tdata = move.getLastTD('D:/lisan/_%d.txt', i,i+step-1, 30)
	tlen = len(tdata)
	random.shuffle(tdata)
	
	if cnt%10==1:
		print(i, tlen)
		print ("global test accuracy %g(%d / %d)"%getAccuracy(g_testData))
		#print ("test accuracy %g(%d / %d)"%getAccuracy(tdata))
	for j in range(epoch):
		for k in range(0, tlen, batch):
			ed = min(tlen, k+batch)
			#print(k,ed)
			xs, ys = [], []
			for index in range(k, ed):
				xs.append(tdata[index][0])
				ys.append(tdata[index][1])
			train_step.run(feed_dict={x_: xs, y_: ys}) 
		random.shuffle(tdata)
		#print ("epoch %d"%j, end=', ')
		#print ("test accuracy %g(%d / %d)"%getAccuracy(tdata))
	
	if cnt%10==1:
		print ('write data to hard disk')
		saver.save(sess, data_path)
		with open(data_path+".txt", "w") as cin:
			cin.write(str(i))
	cnt+=1
	i += step
		
saver.save(sess, data_path)