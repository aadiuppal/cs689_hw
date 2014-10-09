/* 
 * Demonstrating binary trees
 * code by Daniel A. Jimenez
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* trees will store characters */

typedef char node_data_type;

struct binary_tree_node {
	node_data_type X;
	struct binary_tree_node *llink, *rlink;
};

/* stacks will store pointers to tree nodes */

typedef struct binary_tree_node *stack_data_type;

struct stack_node {
	stack_data_type X;
	struct stack_node *next;
};

/* get new stack node */

struct stack_node *new_stack_node (void) {
	return malloc (sizeof (struct stack_node));
}

/* push onto the stack */

void stack_push (struct stack_node **L, stack_data_type X) {
	struct stack_node *p = new_stack_node ();
	p->next = *L;
	p->X = X;
	*L = p;
} 

/* pop the stack */

stack_data_type stack_pop (struct stack_node **L) {
	if (*L == NULL) {
		printf ("underflow!\n");
		exit (1);
	} else {
		stack_data_type X = (*L)->X;
		struct stack_node *p = (*L)->next;
		free (*L);
		*L = p;
		return X;
	}
}

/* allocate a new binary tree node */

struct binary_tree_node *new_binary_tree_node (node_data_type X) {
	struct binary_tree_node *p = malloc (sizeof (struct binary_tree_node));
	p->X = X;
	p->llink = NULL;
	p->rlink = NULL;
	return p;
}

/* recursive inorder traversal; inserts extra parentheses for fun */

void traverse (struct binary_tree_node *r) {
	if (r) {
		if (r->X == '+' || r->X == '-') putchar ('(');
		traverse (r->llink);
		putchar (r->X);
		traverse (r->rlink);
		if (r->X == '+' || r->X == '-') putchar (')');
	}
}

void F (struct binary_tree_node *T) {

	/* A is an empty stack */

	struct stack_node *A = NULL;

	/* P is the node currently under consideration, starting at the root */

	struct binary_tree_node *P = T;

	do {

		/* go left as far as possible, pushing nodes we find onto the stack */

		while (P) {
			stack_push (&A, P);
			P = P->llink;
		}

		/* pop and print from the top of the stack */

		P = stack_pop (&A);
		putchar (P->X);

		/* go right here and start the process over */

		P = P->rlink;
	} while (A || P);
	putchar ('\n');
}

/* let's put 

(a+b)/(c-(d*e))

into a syntax tree that looks like this:

                            div
                           /   \
                          +     -
                         / \   / \
                        5   4 3   *
                                 / \
                                2   1
*/

struct binary_tree_node *make_interesting_syntax_tree (void) {
	struct binary_tree_node *mul = new_binary_tree_node ('*');
	mul->llink = new_binary_tree_node ('2');
	mul->rlink = new_binary_tree_node ('1');
	struct binary_tree_node *minus = new_binary_tree_node ('-');
	minus->llink = new_binary_tree_node ('3');
	minus->rlink = mul;
	struct binary_tree_node *div = new_binary_tree_node ('/');
	div->rlink = minus;
	struct binary_tree_node *plus = new_binary_tree_node ('+');
	plus->llink = new_binary_tree_node ('5');
	plus->rlink = new_binary_tree_node ('4');
	div->llink = plus;
	return div;
}

int eval (struct binary_tree_node *T) {
	if (!T) {
		fprintf (stderr, "there has been some kind of error.\n");
		exit (1);
	}
	if (T->X >= '0' && T->X <= '9') return (int) T->X - '0';
	switch (T->X) {
		case '-': return eval (T->llink) - eval (T->rlink);
		case '+': return eval (T->llink) + eval (T->rlink);
		case '*': return eval (T->llink) * eval (T->rlink);
		case '/': return eval (T->llink) / eval (T->rlink);
		default: return eval (NULL);
	}
}





struct binary_tree_node *buildtree(char infix[],int start, int end){
        int i= start;
        int j= end;
        int len =end;
        struct binary_tree_node *node[30];      //Declare node
        struct stack_node *A = NULL;

        j--;
        node[j] = new_binary_tree_node(infix[j]);
        node[j]->rlink = new_binary_tree_node(infix[j+1]);


        while(j>=0){

        j--;
        node[j]=node[j+2]->llink=new_binary_tree_node(infix[j]);
        node[j]->rlink = new_binary_tree_node(infix[j+1]);
        j--;
        }
        return node[len-1];
}






int main () {

	/* make a syntax tree containing an expression */
	char str[30];
	struct binary_tree_node *root;
	int i=0;
	int len;
	
//	scanf("%s",&str);
	gets(str);
	len=strlen(str);
	root = buildtree(str,0,len);
	

//	struct binary_tree_node *T = make_interesting_syntax_tree ();

	/* print the tree in inorder using Knuth's algorithm */

	
	putchar ('\n');

	/* print the tree in inorder with the recursive algorithm */

	traverse (root);
	putchar ('\n');

	/* evaluate the tree */

	printf ("eval = %d\n", eval (root));
	return 0;
}


