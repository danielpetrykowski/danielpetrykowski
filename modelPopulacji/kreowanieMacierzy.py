# -*- coding: utf-8 -*-
"""
Created on Wed Nov 15 02:04:10 2017

@author: Daniel Petrykowski
"""

import numpy as np
import pandas as pd
import random


print('Jeżeli chcesz kreować macierz drzew wcinij (1). Jeżeli chesz kreować macierz zarazy kliknij (2)')
war = int(input())
if war == 1:
    print('Kreowanie macierz drzew')
    print('Podaj liczbe wierszy macierzy\n')
    row = int(input())
    print('Podaj liczbe kolumn macierzy\n')
    col = int(input())
    print('Z jakim prawdopodobieństwem losować liczbę 0')
    praZera = int(input())
    print('Z jakim prawdopodobieństwem losować liczbę 1')
    praJeden = int(input())
    if praZera+praJeden<100:
        print('Podaj dolną granice przedziału z którego będą losowane pozostałe liczby')
        minReszta = float(input())
        print('Podaj górną granice przedziału z którego będą losowane pozostałe liczby')
        maxReszta = float(input())
    print('Ile macierzy wygenerować')
    liczbaMacierzy = int(input())
    
    tabLos = []
    for i in range(praZera):
        tabLos.append(0)
        
    for i in range(praJeden):
        tabLos.append(1)
        
    for i in range(100-praZera-praJeden):
        tabLos.append(2)
        
    random.seed()
    random.shuffle(tabLos)
    
    iterat = 0
    for i in range(liczbaMacierzy):
        wynik = np.zeros((row,col))
        for j in range(row):
            for k in range(col):
                random.seed()
                oneLos = random.sample(tabLos,1) 
                if oneLos[0] == 1:
                    wynik[j][k] = 1
                elif oneLos[0] == 2:
                    oneLos = random.uniform(minReszta,maxReszta)
                    oneLos = round(oneLos, 4)
                    wynik[j][k] = oneLos
        
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaDrzew\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynik)
        df.to_csv(sciezka,index=False, header=False)
#  obrot 1
        wynikT = np.zeros((col,row))
        for k in range(row):
            for l in range(col):
                wynikT[l][row-1-k] = wynik[k][l]
        iterat +=1
                 
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaDrzew\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynikT)
        df.to_csv(sciezka,index=False, header=False)
# obrot 2
        wynikT = np.zeros((row,col))
        for k in range(row):
            for l in range(col):
                wynikT[row-1-k][col-1-l] = wynik[k][l]
        iterat +=1
                
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaDrzew\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynikT)
        df.to_csv(sciezka,index=False, header=False)
#obrot 3
        wynikT = np.zeros((col,row))
        for k in range(row):
            for l in range(col):
                wynikT[col-1-l][k] = wynik[k][l]
        iterat +=1    
                
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaDrzew\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynikT)
        df.to_csv(sciezka,index=False, header=False)
        iterat+=1
        
        
elif war == 2:
    print('Kreowanie macierz zarazy')
    print('Podaj liczbe wierszy macierzy\n')
    row = int(input())
    print('Podaj liczbe kolumn macierzy\n')
    col = int(input())
    print('Z jakim prawdopodobieństwem losować liczbę 0 (Podaj z dokładnocia do częci dziesiętnych, np. 32.5)')
    praZera = float(input())
    tabLos = []
    tmp = int(praZera*10)
    for i in range(tmp):
        tabLos.append(0)
    sumPra = praZera
    while sumPra<100:
        print('Podaj kolejna liczbę')
        liczba = int(input())
        print('Podaj prawdopodobieństwo. Pozostało %f' % (100-sumPra))
        pra = float(input())
        sumPra += pra
        tmp = int(pra*10)
        for i in range(tmp):
            tabLos.append(liczba)

    print('Ile macierzy wygenerować')
    liczbaMacierzy = int(input())

    random.seed()
    random.shuffle(tabLos)
    
    iterat = 0
    for i in range(liczbaMacierzy):
        wynik = np.zeros((row,col))
        for j in range(row):
            for k in range(col):
                random.seed()
                oneLos = random.sample(tabLos,1) 
                if oneLos[0] != 0:
                    wynik[j][k] = oneLos[0]
                    
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaZarazy\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynik)
        df.to_csv(sciezka,index=False, header=False)

#  obrot 1
        wynikT = np.zeros((col,row))
        for k in range(row):
            for l in range(col):
                wynikT[l][row-1-k] = wynik[k][l]
        iterat +=1
                 
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaZarazy\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynikT)
        df.to_csv(sciezka,index=False, header=False)
# obrot 2
        wynikT = np.zeros((row,col))
        for k in range(row):
            for l in range(col):
                wynikT[row-1-k][col-1-l] = wynik[k][l]
        iterat +=1
                
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaZarazy\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynikT)
        df.to_csv(sciezka,index=False, header=False)
#obrot 3
        wynikT = np.zeros((col,row))
        for k in range(row):
            for l in range(col):
                wynikT[col-1-l][k] = wynik[k][l]
        iterat +=1    
                
        nazwa = '%dtest' % iterat
        sciezka = 'plikiTestowe\mapaZarazy\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(wynikT)
        df.to_csv(sciezka,index=False, header=False)
        iterat +=1
