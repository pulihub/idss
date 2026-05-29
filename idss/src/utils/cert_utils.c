/** @file cert_utils.c 
 * Utilities to deal with X509v3 digital certificates
 *
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/ui.h>
#include <openssl/safestack.h>
#include "cert_utils.h"


static UI_METHOD *ui_method = NULL;

static int ui_open(UI *ui)
	{
	return UI_method_get_opener(UI_OpenSSL())(ui);
	}
static int ui_read(UI *ui, UI_STRING *uis)
	{
	if (UI_get_input_flags(uis) & UI_INPUT_FLAG_DEFAULT_PWD
		&& UI_get0_user_data(ui))
		{
		switch(UI_get_string_type(uis))
			{
		case UIT_PROMPT:
		case UIT_VERIFY:
			{
			const char *password =
				((PW_CB_DATA *)UI_get0_user_data(ui))->password;
			if (password && password[0] != '\0')
				{
				UI_set_result(ui, uis, password);
				return 1;
				}
			}
		default:
			break;
			}
		}
	return UI_method_get_reader(UI_OpenSSL())(ui, uis);
	}
static int ui_write(UI *ui, UI_STRING *uis)
	{
	if (UI_get_input_flags(uis) & UI_INPUT_FLAG_DEFAULT_PWD
		&& UI_get0_user_data(ui))
		{
		switch(UI_get_string_type(uis))
			{
		case UIT_PROMPT:
		case UIT_VERIFY:
			{
			const char *password =
				((PW_CB_DATA *)UI_get0_user_data(ui))->password;
			if (password && password[0] != '\0')
				return 1;
			}
		default:
			break;
			}
		}
	return UI_method_get_writer(UI_OpenSSL())(ui, uis);
	}
static int ui_close(UI *ui)
	{
	return UI_method_get_closer(UI_OpenSSL())(ui);
	}





int setup_ui_method(void)
	{
	ui_method = UI_create_method("OpenSSL application user interface");
	UI_method_set_opener(ui_method, ui_open);
	UI_method_set_reader(ui_method, ui_read);
	UI_method_set_writer(ui_method, ui_write);
	UI_method_set_closer(ui_method, ui_close);
	return 0;
	}

void destroy_ui_method(void)
	{
	if(ui_method)
		{
		UI_destroy_method(ui_method);
		ui_method = NULL;
		}
	}


X509 *load_cert(BIO *err, const char *file, int format,
	const char *pass, ENGINE *e, const char *cert_descrip)
	{
	ASN1_HEADER *ah=NULL;
	BUF_MEM *buf=NULL;
	X509 *x=NULL;
	BIO *cert;

	if ((cert=BIO_new(BIO_s_file())) == NULL)
		{
		ERR_print_errors(err);
		goto end;
		}

	if (file == NULL)
		{
		setvbuf(stdin, NULL, _IONBF, 0);
		BIO_set_fp(cert,stdin,BIO_NOCLOSE);
		}
	else
		{
		if (BIO_read_filename(cert,file) <= 0)
			{
			BIO_printf(err, "Error opening %s %s\n",
				cert_descrip, file);
			ERR_print_errors(err);
			goto end;
			}
		}

	if 	(format == FORMAT_ASN1)
		x=d2i_X509_bio(cert,NULL);
	else if (format == FORMAT_NETSCAPE)
		{
		unsigned char *p,*op;
		int size=0,i;

		/* We sort of have to do it this way because it is sort of nice
		 * to read the header first and check it, then
		 * try to read the certificate */
		buf=BUF_MEM_new();
		for (;;)
			{
			if ((buf == NULL) || (!BUF_MEM_grow(buf,size+1024*10)))
				goto end;
			i=BIO_read(cert,&(buf->data[size]),1024*10);
			size+=i;
			if (i == 0) break;
			if (i < 0)
				{
				perror("reading certificate");
				goto end;
				}
			}
		p=(unsigned char *)buf->data;
		op=p;

		/* First load the header */
		if ((ah=d2i_ASN1_HEADER(NULL,&p,(long)size)) == NULL)
			goto end;
		if ((ah->header == NULL) || (ah->header->data == NULL) ||
			(strncmp(NETSCAPE_CERT_HDR,(char *)ah->header->data,
			ah->header->length) != 0))
			{
			BIO_printf(err,"Error reading header on certificate\n");
			goto end;
			}
		/* header is ok, so now read the object */
		p=op;
		ah->meth=X509_asn1_meth();
		if ((ah=d2i_ASN1_HEADER(&ah,&p,(long)size)) == NULL)
			goto end;
		x=(X509 *)ah->data;
		ah->data=NULL;
		}
	else if (format == FORMAT_PEM)
		x=PEM_read_bio_X509_AUX(cert,NULL,
			(pem_password_cb *)NULL, NULL);
	else if (format == FORMAT_PKCS12)
		{
		PKCS12 *p12 = d2i_PKCS12_bio(cert, NULL);

		PKCS12_parse(p12, NULL, NULL, &x, NULL);
		PKCS12_free(p12);
		p12 = NULL;
		}
	else	{
		BIO_printf(err,"bad input format specified for %s\n",
			cert_descrip);
		goto end;
		}
end:
	if (x == NULL)
		{
		BIO_printf(err,"unable to load certificate\n");
		ERR_print_errors(err);
		}
	if (ah != NULL) ASN1_HEADER_free(ah);
	if (cert != NULL) BIO_free(cert);
	if (buf != NULL) BUF_MEM_free(buf);
	return(x);
	}

int load_config(BIO *err, CONF *cnf)
	{
	if (!cnf)
		cnf = config;
	if (!cnf)
		return 1;

	OPENSSL_load_builtin_modules();

	if (CONF_modules_load(cnf, NULL, 0) <= 0)
		{
		BIO_printf(err, "Error configuring OpenSSL\n");
		ERR_print_errors(err);
		return 0;
		}
	return 1;
	}
