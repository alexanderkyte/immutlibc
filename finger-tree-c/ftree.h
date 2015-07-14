
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <stdlib.h>
#define allocate(size) malloc(size)

#define Item1 data[0]
#define Item2 data[1]
#define Item3 data[2]
#define Item4 data[3]

typedef enum {
	FingerTreeEmpty,
	FingerTreeSingle,
	FingerTreeDeep
} FingerTreeType;

typedef enum {
	NodeTwo,
	NodeThree
} NodeType;

typedef enum {
	FingerOne = 1,
	FingerTwo = 2,
	FingerThree = 3,
	FingerFour = 4,
	FingerCount,
} FingerType;

#define EMBEDDED_ARRAY 0

typedef struct { 
	FingerType type;
	void *data[EMBEDDED_ARRAY];
 } Finger;

typedef struct EmptyEmbedded {
} EmptyEmbedded;

EmptyEmbedded EmptyTreeSingleton;

typedef struct SingleEmbedded { 
	void *a;
} SingleEmbedded;

typedef struct {
	NodeType type;
	void *data[EMBEDDED_ARRAY];
} Node;

struct FingerTree;

typedef struct DeepEmbedded {
	Finger left;
	struct FingerTree *node;
	Finger right;
} DeepEmbedded;

typedef struct FingerTree { 
	FingerTreeType type;
	void *data[EMBEDDED_ARRAY];
} FingerTree;

#define SingleTree(tree) (*(SingleEmbedded *) &((tree)->data))
#define EmptyTree(tree) (*(EmptyEmbedded *) &((tree)->data))
#define DeepTree(tree) (*(DeepEmbedded *) &((tree)->data))

#define FingerSize(type) (sizeof(Finger) + ((type) - FingerOne + 1) * sizeof(void *))
#define TreeSize(type)


#define NamedConstructor(proper) \
proper * \
make ## proper (proper ## Type type, ...) { \
	proper *item = allocate (sizeof(proper)); \
	item->type = type; \
 \
	va_list ap; \
 \
	va_start (ap, type); \
	for (int i = 0; i < type; i++) { \
		item->data[i] = va_arg (ap, void *); \
	} \
	va_end (ap); \
 \
	return item; \
}

NamedConstructor(FingerTree)

NamedConstructor(Finger)

NamedConstructor(Node)

static void *
bottom (void)
{
	// TODO: IO-less?
	// Annoying to link in
	fprintf (stderr, "Incomplete match. Internal error in immutc.\n");
	exit(-1);

	// Useful for compilers
	return NULL;
}

static void *
raiseEmpty (void)
{
	// TODO: IO-less?
	// Annoying to link in
	fprintf (stderr, "Faulted by accessing empty tree\n");
	exit(-1);

	// Useful for compilers
	return NULL;
}

Finger *
node_to_finger (Node *node)
{
	switch (node->type) {
		case NodeTwo:
			return makeFinger(FingerThree,
			node->data[0],
			node->data[1]);
		case NodeThree:
			return makeFinger(FingerThree,
			node->data[0],
			node->data[1], 
			node->data[2]);
			break;
		default:
			return bottom ();
			bottom ();
	};
}

void *
peekFingerLeft (Finger const *finger)
{
	return finger->data[0];
}

void *
peekFingerRight (Finger const *finger)
{
	return finger->data[finger->type - 1];
}

Finger *
pushFingerLeft (Finger const *finger, void const *a)
{
	switch (finger->type)
	{
		case FingerOne:
			return makeFinger(FingerTwo, a, finger->Item1);
		case FingerTwo:
			return makeFinger(FingerThree, a, finger->Item1, finger->Item2);
			break;
		case FingerThree:
			return makeFinger(FingerFour, a, finger->Item1, finger->Item2, finger->Item2);
		default:
			return bottom ();
	}
}

Finger *
popFingerLeft (Finger const *finger)
{
	switch (finger->type)
	{
		case FingerOne:
			return makeFinger(FingerTwo, finger->Item2);
		case FingerThree:
			return makeFinger(FingerTwo, finger->Item2, finger->Item3);
		case FingerFour:
			return makeFinger(FingerThree, finger->Item2, finger->Item3, finger->Item4);
		default:
			return bottom ();
	}
}

