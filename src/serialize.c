#include<serialize.h>
#include<wavegraph.h>
#include<cJSON.h>
#include<stdio.h>

#include <stdint.h>
#include <stdlib.h>


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

void build_decoding_table() {

    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}


unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    if (decoding_table == NULL) build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}


void base64_cleanup() {
    free(decoding_table);
}

char* export_nodegraph(WaveNode* wavenodes, int n){
    cJSON* object = cJSON_CreateObject();

    cJSON_AddNumberToObject(object, "num_nodes", n);

    cJSON* nodes = cJSON_AddArrayToObject(object, "nodes");
    for(int i = 0; i < n; i++){
        cJSON* wavenode = cJSON_CreateObject();
        if(!cJSON_AddItemToArray(nodes, wavenode)){
            printf("Failed to add item to array\n");
        }
        cJSON* node_type = cJSON_CreateNumber((int)wavenodes[i].type);
        cJSON* node_id = cJSON_CreateNumber((int)wavenodes[i].id);
        cJSON* node_num_inputs = cJSON_CreateNumber((int)wavenodes[i].num_inputs);
        cJSON* node_inputs = cJSON_CreateIntArray(wavenodes[i].inputs, wavenodes[i].num_inputs);
        long unsigned int base64_length = 0;
        char* base64_encoded = base64_encode((unsigned char*)wavenodes[i].value, wavenodes[i].value_len, &base64_length);
        base64_encoded = realloc(base64_encoded, base64_length+1);
        base64_encoded[base64_length] = '\0';
        cJSON* node_value = cJSON_CreateString(base64_encoded);
        cJSON* node_value_len = cJSON_CreateNumber((int)wavenodes[i].value_len);
        cJSON_bool err = 0;
        err |= cJSON_AddItemToObject(wavenode, "node_type", node_type);
        err |= cJSON_AddItemToObject(wavenode, "node_num_inputs", node_num_inputs);
        err |= cJSON_AddItemToObject(wavenode, "node_inputs", node_inputs);
        err |= cJSON_AddItemToObject(wavenode, "node_value", node_value);
        err |= cJSON_AddItemToObject(wavenode, "node_value_len", node_value_len);
        err |= cJSON_AddItemToObject(wavenode, "node_id", node_id);
    }
    char* res = cJSON_Print(object);
    cJSON_Delete(object);
    return res;
}

WaveNode* import_nodegraph(char* data_string){
    cJSON* data = cJSON_Parse(data_string);
    cJSON* num_nodes = cJSON_GetObjectItemCaseSensitive(data, "num_nodes");
    cJSON* nodes = cJSON_GetObjectItemCaseSensitive(data, "nodes");
    WaveNode* wavenodes = malloc((unsigned int)num_nodes->valuedouble*sizeof(WaveNode));
    cJSON* node = NULL;
    int i = 0;
    cJSON_ArrayForEach(node, nodes){
        WaveNode tmp;
        unsigned long trash;
        tmp.id = cJSON_GetObjectItemCaseSensitive(node, "node_id")->valuedouble;
        //tmp.value = malloc(cJSON_GetObjectItemCaseSensitive(node, "node_value_len")->valuedouble);
        tmp.value_len = cJSON_GetObjectItemCaseSensitive(node, "node_value_len")->valuedouble;
        tmp.value = base64_decode(cJSON_GetObjectItemCaseSensitive(node, "node_value")->valuestring, 4 * ((tmp.value_len + 2) / 3), &trash);
        tmp.num_inputs = cJSON_GetObjectItemCaseSensitive(node, "node_num_inputs")->valuedouble;
        tmp.inputs = malloc(tmp.num_inputs*sizeof(int));
        cJSON* node_inputs = cJSON_GetObjectItemCaseSensitive(node, "node_inputs");
        cJSON* node_input = NULL;
        int j = 0;
        cJSON_ArrayForEach(node_input, node_inputs){
            tmp.inputs[j] = node_input->valuedouble;
            j++;
        }
        tmp.type = cJSON_GetObjectItemCaseSensitive(node, "node_type")->valuedouble;
        wavenodes[i] = tmp;
        i++;
    }
    return wavenodes;
}