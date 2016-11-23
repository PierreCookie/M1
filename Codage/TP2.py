#! /usr/bin/python3
import sys
from pprint import pprint
import numpy as np
import math


def LM(vecteur,setup,l):
	print(vecteur)
	if(l==1):
		return vecteur.pop()
	queue = vecteur.pop()
	
	nbsetup = len(setup)-1
	tete = queue
	for i in range(nbsetup):
		tete = int(tete!=vecteur[setup[i+1]-1])
		
	vecteur.insert(0,tete)
	ret = LM(vecteur,setup,l-1)
	if(l==2):
		ret = [ret]
	ret.insert(0,queue)
	
	return ret
def initVec(setup):
	print("Initialisation vecteur pour setup ",setup)
	ret = []
	vec0 = []
	for i in range(setup[0]):
		print("indice ",i+1, "Valeurs possible 0 ou 1")			
		x = input()
		while(x!=0 and x!=1): 
			print("indice ",i+1, "Valeurs possible 0 ou 1")			
			x = input()	
		vec0.append(0)
		ret.append(x)
	while(ret == vec0):
		print("IMPOSSIBLE D'INITIALISER UN VECTEUR A ",vec0)
		ret = []
		for i in range(setup[0]):
			print("indice ",i+1, "Valeurs possible 0 ou 1")			
			x = input()
			while(x!=0 and x!=1): 
				print("indice ",i+1, "Valeurs possible 0 ou 1")			
				x = input()			
			ret.append(x)
	return ret
 
def getSetup(n):
	ret = [n]
	print("Indiquez le prochain connecteur a part ", n," entre [1,n[")
	x = input()
	while(x<1 or x>=n):
		print("Indiquez au moins un connecteur a part ",n ," entre [1,n[")
		x = input()

	ret.append(x)
	print("Indiquez le prochain connecteur a part ", n," entre [1,n[")
	x = input()
	
	while(x>0 and x<n):
		if (x<0 or x>=n):
			ret.append(x)		
		print("Indiquez au moins un connecteur a part ",n," entre [1,n[ si en dehors = sortie")
		x = input()

		
	return set(ret)
def tailleLM():
	print("Indiquer une taille n de Code de Longeur Maximal")
	n = input()
	while(n<2):
		print("Un registre ne peut avoir une taille inferieur a 2")
		n = input()
	return n
def Gold():
	n = tailleLM()
		
	print(" SETUP 1 ")
	setup1 = list(getSetup(n))
	setup1.reverse()

	print(" SETUP 2 ")
	setup2 = list(getSetup(n))	
	setup2.reverse()

	print(setup1)
	tailleSeq = int(math.pow(2,setup1[0])-1)
		
	vec = initVec(setup1)
	
	lm1 = LM(vec,setup1,tailleSeq)
	vec = initVec(setup2)
	lm2 = LM(vec,setup2,tailleSeq)
	
	vec = []
	for i in range(tailleSeq):
		vec.append(int(lm1[i] != lm2[i]))

	return vec

def pgcd(a,b) :

	while (a%b != 0) : 
		a=b
		b = b, a%b 
	return b

def Jpl():
	print("Indiquer un nombre de Code de Longeur Maximal pour le JPL")
	n = input()
	while(n<3):
		print("Code JPL ne peut avoir moins de 3 Codes a Longeur Maximal")
		n = input()
	setup = []
	for i in range(n):
		print("Indiquez taille ",i," registre")
		print(setup)
		taille = tailleLM()
		while(len(setup)>1):
			denom = 2
			print(setup[:])
			print(setup[:,0])
			for j in range(len(setup[:])):
				print(setup[j,0])
				denom = pgcd(setup[j,0],taille )			
				if(denom != 1):
					print("Indiquez taille ",i," registre")
					taille = tailleLM()
					break
			print("test")
			if(denom == 1):				
				break
		setp = list(getSetup(taille))
		setp.reverse()
		setup.append(setp)
		
	print(setup)
Jpl()
	