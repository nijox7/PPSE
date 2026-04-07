#Compte-rendu TP - Integer and Floating-point Coding

## Exercise 1

### 4
On obtient les valeurs intermédiaires suivantes:
x=120
x=-16
x=-91
x=78

Le type char est compris entre -128 et 127, on a donc un overflow lors de l'opération "x + 120", la valeur devient négative et à une distance de 240 - 128 = 112 de -128 donc -16. Puis on soustrait 75 donc on passe à -91.
Enfin lorsque l'on soustrait 87, on repasse dans les valeurs positives et on obtient le bon résultat puisque l'overflow est cyclique.


## Exercice 2

### 1
Il y a un overflow pour les termes 127 et 128.
Leur valeurs inf signifient qu'on ne peut plus les représenter car ils sont trop grands.

### 3
À partir du terme 24, les termes sont tous pairs. Celà est du au pas qui devient plus grand pour les grand nombres, il y a donc un approximation et les termes ne valent pas la vraie valeur de la suite.

## Exercice 3

### 3
sum(2^i)*sum(3^i)
Première méthode: 181308931.000000
Deuxième méthode: 16064396614146.000000

sum(2.13^i)*sum(3.21^i)
Première méthode: 611157330.817420
Deuxième méthode: 103133245822775.937500

Ces différences sont dues aux approximations.
Dans la première méthode on effectue 1 seule opération de multiplication, il y a donc 1 approximation.
Dans la deuxième méthode on fait la somme des multiplications des termes des deux suites. On effectue alors N*N multiplications, donc N*N approximations.
Ceci change donc le résultat.

### 4
Pour le type doubles, on a pas d'overflow et les nombres sont approximés à partir du terme 31.
On a donc bel et bien une meilleure précision.