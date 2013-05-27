/*
 * main.c
 *
 *  Created on: May 27, 2013
 *      Author: nsbuild
 */



#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;

void build_decoding_table() {
    int i;

    decoding_table = (char *)malloc(256);
    /* set undefined values to null */
    memset(decoding_table, 0, 256 * sizeof(decoding_table[0]));

    for (i = 0; i < 0x40; i++)
        decoding_table[encoding_table[i]] = i;
}

unsigned char *base64_decode(const char *data,
                    size_t input_length,
                    size_t *output_length) {

    int i, j;

    if (data == NULL) {
        return NULL;
    }

    if (decoding_table == NULL) build_decoding_table();

    /* if (input_length % 4 != 0) return NULL; */
    
    *output_length = input_length / 4 * 3;
    
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = (unsigned char *)malloc(*output_length);
    if (decoded_data == NULL) return NULL;
    
    for (i = 0, j = 0; i < input_length/4*4;) {

        //skip newline and other non-base64 symbols
        while (decoding_table[data[i]] == NULL) {
            printf("skipping %c \n", data[i]);
            i++;
        }

        unsigned sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        unsigned sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        unsigned sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        unsigned sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        printf("sextet %d \n", sextet_a);
        printf("sextet %d \n", sextet_b);
        printf("sextet %d \n", sextet_c);
        printf("sextet %d \n", sextet_d);

        unsigned triple = (sextet_a << 3 * 6)
                        + (sextet_b << 2 * 6)
                        + (sextet_c << 1 * 6)
                        + (sextet_d << 0 * 6);
        printf("triple %d \n", triple);                        

        if (j < *output_length) {
            decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
            printf("triplet %d \n", decoded_data[j - 1]);                                        
        } 
        if (j < *output_length) {
            decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
            printf("triplet %d \n", decoded_data[j - 1]);                                        
        }
        if (j < *output_length) {            
            decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
            printf("triplet %d \n", decoded_data[j - 1]);                                        
        }
    }

    return decoded_data;
}

int main(int argc, char **argv) {
    char *s = "YWJj";
    size_t output_length;

    //printf("Starting length: %d\n", strlen (s));
    printf("XXX \n");
    base64_decode(s, strlen (s), &output_length);
    //printf("Decoded: %s", base64_decode(s, strlen (s), &output_length));
    printf("Output: %d", output_length);
}