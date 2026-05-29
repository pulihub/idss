/*
 * This software copyright 2022-2025
 * UNIVERSITY OF SALENTO, ITALY
 *
 *
 *
 */

#ifndef IDSS_CLIENT
#define IDSS_CLIENT

#include <libxml/xmlreader.h>
#include "idssH.h"


/*
 * This function saves the xml document contained in the dimeData struct into
 * the 'name' file
 *
 */
void save_dime_data(char *xml_buf, char *name);

/*
 * Returns a message at the client web services.
 *
 */
char *client_message(int cod_err);

#endif
