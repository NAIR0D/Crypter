#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100000
#define KEY "KEY"

void encrypt(char* source_file, char* dest_file);
void decrypt(char* source_file, char* dest_file);
void normalize_text(char str[]);
int char_to_index(char c);
char index_to_char(int i);

int main()
{
    printf("Press 1 to encrypt\nPress 2 to decrypt\n");

    char choice = ' ';
    do{
    choice = getchar();
    }while(choice != '1' && choice != '2');

    char source_file[256], dest_file[256];

    if(choice == '1')
    {
        printf("\nFile to encrypt : ");
        scanf("%s", source_file);
        printf("Destination file :  ");
        scanf("%s", dest_file);
        encrypt(source_file, dest_file);
    }
    else if(choice == '2')
    {
        printf("\nFile to decrypt : ");
        scanf("%s", source_file);
        printf("Destination file :  ");
        scanf("%s", dest_file);
        decrypt(source_file, dest_file);
    }

    printf("\n");
    return 0;
}

void encrypt(char* source_file, char* dest_file)
{
    // Read from text file

    FILE *fp_source;
    char str[MAX_SIZE];
    char base_str[MAX_SIZE]; // Text to encrypt

    fp_source = fopen(source_file, "r");
    if (fp_source == NULL){
        printf("Could not open file %s", source_file);
        return;
    }

    while (fgets(str, MAX_SIZE, fp_source) != NULL)
        strcat(base_str, str);

    fclose(fp_source);

    // Vinegere cipher

    normalize_text(base_str);

    char key_str[MAX_SIZE]; // Key size must match text size

    for(int i = 0; i < strlen(base_str); i++)
        key_str[i] = KEY[i % strlen(KEY)];

    char vin_tab[26][26];
    int n_ascii = 0;
    for(int i = 0; i < 26; i++)
        for(int j = 0; j < 26; j++)
        {
            n_ascii = (i + j)%26 + 65;
            vin_tab[i][j] = (char) (n_ascii);
        }

    char vin_str[MAX_SIZE];
    int row, col;

    for(int i = 0; i < strlen(base_str); i++)
    {
        row = char_to_index(base_str[i]);
        col = char_to_index(key_str[i]);
        vin_str[i] = vin_tab[row][col];
    }

    // Ascii Caesar cipher (Transforms to non letter characters)

    char final_str[MAX_SIZE]; // Final encrypted text

    for (int i = 0; i < strlen(vin_str); i++)
        final_str[i] = vin_str[i] + 25 + strlen(KEY);

    // Write in destination file

    FILE *fp_dest;

    fp_dest = fopen(dest_file ,"w");
    if (fp_dest == NULL){
        printf("Could not open file %s", dest_file);
        return;
    }
    fprintf(fp_dest, "%s", final_str);
    printf("\nText to encrypt : \n\n%s\n\nEncrypted text : \n\n%s", base_str, final_str);
    fclose(fp_dest);

    printf("\n\nFile encrypted");
}

void decrypt(char* source_file, char* dest_file)
{
    // Read encrypted file

    FILE *fp_source;
    char str[MAX_SIZE];
    char encrypt_str[MAX_SIZE]; // Text to decrypt

    fp_source = fopen(source_file, "r");
    if (fp_source == NULL){
        printf("Could not open file %s", source_file);
        return;
    }

    while (fgets(str, MAX_SIZE, fp_source) != NULL)
        strcat(encrypt_str, str);

    fclose(fp_source);

    // Ascii Caesar cipher (Transforms to non letter characters)

    char str_1[MAX_SIZE];

    for (int i = 0; i < strlen(encrypt_str); i++)
        str_1[i] = encrypt_str[i] - 25 - strlen(KEY);

    // Vinegere cipher

    char key_str[MAX_SIZE]; // Key size must match text size

    for(int i = 0; i < strlen(str_1); i++)
        key_str[i] = KEY[i % strlen(KEY)];

    char vin_tab[26][26];
    int n_ascii = 0;
    for(int i = 0; i < 26; i++)
        for(int j = 0; j < 26; j++)
        {
            n_ascii = (i + j)%26 + 65;
            vin_tab[i][j] = (char) (n_ascii);
        }

    char base_str[MAX_SIZE]; // Original text
    int row, col;

    for(int i = 0; i < strlen(str_1); i++)
    {
        col = char_to_index(key_str[i]);
        for(int row = 0; row < 26; row++)
        {
            if(vin_tab[row][col] == str_1[i])
            {
                base_str[i] = index_to_char(row);
                break;
            }
        }
    }

    // Write in destination file

    FILE *fp_dest;

    fp_dest = fopen(dest_file ,"w");
    if (fp_dest == NULL){
        printf("Could not open file %s", dest_file);
        return;
    }
    fprintf(fp_dest, "%s", base_str);
    printf("\n\nText to decrypt : \n\n%s\n\nDecrypted text : \n\n%s", encrypt_str, base_str);
    fclose(fp_dest);

    printf("\n\nFile decrypted");
}

void normalize_text(char str[])
{
    // To  uppercase
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] >= 97 && str[i] <= 122)
            str[i] -= 32;
    }

    // Remove non-letter or digit characters
    int count = 0;
    for (int i = 0; i < strlen(str); i++)
        if((str[i] >= 65 && str[i] <= 91) || (str[i] >= 48 && str[i] <= 57))
            str[count++] = str[i];
    str[count] = '\0';
}

int char_to_index(char c)
{
    // Convert uppercase letter to digit 0-25

    if(c >= 65 && c <= 91)
        return c - 65;

    return -1;
}

char index_to_char(int i)
{
    // Convert digit 0-25 to uppercase letter

    if(i >= 0 && i <= 25)
        return (char) (i + 65);

    return ' ';
}
