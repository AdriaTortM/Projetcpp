# Projetcpp

Le programme consiste en un Jeu de Tetris. 
Pour y jouer (désolé j'ai un clavier espagnol donc QWERTY):
        - "a" pour déplacer la pièce à gauche
        - "d" pour la déplacer à droite
        - "w" pour tourner la pièce dans le sens horaire


Il faut noter que la programme génère des pieces de façon aléatoire, que lorsqu'une ligne est complète,
le programme la supprime pour en laisser descendre celles en haut, et que avant de déplacer ou de tourner 
une pièce le programme vérifie qu'il est en effet possible de faire cette action.

En revanche, j'aurais aimé pouvoir faire en sorte que les pièces soient différentiables selon la couleur, 
mais j'ai pas réussi à comprendre comment fonctionnait ncurses dans ce sens là, ce qui rend l'interface 
peu esthétique