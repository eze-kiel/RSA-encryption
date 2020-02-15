/*
Authors : Baptiste Porte, Hugo Blanc
*/

#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define BITS_FOR_ENCRYPTION 2048
#define SIZE_BUFF 2048

void pub_key(mpz_t* n, mpz_t p, mpz_t q, mpz_t phin, mpz_t* e);
void priv_key(mpz_t* e, mpz_t phin, mpz_t* d);

void chiffrement(mpz_t *plain_message, mpz_t e, mpz_t n, mpz_t *crypted_message, FILE *ptr_crypted_file);

int main(int argc, char const *argv[])
{
    clock_t start_script = clock();

    int char_in_file = 42; // just be be sure that we do not init as EOF (-1)
    unsigned long password;

    //initialiser toutes les variables gmp
    mpz_t rand_Num; mpz_init(rand_Num);
    mpz_t p; mpz_init(p);
    mpz_t q; mpz_init(q);
    mpz_t phin; mpz_init(phin);
    mpz_t n; mpz_init(n);
    mpz_t e; mpz_init(e);
    mpz_t d; mpz_init(d);

    mpz_t mod; mpz_init_set_ui(mod,1);

    mpz_t crypted_message, plain_message, encoded_key, mpz_password;
    mpz_init(encoded_key);
    mpz_init(crypted_message);
    mpz_init(mpz_password);
    gmp_randstate_t r_state;

    //generer un seed aleatoire
    int seed;
    srand(time(NULL));
    seed = rand();
    printf("\nrandom seed: %d\n", seed);

    //initialiser les fonctions random de gmp
    gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, (int)seed);

    //generer p
    mpz_urandomb(rand_Num,r_state,BITS_FOR_ENCRYPTION);
    mpz_nextprime(p, rand_Num);

    //stocker e pour le calcul du pgcd
    mpz_set(e, p);

    //generer q
    mpz_urandomb(rand_Num,r_state,BITS_FOR_ENCRYPTION);
    mpz_nextprime(q, rand_Num);

    pub_key(&n, p, q, phin, &e);
    priv_key(&e, phin, &d);

    //ouverture des fichiers
    FILE *ptr_plain_file = NULL;
    FILE *ptr_crypted_file = NULL;
    FILE *ptr_key_file = NULL; 

    ptr_plain_file = fopen("message.txt","r");
    ptr_crypted_file = fopen("crypted_file.txt","w");
    ptr_key_file = fopen("private_key.txt","w");

    if (ptr_key_file == NULL)
    {
        printf("Impossible d'ouvrir le fichier private_key.txt\n");
        return 1;
    }
    printf("\n\nRentrez un mot de passe numérique : ");
    scanf("%lu", &password);

    mpz_mul_ui(encoded_key, d, password);
    mpz_out_str(ptr_key_file, 10, encoded_key);
    fprintf(ptr_key_file, "\n");
    mpz_out_str(ptr_key_file, 10, n);

    fclose(ptr_key_file);

    //inserer le message chiffré dans crypted_file.txt
    if (ptr_plain_file == NULL)
    {
        printf("Impossible d'ouvrir plain_message 'message.txt'\n");
        return 1;
    }

    printf("\n\n[+] Chiffrement en cours...\n\n");

    while (char_in_file != EOF)
    {
        char_in_file = fgetc(ptr_plain_file);
        mpz_init_set_ui(plain_message, char_in_file);
        chiffrement(&plain_message, e, n, &crypted_message, ptr_crypted_file);
    }

    fclose(ptr_plain_file);
    fclose(ptr_crypted_file);

    mpz_clear(crypted_message);
    clock_t end_script = clock();
    double time_spent = (double)(end_script - start_script) / CLOCKS_PER_SEC;
    printf("\n\ntime spent (sec): %f\n", time_spent);
    return 0;
}

void pub_key(mpz_t* n, mpz_t p, mpz_t q, mpz_t phin, mpz_t* e)
{
    mpz_t gcd; mpz_init(gcd);
    mpz_mul(*n, p, q);

    //calculer phin
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phin, p, q);

    //determiner e pour avoir pgcd(e,phin) == 1
    do
    {
        mpz_add_ui(*e, *e, 1);
        mpz_gcd(gcd, *e, phin);
    } while (mpz_cmp_ui(gcd, 1) != 0);

    //gmp_printf("\ne = %Zd\n", e);

    gmp_printf("\nPUBLIC KEY (e,n):\n(%Zd , ", e);
    gmp_printf("%Zd)", n);
}

void priv_key(mpz_t* e, mpz_t phin, mpz_t* d)
{
    mpz_t mul; mpz_init(mul);
    mpz_t mod; mpz_init(mod);

    mpz_invert(*d, *e, phin);

    mpz_mul(mul, *e, *d);
    mpz_mod(mod, mul, phin);
    assert(mpz_cmp_ui(mod, 1) == 0);

    gmp_printf("\n\nPRIVATE KEY (d):\n%Zd", d);
}

// BP

void chiffrement(mpz_t *plain_message, mpz_t e, mpz_t n, mpz_t *crypted_message, FILE *ptr_crypted_file)
{
    //(plain_message^e)%n
    mpz_powm(*crypted_message, *plain_message, e, n); 
    mpz_out_str(ptr_crypted_file, 10, *crypted_message);   
    fprintf(ptr_crypted_file, "\n");
}
