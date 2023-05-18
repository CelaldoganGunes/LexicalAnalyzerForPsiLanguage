#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_IDENTIFIER_SIZE 30
#define MAX_INTEGER_SIZE 10

enum States {
    ID,
    OPERATOR,
    COMMENT,
    STRING,
    PARANTEZ,
    ENDOFLINE
};

enum States active_state = ID;

int isIdentifier(char str[])
{
    printf("\n");
    //printf("isIdentifier:%s\n",str);
    int length = strlen(str);

    bool isInteger = true;
    for (int i = 0; i < length; i++)
    {
        if (isdigit(str[i]) == false)
        {
            isInteger = false;
            break;
        }
    }

    if (isInteger == true)
    {
        if (length > MAX_INTEGER_SIZE)
        {
            printf("Error: Integer must be shorter than 11 chars.");
            return 0;
        }

        //NEGATİFLİĞİ UNUTMA

        printf("IntConst(%s)",str);
        return 0;
    }

    if (!isalpha(str[0]))
    {
        printf("Error: Identifier must start with a letter.");
        return 0;
    }

    if (length > MAX_IDENTIFIER_SIZE)
    {
        printf("Error: Identifier must be shorter than 31 chars.");
        return 0;
    }

    char keywords[18][10] = {"break","case","char","const","continue","do","else","enum",
                             "float","for","goto","if","int","long","record","return","static","while"};

    for(int i = 0; i < 18; ++i){
        if(strcmp(keywords[i], str) == 0){
            printf("Keyword(%s)",str);
            return 0;
        }
    }

    printf("Identifier(%s)",str);
    return 0;
}

int isOperator(char str[]){
    char operators[7][3] = {"+","-","*","/","++","--",":="};
    for(int i = 0; i < 7; ++i)
    {
        if(strcmp(operators[i], str) == 0) //aynıysa 0dır
        {
            printf("\nOperator(%s)", str);
            return 0;
        }
    }
    printf("\nUnvalid Operator(%s)", str);
    return 0;

}


//satırı gönderiyoruz
void handle(char line[]) {
    char token[100] = "";

    int length = strlen(line);
    printf("length: %d",length);

    for (int i = 0; i < length; i++)
    {
        //int token_length = strlen(token);
        //printf("token: %s, token len:%d\n", token, token_length);
        //printf("char: %c\n", line[i]);
        char new_char = line[i];

        if (active_state == ID)
        {
            if (new_char == ' ' || new_char == '\n' || new_char == '\0')
            {
                //printf("\nbosluk");
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isIdentifier(token);
                    strcpy(token, "");
                }
                continue;
            }
            else if (isalnum(new_char) || new_char == '_')
            {
                strncat(token,&new_char, 1);
                continue;
            }
            else if (new_char=='+' || new_char=='-' || new_char=='*' || new_char=='/' || new_char==':' || new_char=='=')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isIdentifier(token);
                }
                active_state = OPERATOR;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
            else if (new_char=='(' || new_char==')' || new_char=='{' || new_char=='}' || new_char=='[' || new_char==']')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isIdentifier(token);
                }
                active_state = PARANTEZ;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
            else if (new_char=='"')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isIdentifier(token);
                }
                active_state = STRING;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
            else if (new_char==';')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isIdentifier(token);
                }
                active_state = ENDOFLINE;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
        }
        else if (active_state == OPERATOR)
        {
            if (new_char == ' ' || new_char == '\n' || new_char == '\0')
            {
                //printf("\nbosluk");
                //printf("token: %s\n", token);
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isOperator(token);
                    strcpy(token, "");
                }
                continue;
            }
            else if (isalnum(new_char) || new_char == '_')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isOperator(token);
                }
                active_state = ID;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
            else if (new_char=='+' || new_char=='-' || new_char=='*' || new_char=='/' || new_char==':' || new_char=='=')
            {
                strncat(token,&new_char, 1);

                //alttakinin doğrusu
                int token_length = strlen(token);

                if (token_length == 2 && strcmp(token, "/*") == 0)
                {
                    active_state = COMMENT;
                    strcpy(token, "");
                }
                else if (token_length > 2 && token[token_length-2] == '/' && token[token_length-1] == '*')
                {
                    //For döngüsü ile ekledik çünkü stringin sonunda \0 koymadığımızdan strncopy bozuk çalışıyor.
                    char sub_token[] = "";
                    for(int a = 0; a < token_length-2; a++)
                    {
                        strncat(sub_token,&token[a], 1);
                    }

                    isOperator(sub_token);

                    active_state = COMMENT;
                    strcpy(token, "");
                }
                continue;
            }
            else if (new_char=='(' || new_char==')' || new_char=='{' || new_char=='}' || new_char=='[' || new_char==']')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isOperator(token);
                }
                active_state = PARANTEZ;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
            else if (new_char=='"')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isOperator(token);
                }
                active_state = STRING;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
            else if (new_char==';')
            {
                if (strcmp(token, "") != 0)//farklıysa, kesinlikle != 0 kullan çünkü 0dan farklı herhangi bir değer olabilir.
                {
                    isOperator(token);
                }
                active_state = ENDOFLINE;
                strcpy(token, "");
                strncat(token,&new_char, 1);
                continue;
            }
        }
        else if (active_state == COMMENT)
        {

        }
        else if (active_state == STRING)
        {

        }
        else if (active_state == PARANTEZ)
        {

        }
        else if (active_state == ENDOFLINE)
        {

        }
    }
}

int main() {
    FILE *inputFile, *outputFile;
    char line[100];

    // Open the input file
    inputFile = fopen("code.psi", "r");
    if (inputFile == NULL) {
        printf("Error opening the file!\n");
        return 1;
    }

    while (fgets(line, sizeof(line), inputFile))
    {
        //printf("%s", line);

        handle(line);
        break;
    }


/*
    // Open the output file
    outputFile = fopen("code.lex", "w");
    if (outputFile == NULL) {
        printf("Error opening the file!\n");
        return 1;
    }*/

    return 0;
}