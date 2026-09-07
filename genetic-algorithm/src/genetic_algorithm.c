#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <direct.h>
#define make_dir(path) _mkdir(path)
#else
#include <sys/stat.h>
#define make_dir(path) mkdir(path, 0755)
#endif

#define BINARY 2

// Paths are relative to the project directory (genetic-algorithm/)
#define OUTPUT_DIR "output"
#define OUTPUT_CSV OUTPUT_DIR "/output.csv"

// 0 to n exclusive
unsigned generate_rand(unsigned n) {
    return rand() % n;
}


float generate_rand01(void) {
    return (float)generate_rand(100001) / 100001.0f;
}


// Copy src to dest
void arr_cpy(unsigned size, float* dest, float* src) {
    for(unsigned i=0; i<size; i++) {
        dest[i] = src[i];
    }
}


void arr_fill(unsigned size, float* arr, float value) {
    for(unsigned i=0; i<size; i++) {
        arr[i] = value;
    }
}


void arr_zero(unsigned size, float* arr) {
    for(unsigned i=0; i<size; i++) {
        arr[i] = 0.0f;
    }
}


float arr_sum(unsigned size, float* arr) {
    unsigned sum = 0.0f;
    for(unsigned i=0; i<size; i++) {
        sum += arr[i];
    }
    return sum;
}


