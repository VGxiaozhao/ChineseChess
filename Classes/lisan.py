import random, os
lst = [i for i in range(1000001)]
random.shuffle(lst)

i = 1
for items in lst:
	print (i, items)
	os.system('cp D:/qipu/_%d.txt D:/lisan/_%d.txt'%(items, i))
	i += 1