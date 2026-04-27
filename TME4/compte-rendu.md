# PPSE: Compte-rendu TME4

## Notes

TODO: rajouter --help ou -h (pareil)\
TODO: voir getop, rajouter options longues (plusieurs caractère: "-ENABLE") et doubles tiret (--src-all-zeros).

faire fonctions:
- void source_generate_all_zeros(uint8_t *U_K, size_t K); // write only zeros in U_K
- void modem_BPSK_modulate_all_ones(const uint8_t *C_N, int32_t *X_N, size_t N); // write only ones in X_N