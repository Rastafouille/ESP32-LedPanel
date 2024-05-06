# ESP32 LED PANEL

Panneaux d'affichage connecté permettant la récupération et l'affichage déroulant de données custom comme par exemple :

- cours de cryptomonnaies (API www.livecoinwatch.com)

- heure sur differents fuseaux

- production panneaux solaires et consommation électrique de la maison (API www.solaxcloud.com dans mon cas

- couleur EDF Tempo (API www.particulier.edf.fr)

- Prévision météo (API www.openweathermap.org)

Il va falloir 

 - un ESP32 pour se connecter au réseau et récupérer les infos
<https://fr.aliexpress.com/item/1005006422498371.html>

 - des panneaux led P5
<https://fr.aliexpress.com/item/32760855293.html>
 
Pour le cablage :

<img src="IMG/ESP32P5RGBInstallation.png" width="500"/>

Voila ce que ca peut donner, à vous de modifier !

Pensez à modifier credentials.h avec vos token, ssid et password.

<img src="IMG/1.png" width="500"/>

<img src="IMG/2.png" width="500"/>

<img src="IMG/3.png" width="500"/>

<img src="IMG/4.png" width="500"/>

sources:

<https://www.youtube.com/watch?v=KuOBKi-yVoc&t=408s>
<https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA/blob/master/examples/BitmapIcons/BitmapIcons.ino>