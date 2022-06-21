A homemade tool to quickly encrypt and decrypt files and folders.
## Usage:
### Compilation:
```
gcc crypt.c -o crypt
```
### Execution
Use as
```
crypt -flag path
```
where ```-flag``` is ```-f``` if you wish to encrypt a file and ```-d``` if you wish to encrypt a directory. You will be prompted to enter a key, which should be atmost 256 characters long. To perform decryption run the same command again, and enter the same key as was used during encryption.
