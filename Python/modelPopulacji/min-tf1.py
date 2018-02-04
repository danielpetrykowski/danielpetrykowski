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
import tensorflow as tf

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

def wczytMapaDrzew(i):
    drzewa = mapadrzew.mapaDrzew()
    drzewa.readMap(i)
#    drzewa.wykres()
    return drzewa

def wczytMapaZarazy(i):
    robaki = mapazarazy.mapaZarazy()
    robaki.readMap(i)
#    robaki.wykres()
    return robaki

sess = tf.InteractiveSession()

daneDrzewa = tf.placeholder(tf.float32, [10, 10])
daneZarazy = tf.placeholder(tf.float32, [10, 10])
finalZaraza = tf.placeholder(tf.float32, [10, 10])

daneDrzewa_flat = tf.reshape(daneDrzewa, [1,-1])
daneZarazy_flat = tf.reshape(daneZarazy, [1,-1])
finalZaraza_flat = tf.reshape(finalZaraza, [1,-1])


W_initial = tf.abs(tf.truncated_normal([100,100], stddev=0.1))
b_initial = tf.abs(tf.truncated_normal([100], stddev=0.1))

W_d = tf.Variable(W_initial)
b_d = tf.Variable(b_initial)

W_z = tf.Variable(W_initial)
b_z = tf.Variable(b_initial)

nn_d = (tf.matmul(daneDrzewa_flat, W_d) + b_d)
nn_z = (tf.matmul(daneZarazy_flat, W_z) + b_z)

W_z2 = tf.Variable(W_initial)
W_d2 = tf.Variable(W_initial)
b_2 = tf.Variable(b_initial)

nn_3 = tf.nn.relu((tf.matmul(nn_d, W_d2) + tf.matmul(nn_z, W_z2) + b_2))

W_3 = tf.Variable(W_initial)
b_3 = tf.Variable(b_initial)

out = tf.nn.relu(tf.matmul(nn_3, W_3) + b_3)

cost = tf.losses.mean_squared_error(finalZaraza_flat, out)
train_step = tf.train.AdamOptimizer(0.001).minimize(cost)

sess.run(tf.global_variables_initializer())

sprawdzam = 0

iteracja = iteracja.iteracja()

for i in range(2000):
    
    """wczytywanie mapy drzew"""
    drzewa = wczytMapaDrzew(i)
    print('Liczba drzew = %f' % drzewa.liczbaWszystkichDrzew())

    """wczytywanie mapy patogenu"""
    robaki = wczytMapaZarazy(i)
    print('Liczba kornikow = %d' % robaki.liczbaWszystkichKornikow())    
    
    
    j=0
#    while robaki.liczbaWszystkichKornikow() > 0:
    while j < 10:       
        j+=1
        inRobaki = robaki.getMapaZarazy()
        inDrzewa = drzewa.getMapaDrzew()
        wynik = iteracja.nextStep(robaki,drzewa)
        robaki = wynik[0]
        drzewa = wynik[1]
        outRobaki = robaki.getMapaZarazy()
#        print('Mapa Drzew - iteracja %d' % j) 
#        drzewa.wykres()
        #    drzewa.getMapaDrzew()
        print('Liczba drzew = %f' % drzewa.liczbaWszystkichDrzew())
#        print('Mapa Robakow - iteracja %d' % j) 
#        robaki.wykres()
        #    robaki.getMapaZarazy()
        print('Liczba kornikow = %d' % robaki.liczbaWszystkichKornikow())    
#        if sprawdzam % 50 == 0:
        train_accuracy = cost.eval(feed_dict={daneDrzewa: inDrzewa, daneZarazy: inRobaki, finalZaraza: outRobaki})
        outR = out.eval(feed_dict={daneDrzewa: inDrzewa, daneZarazy: inRobaki})
        outR = outR.reshape([10,10])
        print('step %d, training accuracy %g' % (i, train_accuracy))
        train_step.run(feed_dict={daneDrzewa: inDrzewa, daneZarazy: inRobaki, finalZaraza: outRobaki})
        sprawdzam+=1

    
     
    
        
        
        
