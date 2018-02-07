import os
import signal, time, sys
import threading,qipu2
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()
data_path = "d:/serialize/nonpool/data"

def weight_variable(shape):
	initial = tf.truncated_normal(shape, stddev=0.1)
	return tf.Variable(initial)

def bias_variable(shape):
	initial = tf.constant(0.1, shape=shape)
	return tf.Variable(initial)
	
def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def max_pool_2x2(x):
	return tf.nn.max_pool(x, ksize=[1, 2, 2, 1],strides=[1, 2, 2, 1], padding='SAME')

#占位符
x = tf.placeholder("float", shape=[None, 3136])
y_ = tf.placeholder("float", shape=[None, 1440])

W_conv1 = weight_variable([5, 5, 1, 64])
b_conv1 = bias_variable([64])
W_conv2 = weight_variable([3, 3, 64, 64])
b_conv2 = bias_variable([64])
W_conv3 = weight_variable([3, 3, 64, 64])
b_conv3 = bias_variable([64])
W_conv4 = weight_variable([3, 3, 64, 1])
b_conv4 = bias_variable([1])

x_image = tf.reshape(x, [-1,56,56,1])
h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_conv2 = tf.nn.relu(conv2d(h_conv1, W_conv2) + b_conv2)
h_conv3 = tf.nn.relu(conv2d(h_conv2, W_conv3) + b_conv3)
h_conv4 = tf.nn.relu(conv2d(h_conv3, W_conv4) + b_conv4)

W_fc1 = weight_variable([56*56, 1440])
b_fc1 = bias_variable([1440])
h_pool2_flat = tf.reshape(h_conv4, [-1, 56*56])	#降维打击
y_conv = tf.nn.softmax(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

#cross_entropy = -tf.reduce_sum(y_*tf.log(y_conv))
loss = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(y_conv), 1) )
train_step = tf.train.AdadeltaOptimizer(0.4).minimize(loss)
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
		for i in range(128):
			tx.append(0.0)
		for xx in item[0]:
			tx.append(float(xx[0]))
		for i in range(128):
			tx.append(0.0)
		tx = np.array(tx)
		x.append(tx)
		
		ty = []
		for yy in item[1]:
			ty.append(float(yy[0]))
		ty = np.array(ty)
		y.append(ty)
	return np.array(x), np.array(y)
	
test_data = qipu2.getTrainData('D:/qipu_dp/_%d.txt', 81950,82000)
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
		sum += accuracy.eval(feed_dict={x: test_x[i], y_: test_y[i]})
	sum /= ss
	return sum
	
first_step = 10300
while True:
	step = 1
	i = 1
	if first_step != -1:
		i = first_step
		first_step = -1
		
	while i<75000:
		training_data = qipu2.getTrainData('D:/qipu_xqd/_%d.txt', i,i+step-1)
		if i%100==0:
			print ('on training %d (len:%d)...'%(i,len(training_data)))
		if len(training_data)>200 or len(training_data)==0:
			i+=step
			continue
		xs,ys = data_process(training_data)
		
		train_step.run(feed_dict={x: xs, y_: ys})
		
		if i%100==1:
			print ("test accuracy %g"%getFuckingAccuracy())
		if i%300==161:	
			print ('write data to hard disk')
			saver.save(sess, data_path)
		i += step
		
saver.save(sess, data_path)