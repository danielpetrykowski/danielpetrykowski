# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import mapadrzew
import mapazarazy
import iteracja
import patogen
import pandas as pd

def parametryWejsciowe(robaki, drzewa, iteracja):
    p=patogen.patogen()
    l=open("parametrWejsciowy.txt", "w")
    l.write('Parametry Nastaw:\n')
    l.write("Mapa drzew:\n")
    l.write("Stała destrukcji = %f\n" % drzewa.stalaDestru())
    l.write("Rozmiar mapy = (%d,%d)\n" % (drzewa.sizeMapa()[0], drzewa.sizeMapa()[1]))
    l.write("Mapa zarazy:\n")
    l.write("Minimalny współczynnik rozmnażania = %f\n" % robaki.minWspRozm())
    l.write("Maksymalny współczynnik rozmnażania = %f\n" % robaki.maxWspRozm())
    l.write("Mnożnik inicjalizacji = %d\n" % robaki.mnoznikInicjalizacja())
    l.write("Patogen:\n")
    l.write("Maksymalne życie = %d\n" % p.life)
    l.write("Początkowy głód = %d\n" % p.glod)
    l.write("Współczynnik umieralnoci = %d\n" % p.wspUmiera())
    l.write("Współczynnik glodu = %d\n" % p.wspGlod())
    l.write("Współczynnik jedzenie = %d\n" % p.wspJedzenie())
    l.write("Iteracja:\n")
    l.write("minPoleGlowneBezDrzewa = %f\n" % iteracja.minPoleGlowneBezDrzewa())
    l.write("maxPoleGlowneBezDrzewa = %f\n" % iteracja.maxPoleGlowneBezDrzewa())
    l.write("minPoleGlowneZDrzewem = %f\n" % iteracja.minPoleGlowneZDrzewem())
    l.write("maxPoleGlowneZDrzewem = %f\n" % iteracja.maxPoleGlowneZDrzewem())
    l.write("rozrzutPolaZDrzewemWDol = %f\n" % iteracja.rozrzutPolaZDrzewemWDol())
    l.write("rozrzutPolaZDrzewemWGore = %f\n" % iteracja.rozrzutPolaZDrzewemWGore())
    l.write("stosunek = %f\n" % iteracja.stosunek())
    l.close()



"""wczytywanie mapy drzew"""

print('Podaj numer próbki testowej mapy drzew')
i = int(input())
drzewa = mapadrzew.mapaDrzew()
drzewa.readMap(i)
drzewa.wykres()
print('Liczba drzew = %f' % drzewa.liczbaWszystkichDrzew())



"""wczytywanie mapy patogenu"""
print('Podaj numer próbki testowej mapy zarazy')
i = int(input())
robaki = mapazarazy.mapaZarazy()
robaki.readMap(i)
robaki.wykres()
print('Liczba kornikow = %d' % robaki.liczbaWszystkichKornikow())    


iteracja = iteracja.iteracja()
parametryWejsciowe(robaki,drzewa,iteracja)

print('Start (T/N)')
liczbaIteracji = input()
i=0
while robaki.liczbaWszystkichKornikow() > 0:
    i+=1
    wynik = iteracja.nextStep(robaki,drzewa)
    robaki = wynik[0]
    drzewa = wynik[1]
    print('Mapa Drzew - iteracja %d' % i) 
    drzewa.wykres()
    print('Liczba drzew = %f' % drzewa.liczbaWszystkichDrzew())
    print('Mapa Robakow - iteracja %d' % i) 
    robaki.wykres()
    print('Liczba kornikow = %d' % robaki.liczbaWszystkichKornikow())    




 

    
    
    
