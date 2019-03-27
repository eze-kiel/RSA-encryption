# RSA-encryption
## About this project
This is a school project, developed with my Baptiste Porte.

## Compilation
Be sure to have gmp.h on your device. If not, refer to [link](https://gmplib.org/#DOWNLOAD "The official GMP website").
Once you are ready to go, just enter `make all` to execute the makefile.

## How to use it ?
A file called "message.txt" is created during the execution of the makefile. You can modify the content but DON'T modify the name of the file.
By executing encryptor.c, you will generate a crypted file (called crypted_file.txt) and private_key.txt which contain your private key hashed with the password you entered.
By executing decryptor.c, you will decrypt crypted_file.txt. Your initial message should now appear in decrypted_file.txt.

## Notes
* gmp.h is needed to compile those scripts.
* You have to enter a NUMERICAL password during encryption !
