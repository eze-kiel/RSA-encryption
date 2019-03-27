# RSA-encryption
## About this project
This is a school project, developed with my mate Baptiste Porte.

## How to use it ?
You need to create a file called "message.txt" with the text you want to encrypt.
By executing rsa_chiffr.c, you will generate a crypted file (called crypted_file.txt) and private_key.txt which contain your private key hashed with the password you entered.
By executing rsa_dechiffr.c, you will decrypt crypted_file.txt. Your initial message should now be in decrypted_file.txt.

## Notes
gmp.h is needed to compile those scripts.
You have to enter a NUMERICAL password during encryption !
