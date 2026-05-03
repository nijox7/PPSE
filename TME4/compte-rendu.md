# PPSE: Compte-rendu TME4

## Notes
faire fonctions:
- void source_generate_all_zeros(uint8_t *U_K, size_t K); // write only zeros in U_K
- void modem_BPSK_modulate_all_ones(const uint8_t *C_N, int32_t *X_N, size_t N); // write only ones in X_N

### Écrire dans fichier csv
On utiliser *fprintf*.

### Définition d'une macro préprocesseur en ligne de commande
Pour définir une macro préprocesseur il faut compiler avec -D...\n
Dans notre cas: gcc optlong.c -o optlong -DENABLE_STATS

L'activation des stats se fait en compilant "make stats", cela rajoute -DENABLE_STATS à la compilation.\n
Ceci va définir une macro préprocesseur ENABLE_STATS qui va donc permettre à certains bouts du programme de main.c de s'exécuter.

### Calcul du temps en millisecondes
Pour calculer le temps en millisecondes il faut utiliser la fonction *times()*.

### Options longues à double tiret
Utiliser *getopt_long()*.

### Task 2 et 3
<!-- TODO  A RETESTER JE PENSE QUE C'EST PAS BON!!!
Pour la simulation 1:
>
    ./simulator -m 0 -M 15 -s 1 -e 100 -K 32 -N 128 -D "rep-hard" --mod-all-ones --src-all-zeros

On passe d'un débit de 1.57 mbps à 1.71 mbps avec les options activées.\n
Le temps est également réduit, on passe de 32 secondes à 13 secondes.
-->

## Task 2
>
        $ ./simulator > results/task2_src_normal
        $ ./simulator --src-all-zeros > results/task2_src_all_zeros

L'étape de decode semble être plus rapide pour des SNR grands.\n
Pour SNR = 14, on passe d'un débit de 12 mbps à 45 mbps en décode avec --src-all-zeros\n

Le débit de simulation est légèrement supérieur:\n
On passe de 1.25 mbps à 1.28 mbps avec l'option.

## Task 3
Avec l'option --mod-all-ones, la simulation est presque instantanée, il n'y pas de délai, la durée de simulation est suffisament courte pour la considérer comme instantanée.