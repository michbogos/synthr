#ifndef SERIALIZE
#define SERIALIZE

#include<wavegraph.h>
#include<cJSON.h>

char* export_nodegraph(WaveNode* wavenode);
WaveNode* import_nodegraph(char* data_string);

#endif