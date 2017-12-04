# -*- coding: utf-8 -*-
"""
Created on Sun Nov  5 15:20:01 2017

@author: Daniel Petrykowski
"""

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

#klsa mapy drzew
class mapaDrzew:
#    stałe
#   wspołczynnik destrukcji 
    __stalaDestru=0.01

#   zmienne    
#    zmienna rozkładu drzew na mapie w postaci macierzowej - biblioteki numpy
    mapa=0
    mapaOdjemna=0
    size=0
           
    
#    wczytwanie początkowe mapy
    def readMap(self,i):
        Location = r'D:\Daniel Petrykowski\Documents\GitKraken\ModelROZ\modelrozwojuorganizmowzywych-kornik\plikiTestowe\mapaDrzew\%dtest.csv' % i
        df = pd.read_csv(Location, header=None)
        mapa = np.array(df, dtype=np.float64)
        self.size=mapa.shape
        self.mapaOdjemna=np.zeros((self.size[0],self.size[1]))
        self.mapa=mapa
        
#        zapis do pliku
    def zapisMapa(self, iteracja):
        nazwa = '%dmapaDrzew' % iteracja
        sciezka = 'zapisy\mapaDrzew\%s%s' % (nazwa,'.csv')
        df = pd.DataFrame(self.mapa)
        df.to_csv(sciezka,index=False, header=False)
                 
#        rysowanie mapy rozkładu drzew na podstawie aktualnego stanu
    def wykres(self):
        plt.figure(1)
        plt.imshow(self.mapa, interpolation='nearest')
        plt.grid(True)
        plt.show()
        
#        dodaje pole do macierzy mapaOdjemna
    def destrukcja(self,x,y,liczZjadzDrewna):
        if self.mapa[x][y] - liczZjadzDrewna*self.__stalaDestru > 0.01:
            self.mapaOdjemna[x][y] = liczZjadzDrewna*self.__stalaDestru
        else:
            self.mapa[x][y] = 0
            self.mapaOdjemna[x][y] = 0

        
#        zwraca Pole z macierzy mapa ile robali moze sie tym najesc
    def getPole(self,x,y):
        drewno = self.mapa[x][y]/self.__stalaDestru
        drewno = drewno.astype(int)           
        return drewno
    
    def sizeMapa(self):
        return self.size
    
#    tworzy końcową mape po danej iteracji poprzez mapa-mapaOdjemna 
    def finallMap(self):
        self.mapa=self.mapa-self.mapaOdjemna
        self.mapaOdjemna = np.zeros((self.size[0],self.size[1]))
        
    def liczbaWszystkichDrzew(self):
        return self.mapa.sum()
        
    def stalaDestru(self):
        return self.__stalaDestru
        
        
                    
                    