import numpy as np
#import matplotlib.pyplot as plt
import math 
import scipy.stats as stats
from scipy import pi
from random import randint
from sklearn import datasets


def calculer_dist(fetch1, fetch2, x1, x2):

    res = pow(   (pow((fetch1-x1),2)+pow((fetch2-2),2))   ,0.5)
    
    return res




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

nbZero = apprentissageTar.tolist().count(0)
nbUn = apprentissageTar.tolist().count(1)
nbDeux = apprentissageTar.tolist().count(2)


tab = [nbZero,nbUn,nbDeux]

tabClassZero = []
tabClassUn = []
tabClassDeux = []

for i in range(100):
    if(apprentissageTar[i]==0):
        tabClassZero.append(apprentissageData[i].tolist())
    elif(apprentissageTar[i]==1):
        tabClassUn.append(apprentissageData[i].tolist())
    elif(apprentissageTar[i]==2):
        tabClassDeux.append(apprentissageData[i].tolist())




print("\n SECTION APPRENTISSAGE\n")
# print('Affichage des donnees en fonction de la classe')
# print(matClassZero)
# print("\n=============================\n")
# print(matClassUn)
# print("\n=============================\n")
# print(matClassDeux)
# print("\n=============================\n")



print('\n CHOIX DE DISTANCE EUCLIDIENNE \n')

print("\n calcul des distances pour chaque point\n")



fetcher1 = apprentissageData[:,0]
fetcher2 = apprentissageData[:,1]
fetcher3 = apprentissageData[:,2]
fetcher4 = apprentissageData[:,3]

fetchers=[fetcher1,fetcher2,fetcher3,fetcher4]


#print(tabDistCurrent)
#print(tabTouteDistances)


print("\n SECTION DEVELOPPEMENT\n")
tabModele= []

print("\nChoix de k=3\n")

tabDist = []
for i in range(4):
    for j in range(4):
        for x in range(30):
            for y in range(100):
                if j!=i:
                    dist=calculer_dist(apprentissageData[y,i],apprentissageData[y,j],devData[x,i],devData[x,j])
                    tabModele.append([x,i,j,dist,apprentissageTar[y]])
  

tabModele = np.reshape(tabModele, (-1,5) )


modele1=tabModele[0:3000,:]

modele2=tabModele[3001:6000,:]

modele3=tabModele[6001:9000,:]

modele4=tabModele[9001:12000,:]

modele5=tabModele[12001:15000,:]

modele6=tabModele[15001:18000,:]

modele7=tabModele[18001:21000,:]

modele8=tabModele[21001:24000,:]

modele9=tabModele[24001:27000,:]

modele10=tabModele[27001:30000,:]

modele11=tabModele[30001:33000,:]

modele12=tabModele[33001:36000,:]


modeles = [modele1,modele2,modele3,modele4,modele5,modele6,modele7,modele8,modele9,modele10,modele11,modele12]

print(tabModele[0:100,3])
# def determineClassPoint(k, x):
    
    # x=x[x[:,3].argsort()]
    # x=x[0:(k-1),:]
    # nbClass0 = x[:,4].tolist().count(0)
    # nbClass1 = x[:,4].tolist().count(1)
    # nbClass2 = x[:,4].tolist().count(2)
    # if nbClass0 > (nbClass1 and nbClass2):
        # resClass=0
    # elif nbClass1 > (nbClass0 and nbClass2):
        # resClass=1
    # elif nbClass2 > (nbClass0 and nbClass1):
        # resClass=2
    # else:
        # resClass=randint(0,2)

    # return resClass

# tabClassModele = []
# subMatrix = []

# for x in range(12):
    # for i in range(0,2900,100):
        # y = i+100
        # subMatrix=modeles[x]
        # tabClassModele.append(determineClassPoint(5, subMatrix[i:y,:] ))


# tabClassModele = np.reshape(tabClassModele, (12,-1) )
# print(tabClassModele)



# cptError=0
# nbErrorModele = []

# for x in range(12):
    # for y in range(29):
        # if(tabClassModele[x,y]!=devTar[y]):
            # cptError=cptError+1

    # nbErrorModele.append([x,cptError])
    # cptError=0

# nbErrorModele = np.reshape(nbErrorModele, (-1,2) )



# Choix du meilleur modele

# nbErrorModele=nbErrorModele[nbErrorModele[:,1].argsort()]
# print(nbErrorModele)




# print("\n SECTION DE TEST \n")



# modeleChoisi = nbErrorModele[0,0]
# print("Le modele choisi est le modele numero : " + str(modeleChoisi))


# tabDistTest=[]

# for x in range(20):
    # for y in range(100):
        # dist=calculer_dist(apprentissageData[y,2],apprentissageData[y,3],testData[x,2],testData[x,3])
        # tabDistTest.append([x,2,3,dist,apprentissageTar[y]])
  

# tabDistTest = np.reshape(tabDistTest, (-1,5) )
# tabClassTest=[]



# for i in range(0,1900,100):
    # y = i+100
    # tabClassTest.append(determineClassPoint(5, tabDistTest[i:y,:] ))



# cptErrorTest=0
# nbErrorTest = []

# for y in range(19):
    # if(tabClassTest[y]!=testTar[y]):
        # cptErrorTest=cptErrorTest+1

# print(cptErrorTest)
