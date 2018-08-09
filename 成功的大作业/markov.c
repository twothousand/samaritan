#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
char store[7000000][100];
struct prefix{
	char fword[100];
	char bword[100];
	char word[200];
	int suffix [200000];
	int num;
	struct prefix *lchild;
	struct prefix *rchild;
};
struct prefix *p_root;
struct prefix * add_prefix(char fword[100], char bword[100]){
	char word[200];
	strcpy(word, fword);
	strcat(word, bword);
	struct prefix *q, *p, *tem = p_root;
	while((strcmp(tem -> word, word) < 0 && tem -> rchild != NULL) || (strcmp(tem -> word, word) > 0 && tem -> lchild != NULL)){
		if(strcmp(tem -> word, word) < 0)
			tem = tem -> rchild;
		else
			tem = tem -> lchild;
	}
	q = (struct prefix *)malloc(sizeof(struct prefix));
	if(strcmp(tem -> word, word) < 0)
		tem -> rchild = q;
	else
		tem -> lchild = q;
	strcpy(q -> fword, fword);
	strcpy(q -> word, word);
	strcpy(q -> bword, bword);
	q -> lchild = NULL;
	q -> rchild = NULL;
	q -> num = 0;
	return q;
}
void add_suffix(struct prefix *now, int i){
	now -> suffix[now -> num ++] = i;
}
struct prefix *search_prefix(char fword[100], char bword[100]){
	struct prefix *tem = p_root;
	char word[200];
	strcpy(word, fword);
	strcat(word, bword);
	while(tem != NULL){
		if(strcmp(tem -> word, word) < 0)
			tem = tem -> rchild;
		else if(strcmp(tem -> word, word) > 0)
			tem = tem -> lchild;
		else
			return tem;
	}
	return tem;
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
	char ch = 0, end[20] = {'e', 'n', 'd', 'o', 'f', 'm', 'a', 'r', 'k', 'o', 'v', 0};
	int word_num = 0, per_word_num = 0, i, j, k, N, n;
	FILE *in, *out;
	struct prefix *p_search;
	p_root = (struct prefix*)malloc(sizeof(struct prefix));
	p_root -> word[0] = 0;
	p_root -> lchild = NULL;
	p_root -> rchild = NULL;
	if((in = fopen("article.txt", "r")) == NULL){
		printf("Can not find file!");
		return 1;
	}
	out = fopen("markov.txt", "w");
	while(ch != EOF){
		while((ch = fgetc(in)) == ' ')
			continue;
		per_word_num = 0;
		while(ch != ' ' && ch != EOF && ch != '\n'){
			store[word_num][per_word_num ++] = ch;
			ch = fgetc(in);
		}
		word_num ++;
	} 
	strcpy(store[word_num ++], end);                                                           //这段写法值得分析 
	for(i = 0; i < word_num - 2; i ++){
		if((p_search = search_prefix(store[i], store[i+1])) == NULL)
			p_search = add_prefix(store[i], store[i+1]);
		add_suffix(p_search, i+2);
	}
	scanf("%d", &N);
	p_search = p_root -> rchild;
	fprintf(out, "%s", p_search -> fword);
	for(i = 0; i < N - 1; i ++){
		fprintf(out, " %s", p_search -> bword);
		if(p_search -> num > 1)
			n = (int)(rrand() * p_search -> num);
		else
			n = 0;
		if(strcmp(end, store[p_search -> suffix[n]]) == 0)
			break;
		p_search = search_prefix(p_search -> bword, store[p_search -> suffix[n]]);
	}
	return 0;
}
