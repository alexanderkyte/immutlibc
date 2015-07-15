
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <stdlib.h>
#define allocate(size) malloc(size)

typedef enum {
	FingerTreeEmptyType,
	FingerTreeSingleType,
	FingerTreeDeepType
} FingerTreeType;

typedef enum {
	NodeTwoType,
	NodeThreeType
} NodeType;

typedef struct {
	NodeType type;
	void *Item1;
	void *Item2;
} NodeTwo;

typedef struct {
	NodeType type;
	void *Item1;
	void *Item2;
	void *Item3;
} NodeThree;

typedef struct {
	NodeType type;
} Node;

typedef enum {
	FingerOneType = 1,
	FingerTwoType = 2,
	FingerThreeType = 3,
	FingerFourType = 4,
	FingerCount,
} FingerType;

#define FingerTree_ARRAY 0

typedef struct { 
	FingerType type;
 } Finger;

typedef struct { 
	FingerType type;
	void *Item1;
 } FingerOne;

typedef struct { 
	FingerType type;
	void *Item1;
	void *Item2;
 } FingerTwo;

typedef struct { 
	FingerType type;
	void *Item1;
	void *Item2;
	void *Item3;
 } FingerThree;

typedef struct { 
	FingerType type;
	void *Item1;
	void *Item2;
	void *Item3;
	void *Item4;
 } FingerFour;

typedef struct EmptyFingerTree {
	FingerTreeType type;
} EmptyFingerTree;

EmptyFingerTree EmptyTreeSingleton;

typedef struct SingleFingerTree { 
	FingerTreeType type;
	void *a;
} SingleFingerTree;

typedef struct FingerTree { 
	FingerTreeType type;
} FingerTree;

typedef struct DeepFingerTree {
	FingerTreeType type;
	Finger left;
	FingerTree *node;
	Finger right;
} DeepFingerTree;


#define SingleTree(tree) (*(SingleFingerTree *) &((tree)->data))
#define EmptyTree(tree) (*(EmptyFingerTree *) &((tree)->data))
#define DeepTree(tree) (*(DeepFingerTree *) &((tree)->data))

#define FingerSize(type) (sizeof(Finger) + ((type) - FingerOne + 1) * sizeof(void *))
#define TreeSize(type)

#define MakeFingerOne(a) ((const Finger *)((FingerOne) {FingerOneType, a}))
#define MakeFingerTwo(a, b) ((const Finger *)((FingerTwo) {FingerTwoType, a, b}))
#define MakeFingerThree(a, b, c) ((const Finger *)((FingerThree) {FingerThreeType, a, b, c}))
#define MakeFingerFour(a, b, c, d) ((const Finger *)((FingerFour) {FingerFourType, a, b, c, d}))

#define MakeNodeTwo(a, b) ((const Node *)((NodeTwo) {NodeTwoType, a, b}))
#define MakeNodeThree(a, b, c) ((const Node *)((NodeThree) {NodeThreeType, a, b, c}))

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

const Finger *
nodeToFinger (Node *node)
{
	switch (node->type) {
		case NodeTwoType: {
			NodeTwo *in = (NodeTwo *)node;
			return MakeFingerTwo (in->Item1, in->Item2);
		}

		case NodeThreeType: {
			NodeThree *in = (NodeThree *)node;
			return MakeFingerThree (in->Item1, in->Item2, in.Item3);
			break;
		}

		default:
			return bottom ();
	};
}

void *
peekFingerLeft (const Finger *finger)
{
	return finger->Item1;
}

void *
peekFingerRight (const Finger *finger)
{
	switch (finger->type)
	{
		case FingerOne:
			return finger->Item1;
		case FingerTwo:
			return finger->Item2;
		case FingerThree:
			return finger->Item3;
		case FingerFour:
			return finger->Item4;
	}
}

const Finger *
pushFingerLeft (const Finger *finger, const void *a)
{
	switch (finger->type)
	{
		case FingerOne:
			return (FingerTwo) {FingerTypeTwo, a, finger->Item1};
		case FingerTwo:
			return (FingerThree) {FingerTypeThree, a, finger->Item1, finger->Item2};
			break;
		case FingerThree:
			return (FingerFour) {FingerTypeFour, a, finger->Item1, finger->Item2, finger->Item2};
		default:
			return bottom ();
	}
}

