from model import *
import os
import move


def getAccuracy(tdata):
    sum, cnt = 0.0, 0.0
    for x, y in tdata:
        ret = sess.run(h_output, feed_dict={x_: [x], y_: [y]})
        if np.argmax(y) == np.argmax(ret):
            sum += 1.0
        cnt += 1.0
    return sum / cnt, sum, cnt


# 训练进度
i = 1
if os.path.exists(data_path + ".txt"):
    with open(data_path + ".txt", "r") as cin:
        i = cin.readline()
    i = int(i)

# 全局验证集
# g_testData = move.getLastTD('D:/qipu_dp/_%d.txt', 1,50, 16)
g_testData = move.getTrainData('D:/qipu_dp/_%d.txt', 1, 50)
# 步长，训练次数，小批量大小
step, epoch, batch = 25, 1, 40
cnt = 1
while i < 1075000:
    # tdata = move.getTrainData('D:/lisan/_%d.txt', i,i+step-1)
    # 获得30步后的棋谱
    tdata = move.getLastTD('D:/lisan/_%d.txt', i, i + step - 1, 30)
    tlen = len(tdata)
    random.shuffle(tdata)

    if cnt % 10 == 1:
        print(i, tlen)
        print("global test accuracy %g(%d / %d)" % getAccuracy(g_testData))
        print('write data to hard disk')
        saver.save(sess, data_path)
        with open(data_path + ".txt", "w") as cin:
            cin.write(str(i))

    for j in range(epoch):
        for k in range(0, tlen, batch):
            ed = min(tlen, k + batch)
            # print(k,ed)
            xs, ys = [], []
            for index in range(k, ed):
                xs.append(tdata[index][0])
                ys.append(tdata[index][1])
            train_step.run(feed_dict={x_: xs, y_: ys})
        random.shuffle(tdata)
    # print ("epoch %d"%j, end=', ')
    # print ("test accuracy %g(%d / %d)"%getAccuracy(tdata))
    cnt += 1
    i += step

saver.save(sess, data_path)
