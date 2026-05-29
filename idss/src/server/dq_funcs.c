#include "tree.h"
#include "parser_result.h"
#include "dq_funcs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "db.h"
#include "utils.h"
#include "p2p_comm.h"


static int getQueries(SNode *node, char **subqs, int *index)
{
	int temp = 0;

	if (node != NULL)
	{
		if ((node->m_type == eSUBQ) || (node->m_type == eSUBQNOAGG))
		{
			*(subqs + *index) = strdup(node->m_query);
			if (!(*(subqs + *index)))
				return DQF_NO_MEMORY;
			(*index)++;
		}
		else if ((node->m_type == eAND) || (node->m_type == eOR))
		{
			if (node->m_left != NULL)
				temp = getQueries(node->m_left, subqs, index);
			if (temp != DQF_OK)
				return temp;
			if (node->m_right != NULL)
				temp = getQueries(node->m_right, subqs, index);
			if (temp != DQF_OK)
				return temp;
		}
	}
	return DQF_OK;
}


int getAllQueries(parser_result e, char **subqs)
{
	int index = 0;
	if (e.parentq != NULL)
		{
		 *(subqs + index) = strdup(e.parentq);
		 if (!(*(subqs + index)))
			return DQF_NO_MEMORY;
		 index++;
		}

	if (e.root != NULL)
	{
		return getQueries(e.root, subqs, &index);
	}

	return DQF_OK;
}


int freeResult(parser_result e)
{
	if (e.root)
	{
		deleteNode(e.root);
		e.root = NULL;
	}
	if (e.parentq)
	{
		free(e.parentq);
		e.parentq = NULL;
	}
	return DQF_OK;
}


static int insertResult(SNode *node, double *values, int *index)
{
	if (node != NULL)
	{
		if (node->m_type == eSUBQ)
		{
			node->m_qres = *(values + *index);
			(*index)++;
		}
		else if ((node->m_type == eAND) || (node->m_type == eOR))
		{
			if (node->m_left != NULL)
				insertResult(node->m_left, values, index);
			if (node->m_right != NULL)
				insertResult(node->m_right, values, index);
		}
	}
	return DQF_OK;
}


int insertQueryResults(parser_result e, double *values)
{
	int index = 0;
	if (e.root != NULL)
	{
		insertResult(e.root, values, &index);
	}
	return DQF_OK;
}


