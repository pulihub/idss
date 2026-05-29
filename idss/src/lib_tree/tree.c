#include <string.h>
#include <stdlib.h>
#include "tree.h"

static SNode* allocateNode()
{
        SNode* b = (SNode *)malloc( sizeof(SNode) );
 
        if ( NULL != b )
        {
                b->m_string = NULL;
 		b->m_query = NULL;
		b->m_fname = NULL;
                b->m_left = NULL;
                b->m_right = NULL;
        }
 
        return b;
}
 
SNode* createLeaf(ENodeType type, char *fieldname, char *operat, char *subquery, double numv, char *string)
{
        SNode* b = allocateNode();
 
        if (NULL != b)
        {
                b->m_type = type;
		if (strstr(operat, "=="))
			b->m_oper = 0;
		else if (strstr(operat, ">="))
			b->m_oper = 1;
		else if (strstr(operat, "<="))
			b->m_oper = 2;
		else if (strstr(operat, ">"))
			b->m_oper = 3;
		else if (strstr(operat, "<"))
			b->m_oper = 4;
		else if (strstr(operat, "!="))
			b->m_oper = 5;
		else if (strstr(operat, "="))
			b->m_oper = 0;
		else if (strstr(operat, "not in"))
			b->m_oper = 6;
		else if (strstr(operat, "in"))
			b->m_oper = 7;

		b->m_num = numv;
		if (subquery != NULL)
			b->m_query = strdup(subquery);
		if (string != NULL)
			b->m_string = strdup(string);
		if (fieldname != NULL)
			b->m_fname = strdup(fieldname);
        }
 
        return b;
}
 
SNode *createOperation( 
                        ENodeType type,
                        SNode *left,
                        SNode *right)
{
        SNode* b = allocateNode();
 
        if ( NULL != b )
        {
                b->m_type = type;
                b->m_left = left;
                b->m_right = right;
        }
 
        return b;
}
 
void deleteNode(SNode *b)
{
        if (b == NULL)
        {
                return;
        }

	if (b->m_query)
		free (b->m_query);
	if (b->m_fname)
		free (b->m_fname);
	if (b->m_string)
		free (b->m_string);
        deleteNode(b->m_left);
        deleteNode(b->m_right);
 
        free(b);
}
 
