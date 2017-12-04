# -*- coding: utf-8 -*-
"""
Created on Sun Nov  5 16:43:25 2017

@author: Daniel Petrykowski
"""

import patogen
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import random
import math


# klasa mapy zarazy/robakow
class mapaZarazy:
    
    
    __minWspRozm=0
    __maxWspRozm=0.25
    __mnoznikInicjalizacja = 100#    zmienna rozkładu robakow na mapie w postaci macierzowej - biblioteki numpy
    
    mapa=0
    
    mapaOdjemna=0
#    lista o trzech wymiarach umozliwia dostęp do każdego robaka na danym polu
#    1. wspołrzędna x
#    2. wspołrzędna y
#    3. obiekt patogen/robaki 
    D3mapa=0
#   lista z tablicami, gdzie w tablicy znajdują się na pozycji:
#    1. położenie w osi x zarażonego pola
#    2. położenie w osi y zarażonego pola
#    3. liczba robali znajdujących się w danym momencie na polu
    zarazPola=[]
    size=0
    


    
    
#    wstępne wczytwanie zmiennych z pliku
    def readMap(self,i):
        Location = r'D:\Daniel Petrykowski\Documents\GitKraken\ModelROZ\modelrozwojuorganizmowzywych-kornik\plikiTestowe\mapaZarazy\%dtest.csv' % i
        df = pd.read_csv(Location, header=None)
        mapa = np.array(df)
        mapa=mapa.astype(int)           
        self.size=mapa.shape
        D3mapa=mapa.astype(int)   
        D3mapa=np.dstack((D3mapa,))
        D3mapa=D3mapa.tolist()
        D3mapa=D3mapa
        
        for k in range(self.size[0]):
            for l in range(self.size[1]):
                del D3mapa[k][l][0]   
                if mapa[k][l] > 0:
                    mapa[k][l]=mapa[k][l]*self.__mnoznikInicjalizacja
                    self.zarazPola.append([k,l])
                    for m in range(mapa[k][l]):
                        D3mapa[k][l].append(patogen.patogen())
                  
        mapaOdjemna=np.zeros((self.size[0],self.size[1]))
        self.mapaOdjemna = mapaOdjemna.astype(int)
        self.mapa=mapa
        self.D3mapa=D3mapa
        
#        zapis do pliku
    def zapisMapa(self, iteracja):
        nazwa = '%dmapaZarazy' % iteracja
        sciezka = 'zapisy\mapaZarazy\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(self.mapa)
        df.to_csv(sciezka,index=False, header=False)

#   rysowanie rozładu patogenu na podstawie bieżącej wartoci macierzy 'mapa'
    def wykres(self):
        plt.figure(2)
        plt.imshow(self.mapa, interpolation='nearest')
        plt.grid(True)
        plt.show()
        
#    liczba zarażonych pol = liczba pol w zmiennej zarazPola
    def liczbaZarazPola(self):
        return len(self.zarazPola)
    
#    zwraca zarażone pole z zmiennej zarazPole
    def getZarazPola(self, i):
        return self.zarazPola[i]
      
    def minWspRozm(self):
        return self.__minWspRozm
        
    def maxWspRozm(self):
        return self.__maxWspRozm
    
    def mnoznikInicjalizacja(self):
        return self.__mnoznikInicjalizacja

    
    
#    tworzy końcową mape po danej iteracji poprzez mapa-mapaOdjemna 
    def finallMap(self):
        self.mapa = self.mapa + self.mapaOdjemna
        self.mapaOdjemna = np.zeros((self.size[0],self.size[1]))
        self.mapaOdjemna = self.mapaOdjemna.astype(int)
        self.creatZarazPola()
    
    
    def liczbaWszystkichKornikow(self):
        return self.mapa.sum()
    
    
#zmiana parametrow w tabeli 3Dmapa
    
# dostepneDrewno w przeliczeniu ile robali może się tym najesc
    def mamyDrewno(self,x,y,dostepneDrewno):
        iloscRobali = self.mapa[x][y]
        if dostepneDrewno >= iloscRobali:
            count = list(range(iloscRobali))
            count.reverse()
            for i in count:
                self.D3mapa[x][y][i].jestJedzenie()
                tmp = self.D3mapa[x][y][i].minusLife()
                if tmp == 0:
                    self.usunRobala(x,y,i)
            return iloscRobali
        else:
            random.seed()
            wylosowane = random.sample(range(iloscRobali),dostepneDrewno)
            wylosowane.sort()
            wylosowane.reverse()
            niewylosowane = list(range(iloscRobali))
            for i in wylosowane:
                del niewylosowane[i]
            niewylosowane.reverse()
            zmiennaKontrolna = 0
            for i in wylosowane:
                self.D3mapa[x][y][i].jestJedzenie()
                tmp = self.D3mapa[x][y][i].minusLife()
                if tmp == 0:
                    self.usunRobala(x,y,i)
                    zmiennaKontrolna += 1
            for i in niewylosowane:
                tmp = self.D3mapa[x][y][i-zmiennaKontrolna].brakJedzenia()            
                tmp += self.D3mapa[x][y][i-zmiennaKontrolna].minusLife()
                if tmp < 2:
                    self.usunRobala(x,y,i-zmiennaKontrolna)
            return dostepneDrewno

            
            
    def nieMaDrewna(self,x,y):
        iloscRobali = self.mapa[x][y]
        wylicz = list(range(iloscRobali))
        wylicz.reverse()
        for i in wylicz:
            tmp = self.D3mapa[x][y][i].brakJedzenia()
            tmp += self.D3mapa[x][y][i].minusLife()
            if tmp < 2:
                self.usunRobala(x,y,i)


    
    def zmianaPozycji(self,x,y,x2,y2,liczNaInnePole,iloscRobali):
        random.seed()
        wylosowane = random.sample(range(iloscRobali),liczNaInnePole)
        wylosowane.sort()
        wylosowane.reverse()
        for i in wylosowane:
            robal = self.D3mapa[x][y][i]
            del self.D3mapa[x][y][i]
            self.D3mapa[x2][y2].append(robal)
        self.zmianaMapaOdjemna(x,y,-liczNaInnePole)
        self.zmianaMapaOdjemna(x2,y2,liczNaInnePole)

    def rozmnazanie(self,x,y,iloscRobali):
        if iloscRobali>1:
            random.seed()
            wsp = random.uniform(self.__minWspRozm, self.__maxWspRozm)
            newRobale = iloscRobali*wsp
            newRobale = math.ceil(newRobale)
            self.dodajRobale(x,y,newRobale)
        
    def aktualnaLiczbaRobakow(self,x,y):
        return self.mapa[x][y] + self.mapaOdjemna[x][y]

#################################################################



#    metoda już użyta w metodzie finallMap
    def creatZarazPola(self):
        self.zarazPola=[]
        for i in range(self.size[0]):
            for j in range(self.size[1]):
                if self.mapa[i][j] > 0:
                    self.zarazPola.append([i,j])

# uwzgledniona już w metodach - zmianaPozycji, nieMaDrewna, mamyDrewno
    def zmianaMapaOdjemna(self, x, y, plusLiczRobali):
        self.mapaOdjemna[x][y] = self.mapaOdjemna[x][y] + plusLiczRobali

    def dodajRobale(self,x,y,liczbaRobali):
        for i in range(liczbaRobali):
            self.D3mapa[x][y].append(patogen.patogen())
        self.zmianaMapaOdjemna(x,y,liczbaRobali)

    def usunRobala(self,x,y,i):
        del self.D3mapa[x][y][i]
        self.zmianaMapaOdjemna(x,y,-1)




        