import os
import signal, time, sys
import threading,qipu2
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()
data_path = "d:/cnn_chess/data.ckpt"

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

#第一层卷积
W_conv1 = weight_variable([5, 5, 1, 32])
b_conv1 = bias_variable([32])
x_image = tf.reshape(x, [-1,56,56,1])
h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_pool1 = max_pool_2x2(h_conv1)

#第二层卷积
W_conv2 = weight_variable([5, 5, 32, 64])
b_conv2 = bias_variable([64])
h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
h_pool2 = max_pool_2x2(h_conv2)

#密集连接层
W_fc1 = weight_variable([14 * 14 * 64, 128])
b_fc1 = bias_variable([128])
h_pool2_flat = tf.reshape(h_pool2, [-1, 14*14*64])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

#Dropout
keep_prob = tf.placeholder("float")
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

#输出层
W_fc2 = weight_variable([128, 1440])
b_fc2 = bias_variable([1440])
y_conv=tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)

#cross_entropy = -tf.reduce_sum(y_*tf.log(y_conv))
loss = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(y_conv), 1) )
train_step = tf.train.AdadeltaOptimizer(0.05).minimize(loss)
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
test_x,test_y = data_process(test_data)
print(test_x.shape)
print(test_y.shape)

first_step = 23811
i = 1
while True:
	step = 10
	while i<78000:
		if first_step != -1:
			i = first_step
			first_step = -1
		print ('on training %d...'%i)
		training_data = qipu2.getTrainData('D:/qipu_xqd/_%d.txt', i,i+step-1)
		xs,ys = data_process(training_data)
		for _ in range(10):
			train_step.run(feed_dict={x: xs, y_: ys, keep_prob: 0.618})
		if i%100==1:
			print ("test accuracy %g"%accuracy.eval(feed_dict={x: test_x, y_: test_y, keep_prob: 1.0}) )
		if i%300==161:	
			print ('write data to hard disk')
			saver.save(sess, data_path)
		i += step
		
saver.save(sess, data_path)