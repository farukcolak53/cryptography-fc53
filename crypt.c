// Ahmet Faruk Çolak 

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Word{
	char wordName[30];
	struct Word *nextWord;
};
typedef struct Word Word;

FILE* fp;

int insert(Word **header, char word[30]){
	
	Word *newWord;
	Word *last = *header;

	// create node to insert and assign values to its fields
	newWord = malloc(sizeof(Word));
	strcpy(newWord->wordName,word);
	newWord->nextWord = NULL;
	
	//If LL is empty
	if (*header == NULL){	
		*header = newWord;
		return 1;
	}
   
	else{
  		//Else traverse till the last node
   		while (last->nextWord != NULL) 
       		last = last->nextWord; 
   
    	//Change the next of last node
		last->nextWord = newWord; 
    	return;
	}	
}

int encryption(Word **header, int a, int b){
	
	Word *current = *header;
	int i;
	
	while(current != NULL){
		for(i = 0; i < strlen(current->wordName); i++){
			
			char ch = current->wordName[i];
			int ascii = ch;
			
			if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){ //isalpha
				if(ch >= 'A' && ch <= 'Z'){ //isupper
					ascii = ascii - 65; //makes base 0 ( 0 is A, 1 is B etc.)
					ascii = (a*ascii + b)%26;
					ascii = ascii + 65;
				}
				else if(ch >= 'a' && ch <= 'z'){ //islower
					ascii = ascii - 97; //makes base 0 ( 0 is a, 1 is b etc.)
					ascii = (a*ascii + b)%26;
					ascii = ascii + 97;
				}			
				ch = (char)ascii;
				current->wordName[i] = ch;
			}
			else{ //If it's a non-alphabetic character
				continue; //Continues with the next character
			}
		}
		current = current->nextWord;
	}
	return 1;	
}

int searchDictionary(Word **decrypted, Word **dict){
	
	int m, n;
	char word[30];
	char wordInD[30];
	
	int wordFound = 0; //counts the words that has found in the dictionary
	int wordCounter = 0; //counts the total number of words
	
	Word *iterDict;
	
	while(*decrypted != NULL){
		
		iterDict = *dict;

		int digitCounter = 0;
		wordCounter++;

		strcpy(word, (*decrypted)->wordName);
	
		for(m = 0; m < strlen(word); m++){
			if(isdigit(word[m])){
				digitCounter++;
			}
		}
		if(digitCounter == strlen(word))
			wordCounter--; //If the string just contains digits, decrements the number of words not to count it as a word.
					
		//Removes non-alphabetic characters		
		for(m = 0; word[m] != '\0'; ++m){
        	while (!( (word[m] >= 'a' && word[m] <= 'z') || (word[m] >= 'A' && word[m] <= 'Z') || word[m] == '\0')){
            	for(n = m; word[n] != '\0'; ++n){
                	word[n] = word[n+1];
          	  }
           	 word[n] = '\0';
        	}
   		}
   		
   		if((word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z') ){
   			while(iterDict != NULL){
   				strcpy(wordInD,iterDict->wordName);
   				if(strcasecmp(word, wordInD) == 0){
    				wordFound++;
    				break;
    			}
   				iterDict = iterDict->nextWord;
			}
		}
		*decrypted = (*decrypted)->nextWord;
	}
	printf("Searching for true decryption..\n");
	
	int percentage = (wordFound*100.0)/wordCounter;
	return percentage;	
}

int decryption(Word *header, Word **decrypted, int aInv, int b){

	int k;
	while(header != NULL){
		for(k = 0; k < strlen(header->wordName); k++){		
			char ch = header->wordName[k];
			int ascii = ch;
	
			if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){//isalpha
				if(ch >= 'A' && ch <= 'Z'){//isupper
					ascii = ascii - 65 ; 
					ascii = (aInv*(ascii - b))%26;
							
					while(ascii < 0) //If a negative value comes from decryption formula
						ascii = ascii + 26;
								
					ascii = ascii + 65;	
				}
				else if(ch >= 'a' && ch <= 'z'){//islower
					ascii = ascii - 97;
					ascii = (aInv*(ascii - b))%26;
					
					while(ascii < 0)
						ascii = ascii +26;
							
					ascii = ascii + 97;
				}		
				ch = (char)ascii;
		
				header->wordName[k] = ch;
			}
			else{
				continue; //Continues with the next characteR
			}				
		}
		insert(&(*decrypted), header->wordName);
		header = header->nextWord;
	}
	return 1;
}

int main(){

	Word *dict = NULL; //Stores the dictionary
	
	char dictWord[30];
	
	fp = fopen("dictionary.txt", "r");
	while(!feof(fp)){
		fscanf(fp, "%s", dictWord);
		insert(&dict, dictWord);
	}
	fclose(fp);
	
	Word *hdr = NULL; //Will store the input file's words
	Word *iter;
	
	char word[30];
	
	//Reading the input file	
	fp = fopen("input1.txt", "r");
	while (!feof(fp)) { 
		fscanf(fp,"%s", word); 
    	insert(&hdr, word);
	}
	fclose(fp);
	
	//Choosing a and b values for encryption
	int i, gcd;
	int a = 0;
	int b = 0;
	
	do{
		srand(time(NULL));
		a = rand() % 26;
		b = rand() % 26;
	
		for(i = 1; i <= a && i <= 26; i++){
	
    		// Checks if i is factor of both integers
       		if(a % i == 0 && 26 % i == 0)
            gcd = i;
    	}
	}
	while(gcd != 1);
	
	printf("For encryption a = %d, b = %d\n", a, b);
	
	encryption(&hdr, a, b);
	
	//Writing out the encrypted version	
	fp = fopen("encrypted.txt", "w");
	iter = hdr;
	while(iter != NULL){
		fprintf(fp, iter->wordName);
		fprintf(fp, " ");
		iter = iter->nextWord;
	}
	fclose(fp);
	
	////////////////////////DECRYPTION///////////////////////

	int aInv[12] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25}; //possible values for inverse of a
	int bDecrypt[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ,15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
	int j;
	
	for(i = 0; i < 12; i++){
		for(j= 0; j < 26; j++){
			
			Word *enc = NULL;
			char wordEnc[30];
			fp = fopen("encrypted.txt", "r");
			while(!feof(fp)){
				fscanf(fp, "%s", wordEnc);
				insert(&enc, wordEnc);
			}
			fclose(fp);
			
			Word *decrypted = NULL; //Will consist truely decrypted version
			decryption(enc, &decrypted, aInv[i], bDecrypt[j]);
			int percentage = 0;
			Word *decHead = decrypted; 
			percentage = searchDictionary(&decHead, &dict);
			if(percentage > 70){
				printf("For decryption--> a = %d, b = %d\n", aInv[i], j);
				Word *currentWord = decrypted;
				fp = fopen("decrypted.txt", "w");
				while(currentWord->nextWord != NULL){
					fprintf(fp, currentWord->wordName);
					fprintf(fp, " ");
					currentWord = currentWord->nextWord;
				}
				fclose(fp);
				printf("Percentage is %d", percentage);
				exit(1);
			}
		}
	}
	return 1;
}

/*References
https://www.geeksforgeeks.org/linked-list-set-2-inserting-a-node/  
https://www.programiz.com/c-programming/examples/remove-characters-string
*/
