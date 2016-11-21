import numpy as np
#import matplotlib.pyplot as plt
import math 
import scipy.stats as stats
from scipy import pi
from random import randint
from sklearn import datasets

irisData = datasets.load_iris()

CirisData = np.c_[irisData.data.reshape(len(irisData.data), -1), irisData.target.reshape(len(irisData.target), -1)]
np.random.seed(123456789) ;
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

#calcul des distances

def calcDist(carac1,carac2,pos1,pos2):
	ret = math.sqrt(pow(carac1-pos1,2)+pow(carac2-pos2,2))
	return ret


modele = []
for i in range(4):
	for j in range(4):
		if j!=i:
			for x in range(30):
				dev1 = devData[x,i]
				dev2 = devData[x,j]
				for y in range(100):
					learn1 = apprentissageData[y,i]
					learn2 = apprentissageData[y,j]
					modele.append([calcDist(learn1,learn2,dev1,dev2),apprentissageData[y,4]])

modele = np.reshape(modele,(-1,2))
def chooseClass(k,x):
	
	x = x[x[:,0].argsort()]
	x = x[0:k]
	nbClass0 = x[:,1].tolist().count(0)
	nbClass1 = x[:,1].tolist().count(1)
	nbClass2 = x[:,1].tolist().count(2)
	
	if nbClass0 > (nbClass1 or nbClass2):
		return 0
	elif nbClass1 > (nbClass0 or nbClass2):
		return 1
	elif nbClass2 > (nbClass0 or nbClass1):
		return 2
	else:
		return randint(0,2)
tabClass=[]
tmp=[]
for x in range(12):
	for i in range(0,3000,100):
		add=[]
		tmp = modele[i+x*3000:(i+100)+x*3000]
		for k in range(10):
			add.append(chooseClass(k+1,tmp))
		tabClass.append(add)
tabClass = np.reshape(tabClass,(-1,10))
for i in range(10):
	print(tabClass[:,i])
tabError=[]
for k in range(10):
	add = []
	for x in range(12):
		error = 0
		for y in range(30):
			if (tabClass[x*30+y,k] != devTar[y]):
				error+=1
		add.append(error)
	tabError.append(add)

print(tabError)
tabError = np.reshape(tabError, (-1,10))
print(tabError)
# choix du meilleur modèle avec un nmbre de voisins elevé
max = 30
ret = []
for k in range (10):
	if max > 0:
		kError = tabError[:,9-k]
		tri = kError.argsort()
		if kError[tri[0]]<max:
			print(tri[0],kError,k,max)
			max = kError[tri[0]]
			ret = [10-k,tri[0]]
			
print(ret)

print("\n TEST \n")

print("Le modele choisi est le modele " +str(ret[1]) +" avec " + str(ret[0])+" voisins")

inc = 0
model = []
for i in range(4):
	for j in range(4):
		if i!=j:
			
			if(inc == ret[1]):
				modele	= [i,j]
			inc = inc +  1
			
print(modele)
tabDist = []
m1 = modele[0]
m2 = modele[1]
print(m1,m2)

for x in range(20):
	test1 = testData[x,m1]
	test2 = testData[x,m2]
	for y in range (100):
		learn1 = apprentissageData[y,m1]
		learn2 = apprentissageData[y,m2]
		tabDist.append([calcDist(learn1,learn2,test1,test2),apprentissageData[y,4]])

tabDist = np.reshape(tabDist,(-1,2))
tabClass = []
k = ret[0]
for i in range(0,2000,100):
	tabClass.append(chooseClass(k,tabDist[i:i+100]))

print(tabClass)

error = 0
conf = [0,0,0,0,0,0,0,0,0]
for y in range (20):
	if(tabClass[y] != testTar[y]):
		error +=1
	conf[tabClass[y]*3+int(testTar[y])]= conf[tabClass[y]*3+int(testTar[y])]+1
conf = np.reshape(conf,(3,3))
print(conf)
print(error)