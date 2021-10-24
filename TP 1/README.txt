Nous avons décidé de faire passer la config en paramètre des méthodes plutôt que d'en faire une variable globale pour plus de flexibilité.
Dans le Makefile, vérifier que la lib openssl est bien linké avec le bon chemin (travaillant sur mac, ce n'est pas forcement le même pour un linux).

Question 4
-----------
La complexité en temps de recherche dichotomique est de O(log N). Et en espace, c'est O(1).

Question 7
-----------
L'ajout du paramètre t dans la fonction h2i permet d'augmenter la couverture de la table car il y a moins de chance de "collision" avec 2 entrées différentes.

Question 10
-----------
Pas besoin de height et width dans les paramètres de méthode car ils sont dans la structure "table"

Question 11
-----------
La complexité de la recherche dans la table arc-en-ciel est O(log(n)), la complexité d'une recherche dichotomique.

Question 13
-----------
Coverage pour une table de hauteur 1000 et largeur 10000 :
size of table in bytes : 160000
coverage of table : 99.32%
Cela semble être satisfaisant pour pouvoir trouver rapidement le résultat pour du md5.
Pour SHA1, j'ai utilisé une hauteur de 50000 avec une largeur de 10000.
malgré une taille conséquente et un temps de traitement assez long, cela permet quand même d'obtenir un converage de plus de 90%,
ce qui permet d'obtenir dans la plupart des cas un résultat avec la table.

Question 14
-----------
config : algo : md5
config : alphabet size : 26
config : alphabet : ABCDEFGHIJKLMNOPQRSTUVWXYZ
config : min : 4
config : max : 4
table : width : 1000
table : height : 10000
size of table in bytes : 160000
coverage of table : 99.32%
took 1.973441 seconds to generate table

Question 15
-----------
config : algo : md5
config : alphabet size : 26
config : alphabet : ABCDEFGHIJKLMNOPQRSTUVWXYZ
config : min : 4
config : max : 4
table : width : 1000
table : height : 10000
size of table in bytes : 160000
coverage of table : 99.32%

ABCD -> CB08CA4A7BB5F9683C19133A84872CA7
found inverse : ABCD
took 0.617662 seconds to find result

DCBA -> DB57F605B4C439BE767A77C9BB157FD8
found inverse : DCBA
took 0.061418 seconds to find result

AZER -> 807963849EB452C987D8E657BECB4677
found inverse : AZER
took 0.082749 seconds to find result

08054846bbc9933fd0395f8be516a9f9 -> CODE
took 0.508148 seconds to find result
