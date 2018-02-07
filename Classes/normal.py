import os
import signal, time, sys
import threading,qipu2
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()

data_path = 'D:/pusu_net/data.ckpt'

tf_x = tf.placeholder(tf.float32, [None, 3136])
tf_y = tf.placeholder(tf.float32, [None, 1440])

layer1 = tf.layers.dense(tf_x, 100, tf.nn.relu)
layer2 = tf.layers.dense(layer1, 100, tf.nn.relu)
layer_output = tf.layers.dense(layer2, 1440, tf.nn.softmax)

loss = tf.reduce_mean(-tf.reduce_sum(tf_y*tf.log(layer_output), 1) )
train = tf.train.AdadeltaOptimizer(0.5).minimize(loss)
correct_prediction = tf.equal(tf.argmax(layer_output,1), tf.argmax(tf_y,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

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

saver = tf.train.Saver()

while True:
	for i in range(1,78000,20):
		print ('on training %d...'%i)
		training_data = qipu2.getTrainData('D:/qipu_xqd/_%d.txt', i,i+9)
		xs,ys = data_process(training_data)
		sess.run(train, feed_dict={tf_x: xs, tf_y: ys})
		if i%100==1:
			print ("test accuracy %g"%accuracy.eval(feed_dict={tf_x: test_x, tf_y: test_y}) )
		if i%300==1:
			print ('write data to hard disk')
			saver.save(sess, data_path)