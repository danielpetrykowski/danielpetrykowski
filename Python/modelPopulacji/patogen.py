# -*- coding: utf-8 -*-
"""
Created on Sun Nov  5 16:40:37 2017

@author: Daniel Petrykowski
"""

class patogen:
#    stałe
#    wspołczynnik umieralnosci
    __wspUmiera = 5
#    wspolczynnik glodu
    __wspGlod = 20
#    wspolczynnik jedzenia
    __wspJedzenie=15
    
#    z,ienne
    life = 100
#    maksymalna wartosc głodu =100
    glod = 0  
        
    def __init__(self):
        self.__wspUmiera = 5
        self.__wspGlod = 20
        self.__wspJedzenie=15
    
#        odejmuje życie
    def minusLife(self):
        self.life = self.life - self.__wspUmiera
        if self.life <= 0:
            return 0
        return 1
    
#    zwiększa głod
    def brakJedzenia(self):
        self.glod = self.glod + self.__wspGlod 
        if self.glod >= 100:
            return 0
        return 1
        
#    zmniejsza glod
    def jestJedzenie(self):
        if self.glod == 0 :
            return
        elif self.glod <= self.__wspJedzenie:
            self.glod = 0
        else:
            self.glod = self.glod - self.__wspJedzenie
        
#        zmienia pozycje
        
    def wspUmiera(self):
        return self.__wspUmiera
    
    def wspGlod(self):
        return self.__wspGlod
    
    def wspJedzenie(self):
        return self.__wspJedzenie
        
        