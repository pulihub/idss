
#ifndef __TREE_H__
#define __TREE_H__
 
 
/**
 * The node type
 */
typedef enum tagENodeType
{
    eSUBQ,
    eSUBQNOAGG,
    eCOND_STR,
    eCOND_BOOL,
    eCOND_NUM,
    eAND,
    eOR
}ENodeType;
 
/**
 * The node structure
 */
typedef struct tagSNode
{
        ENodeType m_type;///< type of node
	char *m_fname;
	int m_oper;///0: 1: 2: 3: 4: 5: 6: 7:
	double m_qres;///< valid only when m_type is eSUBQ	(subquery result)
	char *m_query;///< valid only when m_type is eSUBQ or eSUBQNOAGG
	double m_num;///< valid only when m_type is eCOND_XXX
	char *m_string;///< valid only when m_type is eCOND_XXX
        struct tagSNode* m_left; ///< left side of the tree
        struct tagSNode* m_right;///< right side of the tree
}SNode;
 

//It creates a Leaf
SNode* createLeaf(ENodeType type, char *fieldname, char *operat, char *subquery, double numv, char *string);
 
//It creates an operation
SNode* createOperation(
                        ENodeType type,
                        SNode *left,
                        SNode *right);
 
//Delete a node
void deleteNode(SNode *b);
 
#endif // __TREE_H__
 
