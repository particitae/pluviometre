Projet Pluviomètre 2017-2018

Auteurs : Clément Killisly, Arnaud Motard, Hugo Herouard, Thibaud Estarellas, Riad Mostefaoui, Pierre Brochard, Laure Turcati, Jérôme Gaudin, Baptiste Fabre

[Université de Bordeaux](https://www.u-bordeaux.fr/) - [Particitae](http://particitae.fr/) - [Celia](http://www.celia.u-bordeaux1.fr)

# Résumé

L’étude de l’environnement urbain est une problématique d’actualité qui s’intéresse aux impacts engendrés par les activités humaines sur la nature. C’est autour de cet axe que l’observatoire participatif PartiCitaE s’articule. Celui-ci poursuit deux objectifs conjoints, l’un de construire une connaissance systémique de l’environnement urbain et l’autre d’accompagner la transition environnementale. L’eau de pluie, interagissant à la fois avec l’atmosphère urbaine et les êtres vivants, s’inscrit au cœur des problématiques de PartiCitaE. En vue d’approfondir cette thématique PartiCitaE a sollicité les étudiants du Master d’instrumentation physique de l’Université de Bordeaux afin de concevoir un pluviomètre.

Destiné à être utilisé dans un contexte d’observatoire participatif, ce pluviomètre doit répondre à certaines spécifications techniques : Il doit être autonome, facilement reproductible et doit permettre d’effectuer des mesures standardisées que seront sauvegardées et transmises à un serveur.

Les travaux effectués se répartissent en plusieurs parties :

-   La conception du prototype qui comprend le choix de l’électronique, la réalisation du corps (structures externe et interne), la schématisation du montage électronique et du prototype, la disposition et le câblage de l’électronique et enfin l’assemblage du pluviomètre.

-   La programmation qui comprend deux sous-parties :

    - Le codage Arduino des différents capteurs, de l’automatisation des mécanismes (ouverture et fermeture d’un capot et d’un robinet pour la collecte et vidange de l’eau) et de l’envoi des données par transmission radio sur un Rasbperry.

    - Le codage Python pour la prise en charge des données et de leurs envoie et stockage sur un serveur web par Rasbperry.

-   La recherche sur des méthodes de détections de polluant dans l’eau.

----------
# Introduction


Cette documentation présentera deux études de l’art réalisées : l’une sur la détection de polluant dans l’eau et l’autre sur les différentes méthodes de mesures de hauteur d’eau. Ensuite l’architecture, le dimensionnement de la structure et les différents systèmes du pluviomètre seront décrites avant de finir sur la partie modélisation et résultats obtenus.
L'ensemble des fichiers sont sous licence GNU General Public License v3.0.

----------
# État de l’art

## 1)  Mesure du niveau d'eau

Plusieurs capteurs peuvent être envisagés

-   Capteur immersif:

Le capteur de niveau d’eau ST045 fonctionne à partir du principe de conductimétrie. Une piste sur deux est reliée à la sortie analogique et le reste des pistes sont reliées à la tension de référence (5 Volts).
Lorsque la hauteur d’eau augmente, l’électricité est conduite par l’eau entre les différentes pistes et la tension sur les pistes reliées à la sortie analogique augmente. Avec un niveau d’eau croissant, la tension en sortie du capteur augmente et on peut ainsi remonter à la hauteur d’eau. Nous avons réalisé un test avec de l’eau distillée et nous avons constaté que la réponse du capteur dépend aussi de la composition en minéraux de l’eau. En effet, le capteur étant un capteur basé sur la conductivité, la composition minérale de l’eau influencera la
conductivité de celle-ci et donc la tension renvoyée par le capteur. Cela pose problème pour la mesure de
hauteur d’eau puisque la composition de l’eau de pluie varie dans le temps et que nous ne pouvons à priori pas la prévoir et corriger les mesures. Cela rajoute une forte incertitude à la mesure de hauteur
d’eau. Nous avons conclu que l’incertitude était de l’ordre de 5 millimètres sur la mesure de hauteur d’eau.

Ce capteur n’est donc pas adapté à des mesures pluviométriques de précision. Dans un souci de délais et de budget nous n’avons pas pu changer de solution technique pour la mesure de hauteur d’eau et nous
avons donc gardé le capteur ST045 malgré ses défauts mais pour la reproduction du pluviomètre, il faudrait ne pas utiliser ce capteur.

-   Flotteur vertical:

Ce type de capteur est utilisé pour réaliser des mesures de hauteurs de carburant dans les réservoirs des voitures. Il est approximatif et s’alimente avec une tension supérieure à celle que peut délivrer notre carte électronique.

-   Capteur de pression:

Un capteur de pression est trop imprécis du fait des faibles quantités d’eau à mesurer.

-   Capteur CCD et laser:

L’idée est d’utiliser un laser qui va venir se réfléchir sur la surface de l’eau. La réflexion du laser est réceptionnée par une barrette CCD. Plus le niveau d’eau augmente et plus la réflexion se fait avec un angle différent. Ce système n’a pas pu être testé étant donné le coup du capteur CCD.

-   Plaques de prototypage:

Un capteur peut être réalisé à partir de plaques de prototypage. La plaque possède des pistes parallèles entre elles. Si l’on relie les pistes entre elles en plaçant une résistance entre chacune des pistes, nous pouvons obtenir la hauteur d’eau avec comme incertitude la hauteur de la piste. Lorsque le niveau d’eau augmente, l’eau va venir
court-circuiter les résistances et modifier la tension analogique reçu par la carte électronique. Cependant tous les capteurs immersifs sont soumis à des problèmes d’oxydation.

-   Mesure de distance infrarouge:

Un émetteur/récepteur infrarouge est également une bonne alternative. Des tests ont été réalisés avec le capteur Sharp GP2Y0A21YK0F. Il faut étalonner ce capteur en fonction de la plage de mesure utile car il
n’est pas linéaire. De plus lorsqu’on observe le signal à l’oscilloscope on constate qu’il est très bruité. On peut diminuer de manière conséquente ce bruit en utilisant un condensateur. Lors des tests nous avons constaté que la lumière infrarouge se réfléchit bien sur la surface de l’eau. Nous n’avons pas eu de secondes réflexions au fond du réservoir d’eau.

-   Capteur ultrason:

Le capteur ultrason est le plus adapté pour une mesure de hauteur d’eau. Un capteur peu onéreux et possédant une incertitude de mesure de +/- 2 mm est le HC-SR04. Cependant nous avons dimensionné certaines parties du pluviomètre en fonction du capteur ST045. Le capteur ultrason n’a pas pu s’adapter sur notre pluviomètre en raison de ses dimensions.

## 2)  Mesure de la qualité de l'eau

### -   Mesure du pH
Une première mesure simple à mettre en œuvre est celle du pH. Les pH de l'eau de pluie pouvant varier en fonction de divers paramètres cette mesure paraît pertinente.

### - Mesure de concentrations en polluants
Un des objectifs du pluviomètre participatif est de détecter des polluants dans l’eau de pluie.

L’objectif est de trouver une méthode permettant de détecter plusieurs substances au même moment de manière non contaminatrice.
Différentes méthodes répondant à nos critères ont été identifiées :

-   Les réactions chimiques colorimétriques
Des réactions colorimétriques permettraient de détecter certains polluants, toutefois cet technologie requière de concevoir différents réservoirs pour réaliser des mesures de différents polluants une par une dans l’obscurité.
Par ailleurs l'élimination des échantillons contaminés pose potentiellement problème étant donné que la vidange se fait dans le jardin des participants. Ces raisons nous dissuadent d’avoir recours à des moyens chimiques.

-   La spectrométrie
La technique de spectrométrie est la plus adaptée pour l’étude des métaux lourds. Pour le cas particulier des particules, on peut utiliser la spectrométrie de masse. Par l’analyse d’un spectre lumineux, cette technique permet de détecter et d’identifier des éléments chimiques.

Dans notre cas, il y a deux solutions : acheter un spectromètre miniaturisé ou en créer un.

Les spectromètres automatisés permettant les mesures de polluants dans l’eau de pluie. Néanmoins, étant des technologies de pointe, leur prix ne peut pas correspondre à un budget raisonnable pour le pluviomètre.

La construction d’un spectromètre étant difficilement réalisable avec le budget alloué au projet et les délais dont nous disposions, nous n’avons pas pu la réaliser.

-   La voltamétrie

La voltampérométrie permet d’identifier et de mesurer quantitativement, les ions ou composés organiques présents dans l’échantillon. Cependant, cette méthode requiert d’avoir fait un traitement au préalable de l’eau
afin d’analyser un résidu solide et n’est donc pas adaptable simplement dans le cadre d’un pluviomètre autonome.

-   La mousse hydrophobe

Une autre possibilité consiste à faire passer l’eau à travers une mousse hydrophobe dans laquelle les métaux sont retenus pour analyser par la suite en laboratoire la mousse et le résidu de métaux. L'envoie de la
mousse en laboratoire complique la démarche participative et n'a pas été retenue pour l'instant.

Pour conclure, les méthodes d’analyse de polluants au sein de l’eau de pluie étaient difficilement réalisables dans le cadre de notre projet et nous n’avons donc pas pu répondre à cette problématique. La méthode de
spectrométrie semble cependant la plus adaptée à de telles mesures dans le cadre d’un pluviomètre participatif.

# Technique

## 1) Architecture et dimensionnement

L’architecture du pluviomètre et les dimensions de ses différents composants ont été définit en fonction de plusieurs facteurs.

![Architecture du Pluviomètre](https://upload.wikimedia.org/wikipedia/commons/a/a6/Architecture-du-Pluviom%C3%A8tre.jpg  "Architecture du Pluviomètre")

-   L’architecture:

La récolte de l’eau de pluie se fait par un entonnoir (1) reposant sur un tube de PVC extérieur (2) contenant un boîtier de dérivation (3) vissé à la paroi, l’électronique est disposée autour de ce boîtier. Un manchon (4) a été fabriqué afin de faire la jonction entre l’entonnoir et le tube de PVC extérieur du pluviomètre. Ce manchon est fixé sur l’entonnoir et vient s’emboîter sur le tube de PVC. Cette solution a été choisie afin de pouvoir accéder facilement à l’électronique en cas de problèmes.

L’entonnoir amène l’eau dans un petit tube de PVC (5) contenant les capteurs, ce tube est fermé par un manchon visé (6) sur lequel est monté un robinet. Un tuyau allant de l’entonnoir au fond du tube de PVC a été installer afin de ne pas provoquer d’éclaboussure lors de la chute des gouttes d’eau de pluie. En effet, ces éclaboussures vont perturber la surface d’eau et donc fausser les mesures de hauteur d’eau. Un troisième
manchon (7) constitue le fond du pluviomètre. La stabilité du pluviomètre est assurée par un trépied fixer sur un deuxième tube de PVC qui vient s’emboîter sur ce dernier manchon.

-   Le dimensionnement:

Le diamètre et la hauteur du tube extérieur ne devaient pas être trop grand pour permettre le transport aisé du pluviomètre mais ce tube devait toutefois représenter un espace suffisant pour y accueillir l’électronique. La différence de diamètre entre le tube intérieur et l’entonnoir (donc le tube extérieur) a été réfléchit en fonction des quantités de précipitations courantes et du bon fonctionnement des capteurs. Le pluviomètre doit pouvoir fournir des mesures régulièrement (de l’ordre de la minute) même lorsque qu’il ne pleut pas abondamment.
Le PH-mètre doit être immergé dans un minimum de 2 cm d’eau. La précision du capteur de hauteur d’eau est de l’ordre du millimètre. Une grande part des pluies débitant moins de 1 mm par jour, il a été décidé d’avoir un facteur 5 entres les diamètres des tubes afin d’obtenir une multiplication 25 entres les vraies précipitations et la hauteur d’eau collectée. Un schéma du dimensionnement est disponible à cette [adresse](https://upload.wikimedia.org/wikipedia/commons/3/30/Sch%C3%A9ma_du_dimensionnement_du_pluviom%C3%A8tre_-_Particitae.png).

## 2) Systèmes

**Le système mécanique**

Les actions mécaniques sont réalisées avec des servomoteurs. Les servomoteurs sont alimentés en 5V et délivrent un couple relativement faible. L’ouverture du capot est réalisée autour d’un pivot vertical.
Cette rotation du capot s’effectue à l’horizontale pour éviter d’engendrer un effort trop important de la part du servomoteur et également pour limiter les prises d’air pouvant faire bouger le pluviomètre et fausser les mesures. Une gorge a ensuite été faite sur le capot afin de stopper la mise en rotation de celui-ci par rapport à l’axe du servomoteur.

Les servomoteurs ne sont pas étanches. Ils supportent l’humidité mais ne peuvent résister à de fortes pluies. Il a donc été pensé dans un premier temps à placer le servomoteur responsable de l’ouverture du capot à l’intérieur du pluviomètre. Cependant le système permettant d’ouvrir le capot nécessité un renvoi d’angle difficile à concevoir mécaniquement. Le servomoteur a finalement été fixé à l’extérieur du pluviomètre pour agir directement sur la mise en rotation du capot. Pour protéger ce servomoteur de l’eau, un boîtier a été usiné et fixé sur un des coté du pluviomètre. Ce boîtier peut être réalisé avec une imprimante 3D. Un joint torique a également été placé pour empêcher l’humidité de pénétrer dans le boîtier. Le boîtier est fixé au pluviomètre à l’aide de deux vis.

 Le capot du pluviomètre est conçu pour s’ouvrir uniquement quand il pleut. Un interrupteur permet toutefois l’ouverture manuelle du capot en cas de problème. Un autre interrupteur permet la mise sous tension du de la carte électronique.

Le système permettant la vidange du pluviomètre a dû répondre à plusieurs critères. Il devait permettre l’évacuation de l’eau de pluie de manière rapide et sans fuite, le tout contrôlé avec le second servomoteur. Le plus simple est d’utiliser une électrovanne. Cependant nous voulions un fond plat afin d’avoir une section qui reste constante lors de la montée des eaux de pluies dans le tube. Un robinet en plastique permet de remplir ces conditions. Le servomoteur possède un couple suffisamment important pour arriver à ouvrir et fermer le robinet. Il a fallu réaliser un filetage sur notre tube de PVC récupérateur d’eau de pluie afin de pouvoir visser le robinet.

**Le système d’acquisition**

Dans cette partie nous allons décrire les choix effectués pour acquérir les données utiles à notre projet c'est-à-dire lla hauteur d'eai d’une pluie ainsi que son pH.

Pour acquérir les données et contrôler les différents capteurs, nous utilisons une carte Arduino nano. Cette carte est un micro-contrôleur qui permet d’acquérir des données mais aussi de contrôler des composants électroniques. A l’aide de plusieurs programmes elle contrôle la sonde pH, l’émetteur radio, le module RTC, le capteur de niveau d’eau, le capteur de pluie ainsi que deux servomoteurs.

Le pluviomètre est programmé pour s’ouvrir quand il commence à pleuvoir et se fermer lorsqu’il ne pleut plus où quand la hauteur d’eau maximale est atteinte. Le capteur de pluie commande l’ouverture du capot. Un filtre en software a été réalisé afin de ne pas détecter la rosée.  Lorsque la pluie tombe sur ce capteur, elle fait contact entre des pistes et renvoie une donnée analogique.

Le capteur de niveau d'eau ST045 détermine la hauteur des précipitations. Il renvoie une tension pour des plages de 5mm, de 0 à 40mm.

A l’aide de l’horloge RTC, le pluviomètre connaît la date, l’heure et combien de temps le capot est resté ouvert durant une précipitation. Il en déduit donc l’intensité de la pluie ainsi que l’intervalle de temps entre deux pluies.

Le pHmètre est constitué de deux électrodes. Une première dont le potentiel est constant et connu et une autre dont le potentiel varie en fonction de la concentration des ions H30+. Au préalable il a été étalonné avec des solutions tampons.

**Stockage et transmission des données sur un serveur web**

La transmission des données vers un serveur web se déroule en 2 étapes :

-   L’envoi des données du micro-contrôleur Arduino par antenne radio :

L’envoi des données se fait grâce à un module RF 433 MHz (onde radio)  connecté à la carte Arduino. Le choix d’une transmission radio s’explique par le fait que les onde radio sont peu sujettes aux perturbations extérieures : Une seule voie de transmission (433 MHz) à l’instar des ondes wifi. Elles ont également une bonne portée : environ 40 m, contre 10 m pour le wifi et quelques mètres pour le Bluetooth. C’est aussi un système assez simple à mettre en place et bas prix en comparaison à des méthodes utilisant un GSM. Le code Arduino, [https://github.com/particitae/pluviometre/tree/master/Arduino](https://github.com/particitae/pluviometre/tree/master/Arduino) , permet d’envoyer une chaine de caractère composé du numéro d’identification du pluviomètre, du pH et de la hauteur d’eau (mm) de façons répétées toute les minutes.

-   La réception des données par un micro-processeur Raspberry par récepteur radio et envoie sur un serveur-web distant :

Cette envoi est réceptionné par un module récepteur connecté sur une carte Raspberry situé dans la maison du participant. Un programme python [https://github.com/particitae/pluviometre/tree/master/Raspberry-Python](https://github.com/particitae/pluviometre/tree/master/Raspberry-Python) surveille la réception des données et envoie au serveur distant dès qu’une donnée est reçue. 

Une fois le programme démarré il tourne en permanence et est en attende d’une communication via le buffer radio (ligne 25). Quand une transmission est reçue, les données sont récupérées sous forme d’une chaîne de caractères formée de l’identifiant du pluviomètre, du pH et de la hauteur d’eau (ligne 28). Par exemple : données = 0107.1512.50, avec l’identifiant sur les 2 premier caractère, le pH sur les 5 suivant et la hauteur sur les 5 dernier.


**Identification : Données\[0,2\] = 01**

**pH : Données\[2,7\] = 07.15**

**Hauteur : Données\[7,12\] = 12.50**

On réattribue ces données à leurs grandeur (ligne 29-31). La date du système est transformé en chaîne de caractère (heure, minute, ect..) (ligne 34) et est ajoutée aux autre données dans une chaîne de caractère (ligne 35). Cette chaîne est envoyée sur le serveur web distant par une requête web qui utilise l’adresse URL du serveur (ligne 38). Enfin une fois l’envoi effectué on retourne à l’étape d’attente d’une nouvelle transmission (ligne 40 ligne 23).

L’objectif à terme étant de pouvoir stocker et visualiser graphiquement les données sur plusieurs plages de temps pour plusieurs pluviomètre situé en différents endroits.

Stockage SD :

Les données sont stockées sur une carte SD relié à la carte Arduino. Ce système est mis en place par sécurité au cas où la transmission radio ne fonctionne pas ou dans la cas où le pluviomètre est situé à plus de 40m
de la maison du participant. 

** - Le système d’alimentation**

Un panneau solaire est utilisé pour recharger la batterie qui alimente le système.

Le panneau solaire délivre en moyenne une tension de 5,5V sous 540 mA mais la tension peut monter jusqu’à 10V dans le cas d’un fort ensoleillement. La batterie lithium-ion de 3,7V et 1050mAh doit être rechargée par une tension maximale de 4,2V. Le régulateur de charge TP4056 est placé entre le panneau solaire et la batterie. Il permet de recharger la batterie lithium-ion en limitant la tension à 4,2V.

La carte Arduino doit être alimentée par une tension de 5V or la batterie délivre une tension de 3,7V. Un booster de tension est donc intégré entre la batterie et la carte Arduino. Il permet de délivrer une tension de 5V à partir d’une tension comprise entre 0,5 et 5V.

Le montage électronique a été schématisé sur le logiciel Fritzing afin qu’il soit reproductible ( [lien](https://upload.wikimedia.org/wikipedia/commons/6/6c/C%C3%A2blage_%C3%A9lectronique_du_pluviom%C3%A8tre_%28Fritzing%29_-_Particitae.jpg) )

## 3) Modélisation

![Modélisation du Pluviomètre](https://upload.wikimedia.org/wikipedia/commons/8/83/Mod%C3%A9lisation-du-pluviometre.jpg  "Modélisation du Pluviomètre")

Le pluviomètre est modélisé à l’aide d’un logiciel de conception assisté par ordinateur (CAO). Chaque composant du pluviomètre a été dessiné à échelle réelle afin de permettre à un fabricant de pouvoir réaliser certaines pièces complexes avec une imprimante 3D. Lors de l’impression, il est important d’utiliser une bobine de fil ABS car le PLA est moins résistant et se dégrade au contact de l’eau.
Les fichiers de modélisation sont disponibles à cette [adresse](https://github.com/particitae/pluviometre/tree/master/Fichiers_Solidworks)

## 4)  Résultats

-   Capteur de niveau d’eau:

Contrairement aux indications de la datasheet du fournisseur, l’évolution de la réponse du capteur avec la hauteur d’eau n’est pas linéaire. Nous avons donc effectué un étalonnage du capteur à l’aide d’un bécher dans lequel nous faisions varier la hauteur d’eau pas à pas pour obtenir la courbe suivante :

Évolution de la réponse du capteur en fonction de la hauteur d’eau


![Evolution de la réponse du capteur en fonction de la hauteur d’eau](https://upload.wikimedia.org/wikipedia/commons/e/e2/Evolution_de_la_r%C3%A9ponse_du_capteur_en_fonction_de_la_hauteur_d%E2%80%99eau_-_R%C3%A9sultats_scientifiques.jpg) 


Puisque les valeurs ne varient plus au-dessus de 1.6 centimètres d’eau, nous avons conclu que notre capteur ST045 ne pouvait être utilisé que dans la gamme de hauteur de 0 à 1,6 centimètres.

Dans cette gamme de hauteur d’eau, nous avons pu mettre en évidence deux évolutions distinctes selon la hauteur d’eau :

-   De 0 à 0,9 centimètres, évolution exponentielle :


![Valeur en sortie du capteur](https://upload.wikimedia.org/wikipedia/commons/b/b4/Valeur_en_sortie_du_capteur_-_R%C3%A9sultats_scientifiques.jpg  "Valeur en sortie du capteur")


-   De 0,9 à 1,6 centimètres, évolution linéaire :


![Valeur en sortie du capteur](https://upload.wikimedia.org/wikipedia/commons/f/f5/Valeur_en_sortie_du_capteur2_-_R%C3%A9sultats_scientifiques.jpg  "Valeur en sortie du capteur")


𝐻(𝑉) =𝑎. 𝑥+𝑏

Avec 𝑎=0,00589 𝑐𝑚. 𝑉^−1^

𝑏= −2,50165𝑐𝑚

𝑅² = 0,997

Par ailleurs, nous avons pu observer une dérive non négligeable du capteur lors de nos mesures : les valeurs varient jusqu’à 50 mV pour une hauteur d’eau donnée. Via un test avec de l’eau distillée, nous avons constaté que la réponse du capteur dépend aussi de la composition en minéraux de l’eau. En effet, le capteur étant un capteur basé sur la conductivité, la composition minérale de l’eau influencera la conductivité de celle-ci et donc la tension renvoyée par le capteur. Cela pose problème pour la mesure de hauteur d’eau puisque la composition de l’eau de pluie varie dans le temps et que nous ne pouvons à priori pas la prévoir et corriger les mesures. Cela rajoute une forte incertitude à la mesure de hauteur d’eau.

**La sonde pH:**

Dans l’optique de tester la dérive de notre Sonde PH et son interface SEN0161, nous avons testé le capteur dans deux conditions :

-   Dérive à température constante :


![Dérive à température constante](https://upload.wikimedia.org/wikipedia/commons/4/44/D%C3%A9rive_du_pHmetre_temperature_constante_-_R%C3%A9sultats_scientifiques.jpg  "Dérive à température constante")

Nous constatons une dérive maximale de 0,04 pour la mesure du pH. Il semble donc que les mesures du pH-mètre soient fiables quel que soit le moment où l’on les prendra.

-   Dérive en condition normal d'utilisation du pluviomètre, à l'extérieur toute la journée :


![Dérive du pH-mètre durant une journée](https://upload.wikimedia.org/wikipedia/commons/1/13/D%C3%A9rive_du_pHmetre_durant_journee_-_R%C3%A9sultats_scientifiques.jpg  "Dérive du pH-mètre durant une journée")

Au cours de mesure effectuées sur une journée complète en extérieurs, la dérive observée n'a été que de quelques centièmes d'unité pH. 
On peut donc conclure que le pH-mètre restera fiable quelle que soit la gamme de température à laquelle il effectue la mesure. Nous avons donc validé la sonde pH choisie pour les conditions d’utilisation du pluviomètre avec une incertitude sur les mesures de plus ou moins 0,05.

-------------------------------------
Conclusion
-------------------------------------

En l'état actuel, le prototype s'ouvre aux premières gouttes de pluies, réalise et transmet les mesures de hauteurs d'eau et de pH à la fin de la pluie puis vidange et referme le capot. Il reste néanmoins des améliorations à lui apporter : changer le capteur de niveau d'eau pour un capteur à ultrasons HC-SR04, envisager une transmission des données avec un moduel GSM envoyant des sms pour les cas où le pluviomètre serait situé loin de toutes habitations ou réseau internet. Il est nécéssaire d'effectuer d'autres tests en conditions réelles afin de mieux appréhender la dérive des capteurs utilisés. Enfin, des recherches sur la possibilité de mesure de taux de polluants chimiques dans l'eau de pluie à l'aide de ce pluviomètre et en contexte participatif doivent être poursuivies.

-------------------------------------
Liste du matériel utilisé
-------------------------------------

Booster 5 V U1V11F5	1

Accu Li-Ion 3,7 V 1050 mAh	1

Cellule solaire SOL3W	1

Sonde PH + interface SEN0161 	1

Module capteur de niveau d'eau ST045 	1

Module horloge temps réel ADA3295 	1

Kit émetteur-récepteur 433MHZ 	1

Servomoteur FS5103B	2

Carte Nano 3.0	1

Raspberry Pi 2 Modèle B	1

Module chargeur accus Li-Ion et LiPo MM104 	1

Détecteur de vapeur et de pluie EF04024 	1

Boîtier de dérivation 	1

Tube PVC : 160mm, longueur 30cm 	1

Tube PVC : 160mm, longueur 15cm 	1

Tube PVC : 30mm, longueur 17cm 	1

Manchon 30/30mm 	1

Entonnoir vidange d’huile  	1

Plaque PVC à découper 	1

Robinet plastique de cuve d’eau 	1

Glissières (pour les pieds du pluviomètre)	4

Interrupteurs	2

Plaque de prototypage	1

Fils électriques	

Gaine thermo rétractable	

-------------------------------------
Quelques liens
-----------------------------------------

-   https://www.mouser.com/ds/2/321/gp2y0a21yk\_e-3493.pdf

    Capteur de mesure par ultrason :

-   http://www.robot-maker.com/shop/img/cms/datasheet-capteur-ultrasons-hc-sr04.pdf

    Capteur de mesure par résistances :

-   http://pompie-arduino.blogspot.fr/search/label/water%20level

    Principe de la spectroscopie :

-   https://fr.wikipedia.org/wiki/Spectroscopie

    Principe de la spectrométrie de masse :

-   [h](http://masse-spec.fr/)[ttp://masse-spec.fr](http://masse-spec.fr/)
-   <http://www.snv.jussieu.fr/bmedia/lafont/spectro/C4.html>
-   <http://www.cnrs.fr/insis/recherche/actualites/2016/06/spectrometre-analyse-polluants.htm>
-   http://masse-spec.fr/analyse-environnement

    Principe de voltammétrie :

-   https://fr.wikipedia.org/wiki/Voltammétrie

Polluants dans l’eau de pluie :

Métaux lourds :

-   http://www.institut-numerique.org/chapitre-i-la-pollution-des-eaux-par-les-metaux-lourds-5306014f1e9bf
-   http://www.cnrs.fr/cw/dossiers/doseau/decouv/degradation/10\_pollution.htm
-   http://www.jeanlouisetienne.com/clipperton/encyclo3.cfm?fiche=44
-   http://www.dictionnaire-environnement.com/pollution\_de\_eau\_aux\_metaux\_lourds\_ID4363.html
-   http://www.fao.org/docrep/005/V3640F/V3640F04.htm

Composition de l’eau de pluie :

-   https://recuperation-eau-pluie.ooreka.fr/astuce/voir/268918/composition-de-l-eau-de-pluie-a-quelle-qualite-s-attendre

Programmation :

Timers :

-   http://www.locoduino.org/spip.php?article84

Communication Arduino - Raspberry :

-   https://www.carnetdumaker.net/articles/communiquer-sans-fil-en-433mhz-avec-la-bibliotheque-virtualwire-et-une-carte-arduino-genuino/

Alimentation par panneau solaire :

-   https://hackaday.io/project/6477-solar-powered-arduino-weather-station\#j-discussions-title
-   http://www.instructables.com/id/ARDUINO-WIRELESS-HOME-SECURITY-SYSTEM/

Transmission des données Arduino/Raspberry :

-   http://electroniqueamateur.blogspot.fr/2014/10/communication-rf-433-mhz-entre.html

Données de précipitations :

-   [Meteociel](https://www.meteociel.fr/observations-meteo/rr.php?mode=1h)

Laboratoires :

-   [LDAR](https://www.gironde.fr/environnement/laboratoire-departemental-danalyse-lda33)

labo.dordogne.fr/

-   [LYRE](http://www.le-lyre.fr/)
-   [EPOC](http://www.epoc.u-bordeaux.fr)