const Finger *
popFingerLeft (const Finger *finger)
{
	switch (finger->type)
	{
		case FingerTwo:
			return MakeFingerOne(finger->Item2);
		case FingerThree:
			return MakeFingerTwo(finger->Item2, finger->Item3);
		case FingerFour:
			return MakeFingerThree(finger->Item2, finger->Item3, finger->Item4);
		default:
			return bottom ();
	}
}

const Finger *
pushFingerRight (const Finger *finger, const void *b)
{
	switch (finger->type)
	{
		case FingerOne:
			return MakeFingerTwo(finger->Item1, b);
		case FingerTwo:
			return MakeFingerThree(finger->Item1, finger->Item2, b);
		case FingerThree:
			return MakeFingerFour(finger->Item1, finger->Item2, finger->Item2, b);
		default:
			return bottom ();
	}
}

const Finger *
popFingerRight (const Finger *finger)
{
	switch (finger->type)
	{
		case FingerTwo:
			return MakeFingerOne(finger->Item1);
		case FingerThree:
			return MakeFingerTwo(finger->Item1, finger->Item2);
		case FingerFour:
			return MakeFingerThree(finger->Item1, finger->Item2, finger->Item3);
		default:
			return bottom ();
	}
}


void *
peekTreeLeft (const FingerTree *tree)
{
	switch(tree->type) {
		case FingerTreeEmptyType:
			return raiseEmpty ();
		case FingerTreeSingleType:
			return ((SingerFingerTree *)tree)->a;
		case FingerTreeDeepType:
			return peekFingerLeft(&DeepTree(tree).left);
	}
}

void *
peekTreeRight (const FingerTree *tree)
{
	switch(tree->type) {
		case FingerTreeEmptyType:
			return raiseEmpty ();
		case FingerTreeSingleType:
			return SingleTree(tree).a;
		case FingerTreeDeepType:
			return peekFingerRight(&DeepTree(tree).right);
	}
}

void *
pushTreeLeft (const FingerTree *tree, const void *a)
{
	switch(tree->type) {
		case FingerTreeEmptyType:
			return makeFingerTreeSingle(a);

		case FingerTreeSingleType: {
			Finger *left = makeFinger(FingerOne, a);
			Finger *right = makeFinger(FingerOne, SingleTree(tree).a);
			return MakeFingerTreeDeep(left, EmptyTreeSingleton, right);
		}

		case FingerTreeDeepType: {
			const Finger *currLeft = &DeepTree(tree).left;
			const Finger *currRight = &DeepTree(tree).right;

			if (currLeft->type == FingerFour) {
				const Finger *left = makeFinger (FingerTwo, a, currLeft->Item1);
				const Node *rest = makeNode (NodeThreeType, currLeft->Item2, currLeft->Item3, currLeft->Item4);
				const FingerTree *node = pushTreeLeft (DeepTree(tree).node, rest);
				return MakeFingerTreeDeep(left, node, currRight);

			} else {
				const Finger *left = pushFingerLeft(a, currLeft);
				const FingerTree *node = DeepTree(tree).node;
				return MakeFingerTreeDeep(left, node, currRight);
			}
		}
	}
}

void *
popTreeLeft (const FingerTree *tree)
{
	switch(tree->type) {
		case FingerTreeEmptyType:
			return raiseEmpty ();
		case FingerTreeSingleType: {
			return MakeFingerTreeEmpty();
		}
		case FingerTreeDeepType: {
			const Finger *currLeft = &DeepTree(tree).left;
			const Finger *currRight = &DeepTree(tree).right;
			const FingerTree *currNode = DeepTree(tree).node;

			if (currLeft->type == FingerOne) {
				if (currNode->type == FingerTreeEmptyType && currRight->type == FingerOne) {
					return MakeFingerTreeSingle(currRight->Item1);
				} else if (currNode->type == FingerTreeEmptyType) {
					const Finger *left = makeFinger (FingerOne, peekFingerLeft(currRight));
					const Finger *right = popFingerLeft (currRight);
					return MakeFingerTreeDeep(left, currNode, right);
				} else {
					const Finger *left = nodeToFinger (peekTreeLeft (currNode));
					const FingerTree *node = popTreeLeft (currNode);
					return MakeFingerTreeDeep(left, node, currRight);
				}
			} else {
				const Finger *left = popFingerLeft(currLeft);
				return MakeFingerTreeDeepType(left, currNode, currRight);
			}

		}
	}
}


