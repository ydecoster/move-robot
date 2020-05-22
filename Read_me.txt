%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Package made by : BOUDET Julien & DE COSTER Yann
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Projet de recherche : Localiser des QR-codes dans l'espace à l'aide d'un robot muni d'une caméra
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Vous trouverez dans ce package :
- quatre fichiers .cpp : listener2.cpp main_move.cpp qrcodes.cpp qr_movement.cpp
- un fichier launch PDR.launch
- un fichier package.xml
- un CMakeLists.txt
- le fichier Read_me.txt
- un fichier Microsoft Word pdrfinal
- un fichier info_Minoru_3D.txt donnant des informations concernant la caméra
- un dossier contenant nos qr codes qu'on a utilise : site pour en creer : http://goqr.me/
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Ce qu'il faut ?
Il faut au préalable télécharger un environnement Linux sur sa machine. Personellement, nous vous recommandons Ubuntu 16.04.LTS
Une fois cette action effectuée, il faut télécharger Ros, OpenCV et Zbar.

Un turtlebot et une caméra Minoru 3D sont également nécessaires pour lancer ce package.

Open CV : https://sourceforge.net/projects/opencvlibrary/?SetFreedomCookie
ZBAR (lecture de QRcode sous openCV): http://zbar.sourceforge.net/download.html
ROS :http://www.ros.org/
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Comment m'utiliser ?
Créer un espace de travail et utiliser les tutoriels ros : http://wiki.ros.org/fr/ROS/Tutorials
Me mettre au même endroit que les autres packages.
Ouvrez deux terminals (Ctrl + Alt + t).
Dans le premier, tapez : $ roscore
Dans le deuxième : $ cd "votre espace de travail ros aka catkin"
$ catkin_make
$ roslaunch move_robot PDR.launch
La dernière commande se fait juste après avoir branché, allumé le robot, et branché la caméra Minoru 3D.

Et utiliser les qr codes : droite, gauche et fin pour controler le robot.
Si vous perdez le controle du robot alors il faudra cliquer sur q et ensuite Ctrl + C. Et le programme ne s'executera plus.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Me developper ?
On vous propose différents points de développement. Changer les bonnes valeurs pour faire en sorte que la distance du qr code indiqué par le programme soit la bonne, et que le robot fasse un angle précis de pi/2.
Creer une nouvelle commande demi-tour, avec son qr code associé
Faire en sorte que le robot se remette droit s'il n'est pas strictement en face du qr code.
Des commentaires sont présents dans le code pour vous aider.
Vous modifierez essentiellement :
- qrcodes.cpp 
- qr_movement.cpp
- PDR.launch
- CMakeLists.txt
- Read_me.txt (une fois le pdr effectué)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Bonus :
- Un rapport technique, qui peut vous aider...
- listener2.cpp & main_move.cpp qui nous a bien aidé à comprendre les noeuds et le fonctionnement du robot.
