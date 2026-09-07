#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float metropolis(float df, float k, float temp) {
    return expf(-df / (k * temp));
}

float urand01(void) {
    return (float)rand() / ((float)RAND_MAX + 1.0f);
}

float calculate_df(float next, float cur) {
    return next - cur;
}

float fitness(float x1, float x2) {
    return 500 - 20*x1 - 26*x2 - 4*x1*x2 + 4*x1*x1 + 3*x2*x2;
}

float calculate_range(float x, float interval) {
    float lower= x - interval;
    float upper= x + interval;
    float rand = urand01();
    return lower + rand * (upper - lower);
}

void setup(float temp, float c, float k, float x1, float x2, float interval) {
    printf("Simulation Parameters:\n");
    printf("Temperature:\t%f\n", temp);
    printf("Reductor (c):\t%f\n", c);
    printf("Boltzmann (k):\t%f\n", k);
    printf("Initial x1:\t%f\n", x1);
    printf("Initial x2:\t%f\n", x2);
    printf("Interval:\t%f\n\n", interval);
}

void display_worse(unsigned idx, float r1, float r2, float f, float f_new, float f_best, float df, float rand, float prob) {
    printf("idx:\t%u\nr1:\t%f\nr2:\t%f\nf:\t%f\nf_new:\t%f\nf_best:\t%f\ndf:\t%f\nrand:\t%f\nprob:\t%f\n\n", idx, r1, r2, f, f_new, f_best, df, rand, prob);
}

void display_better(unsigned idx, float r1, float r2, float f, float f_new, float f_best, float df) {
    printf("idx:\t%u\nr1:\t%f\nr2:\t%f\nf:\t%f\nf_new:\t%f\nf_best:\t%f\ndf:\t%f\n\n", idx, r1, r2, f, f_new, f_best, df);
}

void display_intermediate(unsigned iter, float temp) {
    printf("Iter\t%d\nTemp\t%f\n\n", iter+1, temp);
}

// TODO:
// Add different objective functions

int main() {
    srand(34);

    float temp = 384.25, c = 0.9, k_boltzmann = 1, x1 = 4.0f, x2 = 5.0f, interval = 6.0f; 
    float r1 = 0, r2 = 0, f = 0, f_new = 0, df = 0, rand = 0, prob = 0;
    float f_best = fitness(x1, x2);
    unsigned n = 3, iter = 5;
    

    setup(temp, c, k_boltzmann, x1, x2, interval);

    // Minimization
    for(unsigned i=0; i<iter; i++) {
        display_intermediate(i, temp);
        for(unsigned j=0; j<n; j++) {
            r1 = calculate_range(x1, interval);
            r2 = calculate_range(x2, interval);
            f = fitness(x1, x2);
            f_new = fitness(r1, r2);
            df = calculate_df(f_new, f);

            char accepted = 0;
            if(df <= 0.0f) {
                // better solution -> unconditional acceptance
                accepted = 1;
                x1 = r1;
                x2 = r2;
                display_better(j+1, r1, r2, f, f_new, f_best, df);
            }
            else {
                // worse solution -> conditional acceptance
                rand = urand01();
                prob = metropolis(df, k_boltzmann, temp);
                display_worse(j+1, r1, r2, f, f_new, f_best, df, rand, prob);

                if(prob > rand) {
                    // accept worse solution
                    x1 = r1;
                    x2 = r2;
                    accepted = 1;
                }
                    // reject worse solution
            }
            // update best solution found so far
            if(accepted && (f_new < f_best))
                f_best = f_new;
        }
        temp *= c;
    }
    
    
    return 0;
}

// Simulation Parameters:
// Temperature:    384.250000
// Reductor (c):   0.900000
// Boltzmann (k):  1.000000
// Initial x1:     4.000000
// Initial x2:     5.000000
// Interval:       6.000000

// Iter    1
// Temp    384.250000

// idx:    1
// r1:     0.027640
// r2:     -0.656949
// f:      349.000000
// f_new:  517.898315
// f_best: 349.000000
// df:     168.898315
// rand:   0.772811
// prob:   0.644324

// idx:    2
// r1:     2.375803
// r2:     1.854074
// f:      349.000000
// f_new:  419.548920
// f_best: 349.000000
// df:     70.548920
// rand:   0.487580
// prob:   0.832267

// idx:    3
// r1:     -0.600017
// r2:     1.948952
// f:      419.548920
// f_new:  478.840546
// f_best: 349.000000
// df:     59.291626
// rand:   0.362560
// prob:   0.857011

// Iter    2
// Temp    345.824982

// idx:    1
// r1:     3.622478
// r2:     -0.516971
// f:      478.840546
// f_new:  501.773743
// f_best: 349.000000
// df:     22.933197
// rand:   0.475974
// prob:   0.935837

// idx:    2
// r1:     4.973037
// r2:     -3.784873
// f:      501.773743
// f_new:  716.135315
// f_best: 349.000000
// df:     214.361572
// rand:   0.680793
// prob:   0.538022

// idx:    3
// r1:     -1.300506
// r2:     1.114293
// f:      501.773743
// f_new:  513.325256
// f_best: 349.000000
// df:     11.551514
// rand:   0.945036
// prob:   0.967149

// Iter    3
// Temp    311.242462

// idx:    1
// r1:     -0.988955
// r2:     6.595739
// f:      513.325256
// f_new:  508.804871
// f_best: 349.000000
// df:     -4.520386

// idx:    2
// r1:     3.963205
// r2:     4.114923
// f:      508.804871
// f_new:  362.140503
// f_best: 349.000000
// df:     -146.664368

// idx:    3
// r1:     2.060416
// r2:     4.851743
// f:      362.140503
// f_new:  380.259430
// f_best: 349.000000
// df:     18.118927
// rand:   0.746885
// prob:   0.943447

// Iter    4
// Temp    280.118195

// idx:    1
// r1:     -1.156617
// r2:     -0.375493
// f:      380.259430
// f_new:  536.932007
// f_best: 349.000000
// df:     156.672577
// rand:   0.576686
// prob:   0.571604

// idx:    2
// r1:     2.888020
// r2:     5.927929
// f:      380.259430
// f_new:  358.417206
// f_best: 349.000000
// df:     -21.842224

// idx:    3
// r1:     7.219372
// r2:     8.783175
// f:      358.417206
// f_new:  313.523804
// f_best: 349.000000
// df:     -44.893402

// Iter    5
// Temp    252.106369

// idx:    1
// r1:     8.638610
// r2:     10.388253
// f:      313.523804
// f_new:  320.422699
// f_best: 313.523804
// df:     6.898895
// rand:   0.102587
// prob:   0.973006

// idx:    2
// r1:     12.911922
// r2:     5.844297
// f:      320.422699
// f_new:  557.303711
// f_best: 313.523804
// df:     236.881012
// rand:   0.354602
// prob:   0.390781

// idx:    3
// r1:     11.280111
// r2:     5.651061
// f:      557.303711
// f_new:  477.258911
// f_best: 313.523804
// df:     -80.044800
