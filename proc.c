#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define _TREE_(A, B, C, T) tree = calloc(1, sizeof(struct Node));\
tree->val = calloc(4, sizeof(char));\
tree->array = calloc(2, sizeof(struct Node *));\
tree->val[0] = (A);\
tree->val[1] = (B);\
tree->val[2] = (C);\
tree->val[3] = '\0';\
tree->type = (T);\
tree->array[0] = lf;\
tree->array[1] = rg;\
return tree;

char * s;
int p;
int semicolon_counter;
int check_bracket;

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

struct Node * CreateNode(int fella, struct Node * lf, struct Node * rg);
struct Node * Destroy(struct Node * root);

enum Token {
	NUMBER,
	VAR,
	PLUS,
	MINUS,
	MULT,
	DIVIDE,
	POW,
	LOG,
	SIN,
	COS,
	SH,
	CH,
	ATG
};

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
	tree = Destroy(tree);
	fclose(f_in);
	fclose(f_out);
	free(lab_count);
	system("dot -Tpng Tree.dot -o Tree.png");
	return 0;
}

struct Node * G(char * str)
{
	s = str;
	struct Node * root = calloc(1, sizeof(struct Node));
	root->array = calloc(100, sizeof(struct Node *));
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
		else if (s[p] == 'a' || s[p] == 'b' || s[p] == 'c' || s[p] == 'd')
		{
			if(s[p+1] == 'x')
				root->array[counter] = GetR_E();
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

struct Node * GetR_E()
{
	struct Node * root = calloc(1, sizeof(struct Node));
	root->array = calloc(2, sizeof(struct Node *));
	root->array[0] = GetR();
	root->val = calloc(2, sizeof(char));
	if(s[p] != '=')
	{
		printf("%d: There is must be an appropriation:%s\n", semicolon_counter, arr_out(s+p));
		exit(106); 
	}
	p++;
	if(s[p] == ' ')
		p++;
	root->val[0] = '=';
	root->val[1] = '\0';
	root->array[1] = GetE();
	return root;
}

struct Node * GetE()
{
	struct Node * root_left = NULL;
	int counter = 0;
	 root_left = GetT();
	 if(!root_left)
	 {
		 printf("%d: You must have an expression after the appropriation:%s\n", semicolon_counter, arr_out(s+p));
		 exit(107);
	 }
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
			root = CreateNode(PLUS, root_left, root_right);
			counter++;
		}
		else if( operator == '-')
		{
			root = CreateNode(MINUS, root_left, root_right);
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

struct Node * GetT()
{
	struct Node * root_left = NULL;
	root_left = GetP();
	if(!root_left) assert(0);
	int counter = 0;
	struct Node * root = NULL;
	if( s[p] == '*' || s[p] == '/')
	{
		char operator = s[p];
		p++;
		struct Node * root_right = GetP();
		if(!root_right)
		{
			printf("I need a divider or a second factor\n");
			abort();
		}
		if( operator == '*') 
		{
			if(!check_bracket)
			{
				printf("Try to use a such kind of expression: '(EXPRESSION*EXPRESSION)'\n");
				abort();
			}
			root = CreateNode(MULT, root_left, root_right);
			counter++;
		}
		else if( operator == '/')
		{
			if(!check_bracket)
			{
				printf("Try to use a such kind of expression: '(EXPRESSION/EXPRESSION)'\n");
				abort();
			}
			root = CreateNode(DIVIDE, root_left, root_right);
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

struct Node * GetP()
{
	if(s[p] == '(')
	{
		p++;
		check_bracket++;
		struct Node * root = GetE();
		if(s[p] != ')')
		{
			printf("Each opening bracket must have an ending bracket\n");
			abort();
		}
		check_bracket--;
		p++;
		return root;
	}
	else if (s[p] == 'a' || s[p] == 'b' || s[p] == 'c' || s[p] == 'd')
	{
		if(s[p+1] == 'x')
			return GetR();
	}
	else
		return GetN();
	assert(0);
}

struct Node * GetN()
{
	struct Node * root = calloc(1, sizeof(struct Node));
	root->val = calloc(10, sizeof(struct Node));
	root->type = NUMBER;
	int counter = 0;
	while('0' <= s[p] && s[p] <= '9')
	{
		if(counter == 8)
		{
			printf("You have too big number in initial function\n");
			abort();
		}
		root->val[counter] = s[p];
		p++;
		counter++;
	}
	root->val[++counter] = '\0';
	if(counter == 1) 
	{
		printf("%d:There is not a number in GetN():%s\n", semicolon_counter, arr_out(s+p));
		abort();
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

struct Node * CreateNode(int fella, struct Node * lf, struct Node * rg)
{
	switch(fella)
	{
		struct Node * tree = NULL;
		case NUMBER:
			_TREE_('0', '\0', '\0', NUMBER)
		case VAR:
			_TREE_('1', '\0', '\0', NUMBER)
		case PLUS:
			_TREE_('+', '\0', '\0', PLUS)
		case MINUS:
			_TREE_('-', '\0', '\0', MINUS)
		case MULT:
			_TREE_('*', '\0', '\0', MULT)
		case DIVIDE:
			_TREE_('/', '\0', '\0', DIVIDE)
		case POW:
			_TREE_('^', '\0', '\0', POW)
		case LOG:
			_TREE_('l', 'o', 'g', LOG)
		case COS:
			_TREE_('c', 'o', 's', COS)
		case SIN:
			_TREE_('s', 'i', 'n', SIN)
		case CH:
			_TREE_('c', 'h', '\0', CH)
		case SH:
			_TREE_('s', 'h', '\0', SH)
		default: 
			printf("Wrong Type in CreateNode\n");
			exit(12);
	}
	return NULL;
}

struct Node * Destroy(struct Node * root)
{
	if(root)
	{
		if(root->array)
		{
			int counter = 0;
			for(; root->array[counter] != NULL; counter++)
			{
				root->array[counter] = Destroy(root->array[counter]);
			}
			free(root->array);
		}
		if(root->val) 
			free(root->val);
		else
		{
			printf("There is a tree without a root->val");
			exit(27);
		}
		free(root);
	}
	return root;
}
