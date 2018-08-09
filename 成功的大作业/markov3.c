#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <math.h>
char store[6500000][100];
struct suffix_store{
	int the_order;
	int in_store;
	struct suffix_store *next;
};
struct prefix_store{
	char *f_word;
	char *b_word;
	int num;
	int suffix_hstore[3];
	struct suffix_store *root;
	struct suffix_store *last;
	struct prefix_store *next;
};
struct prefix_root{
	struct prefix_store *next;
	struct prefix_store *last;
};
struct prefix_root prefix[20000000];
int ELFhash(char str1[100], char str2[100])  
{
	char str[200];
	strcpy(str, str1);
	strcat(str, str2);
    int hash = 0;  
    int  x = 0, i = 0;  
    while(str[i])  
    {  
        hash=(hash<<4)+str[i];     //1  
        if((x=hash & 0xf0000000)!=0)         //2  
        {  
            hash^=(x>>24);   //影响5-8位，杂糅一次   3  
            hash&=~x;   //清空高四位    4  
        }  
        i ++;   //5  
    }  
    return (hash & 0x7fffffff) % 10000007;    //6   
}  
double seed = 997;
double rrand()
{
       double lambda = 3125.0;
       double m = 34359738337.0;
       double r;
       seed = fmod(lambda*seed, m); //要包含头文件#include <math.h>
       r = seed / m;
    return r;
}
int main(void){
	char ch = 0, end[20] = {'e', 'n', 'd', 'o', 'f', 'm', 'a', 'r', 'k', 'o', 'v', 0}, *word1, *word2, *op = "open.", *m = "Men", tem[200], *test1 = "whatareyo", *test2 = "lwhywou";
	int word_num = 0, per_word_num = 0, i, j, k, N, n, temp, line=1;
	unsigned int order;
	FILE *in, *out;
	struct prefix_store *q, *p;
	struct suffix_store *t, *r;
	if((in = fopen("article.txt", "r")) == NULL){
		printf("Can not find file!");
		return 1;
	}
	out = fopen("markov.txt", "w");
	for(i = 0; i < 20000000; i ++){
		prefix[i].next = NULL;
		prefix[i].last = NULL;
	}	
	while(fscanf(in, "%s", store[word_num]) == 1){
		word_num ++;
	} 
	strcpy(store[word_num ++], end);
	for(i = 0; i < word_num - 2; i ++){
		order = ELFhash(store[i], store[i+1]);
		if(prefix[order].next == NULL){
			q = (struct prefix_store*)malloc(sizeof(struct prefix_store));
			q -> f_word = store[i];
			q -> b_word = store[i + 1];
			q -> next = NULL;
			q -> num = 0;
			q -> suffix_hstore[q -> num ++] = i + 2;
			prefix[order].next = q;
			prefix[order].last = q;
		}
		else{
			p = prefix[order].next;
			while(p != NULL && (strcmp(p -> f_word, store[i]) != 0 || strcmp(p -> b_word, store[i+1]) != 0))
				p = p -> next;
			if(p == NULL){
				q = (struct prefix_store*)malloc(sizeof(struct prefix_store));	
				q -> f_word = store[i];
				q -> b_word = store[i + 1];
				q -> next = NULL;
				q -> num = 0;
				q -> suffix_hstore[q -> num ++] = i + 2;
				prefix[order].next = q;
				prefix[order].last = q;
			}
			else{
				if(p -> num < 3){
					q = p;
					q -> suffix_hstore[q -> num ++] = i + 2;
				}		
				else if(p -> num == 3){
					q = p;
					r = (struct suffix_store*)malloc(sizeof(struct suffix_store));
					q -> root = r;
					q -> last = r;
					r -> next = NULL;
					r -> the_order = 3;
					r -> in_store = i + 2;
					q -> last -> next = NULL; 	
					q -> num ++;
				}
				else if(p -> num > 3){
					q = p;
					r = (struct suffix_store*)malloc(sizeof(struct suffix_store));
					q -> last -> next = r;
					r -> the_order = q -> last -> the_order + 1;
					r -> in_store = i + 2;
					q -> last = r;
					q -> last -> next = NULL; 	
					q -> num ++;
				}
			}	
		}
	}
	scanf("%d", &N);
	word1 = store[0];
	word2 = store[1];
	fprintf(out, "%s ", store[0]);
	/*if(prefix[26664590].next == NULL)
		printf("NULL");
	else{
		printf("FULL");
		printf("%s", prefix[26664590].next -> f_word);
	}*/	
	for(i = 0; i < N - 1; i ++){
		order = ELFhash(word1, word2);
		p = prefix[order].next;
		while(p != NULL && (strcmp(p -> f_word, word1) != 0 || strcmp(p -> b_word, word2) != 0))
			p = p -> next;
		//printf("%s %s %d %d",word1, word2, p -> num, line ++);
		if(p -> num > 1)
			n = (int)(rrand() * (p -> num));
		else
			n = 0;
		fprintf(out, "%s ", word2);
		word1 = word2;
		t = p -> root;
		if(n >= 3){
			while(t -> the_order != n)
				t = t -> next;
			word2 = store[t -> in_store];
		}
		else if(n < 3)
			word2 = store[p -> suffix_hstore[n]];
		//printf(" %s\n", word2);
		if(strcmp(end, word2) == 0)
			break;
	}
	fclose(in);
	fclose(out);
	return 0;
}