void *
pushTreeRight (const FingerTree *tree, const void *a)
{
	switch(tree->type) {
		case FingerTreeEmptyType:
			return makeFingerTreeSingle(a);
		case FingerTreeSingleType: {
			Finger *right = MakeFingerOne(a);
			Finger *left = MakeFingerOne(SingleTree(tree).a);
			return makeFingerTreeTreeDeep(left, EmptyTreeSingleton, right);
		}
		case FingerTreeDeepType: {
			const Finger *currRight = &DeepTree(tree).left;
			const Finger *left = &DeepTree(tree).left;

			if (currRight->type == FingerFour) {
				const Finger *right = makeFinger (FingerTwo, a, currRight->Item4);
				const Node *rest = makeNode (NodeThreeType, currRight->Item1, currRight->Item2, currRight->Item3);
				const FingerTree *node = pushTreeRight (DeepTree(tree).node, rest);
				return makeFingerTreeTreeDeep(left, node, right);
			} else {
				const Finger *right = pushFingerRight(currRight, a);
				const FingerTree *node = DeepTree(tree).node;
				return makeFingerTreeTreeDeep(left, node, right);
			}
		}
	}
}

void *
popTreeRight (const FingerTree *tree)
{
	switch(tree->type) {
		case FingerTreeEmptyType:
			return raiseEmpty ();
		case FingerTreeSingleType: {
			return makeFingerTree(FingerTreeEmptyType);
		}
		case FingerTreeDeepType: {
			const Finger *currLeft = &DeepTree(tree).left;
			const Finger *currRight = &DeepTree(tree).right;
			FingerTree **currNode = &DeepTree(tree).node;

			if (currRight->type == FingerOne) {

				if ((*currNode)->type == FingerTreeEmptyType && currLeft->type == FingerOne) {
					return makeFingerTree (FingerTreeSingleType, currLeft->Item1);

				} else if ((*currNode)->type == FingerTreeEmptyType) {
					const Finger *left = makeFinger (FingerOne, peekFingerRight(currLeft));
					const Finger *right = popFingerRight (currLeft);
					return makeFingerTreeTreeDeep(left, *currNode, right);

				} else {
					const Finger *left = nodeToFinger (peekTreeRight (*currNode));
					const FingerTree *node = popTreeRight (*currNode);
					return makeFingerTreeTreeDeep(left, node, currRight);
				}

			} else {
				const Finger *right = popFingerRight (currRight);
				return makeFingerTreeTreeDeep(currLeft, *currNode, right);
			}

		}
	}
}

// TODO: Get finger tree with count and use count here
#define INITIAL_STACKLET_SIZE 10

typedef struct {
	FingerTree trees [INITIAL_STACKLET_SIZE];
	void *next;
} Stack;

typedef void *mapper (const void *closure, const void *current_data);

#define foldTree { \
	Stack curr; \
	curr->next = NULL; \
	curr->tree = tree; \
	Stack *top = curr; \
 \
	Stack stackLet; \
 \
	while (top != NULL) { \
		if (tree->type == FingerTreeEmptyType) { \
			continue; \
		} else if(tree->type == FingerTreeSingleType) { \
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
			/* Don't free base of stack, which is on our function's call frame */ \
			free (curr); \
		}\
	} \
}\

void
foldTreeLeft (const FingerTree *tree, mapper iter)
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
foldTreeRight (const FingerTree *tree, mapper iter)
{
#define foldTreeIter(side) \
		for (int i = top->deep.side->type - 1; i >= 0; i--) \
		{ \
			iter (top->deep.side.data [i]); \
		} \
foldTree
#undef foldTreeIter
}