float arr_max(unsigned size, float* arr) {
    float max = arr[0];
    for(unsigned i=0; i<size; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}


float arr_min(unsigned size, float* arr) {
    float min = arr[0];
    for(unsigned i=0; i<size; i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

void arr_display_float(unsigned size, float* arr) {
    for(unsigned i=0; i<size; i++) {
        printf("%f ", arr[i]);
    }
}

void arr_display_uint(unsigned size, unsigned* arr) {
    for(unsigned i=0; i<size; i++) {
        printf("%u ", arr[i]);
    }
}


void mat_cpy(unsigned row, unsigned column, unsigned dest[row][column], unsigned src[row][column]) {
    for(unsigned i=0; i<row; i++) {
        for(unsigned j=0; j<column; j++) {
            dest[i][j] = src[i][j];
        }
    }
}


void create_chromosome(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen]) {
    for(unsigned i=0; i<pop; i++) {
        for(unsigned j=0; j<gen; j++) {
            chr_bin[i][j] = generate_rand(BINARY);
        }
    }
}


float max_weight(float fitness_x) {
    return fitness_x;
}


float min_weight(float fitness_x) {
    return 1.0f / (1.0f + fitness_x);
}


void normalize_prob(unsigned pop, float* chr_weight) {
    float sum = 0.0f;
    for(unsigned i=0; i<pop; i++) {
        sum += chr_weight[i];
    }
    for(unsigned i=0; i<pop; i++) {
        chr_weight[i] /= sum;
    }
}


void roulette_wheel_selection(unsigned pop, float* chr_cum, unsigned* parent, float* rand) {
    for(unsigned i=0; i<pop; i++) {
        rand[i] = generate_rand01();
        for(unsigned j=0; j<pop; j++) {
            if(rand[i] <= chr_cum[j]) {
                // Same parent can be selected multiple times (With Replacement)
                // This is simulating the selection pressure towards fitter chromosomes
                // Think about animals fighting for the females, the stronger ones have higher chances of mating
                parent[i] = j;
                break;
            }
        }
    }
}


void cumulative_prob(unsigned pop, float* chr_weight, float* chr_cum) {
    chr_cum[0] = chr_weight[0];
    for(unsigned i=1; i<pop; i++) {
        chr_cum[i] = chr_cum[i-1] + chr_weight[i];
    }
}


void crossover_one_point(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], unsigned* parent, float p_crossover) {
    unsigned chr_temp[pop][gen];
    mat_cpy(pop, gen, chr_temp, chr_bin);
    
    for(unsigned i=0; i<pop; i+=2) {
        float rand = generate_rand01();
        if(rand <= p_crossover) {
            unsigned point = generate_rand(gen-1) + 1;
            // printf("Crossover occurred between Chromosome %u and Chromosome %u at Point %u\n", parent[i]+1, parent[i+1]+1, point);
            for(unsigned j=0; j<gen; j++) {
                if(j < point) {
                    chr_bin[i][j] = chr_temp[parent[i]][j];
                    chr_bin[i+1][j] = chr_temp[parent[i+1]][j];
                } else {
                    chr_bin[i][j] = chr_temp[parent[i+1]][j];
                    chr_bin[i+1][j] = chr_temp[parent[i]][j];
                }
            }
        } else {
            // No Crossover, Direct Copy
            for(unsigned j=0; j<gen; j++) {
                chr_bin[i][j] = chr_temp[parent[i]][j];
                chr_bin[i+1][j] = chr_temp[parent[i+1]][j];
            }
        } 
    }
}                                                               


void mutation(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float p_mutation) {
    for(unsigned i=0; i<pop; i++) {
        for(unsigned j=0; j<gen; j++) {
            float rand = generate_rand01();
            if(rand <= p_mutation) {
                // printf("Mutation occurred at Chromosome %u Gene %u\n", i+1, j+1);
                chr_bin[i][j] ^= 1u;
            }
        }
    }
}

float fitness(float x) {
    return 255*x - x*x;
}


void convert_dec(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float* chr_dec) {
    for(unsigned i=0; i<pop; i++) {
        unsigned dec_val = 0;
        for(unsigned j=0; j<gen; j++) {
            dec_val = (dec_val << 1) | chr_bin[i][j];
        }
        chr_dec[i] = (float)dec_val;
    }
}


void setup(unsigned pop, unsigned gen, unsigned t, float p_crossover, float p_mutation) {
    printf("Simulation Parameters\n");
    printf("Population: %u\n", pop);
    printf("Genome Count: %u\n", gen);
    printf("Iteration Count: %u\n", t);
    printf("Crossover Probability: %f\n", p_crossover);
    printf("Mutation Probability: %f\n", p_mutation);
}


void info(unsigned pop, unsigned* parent, float* rand) {
    printf("\nGenerated Random Values: ");
    arr_display_float(pop, rand);
    printf("\nSelected Parents: ");
    arr_display_uint(pop, parent);
    printf("\n\n");
}


void display(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float* chr_dec, float* chr_fit,
     float* chr_weight, float* chr_cum, float* avg, float* best, unsigned idx) {

    unsigned col_width = (gen > 20) ? gen : 20;

    printf("\nGeneration %u\n", idx);
    printf("\n%-20s %-*s %-20s %-20s %-20s %-20s\n", "Chromosome", col_width, "Binary", "Decimal", "Fitness", "Probability", "Cumulative");

    for(unsigned i=0; i<pop; i++) {
        printf("%-20u ", i+1);
        for(unsigned j=0; j<gen; j++) {
            printf("%u", chr_bin[i][j]);
        }
        int padding = col_width - gen;
        if(padding < 0) padding = 0;
        printf("%-*s", padding, "");
        printf(" ");
        printf("%-20f %-20f %-20f %-20f\n", chr_dec[i], chr_fit[i], chr_weight[i], chr_cum[i]);
    }
        printf("\nAverage: %f\nBest: %f\n\n", avg[idx], best[idx]);
}


void compute_avg_best(unsigned pop, float* chr_fit, float* avg, float* best, float* temp, unsigned idx, char obj_func) {

    if(!obj_func) { // Max
        float max = arr_max(pop, chr_fit);
        if(max > *temp) *temp = max;
    }
    else {          // Min
        float min = arr_min(pop, chr_fit);
        if(min < *temp) *temp = min;
    }
    
    best[idx] = *temp;

    avg[idx] = arr_sum(pop, chr_fit);
    avg[idx] /= (float)pop;
}


void evaluate(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float* chr_dec, float* chr_fit,
     float* chr_weight, float* chr_cum, float* avg, float* best, unsigned idx, float* temp, char obj_func) {

    // Compute everything for the current pop

    convert_dec(pop, gen, chr_bin, chr_dec);
    arr_cpy(pop, chr_fit, chr_dec);

    for(unsigned i=0; i<pop; i++)
        chr_fit[i] = fitness(chr_fit[i]);

    compute_avg_best(pop, chr_fit, avg, best, temp, idx, obj_func);

    for(unsigned i=0; i<pop; i++)
        chr_weight[i] = (!obj_func) ? max_weight(chr_fit[i]) : min_weight(chr_fit[i]);

    normalize_prob(pop, chr_weight);
    cumulative_prob(pop, chr_weight, chr_cum);

}


void create_csv(unsigned size, float* avg, float* best) {
    make_dir(OUTPUT_DIR); // no-op if the directory already exists

    FILE* file = fopen(OUTPUT_CSV, "w");
    if(file == NULL) {
        printf("Error opening %s for writing. Run this binary from the genetic-algorithm/ directory.\n", OUTPUT_CSV);
        return;
    }

    fprintf(file, "Generation,Average,Best\n");
    for(unsigned i=0; i<size; i++) {
        fprintf(file, "%u,%f,%f\n", i+1, avg[i], best[i]);
    }

    fclose(file);
}


int main() {

    srand(113);
    
    char obj_func = 0; // 0: Max, 1: Min
    unsigned pop = 20;
    unsigned gen = 8;
    unsigned iter = 100;
    float temp = (!obj_func) ? 0.0F : 100000000.0F;
    float p_crossover = 0.70F;
    float p_mutation = 0.001F;

    unsigned chr_bin[pop][gen];
    unsigned parent[pop];
    float chr_dec[pop];
    float chr_fit[pop];
    float chr_weight[pop];
    float chr_cum[pop];
    float rand[pop];
    float best[iter];
    float avg[iter];

    // setup(pop, gen, iter, p_crossover, p_mutation);
    create_chromosome(pop, gen, chr_bin);
    evaluate(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, 0, &temp, obj_func);
    // display(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, 0);

    for(unsigned i=1; i<iter; i++) {
        roulette_wheel_selection(pop, chr_cum, parent, rand);
        // info(pop, parent, rand);
        crossover_one_point(pop, gen, chr_bin, parent, p_crossover);
        mutation(pop, gen, chr_bin, p_mutation);
        evaluate(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, i, &temp, obj_func);
        // display(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, i);
    }

    create_csv(iter, avg, best);

    return 0;
}

// Simulation Parameters
// Population: 20
// Genome Count: 8
// Iteration Count: 7
// Crossover Probability: 0.700000
// Mutation Probability: 0.001000

// Generation 0

// Chromosome           Binary               Decimal              Fitness              Probability          Cumulative          
// 1                    11001101             205.000000           10250.000000         0.046572             0.046572            
// 2                    01100000             96.000000            15264.000000         0.069353             0.115925            
// 3                    01010001             81.000000            14094.000000         0.064037             0.179963            
// 4                    01001110             78.000000            13806.000000         0.062729             0.242692            
// 5                    00001010             10.000000            2450.000000          0.011132             0.253823            
// 6                    10000100             132.000000           16236.000000         0.073770             0.327593            
// 7                    01111101             125.000000           16250.000000         0.073833             0.401427            
// 8                    10101010             170.000000           14450.000000         0.065655             0.467082            
// 9                    01000101             69.000000            12834.000000         0.058313             0.525394            
// 10                   10101000             168.000000           14616.000000         0.066409             0.591803            
// 11                   10101110             174.000000           14094.000000         0.064037             0.655841            
// 12                   00110011             51.000000            10404.000000         0.047272             0.703112            
// 13                   11110110             246.000000           2214.000000          0.010060             0.713172            
// 14                   10010011             147.000000           15876.000000         0.072134             0.785306            
// 15                   00101001             41.000000            8774.000000          0.039866             0.825172            
// 16                   01111001             121.000000           16214.000000         0.073670             0.898841            
// 17                   01000101             69.000000            12834.000000         0.058313             0.957154            
// 18                   00001101             13.000000            3146.000000          0.014294             0.971448            
// 19                   11110000             240.000000           3600.000000          0.016357             0.987805            
// 20                   11110100             244.000000           2684.000000          0.012195             1.000000            

// Average: 11004.500000
// Best: 16250.000000


// Generated Random Values: 0.669073 0.670753 0.149898 0.819832 0.249628 0.135709 0.689113 0.699983 0.363806 0.179988 0.715993 0.500925 0.899421 0.934071 0.643704 0.675283 0.009970 0.845072 0.822022 0.625404 
// Selected Parents: 11 11 2 14 4 2 11 11 6 3 13 8 16 16 10 11 0 15 14 10 

// Crossover occurred between Chromosome 12 and Chromosome 12 at Point 1
// Crossover occurred between Chromosome 5 and Chromosome 3 at Point 5
// Crossover occurred between Chromosome 12 and Chromosome 12 at Point 3
// Crossover occurred between Chromosome 7 and Chromosome 4 at Point 6
// Crossover occurred between Chromosome 14 and Chromosome 9 at Point 2
// Crossover occurred between Chromosome 17 and Chromosome 17 at Point 5
// Crossover occurred between Chromosome 11 and Chromosome 12 at Point 6
// Crossover occurred between Chromosome 1 and Chromosome 16 at Point 7
// Crossover occurred between Chromosome 15 and Chromosome 11 at Point 5

// Generation 1

// Chromosome           Binary               Decimal              Fitness              Probability          Cumulative          
// 1                    00110011             51.000000            10404.000000         0.043014             0.043014            
// 2                    00110011             51.000000            10404.000000         0.043014             0.086028            
// 3                    01010001             81.000000            14094.000000         0.058270             0.144297            
// 4                    00101001             41.000000            8774.000000          0.036275             0.180572            
// 5                    00001001             9.000000             2214.000000          0.009153             0.189725            
// 6                    01010010             82.000000            14186.000000         0.058650             0.248375            
// 7                    00110011             51.000000            10404.000000         0.043014             0.291389            
// 8                    00110011             51.000000            10404.000000         0.043014             0.334403            
// 9                    01111110             126.000000           16254.000000         0.067200             0.401603            
// 10                   01001101             77.000000            13706.000000         0.056665             0.458268            
// 11                   10000101             133.000000           16226.000000         0.067084             0.525352            
// 12                   01010011             83.000000            14276.000000         0.059022             0.584374            
// 13                   01000101             69.000000            12834.000000         0.053060             0.637434            
// 14                   01000101             69.000000            12834.000000         0.053060             0.690494            
// 15                   10101111             175.000000           14000.000000         0.057881             0.748375            
// 16                   00110010             50.000000            10250.000000         0.042377             0.790752            
// 17                   11001101             205.000000           10250.000000         0.042377             0.833129            
// 18                   01111001             121.000000           16214.000000         0.067034             0.900164            
// 19                   00101110             46.000000            9614.000000          0.039748             0.939911            
// 20                   10101001             169.000000           14534.000000         0.060089             1.000000            

// Average: 12093.799805
// Best: 16254.000000


// Generated Random Values: 0.740863 0.860261 0.428046 0.488325 0.675873 0.199048 0.446476 0.455675 0.962000 0.585564 0.922491 0.630864 0.111789 0.944341 0.396736 0.063519 0.557554 0.308607 0.116899 0.646904 
// Selected Parents: 14 17 9 10 13 5 9 9 19 12 18 12 2 19 8 1 11 7 2 13 

// Crossover occurred between Chromosome 10 and Chromosome 11 at Point 3
// Crossover occurred between Chromosome 14 and Chromosome 6 at Point 4
// Crossover occurred between Chromosome 10 and Chromosome 10 at Point 7
// Crossover occurred between Chromosome 20 and Chromosome 13 at Point 1
// Crossover occurred between Chromosome 12 and Chromosome 8 at Point 1

// Generation 2

// Chromosome           Binary               Decimal              Fitness              Probability          Cumulative          
// 1                    10101111             175.000000           14000.000000         0.053232             0.053232            
// 2                    01111001             121.000000           16214.000000         0.061650             0.114882            
// 3                    01000101             69.000000            12834.000000         0.048798             0.163681            
// 4                    10001101             141.000000           16074.000000         0.061118             0.224798            
// 5                    01000010             66.000000            12474.000000         0.047430             0.272228            
// 6                    01010101             85.000000            14450.000000         0.054943             0.327171            
// 7                    01001101             77.000000            13706.000000         0.052114             0.379285            
// 8                    01001101             77.000000            13706.000000         0.052114             0.431399            
// 9                    11000101             197.000000           11426.000000         0.043445             0.474844            
// 10                   00101001             41.000000            8774.000000          0.033361             0.508205            
// 11                   00101110             46.000000            9614.000000          0.036555             0.544760            
// 12                   01000101             69.000000            12834.000000         0.048798             0.593559            
// 13                   01010001             81.000000            14094.000000         0.053589             0.647148            
// 14                   10101001             169.000000           14534.000000         0.055262             0.702411            
// 15                   01111110             126.000000           16254.000000         0.061802             0.764213            
// 16                   00110011             51.000000            10404.000000         0.039559             0.803772            
// 17                   00110011             51.000000            10404.000000         0.039559             0.843331            
// 18                   01010011             83.000000            14276.000000         0.054281             0.897612            
// 19                   01010001             81.000000            14094.000000         0.053589             0.951202            
// 20                   01000101             69.000000            12834.000000         0.048798             1.000000            

// Average: 13150.000000
// Best: 16254.000000


// Generated Random Values: 0.398536 0.097369 0.007880 0.262697 0.000910 0.245858 0.395426 0.415756 0.411056 0.140539 0.864471 0.263077 0.831662 0.148339 0.761982 0.176288 0.293287 0.003620 0.762782 0.684273 
// Selected Parents: 7 1 0 4 0 4 7 7 7 2 17 4 16 2 14 3 5 0 14 13 

// Crossover occurred between Chromosome 8 and Chromosome 2 at Point 3
// Crossover occurred between Chromosome 1 and Chromosome 5 at Point 2
// Crossover occurred between Chromosome 1 and Chromosome 5 at Point 3
// Crossover occurred between Chromosome 8 and Chromosome 8 at Point 2
// Crossover occurred between Chromosome 8 and Chromosome 3 at Point 5
// Crossover occurred between Chromosome 17 and Chromosome 3 at Point 3
// Crossover occurred between Chromosome 15 and Chromosome 4 at Point 7
// Crossover occurred between Chromosome 6 and Chromosome 1 at Point 1
// Crossover occurred between Chromosome 15 and Chromosome 14 at Point 2

// Generation 3

// Chromosome           Binary               Decimal              Fitness              Probability          Cumulative          
// 1                    01011001             89.000000            14774.000000         0.053897             0.053897            
// 2                    01101101             109.000000           15914.000000         0.058056             0.111953            
// 3                    10000010             130.000000           16250.000000         0.059282             0.171235            
// 4                    01101111             111.000000           15984.000000         0.058312             0.229547            
// 5                    10100010             162.000000           15066.000000         0.054963             0.284509            
// 6                    01001111             79.000000            13904.000000         0.050723             0.335233            
// 7                    01001101             77.000000            13706.000000         0.050001             0.385234            
// 8                    01001101             77.000000            13706.000000         0.050001             0.435235            
// 9                    01001101             77.000000            13706.000000         0.050001             0.485236            
// 10                   01000101             69.000000            12834.000000         0.046820             0.532056            
// 11                   01010011             83.000000            14276.000000         0.052081             0.584136            
// 12                   01000010             66.000000            12474.000000         0.045507             0.629643            
// 13                   00100101             37.000000            8066.000000          0.029426             0.659069            
// 14                   01010011             83.000000            14276.000000         0.052081             0.711149            
// 15                   01111111             127.000000           16256.000000         0.059304             0.770453            
// 16                   10001100             140.000000           16100.000000         0.058735             0.829188            
// 17                   00101111             47.000000            9776.000000          0.035664             0.864852            
// 18                   11010101             213.000000           8946.000000          0.032636             0.897488            
// 19                   01101001             105.000000           15750.000000         0.057458             0.954946            
// 20                   10111110             190.000000           12350.000000         0.045054             1.000000            

// Average: 13705.700195
// Best: 16256.000000


// Generated Random Values: 0.881441 0.483675 0.079229 0.245428 0.788042 0.475365 0.473635 0.552514 0.952540 0.542635 0.312177 0.033470 0.862801 0.918161 0.914531 0.181738 0.066629 0.475775 0.758782 0.875631 
// Selected Parents: 17 8 1 4 15 8 8 10 18 10 5 0 16 18 18 3 1 8 14 17 

// Crossover occurred between Chromosome 18 and Chromosome 9 at Point 6
// Crossover occurred between Chromosome 2 and Chromosome 5 at Point 4
// Crossover occurred between Chromosome 16 and Chromosome 9 at Point 5
// Crossover occurred between Chromosome 9 and Chromosome 11 at Point 5
// Crossover occurred between Chromosome 19 and Chromosome 11 at Point 1
// Crossover occurred between Chromosome 6 and Chromosome 1 at Point 1
// Crossover occurred between Chromosome 19 and Chromosome 4 at Point 4
// Crossover occurred between Chromosome 2 and Chromosome 9 at Point 1

// Generation 4

// Chromosome           Binary               Decimal              Fitness              Probability          Cumulative          
// 1                    11010101             213.000000           8946.000000          0.031877             0.031877            
// 2                    01001101             77.000000            13706.000000         0.048839             0.080716            
// 3                    01100010             98.000000            15386.000000         0.054825             0.135541            
// 4                    10101101             173.000000           14186.000000         0.050549             0.186090            
// 5                    10001101             141.000000           16074.000000         0.057277             0.243367            
// 6                    01001100             76.000000            13604.000000         0.048475             0.291842            
// 7                    01001011             75.000000            13500.000000         0.048105             0.339947            
// 8                    01010101             85.000000            14450.000000         0.051490             0.391437            
// 9                    01010011             83.000000            14276.000000         0.050870             0.442306            
// 10                   01101001             105.000000           15750.000000         0.056122             0.498429            
// 11                   01011001             89.000000            14774.000000         0.052644             0.551073            
// 12                   01001111             79.000000            13904.000000         0.049544             0.600617            
// 13                   00101111             47.000000            9776.000000          0.034835             0.635452            
// 14                   01101001             105.000000           15750.000000         0.056122             0.691574            
// 15                   01101111             111.000000           15984.000000         0.056956             0.748530            
// 16                   01101001             105.000000           15750.000000         0.056122             0.804652            
// 17                   01001101             77.000000            13706.000000         0.048839             0.853491            
// 18                   01101101             109.000000           15914.000000         0.056707             0.910197            
// 19                   01111111             127.000000           16256.000000         0.057925             0.968123            
// 20                   11010101             213.000000           8946.000000          0.031877             1.000000            

// Average: 14031.900391
// Best: 16256.000000


// Generated Random Values: 0.360446 0.500805 0.158578 0.827732 0.998680 0.553044 0.769612 0.030090 0.721873 0.555484 0.193238 0.065599 0.337727 0.582314 0.650374 0.321967 0.512865 0.881671 0.653763 0.066359 
// Selected Parents: 7 10 3 16 19 11 15 0 14 11 4 1 6 11 13 6 10 17 13 1 

// Crossover occurred between Chromosome 8 and Chromosome 11 at Point 1
// Crossover occurred between Chromosome 5 and Chromosome 2 at Point 7
// Crossover occurred between Chromosome 14 and Chromosome 7 at Point 4
// Crossover occurred between Chromosome 11 and Chromosome 18 at Point 4

// Generation 5

// Chromosome           Binary               Decimal              Fitness              Probability          Cumulative          
// 1                    01011001             89.000000            14774.000000         0.052553             0.052553            
// 2                    01010101             85.000000            14450.000000         0.051400             0.103953            
// 3                    10101101             173.000000           14186.000000         0.050461             0.154414            
// 4                    01001101             77.000000            13706.000000         0.048754             0.203167            
// 5                    11010101             213.000000           8946.000000          0.031822             0.234989            
// 6                    01001111             79.000000            13904.000000         0.049458             0.284447            
// 7                    01101001             105.000000           15750.000000         0.056024             0.340471            
// 8                    11010101             213.000000           8946.000000          0.031822             0.372293            
// 9                    01101111             111.000000           15984.000000         0.056857             0.429150            
// 10                   01001111             79.000000            13904.000000         0.049458             0.478608            
// 11                   10001101             141.000000           16074.000000         0.057177             0.535784            
// 12                   01001101             77.000000            13706.000000         0.048754             0.584538            
// 13                   01001011             75.000000            13500.000000         0.048021             0.632559            
// 14                   01001111             79.000000            13904.000000         0.049458             0.682017            
// 15                   01101011             107.000000           15836.000000         0.056330             0.738347            
// 16                   01001001             73.000000            13286.000000         0.047260             0.785607            
// 17                   01011101             93.000000            15066.000000         0.053591             0.839198            
// 18                   01101001             105.000000           15750.000000         0.056024             0.895222            
// 19                   01101001             105.000000           15750.000000         0.056024             0.951247            
// 20                   01001101             77.000000            13706.000000         0.048754             1.000000            

// Average: 14056.400391
// Best: 16256.000000


// Generated Random Values: 0.020960 0.760392 0.280217 0.467735 0.261937 0.301237 0.532485 0.827962 0.380066 0.889161 0.679773 0.529855 0.243748 0.635964 0.466585 0.869741 0.179578 0.644114 0.978150 0.466025 
// Selected Parents: 0 15 5 9 5 6 10 16 8 17 13 10 5 13 9 17 3 13 19 9 

// Crossover occurred between Chromosome 1 and Chromosome 16 at Point 3
// Crossover occurred between Chromosome 6 and Chromosome 10 at Point 2
// Crossover occurred between Chromosome 6 and Chromosome 7 at Point 5
// Crossover occurred between Chromosome 11 and Chromosome 17 at Point 2
// Crossover occurred between Chromosome 9 and Chromosome 18 at Point 4
// Crossover occurred between Chromosome 4 and Chromosome 14 at Point 6
// Crossover occurred between Chromosome 20 and Chromosome 10 at Point 4
// Mutation occurred at Chromosome 11 Gene 3

// Generation 6

// Chromosome           Binary               Decimal              Fitness              Probability          Cumulative          
// 1                    01001001             73.000000            13286.000000         0.045703             0.045703            
// 2                    01011001             89.000000            14774.000000         0.050821             0.096524            
// 3                    01001111             79.000000            13904.000000         0.047829             0.144353            
// 4                    01001111             79.000000            13904.000000         0.047829             0.192182            
// 5                    01001001             73.000000            13286.000000         0.045703             0.237885            
// 6                    01101111             111.000000           15984.000000         0.054984             0.292868            
// 7                    10011101             157.000000           15386.000000         0.052927             0.345795            
// 8                    01001101             77.000000            13706.000000         0.047148             0.392943            
// 9                    01101001             105.000000           15750.000000         0.054179             0.447121            
// 10                   01101111             111.000000           15984.000000         0.054984             0.502105            
// 11                   01101111             111.000000           15984.000000         0.054984             0.557089            
// 12                   10001101             141.000000           16074.000000         0.055293             0.612382            
// 13                   01001111             79.000000            13904.000000         0.047829             0.660211            
// 14                   01001111             79.000000            13904.000000         0.047829             0.708040            
// 15                   01001111             79.000000            13904.000000         0.047829             0.755869            
// 16                   01101001             105.000000           15750.000000         0.054179             0.810047            
// 17                   01001111             79.000000            13904.000000         0.047829             0.857876            
// 18                   01001101             77.000000            13706.000000         0.047148             0.905024            
// 19                   01001111             79.000000            13904.000000         0.047829             0.952852            
// 20                   01001101             77.000000            13706.000000         0.047148             1.000000            

// Average: 14535.200195
// Best: 16256.000000