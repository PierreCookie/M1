#! /usr/bin/python3
import sys
from pprint import pprint
import numpy as np
import math
def init():

	nbUser = int(sys.argv[1])

	taille=1;
	
	while(nbUser > 1):
		nbUser = nbUser / 2
		taille += 1
	return taille


def hadamard(taille):
	if (taille == 0):
		return np.ones([1,1])
	matInf = hadamard(taille-1)

	t = pow(2,taille)
	matRet = np.zeros([t,t])
	l = pow(2,taille-1)
	for i in range(t):
		for j in range(t):			
			if not((i>l-1) and (j>l-1)):
			 	
				matRet[i,j] = matInf[i%l,j%l]
			else:
				matRet[i,j] = matInf[i%l,j%l]*-1
	return matRet


def affiche(mat):
	t = int(math.sqrt(mat.size))
	mat = np.asarray(mat)
	print(mat)
	for i in range(t):
		for j in range(t):	
			print(mat[i,j])	
	
affiche(hadamard(init()))