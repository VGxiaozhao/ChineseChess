import os
cout = open('d:/result1.txt', 'w')
with open('d:/result.txt', 'r') as f:
	for line in f:	
		line1 = line.replace('\\', '/')
		cout.write(line1)
		
cout.flush()
cout.close()