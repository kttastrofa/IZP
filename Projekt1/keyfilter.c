/**
 * @name xmeciak00
 * @file keyfilter.c
 * @date 04.11.2023
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


/**
 * @brief meni male znaky na velke, podla ASCII
 * @param string vstup || subor
 * @return 
*/
void aA(char *string)
{
    int i = 0;
    while (string[i]!='\0')
    {
        if (string[i]>='a' && string[i]<='z')
        {
            string[i]-= ('a' - 'A');
        } 
        i++;
    }
}

/**
 * @brief kontroluje opakujuce sa znaky v stringu
 * @param string print
 * @param letter subor[i][j+1]
 * @return bool
*/
bool isin(char *string, char letter)
{
    for (int k=0; k < (int)strlen(string); k++)
    {
        if (letter==string[k])
        {
            return true;
        }        
    }
    
    return false;
}


int main(int argc, char *argv[])
{
    
    //inicializacia premennych
    char vstup[101];
    char subor[101][101];
    char print[101] = "";
    char slovo[101];
    char prve[101];
    char t;
    int i=0, j=0, pocet_zhodnych=0;

    if (argc == 2) //ak boli dane 2 argumenty, tj. zavolanie programu aj prefix
    {
        strcpy(vstup, argv[1]);//kopiruj prefix do stringu vstupu
    }
   
    aA(vstup);

    while (fgets(subor[i], sizeof(subor[i]), stdin) != NULL) //definuje velkost suboru, ulozene v i
    {
        aA(subor[i]);

        subor[i][strlen(subor[i])-1] = '\0';
        
        if(argc == 1 && !isin(prve, subor[i][0]))//ak bol dany 1 argument, tj. zavolanie programu bez prefixu
        {
            prve[strlen(prve)]=subor[i][0];
            prve[strlen(prve) + 1] = '\0';
            
        }
        else if (!(strcmp(vstup, subor[i]))) //ak sa prefix presne zhoduje s adresou
        {
            strcpy(slovo, subor[i]);//kopiruje adresu na danom riadku suboru do stringu slovo
            printf("Found: %s\n", slovo); //vypise zhodne slovo
            return 0;//"ukonci hladanie adresy"
        }
        else //ak ani jedno z prdoslych neplati
        {
            j = 0;
            int posledna_zhoda=0;
            while (j<(int)strlen(vstup) && subor[i][j] == vstup[j])//kym je znak mensi ako dlzka prefixu a zaroven kym sa znak adresy zhoduje so znakom prefixu
            {
                if (j == (int)strlen(vstup)-1 && !isin(print, subor[i][j+1]))//ak ide o posledny znak prefixu a zaroven sa nasledujuci znak adresy nenachadza v stringu na ukladanie nasledujucich znakov
                {
                    if (j>posledna_zhoda)//ak posledne uchovavana hodnota znaku zhody prefixu a adresy je mensia ako hodnota aktualnej zhody
                    {
                        strcpy(slovo, subor[i]);//kopiruj danu adresu do stringu slovo (uklada najdenu adresu)
                    }
                    print[strlen(print)] = subor[i][j+1];
                    print[strlen(print) + 1] = '\0';
                    posledna_zhoda=j;//uchovava hodnotu znaku poslednej zhody prefixu a adresy
                    pocet_zhodnych++;
                }
                
                j++;
            }
        }
        i++;
    }

    for (i = 0; i < (int)strlen(print)-1; i++) //abecedne zoradi
    {
        for (j = i + 1; j < (int)strlen(print); j++) 
        {
            if (print[i] > print[j]) 
            {
                t = print[i];
                print[i] = print[j];
                print[j] = t;
            }
        }
    }

    //skontroluje pocet vhodnych slov a vyhodnoti vysledok
    if(argc == 1) printf("Enable: %s\n", prve);//1 argument == bez zadania prefixu, vypis vsetky prve znaky adries
    else if (pocet_zhodnych==1) printf("Found: %s\n", slovo);//ak si nasiel len 1 slovo
    else if (pocet_zhodnych>=2) printf("Enable: %s\n", print);//vypise nasledujuce vhodne znaky
    else printf("Not found\n"); //nenaslo slovo 

    return 0;
}