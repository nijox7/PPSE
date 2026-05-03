/*
    Generating random number following normal's law
    ---
    Polar method, Box-Muller Transform
*/
float normal_law(float sigma);

/*
    transform numbers from floating-point representation to fixed-point representation
    `s` is the number of bits used in the quantizer block
    `f` is the number of bits of the fractional part (`s` >= `f`)
*/
void quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f);