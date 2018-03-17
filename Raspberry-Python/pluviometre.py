import time
import pigpio
import requests as req
import vw
from time import gmtime, strftime

RX = 11
BPS = 1000

pi = pigpio.pi()
rx = vw.rx(pi, RX, BPS)
start = time.time()

print("En attente de la réception des données")
while True:
    while rx.ready():
        Data = "".join(chr(c) for c in rx.get())
        # on recoit Ident Ph et h
        Ident = Data[0:2]
        Ph = Data[2:7]
        hauteur = Data[7:12]
        date = strftime("%Y-%m-%d %H:%M:%S", gmtime())
        d = {'date': date, 'identification': Ident, 'ph': Ph, 'Hauteur': hauteur}
        resp = req.post("http://data.particitae.fr/communication.php",d)
rx.cancel()
