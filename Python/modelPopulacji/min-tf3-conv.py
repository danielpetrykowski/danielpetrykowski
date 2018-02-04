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


print("1 - wartosci losowe\n2 - wczytaj wartosci")
a = int(input())

daneDrzewa = tf.placeholder(tf.float32, [10, 10])
daneZarazy = tf.placeholder(tf.float32, [10, 10])
finalZaraza = tf.placeholder(tf.float32, [10, 10])

finalZaraza_flat = tf.reshape(finalZaraza, [1,-1])

w_zero = tf.zeros([1,10], tf.float32)
daneDrzewa_1 = tf.concat([w_zero, daneDrzewa, w_zero], 0)
c_zero = tf.zeros([12,1], tf.float32)
daneDrzewa_big = tf.concat([c_zero, daneDrzewa_1, c_zero], 1)
daneDrzewa_big_flat = tf.reshape(daneDrzewa_big, [-1])

filtr_przeksztalcajacy = []

z = [0,1,2,12,13,14,24,25,26]
for i in range(100):
    if (i>9) and (i % 10 == 0):
        for j in range(9):
            z[j]=z[j]+2  
    filtr_przeksztalcajacy.append([z[0]+i,z[1]+i,z[2]+i,z[3]+i,z[4]+i,z[5]+i,z[6]+i,z[7]+i,z[8]+i])

daneDrzewa_transform = []

for i in range(100):
    daneDrzewa_transform.append(tf.gather(daneDrzewa_big_flat, filtr_przeksztalcajacy[i]))
    daneDrzewa_transform[i]=tf.reshape(daneDrzewa_transform[i], [1,-1])

daneDrzew_sqare = tf.concat(daneDrzewa_transform, 0)

W_initial = tf.abs(tf.truncated_normal([9,9], stddev=0.1))
b_initial = tf.abs(tf.truncated_normal([100,9], stddev=0.1))

W_w1 = tf.Variable(W_initial, "W_w1")
b_w1 = tf.Variable(b_initial, "b_w1")

daneDrzew_w1 = (tf.matmul(daneDrzew_sqare, W_w1) + b_w1)

W_conv1 = tf.reshape(daneDrzew_w1, [3,3,1,-1])
b_initial = tf.abs(tf.truncated_normal([100], stddev=0.1))
b_conv1 = tf.Variable(b_initial, "b_conv1")
x_daneZarazy=tf.reshape(daneZarazy, [-1,10,10,1])

after_conv1 = tf.nn.conv2d(x_daneZarazy, W_conv1, strides=[1,1,1,1], padding="SAME")

after_conv1_flat = tf.reshape(after_conv1, [-1])

filtr_przeksztalcajacy = []

z = [0,1,2,12,13,14,24,25,26]
for i in range(100):
    if (i>9) and (i % 10 == 0):
        for j in range(9):
            z[j]=z[j]+2  
    filtr_przeksztalcajacy.append([z[0]+i,z[1]+i,z[2]+i,z[3]+i,z[4]+i,z[5]+i,z[6]+i,z[7]+i,z[8]+i])
    for j in range(9):
        z[j]=z[j]+144

tab_after_conv1 = []

for i in range(100):
    tab_after_conv1.append(tf.gather(after_conv1_flat, filtr_przeksztalcajacy[i]))
    tab_after_conv1[i]=tf.reshape(tab_after_conv1[i], [1,-1])

data_r1 = tf.concat(tab_after_conv1, 0)

data_r2 = tf.reshape(data_r1, [10,-1])

W_initial = tf.abs(tf.truncated_normal([90,10], stddev=0.1))
b_initial = tf.abs(tf.truncated_normal([10,10], stddev=0.1))

W_w2 = tf.Variable(W_initial, "W_w2")
b_w2 = tf.Variable(b_initial, "b_w2")

dane_w2 = tf.nn.relu(tf.matmul(data_r2, W_w2) + b_w2)

out = tf.reshape(dane_w2, [1,-1])
out_int = tf.cast(out, tf.int32)

cost = tf.losses.mean_squared_error(finalZaraza_flat, out)
train_step = tf.train.AdamOptimizer(0.001).minimize(cost)




init = tf.global_variables_initializer()


saver = tf.train.Saver({"myW_w1": W_w1, "myb_w1": b_w1, "myW_w2": W_w2, "myb_w2": b_w2, "myb_conv1": b_conv1})

if a == 1:
    sprawdzam = 0
else:
    plik = open('iteracjeConv.txt')
    try:
        tekst = plik.read()
    finally:
        plik.close()
    sprawdzam = int(tekst)

iteracja = iteracja.iteracja()



with tf.Session() as sess:
    sess.run(init)
    
    if a==2:
        saver.restore(sess, "/tmp/modelConv.ckpt")
    
    for i in range(10000,20000):
        
        """wczytywanie mapy drzew"""
        drzewa = wczytMapaDrzew(i)
        print('Liczba drzew = %f' % drzewa.liczbaWszystkichDrzew())
    
        """wczytywanie mapy patogenu"""
        robaki = wczytMapaZarazy(i)
        print('Liczba kornikow = %d' % robaki.liczbaWszystkichKornikow())    
        
        
        j=0
#        while robaki.liczbaWszystkichKornikow() > 0:
        while j < 5:       
            j+=1
            inRobaki = robaki.getMapaZarazy()
            inRobaki = inRobaki.astype(float)
            inDrzewa = drzewa.getMapaDrzew()
            wynik = iteracja.nextStep(robaki,drzewa)
            robaki = wynik[0]
            drzewa = wynik[1]
            outRobaki = robaki.getMapaZarazy()
            outRobaki = outRobaki.astype(float)
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
            outR = out_int.eval(feed_dict={daneDrzewa: inDrzewa, daneZarazy: inRobaki})
            outR = outR.reshape([10,10])
            print('step %d, training accuracy %g' % (i, train_accuracy))
            train_step.run(feed_dict={daneDrzewa: inDrzewa, daneZarazy: inRobaki, finalZaraza: outRobaki})
            sprawdzam+=1
            
        if sprawdzam % 2000 == 0:
            save_path = saver.save(sess, '/tmp/modelConv.ckpt')
            print("Model saved in file: %s" % save_path)
            plik = open('iteracjeConv.txt', 'w')
            plik.write(str(sprawdzam))
            plik.close()
