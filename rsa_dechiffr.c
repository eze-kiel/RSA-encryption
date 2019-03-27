/*
Authors : Baptiste Porte, Hugo Blanc
*/

#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE_BUFF 2048

void dechiffrement(mpz_t *crypted_message, mpz_t d, mpz_t n, mpz_t *decrypted_message, FILE *ptr_decrypted_file);

int main(int argc, char const *argv[])
{
    clock_t start_script = clock();

    char char_encoded[SIZE_BUFF];
    unsigned long int password;

    mpz_t crypted_message, decrypted_message, d, n, tempo;
    mpz_init(crypted_message);
    mpz_init(decrypted_message);
    mpz_init(d);
    mpz_init(n);
    mpz_init(tempo);

    //ouverture de fichiers
    FILE *ptr_private_key = NULL;
    FILE *ptr_crypted_file = NULL;
    FILE *ptr_decrypted_file = NULL;

    ptr_private_key = fopen("private_key.txt","r");
    
    if (ptr_private_key != NULL)
    {
        fgets(char_encoded, SIZE_BUFF, ptr_private_key);
        mpz_set_str(tempo, char_encoded, 10);
        printf("Rentrez votre mot de passe pour decrypter le message : \n");
        scanf("%lu",&password);
        mpz_cdiv_q_ui(d, tempo, password);

        fgets(char_encoded, SIZE_BUFF, ptr_private_key);
        mpz_set_str(n, char_encoded, 10);

    }else{
        printf("Impossible d'ouvrir private_key.txt\n");
    }


    ptr_crypted_file = fopen("crypted_file.txt","r");
    ptr_decrypted_file = fopen("decrypted_file.txt","w");
    fclose(ptr_decrypted_file);
    ptr_decrypted_file = fopen("decrypted_file.txt","r+");

    //inserer le message dechiffré dans decrypted_file.txt
    if (ptr_crypted_file != NULL)
    {
        printf("\n[+] Déchiffrement en cours ...\n\n");
        while(fgets(char_encoded,SIZE_BUFF,ptr_crypted_file) != NULL)
        {
            mpz_set_str(crypted_message, char_encoded, 10);
            dechiffrement(&crypted_message,d,n,&decrypted_message, ptr_decrypted_file);                   
        } 

        fclose(ptr_crypted_file);
        fclose(ptr_decrypted_file);
    }
    else
    {
        printf("Impossible d'ouvrir votre fichier\n");
    }


    mpz_clear(decrypted_message);
    mpz_clear(crypted_message);
    clock_t end_script = clock();
    double time_spent = (double)(end_script - start_script) / CLOCKS_PER_SEC;
    printf("\n\ntime spent (sec): %f\n", time_spent);
    return 0;
}

void dechiffrement(mpz_t *crypted_message, mpz_t d, mpz_t n, mpz_t *decrypted_message, FILE *ptr_decrypted_file)
{
    char msg = 10;

    //(crypted_message^d)%n
    mpz_powm(*decrypted_message, *crypted_message, d, n);
    msg = mpz_get_ui(*decrypted_message);
    fprintf(ptr_decrypted_file, "%c",msg);
}