Finger *
pushFingerRight (Finger const *finger, void *b)
{
	switch (finger->type)
	{
		case FingerOne:
			return makeFinger(FingerTwo, finger->Item1, b);
		case FingerTwo:
			return makeFinger(FingerThree, finger->Item1, finger->Item2, b);
		case FingerThree:
			return makeFinger(FingerFour, finger->Item1, finger->Item2, finger->Item2, b);
		default:
			return bottom ();
	}
}

Finger *
popFingerRight (Finger const *finger)
{
	switch (finger->type)
	{
		case FingerTwo:
			return makeFinger(FingerTwo, finger->Item1);
		case FingerThree:
			return makeFinger(FingerTwo, finger->Item1, finger->Item2);
		case FingerFour:
			return makeFinger(FingerThree, finger->Item1, finger->Item2, finger->Item3);
		default:
			return bottom ();
	}
}


void *
peekTreeLeft (FingerTree const *tree)
{
	switch(tree->type) {
		case FingerTreeEmpty:
			return raiseEmpty ();
		case FingerTreeSingle: {
			SingleEmbedded s = SingleTree(tree);
			return s.a;
		}
		case FingerTreeDeep:
			return peekFingerLeft(&DeepTree(tree).left);
	}
}

void *
peekTreeRight (FingerTree const *tree)
{
	switch(tree->type) {
		case FingerTreeEmpty:
			return raiseEmpty ();
		case FingerTreeSingle:
			return SingleTree(tree).a;
		case FingerTreeDeep:
			return peekFingerRight(&DeepTree(tree).right);
	}
}

void *
pushTreeLeft (FingerTree const *tree, void const *a)
{
	switch(tree->type) {
		case FingerTreeEmpty:
			return makeFingerTree(FingerTreeSingle, a);
		case FingerTreeSingle: {
			Finger *left = makeFinger(FingerOne, a);
			Finger *right = makeFinger(FingerOne, SingleTree(tree).a);
			return makeFingerTree(FingerTreeDeep, left, EmptyTreeSingleton, right);
		}
		case FingerTreeDeep: {
			Finger *left = NULL;
			FingerTree *node = NULL;

			const Finger *currLeft = &DeepTree(tree).left;
			const Finger *currRight = &DeepTree(tree).right;

			if (currLeft->type == FingerFour) {
				left = makeFinger (FingerTwo, a, currLeft->Item1);
				Node *rest = makeNode (NodeThree, currLeft->Item2, currLeft->Item3, currLeft->Item4);
				node = pushTreeLeft (DeepTree(tree).node, rest);
			} else {
				left = pushFingerLeft(a, currLeft);
				node = DeepTree(tree).node;
			}

			return makeFingerTree(FingerTreeDeep, left, node, currRight);
		}
	}
}

void *
popTreeLeft (FingerTree const *tree)
{
	switch(tree->type) {
		case FingerTreeEmpty:
			return raiseEmpty ();
		case FingerTreeSingle: {
			return makeFingerTree(FingerTreeEmpty);
		}
		case FingerTreeDeep: {
			Finger *left = NULL;
			Finger *right = NULL;
			FingerTree *node = NULL;

			const Finger *currLeft = &DeepTree(tree).left;
			const Finger *currRight = &DeepTree(tree).right;
			const FingerTree *currNode = DeepTree(tree).node;

			if (currLeft->type == FingerOne) {
				if (currNode->type == FingerTreeEmpty && currRight->type == FingerOne) {
					return makeFingerTree (FingerTreeSingle, currRight->Item1);
				} else if (currNode->type == FingerTreeEmpty) {
					left = makeFinger (FingerOne, peekFingerLeft(currRight));
					right = popFingersLeft (currRight);
					node = currNode;
				} else {
					left = nodeToFinger (peekTreeLeft (currNode));
					node = popTreeLeft (currNode);
					right = currRight;
				}
			} else {
				left = popFingerLeft(currLeft);
				node = currNode;
				right = currRight;
			}

			return makeFingerTree(FingerTreeDeep, left, node, right);
		}
	}
}


