#! /usr/bin/python3
import sys
from pprint import pprint
import numpy as np
import math

def init():

	nbUserS = int(sys.argv[1])-1
	taille=1;
	nbUser = nbUserS
	while(nbUser > 1):
		nbUser = nbUser /2
		taille += 1
	mat = hadamard(taille)
	
	print(mat)
	etalement(mat,nbUserS)

def hadamard(taille):
	if (taille == 0):
		return np.ones([1,1]).astype(int)
	matInf = hadamard(taille-1)

	t = pow(2,taille)
	matRet = np.zeros([t,t]).astype(int)
	l = pow(2,taille-1)
	for i in range(t):
		for j in range(t):			
			if not((i>l-1) and (j>l-1)):
			 	
				matRet[i,j] = matInf[i%l,j%l]
			else:
				matRet[i,j] = matInf[i%l,j%l]*-1
	return matRet


def generateMessage(tailleMessage):
	message=[]
	for i in range(tailleMessage):
		x=random.getrandbits(1)
		if x==0:
			x=-1
		else:
			x=1
		message.append(x)
	return message

def etalement(mat,nbUser):
	message	
	

init()