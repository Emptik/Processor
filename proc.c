#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

char * s;
int p;
int semicolon_counter;

float ax;
float bx;
float cx;
float dx;

struct Node 
{
	char * val;
	struct Node ** array;
	int type;
};

struct Lifo
{
	float stack[1000];
	int size;
} stack;

void arr_fill(char * array, FILE * stream);
char * arr_out(char * array);

struct Node * G(char * str);
struct Node * GetF_R();
struct Node * GetF();
struct Node * GetR_E();
struct Node * GetE();
struct Node * GetT();
struct Node * GetP();
struct Node * GetR();
struct Node * GetN();

void Dot_print(struct Node * root, FILE * stream, int lab, int * lab_count);
void tree_png(struct Node * root, FILE * stream, int lab, int * lab_count);
void digraph(FILE * stream, int * lab_count);

int main()
{
	struct Node * tree = NULL;
	char str[100] = {0};
	FILE * f_out = NULL;
	FILE * f_in = NULL;
	int lab = 0;
	int * lab_count = calloc(1, sizeof(int));
	f_out = fopen("Tree.dot", "w");
	f_in = fopen("INPUT", "r");
	assert(f_out);
	assert(f_in);
	arr_fill(str, f_in);
	tree = G(str);
	Dot_print(tree, f_out, lab, lab_count);
	system("dot -Tpng Tree.dot -o Tree.png");
	return 0;
}

struct Node * G(char * str)
{
	s = str;
	struct Node * root = calloc(1, sizeof(struct Node));
	root->array = calloc(10, sizeof(struct Node *));
	root->val = calloc(2, sizeof(char));
	root->val[0] = 'G';
	root->val[1] = '\0';
	int counter = 0;
	for(; counter < 10; counter++)
	{
		if((s[p] == 'i' && s[p+1] == 'n' && s[p+2] == ' ') || (s[p] == 'p' && s[p+1] == 'u' && s[p+2] == 's' && s[p+3] == 'h' && s[p+4] == ' ') || (s[p] == 'p' && s[p+1] == 'o' && s[p+2] == 'p' && s[p+3] == ' ') || (s[p] == 'o' && s[p+1] == 'u' && s[p+2] == 't' && s[p+3] == ' '))
			root->array[counter] = GetF_R();
		else if((s[p] == 'm' && s[p+1] == 'u' && s[p+2] == 'l' && s[p+3] == ';') || (s[p] == 'a' && s[p+1] == 'd' && s[p+2] == 'd'))
		{
			root->array[counter] = GetF();
		} 
		else
		{
			printf("%d:Error: unknown type name:\n%s \n", semicolon_counter, arr_out(s + p));
			exit(100);
		}
		if(s[p] == ';')
		{
			semicolon_counter++;
			p++;
			if(s[p] == '\n')
			{
				p++;
			}
			else if(s[p] == '$')
				break;
		}
		else
		{
			printf("%d:Expected '\n' or '$'\n", semicolon_counter);
			exit(103);
		}
	}
	return root;
}

struct Node * GetF_R()
{
	struct Node * root = calloc(1, sizeof(struct Node));
	root->val = calloc(10, sizeof(char));
	root->array = calloc(1, sizeof(struct Node *));
	root->array[0] = calloc(1, sizeof(struct Node));
	root->array[0]->val = calloc(5, sizeof(char));
	int counter = 0;
	for(; s[p] != ' '; counter++)
	{
		root->val[counter] = s[p];
		p++;
	}
	root->val[counter] = '\0';
	counter = 0;
	assert(s[p] == ' ');
	p++;
	root->array[0] = GetR();
	if(s[p] == '\n')
	{
		printf("%d:Expected ';' \n", semicolon_counter);
		exit(101);
	}
	if(s[p] != ';')
	{
		printf("%d:The function must be used with a register\n", semicolon_counter);
		exit(102);
	}
	return root;
}

struct Node * GetF()
{
	struct Node * root = calloc(1, sizeof(struct Node));
	root->val = calloc(10, sizeof(char));
	int counter = 0;
	for(; s[p] != ';'; counter++)
	{
		root->val[counter] = s[p];
		p++;
	}
	root->val[counter] = '\0';
	return root;
}

struct Node * GetE()
{
	struct Node * root_left = NULL;
	 //root_left = GetT();
	 if(!root_left) assert(0);
	int counter = 0;
	struct Node * root = NULL;
	if( s[p] == '+' || s[p] == '-')
	{
		char operator = s[p];
		p++;
		struct Node * root_right = GetE();
		if(!root_right)
		{
			printf("I need a subtrahend or a second term\n");
			abort();
		}
		if( operator == '+') 
		{
			//root = CreateNode(PLUS, root_left, root_right);
			counter++;
		}
		else if( operator == '-')
		{
			//root = CreateNode(MINUS, root_left, root_right);
			counter++;
		}
		assert(counter);
	}
	else
	{
		root = root_left;
	}
	return root;
}

struct Node * GetR()
{
	int counter = 0;
	struct Node * root = calloc(1, sizeof(struct Node));
	root->val = calloc(3, sizeof(char));
	if(s[p] != 'a' && s[p] != 'b' && s[p] != 'c' && s[p] != 'd')
	{
		printf("%d: There is wrong such register: %s", semicolon_counter, arr_out(s+p));
		exit(104);
	}
	for(; counter < 2; counter++)
	{
		root->val[counter] = s[p];
		p++;
	}
	if(s[p - 1]!= 'x')
	{
		printf("%d: There is wrong such register: %s", semicolon_counter, arr_out(s+p-1));
		exit(105);
	}
	return root;
}

void Dot_print(struct Node * root, FILE * stream, int lab, int * lab_count)
{
	digraph(stream, lab_count);
	tree_png(root, stream, lab, lab_count);
	digraph(stream, lab_count);
}

void tree_png(struct Node * root, FILE * stream, int lab, int * lab_count)
{
	assert(stream);
	int counter = 0;
	if(root) 
	{
		fprintf(stream, "n%03d ", lab);
		fprintf(stream, "[label=\"%s\"] ;\n", root->val);
		assert(lab < 990 && (*lab_count) < 990);
		(*lab_count)++;
		if(root->array)
		{
			for( ; root->array[counter] != NULL; counter++)
			{
				assert(root->array[counter]);
				fprintf(stream, "n%03d -> n%03d;\n", lab, (*lab_count));
				tree_png(root->array[counter], stream, (*lab_count), lab_count);
			}
		}
	}
}

void digraph(FILE * stream, int * lab_count)
{
	assert(lab_count);
	if(!(*lab_count)) {
		fprintf(stream, "digraph G{\n");
		fprintf(stream, "node [style=filled, color=darkolivegreen1];\n");
	}
	else {
		fprintf(stream,"}\n");
		(*lab_count) = 0;
	}
}

void arr_fill(char * array, FILE * stream)
{
	assert(array);
	assert(stream);
	int counter = 0;
	int c = 0;
	for(; ; counter++)
	{
		c = fgetc(stream);
		if(c == EOF) break;
		array[counter] = c;
	}
}

char * arr_out(char * array)
{
	int counter = 0;
	for(; array[counter] != '\n'; counter ++)
		;
	array[counter] = '\0';
	return array;
}
