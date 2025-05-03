#ifndef SERIALIZE
#define SERIALIZE

#include<wavegraph.h>
#include<cJSON.h>

char* export_nodegraph(WaveNode* wavenodes, int n);
WaveNode* import_nodegraph(char* data_string);

#endif