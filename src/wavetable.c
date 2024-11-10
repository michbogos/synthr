#include<wavetable.h>
#include<math.h>
#include<stdlib.h>

Wavetable wtbl_saw(float sample_rate, int table_len, int base){
    Wavetable table;
    table.len = table_len;
    table.base = base;
    table.sample_rate = sample_rate;
    int size = 1;
    while(base*2 < sample_rate/2){
        base*=2;
        size++;
    }
    table.num_tables = size;
    table.tables = (float**)malloc(size*sizeof(float*));
    for(int t = 0; t < table.num_tables; t++){
        table.tables[t] = malloc(table.len*sizeof(float));
        for(int i = 0; i < table.len; i++){
            table.tables[t][i] = 0.5;
            for(int k = 1; k < ceilf((sample_rate/2)/(table.base*(1<<t))); k++){
                table.tables[t][i] -= k%2 ? (1.0/3.1415926535)*(1.0/k)*sin(2*3.1415926535*k*((float)i/table.len)) : (1.0/3.1415926535)*(-1.0/k)*sin(2*3.1415926535*k*((float)i/table.len));
            }
        }
    }
    return table;
}

Wavetable wtbl_sqr(float sample_rate, int table_len, int base){
    Wavetable table;
    table.len = table_len;
    table.base = base;
    table.sample_rate = sample_rate;
    int size = 1;
    while(base*2 < sample_rate/2){
        base*=2;
        size++;
    }
    table.num_tables = size;
    table.tables = (float**)malloc(size*sizeof(float*));
    for(int t = 0; t < table.num_tables; t++){
        table.tables[t] = malloc(table.len*sizeof(float));
        for(int i = 0; i < table.len; i++){
            table.tables[t][i] = 0;
            for(int k = 1; k < ceilf((sample_rate/2)/(table.base*(1<<t))); k+=2){
                table.tables[t][i] += 4/3.1415926/k*sinf(2*3.1415926*k*(float)i/table.len);
            }
        }
    }
    return table;
}

Wavetable wtbl_tri(float sample_rate, int table_len, int base){
    Wavetable table;
    table.len = table_len;
    table.base = base;
    table.sample_rate = sample_rate;
    int size = 1;
    while(base*2 < sample_rate/2){
        base*=2;
        size++;
    }
    table.num_tables = size;
    table.tables = (float**)malloc(size*sizeof(float*));
    for(int t = 0; t < table.num_tables; t++){
        table.tables[t] = malloc(table.len*sizeof(float));
        for(int i = 0; i < table.len; i++){
            table.tables[t][i] = 0;
            int flip_flop = 1;
            for(int k = 1; k < ceilf((sample_rate/2)/(table.base*(1<<t))); k++){
                flip_flop*=-1;
                table.tables[t][i] += (8.0/3.1415926/3.1415926)*flip_flop/(k*k)*sinf(2*3.1415926*k*(float)i/table.len);
            }
        }
    }
    return table;
} 