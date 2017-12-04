# -*- coding: utf-8 -*-
"""
Created on Sat Nov 11 16:07:04 2017

@author: Daniel Petrykowski
"""

import random
import math


class iteracja:


    __minPoleGlowneBezDrzewa = 0.1
    __maxPoleGlowneBezDrzewa = 0.3
    __minPoleGlowneZDrzewem = 0.6
    __maxPoleGlowneZDrzewem = 0.8
    __rozrzutPolaZDrzewemWDol = 0.05
    __rozrzutPolaZDrzewemWGore = 0.05
    __stosunek = 0.33 


    def minPoleGlowneBezDrzewa(self):
        return self.__minPoleGlowneBezDrzewa
    
    def maxPoleGlowneBezDrzewa(self):
        return self.__maxPoleGlowneBezDrzewa

    def minPoleGlowneZDrzewem(self):
        return self.__minPoleGlowneZDrzewem

    def maxPoleGlowneZDrzewem(self):
        return self.__maxPoleGlowneZDrzewem

    def rozrzutPolaZDrzewemWDol(self):
        return self.__rozrzutPolaZDrzewemWDol

    def rozrzutPolaZDrzewemWGore(self):
        return self.__rozrzutPolaZDrzewemWGore

    def stosunek(self):
        return self.__stosunek


    def nextStep(self, robaki, drzewa):
        liczbaZarazPola = robaki.liczbaZarazPola()
        for i in range(liczbaZarazPola):
            x=robaki.getZarazPola(i)[0]
            y=robaki.getZarazPola(i)[1]
            drewno = drzewa.getPole(x,y)
    #        odżywianie
            if drewno>0:
                wykorzystaneDrewno = robaki.mamyDrewno(x,y,drewno)
                drzewa.destrukcja(x,y,wykorzystaneDrewno)
                liczbaRobakow = robaki.aktualnaLiczbaRobakow(x,y)
    #       rozmnażanie
                robaki.rozmnazanie(x,y,liczbaRobakow)
            else:
                robaki.nieMaDrewna(x,y)
                liczbaRobakow = robaki.aktualnaLiczbaRobakow(x,y)
    #        przemieszczanie
            nowaPozycja = self.migracje(x,y,drzewa,liczbaRobakow)
            zmiennaKontrolna = 0
            for j in range(len(nowaPozycja)):
                robaki.zmianaPozycji(x,y,nowaPozycja[j][0],nowaPozycja[j][1],nowaPozycja[j][2],liczbaRobakow-zmiennaKontrolna)
                zmiennaKontrolna += nowaPozycja[j][2]
        robaki.finallMap()
        drzewa.finallMap()
        return [robaki,drzewa]
        

    
        

    def migracje(self,x, y, mapaDrzew, liczbaRobali):
        polaDrzew = []
        polaBezDrzew = []
        wynik = []
        liczbaPolDrzew = 0
        liczbaPolBezDrzew = 0
        
        
    
        for pomX in [-1, 0, 1]:
            for pomY in [-1, 0, 1]:
                if x+pomX>=0 and x+pomX<mapaDrzew.sizeMapa()[0] and y+pomY>=0 and y+pomY<mapaDrzew.sizeMapa()[1] and (pomX!=0 or pomY!=0):
                    if mapaDrzew.getPole(pomX+x, pomY+y)>0:
                        polaDrzew.append([pomX+x,pomY+y,1])
                        liczbaPolDrzew += 1
                    else:
                        polaBezDrzew.append([pomX+x,pomY+y,0])
                        liczbaPolBezDrzew += 1
    
            
        random.seed()
        if mapaDrzew.getPole(x,y) > 0:
            wsp = random.uniform(self.__minPoleGlowneZDrzewem, self.__maxPoleGlowneZDrzewem)    
        else:
            wsp = random.uniform(self.__minPoleGlowneBezDrzewa, self.__maxPoleGlowneBezDrzewa)    
        liczbaRobakowPoleGlowne = liczbaRobali*wsp
        liczbaRobakowPoleGlowne = math.floor(liczbaRobakowPoleGlowne)
        pozostaleRobaki = liczbaRobali - liczbaRobakowPoleGlowne
        if liczbaPolDrzew>0:
            wsp = (100*liczbaPolDrzew)/((8*self.__stosunek-liczbaPolDrzew*self.__stosunek+liczbaPolDrzew)*100)
            random.seed()
            wsp = random.uniform(self.__rozrzutPolaZDrzewemWDol+wsp,self.__rozrzutPolaZDrzewemWGore+wsp)
        else:
            wsp = 0
        liczbaRobakowPolaDrzew = pozostaleRobaki * wsp
        liczbaRobakowPolaDrzew = math.floor(liczbaRobakowPolaDrzew)
        liczbaRobakowPolaBezDrzew = pozostaleRobaki - liczbaRobakowPolaDrzew
        
        if liczbaPolDrzew>0:
            podstawaLiczbaRobaki = math.floor(liczbaRobakowPolaDrzew/liczbaPolDrzew)
            resztaLiczbaRobaki = liczbaRobakowPolaDrzew % liczbaPolDrzew
            for i in range(liczbaPolDrzew):
                polaDrzew[i][2]=podstawaLiczbaRobaki
            random.seed()
            losowanie = random.sample(range(liczbaPolDrzew),resztaLiczbaRobaki)
            losowanie.sort()
            losowanie.reverse()
            for i in losowanie:
                polaDrzew[i][2]+=1
        
        if liczbaPolBezDrzew>0:
            podstawaLiczbaRobaki = math.floor(liczbaRobakowPolaBezDrzew/liczbaPolBezDrzew)
            resztaLiczbaRobaki = liczbaRobakowPolaBezDrzew % liczbaPolBezDrzew
            for i in range(liczbaPolBezDrzew):
                polaBezDrzew[i][2]=podstawaLiczbaRobaki
            random.seed()
            losowanie = random.sample(range(liczbaPolBezDrzew),resztaLiczbaRobaki)
            losowanie.sort()
            losowanie.reverse()
            for i in losowanie:
                polaBezDrzew[i][2]+=1
    
        wynik = polaDrzew + polaBezDrzew
            
        
        return wynik