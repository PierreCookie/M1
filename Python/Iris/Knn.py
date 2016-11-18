import numpy as np
import scipy.stats as stats
from scipy import pi
import math
from sklearn import datasets

def calculDistance(a,b):
	learningDistanceTab = np.zeros([100,99])
	for i in range(100):
		for j in range(99):
			z = j
			if (j >= i):
				z+=1
		
			x = apprentissageData[i][a] - apprentissageData[z][a]
			x = x*x
			y = apprentissageData[i][b] - apprentissageData[z][b]
			y = y*y
			learningDistanceTab[i][j] = math.sqrt(x+y) 

	return learningDistanceTab

#
#
#	PAS D'APPRENTISSAGE
#
#


def calculKDistant(a,b,k)
	tab =[5]
	
	for i in range(5):
		max 
		for i in range(99)

irisData = datasets.load_iris()
CirisData = np.c_[irisData.data.reshape(len(irisData.data), -1), irisData.target.reshape(len(irisData.target), -1)]


np.random.seed(987654321) ;
np.random.shuffle(CirisData);

shuffledIrisData = CirisData[ :, :irisData.data.size//len(irisData.data)].reshape(irisData.data.shape)
shuffledIrisTarget = CirisData[ :, irisData.data.size//len(irisData.data) :].reshape(irisData.target.shape)

apprentissageData = shuffledIrisData[0:100]
devData = shuffledIrisData[100:130]
testData = shuffledIrisData[130:150]

apprentissageTar = shuffledIrisTarget[0:100]
devTar = shuffledIrisTarget[100:130]
testTar = shuffledIrisTarget[130:150]

apprentissageData = np.column_stack((apprentissageData,apprentissageTar))
devData = np.column_stack((devData,devTar))
testData= np.column_stack((testData,testTar))

tabDistance = np.zeros([12,100,99])
x =0
for i in range(4):
	for j in range(4):
		if i!=j:
			
			tabDistance[x] = calculDistance(i,j)
			x+=1

print(tabDistance)



