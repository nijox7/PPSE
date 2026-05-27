#include <stdio.h>
#include <arm_neon.h>

int main() {
    float32x4_t a = vdupq_n_f32(1.0f);
    float32x4_t b = vdupq_n_f32(2.0f);
    float32x4_t c = vaddq_f32(a, b);

    float res[4];
    vst1q_f32(res, c);

    for(int i=0;i<4;i++)
        printf("%f\n", res[i]);

    return 0;
}