void *
pushTreeRight (FingerTree const *tree, void const *a)
{
	switch(tree->type) {
		case FingerTreeEmpty:
			return makeFingerTree(FingerTreeSingle, a);
		case FingerTreeSingle: {
			Finger *right = makeFinger(FingerOne, a);
			Finger *left = makeFinger(FingerOne, SingleTree(tree).a);
			return makeFingerTree(FingerTreeDeep, left, EmptyTreeSingleton, right);
		}
		case FingerTreeDeep: {
			FingerTree *right = NULL;
			FingerTree *node = NULL;

			Finger *currRight = &DeepTree(tree).left;

			if (currRight->type == FingerFour) {
				right = makeFinger (FingerTwo, a, currRight->Item4);
				Node *rest = makeNode (NodeThree, currRight->Item1, currRight->Item2, currRight->Item3);
				node = pushTreeRight (DeepTree(tree).node, rest);
			} else {
				right = pushFingerRight(a, currRight);
				node = DeepTree(tree).node;
			}

			FingerTree *left = &DeepTree(tree).left;

			return makeFingerTree(FingerTreeDeep, left, node, right);
		}
	}
}

void *
popTreeRight (FingerTree const *tree)
{
	switch(tree->type) {
		case FingerTreeEmpty:
			return raiseEmpty ();
		case FingerTreeSingle: {
			return makeFingerTree(FingerTreeEmpty);
		}
		case FingerTreeDeep: {
			Finger *left = NULL;
			Finger *right = NULL;
			FingerTree *node = NULL;

			const Finger *currLeft = &DeepTree(tree).left;
			const Finger *currRight = &DeepTree(tree).right;
			const FingerTree *currNode = &DeepTree(tree).node;

			if (currRight->type == FingerOne) {
				if (currNode->type == FingerTreeEmpty && currLeft->type == FingerOne) {
					return makeFingerTree (FingerTreeSingle, currLeft->Item1);
				} else if (currNode->type == FingerTreeEmpty) {
					left = makeFinger (FingerOne, peekFingerRight(currLeft));
					right = popFingersRight (currLeft);
					node = currNode;
				} else {
					left = nodeToFinger (peekTreeRight (currNode));
					node = popTreeRight (currNode);
					left = currLeft;
				}
			} else {
				left = popFingerRight (currRight);
				node = currNode;
				left = currLeft;
			}

			return makeFingerTree(FingerTreeDeep, left, node, right);
		}
	}
}

// TODO: Get finger tree with count and use count here
#define INITIAL_STACKLET_SIZE 10;

typedef struct {
	FingerTree trees [INITIAL_STACKLET_SIZE];
	void *next;
} Stack;

typedef void *mapper (void *closure, void *current_data);

#define foldTree { \
	Stack curr; \
	curr->next = NULL; \
	curr->tree = tree; \
	Stack *top = curr; \
 \
	Stack stackLet; \
 \
	while (top != NULL) { \
		if (tree->type == FingerTreeEmpty) { \
			continue; \
		} else if(tree->type == FingerTreeSingle) { \
			iter(SingleTree(tree).a); \
			continue; \
		} \
 \
		for (int i = 0; i < DeepTree(next).left.type; i++) \
		{ \
			iter (DeepTree(next).left.data [i]); \
		} \
 \
		next = top->deep.node; \
 \
		Stack *newTop = calloc(sizeof(Stack), 1); \
		newTop->data = top->tree \
		newTop->next = top; \
		top = newTop; \
	} \
 \
 /* INVIARIANT: if it's on the stack I've seen it and$ \
  can assume it's a base case or a deep tree with its left$ i\
  half and nested trees processed$
 */ \
\
	while (top != NULL) { \
		for (int i = 0; i < INITIAL_STACKLET_SIZE && top->deep.right.data [i] != NULL; i++) { \
			FingerTree *curr = top->trees[i]; \
			if (!curr) \
				break; \
			for (int i = 0; i < DeepTree(curr).right.type; i++) \
			{ \
					iter (DeepTree(curr).right.data [i]); \
				} \
			} \
		top = top->next; \
		if (top) { \
			/* Don't free base of stack, which is on our function's call frame */
			free (curr); \
		}\
	} \
}\

void
foldTreeLeft (FingerTree const *tree, mapper iter)
{
#define foldTreeIter(side) \
for (int i = 0; i < top->deep.side.type; i++) \
{ \
	iter (top->deep.side.data [i]); \
} \
foldTree
#undef foldTreeIter
}

void
foldTreeRight (FingerTree const *tree, mapper iter)
{
#define foldTreeIter(side) \
		for (int i = top->deep.side->type - 1; i >= 0; i--) \
		{ \
			iter (top->deep.side.data [i]); \
		} \
foldTree
#undef foldTreeIter
}
