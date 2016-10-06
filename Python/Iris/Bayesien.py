from sklearn import datasets
import numpy as np
import math
irisData = datasets.load_iris()
print (irisData.data)
print (irisData.target)
CirisData = np.c_[irisData.data.reshape(len(irisData.data),-1),irisData.target.reshape(len(irisData.target),-1)]
np.random.seed(987654321);
np.random.shuffle(CirisData);
shuffledIrisData = CirisData[:,:irisData.data.size//len(irisData.data)].reshape(irisData.data.shape)
shuffledIrisTarget = CirisData[:,irisData.data.size//len(irisData.data) :].reshape(irisData.target.shape)

priori =[0,0,0];
tabSet=[]
tabVers=[]
tabVirg=[]
#differenciation des 100 premiers éléments par classe avec calcul de l'apriori
for i in range(100):
	classe = shuffledIrisTarget[i]
	
	priori[int(classe)]+=1	
	if (int(classe) == 0):
		tabSet.append(shuffledIrisData[i].tolist())
	elif(int(classe) == 1):
		tabVers.append(shuffledIrisData[i].tolist())	
	else:
		tabVirg.append(shuffledIrisData[i].tolist())	

# tableau en matrice
tab=np.asarray([tabSet,tabVers,tabVirg])
print(tab)


#attribution des valeurs de moyennes et variances
moyVar = np.zeros([3,4,2])
for laClass in range(3):
	donnees = np.asarray(tab[laClass])
	for leType in range(4):
		moyVar[laClass,leType,0] =np.mean(donnees[:,leType])
		moyVar[laClass,leType,1] =np.var(donnees[:,leType])
print(moyVar)
setupTab =[[0,1],[0,2],[0,3],[1,2],[1,3],[2,3]]

for i in range(6):
	setup = setupTab[i]
	x = setup[0]
	y = setup[1]

	for laClass in range(3):
		donnees = np.asarray(tab[laClass])
		cov = np.cov(donnees[:,x],donnees[:,y])
		