static int evaluation(DB *dbp, SNode *node, int *result)
{
	int temp1 = 0;
	int temp2 = 0;
	int temp = DQF_OK;
	DBT key, data;
	int ret = 0;
	char errbuf[1024] = {0};
	double tempd = 0;
	char *tempc = NULL;

	*result = 0;

	if (node != NULL)
	{
		switch(node->m_type)
		{
			case eSUBQ:
				memset(&key, 0, sizeof(key));
	  			memset(&data, 0, sizeof(data));
				key.data = (char*)node->m_fname;
	  			key.size = strlen((char*)node->m_fname) + 1;
				ret = dbp->get(dbp, NULL, &key, &data, 0);
				if (ret == DB_NOTFOUND)
				{
					if (get_debug_flg() >= 1)
					{
						snprintf(errbuf, BUF_SIZE, "Error! Field %s not found! Function: evaluation", (char*)node->m_fname);
						idss_errmsg_r(errbuf);
					}
					temp =DQF_ERROR;
					break;
				}
				tempd = strtod((char*)(data.data), (char **)NULL);
				//Evaluation
				switch(node->m_oper)
				{
					case 0:	//	= ==
						if (tempd == node->m_qres)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 1: //	>=
						if (tempd >= node->m_qres)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 2: //	<=
						if (tempd <= node->m_qres)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 3: //	>
						if (tempd > node->m_qres)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 4: //	<
						if (tempd < node->m_qres)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 5: //	!=
						if (tempd != node->m_qres)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					default:
						temp = DQF_ERROR;
						break;
				}
				break;
				
			case eCOND_STR:
				memset(&key, 0, sizeof(key));
	  			memset(&data, 0, sizeof(data));
				key.data = (char*)node->m_fname;
	  			key.size = strlen((char*)node->m_fname) + 1;
				ret = dbp->get(dbp, NULL, &key, &data, 0);
				if (ret == DB_NOTFOUND)
				{
					if (get_debug_flg() >= 1)
					{
						snprintf(errbuf, BUF_SIZE, "Error! Field %s not found! Function: evaluation", (char*)node->m_fname);
						idss_errmsg_r(errbuf);
					}
					temp = DQF_ERROR;
					break;
				}
				tempc = (char *)calloc(strlen((char *)(data.data)) + 5, sizeof(char));
				if (tempc == NULL)
				{
					if (get_debug_flg() >= 1)
						idss_errmsg_r("Not enough memory!");

					temp  = DQF_NO_MEMORY;
					break;
				}
				strcat(tempc, "\"");
				strcat(tempc, (char *)(data.data));
				strcat(tempc, "\"");
				switch(node->m_oper)
				{
					case 0:	//	= ==
						if (!strcmp(tempc, node->m_string))
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 5:	//	!=
						if (strcmp(tempc, node->m_string))
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					default:
						temp = DQF_ERROR;
						break;
				}
				break;

			case eCOND_BOOL:
				memset(&key, 0, sizeof(key));
	  			memset(&data, 0, sizeof(data));
				key.data = (char*)node->m_fname;
	  			key.size = strlen((char*)node->m_fname) + 1;
				ret = dbp->get(dbp, NULL, &key, &data, 0);
				if (ret == DB_NOTFOUND)
				{
					if (get_debug_flg() >= 1)
					{
						snprintf(errbuf, BUF_SIZE, "Error! Field %s not found! Function: evaluation", (char*)node->m_fname);
						idss_errmsg_r(errbuf);
					}
					temp =DQF_ERROR;
					break;
				}
				switch(node->m_oper)
				{
					case 0:	//	= ==
						if (node->m_num == 1)
						{	//We are looking for TRUE
							if ((!strcmp((char*)(data.data), "true")) || (!strcmp((char*)(data.data), "TRUE"))
								|| (!strcmp((char*)(data.data), "1")))
							{
								*result = 1;
							}
							else
							{
								*result = 0;
							}
						}
						else
						{	//We are looking for FALSE
							if ((!strcmp((char*)(data.data), "false")) || (!strcmp((char*)(data.data), "FALSE"))
								|| (!strcmp((char*)(data.data), "0")))
							{
								*result = 1;
							}
							else
							{
								*result = 0;
							}
						}
						break;
					case 5:	//	!=
						if (node->m_num == 1)
						{	//We are looking for  != TRUE, or == FALSE
							if ((!strcmp((char*)(data.data), "false")) || (!strcmp((char*)(data.data), "FALSE"))
								|| (!strcmp((char*)(data.data), "0")))
							{
								*result = 1;
							}
							else
							{
								*result = 0;
							}
						}
						else
						{	//We are looking != FALSE, or == TRUE
							if ((!strcmp((char*)(data.data), "true")) || (!strcmp((char*)(data.data), "TRUE"))
								|| (!strcmp((char*)(data.data), "1")))
							{
								*result = 1;
							}
							else
							{
								*result = 0;
							}
						}
						break;
					default:
						temp = DQF_ERROR;
						break;
				}
				break;

			case eCOND_NUM:
				memset(&key, 0, sizeof(key));
	  			memset(&data, 0, sizeof(data));
				key.data = (char*)node->m_fname;
	  			key.size = strlen((char*)node->m_fname) + 1;
				ret = dbp->get(dbp, NULL, &key, &data, 0);
				if (ret == DB_NOTFOUND)
				{
					if (get_debug_flg() >= 1)
					{
						snprintf(errbuf, BUF_SIZE, "Error! Field %s not found! Function: evaluation", (char*)node->m_fname);
						idss_errmsg_r(errbuf);
					}
					temp =DQF_ERROR;
					break;
				}
				tempd = strtod((char*)(data.data), (char **)NULL);
				//Evaluation
				switch(node->m_oper)
				{
					case 0:	//	= ==
						if (tempd == node->m_num)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 1: //	>=
						if (tempd >= node->m_num)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 2: //	<=
						if (tempd <= node->m_num)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 3: //	>
						if (tempd > node->m_num)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 4: //	<
						if (tempd < node->m_num)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					case 5: //	!=
						if (tempd != node->m_num)
						{
							*result = 1;
						}
						else
						{
							*result = 0;
						}
						break;
					default:
						temp = DQF_ERROR;
						break;
				}
				break;

			case eAND:
				temp = evaluation(dbp, node->m_left, &temp1);
				if (temp != DQF_OK)
					break;
				
				temp = evaluation(dbp, node->m_right, &temp2);
				if (temp != DQF_OK)
					break;

				*result = ((temp1 > 0) && (temp2 > 0));
				break;
			case eOR:
				temp = evaluation(dbp, node->m_left, &temp1);
				if (temp != DQF_OK)
					break;
				
				temp = evaluation(dbp, node->m_right, &temp2);
				if (temp != DQF_OK)
					break;

				*result = ((temp1 > 0) || (temp2 > 0));
				break;
			default:
                        	// shouldn't be here
				temp = DQF_ERROR;
				break;
		}
	}
	if (tempc)
	{
		free (tempc);
		tempc = NULL;
	}
	return temp;
}


int evaluate(DB *dbp, parser_result e, int *result)
{
	*result = 0;
	int temp = DQF_OK;

	if (e.root != NULL)
		temp = evaluation(dbp, e.root, result); 
	
	return temp;
}


static int getOField(SNode *node, char ***fields, int *index)
{
	int temp = DQF_OK;

	if (node != NULL)
	{
		switch(node->m_type)
		{
			case eCOND_STR:
			case eCOND_BOOL:
			case eCOND_NUM:
			case eSUBQNOAGG:
			case eSUBQ:
				if (node->m_fname)
					{
					 *(*fields + *index) = strdup(node->m_fname);
					 if (!(*(*fields + *index)))
						temp = DQF_NO_MEMORY;
					 (*index)++;
					}
				break;
			case eAND:
			case eOR:
				temp = getOField(node->m_left, fields, index);
				if (temp != DQF_OK)
					break;
				temp = getOField(node->m_right, fields, index);
				if (temp != DQF_OK)
					break;
				break;
			default:
				temp = DQF_ERROR;
				break;
		}
	}

	return temp;
}


int getOtherFields(parser_result *e, char ***fields)
{
	int index = 0;
	if (e->root != NULL)
	{
		return getOField(e->root, fields, &index);
	}
	return DQF_OK;
}