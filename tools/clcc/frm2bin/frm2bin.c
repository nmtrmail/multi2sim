/*
 *  Multi2Sim
 *  Copyright (C) 2012  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdarg.h>
#include <stdio.h>

/*
#include <arch/fermi/asm/asm.h>
#include <lib/mhandle/mhandle.h>
*/

#include <lib/util/debug.h>
#include <lib/util/elf-encode.h>
#include <lib/util/list.h>

#include "inst.h"
#include "inst-info.h"
#include "frm2bin.h"
#include "parser.h"
#include "symbol.h"
/*
#include "task.h"
#include "stream.h"
*/


int frm2bin_assemble;  /* Command-line option set */
char *frm2bin_file_name;  /* Current file */


/* global variables */

/* output text_section buffer */
struct elf_enc_buffer_t *text_section_buffer;


void frm2bin_yyerror(const char *s)
{
	fprintf(stderr, "%s:%d: error: %s\n",
			frm2bin_file_name, frm2bin_yylineno, s);
	exit(1);
}


void frm2bin_yyerror_fmt(char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "%s:%d: error: ",
			frm2bin_file_name, frm2bin_yylineno);
	vfprintf(stderr, fmt, va);
	fprintf(stderr, "\n");
	fflush(NULL);
	exit(1);
}



/*
 * Public Functions
 */

void frm2bin_init(void)
{
	/* Initialize */
	frm_disasm_init();
	frm2bin_inst_info_init();

	/* task list is for label processing,
	 * lable is not supported now
	 * hence, it's commented
	 */
	//frm_task_list_init();
	//frm_symbol_table_init();
	//frm_stream_init();
	//frm2bin_bin_init();

}


void frm2bin_done(void)
{
	/* Finalize */
	/* task list is for label processing,
	 * lable is not supported now
	 * hence, it's commented
	 */
	//frm_task_list_done();
	//frm_symbol_table_done();
	/* This should be changed for fermi */
	//frm_stream_done();
	frm2bin_inst_info_done();
	frm_disasm_done();
}

//int main(int argc, char **argv)
//{
//	int i;
//
//	++argv, --argc;  /* skip over program name */
//
//	if (argc > 0)
//	{
//		yyin = fopen(argv[0], "r");
//	}
//	else
//	{
//		yyin = stdin;
//	}
//	
//	frm2bin_init();
//	
//	/* create text_section buffer, we only produce text_section now */
//	text_section_buffer = elf_enc_buffer_create();
//	
//	yyparse();
//
//	/* debug: see what's in the text_section_buffer */
//	for (i = 0; i < text_section_buffer->size; i+=2)
//	{
//		printf("0x%x  ", *(unsigned short *)(text_section_buffer->ptr + i));
//	}
//	return 0;
//}


void frm2bin_compile(struct list_t *source_file_list,
		struct list_t *bin_file_list)
{
	//int i;
	int index;

	LIST_FOR_EACH(source_file_list, index)
	{
		/* Open file */
		frm2bin_file_name = list_get(source_file_list, index);
		frm2bin_yyin = fopen(frm2bin_file_name, "r");
		if (!frm2bin_yyin)
			fatal("%s: cannot open input file", frm2bin_file_name);
	
		/* create text_section buffer, we only produce text_section now */
		text_section_buffer = elf_enc_buffer_create();
	
		/* Parse input */
		frm2bin_yyparse();


		/* debug: see what's in the text_section_buffer */
		//for (i = 0; i < text_section_buffer->size; i+=2)
		//{
		//	printf("0x%x  ", *(unsigned short *)(text_section_buffer
		//		->ptr + i));
		//}


		/* call the Fermi disassbmler for text_section 
		 * it's just a temporial implementation */
		frm_disasm_text_section_buffer(text_section_buffer);

		/* free the text_section buffer */
		elf_enc_buffer_free(text_section_buffer);


		/* Process pending tasks */

	        /* task list is for label processing,
	         * lable is not supported now
	         * hence, it's commented
	         */
		//frm_task_list_process();

		/* Close */
		fclose(frm2bin_yyin);
	}
}

