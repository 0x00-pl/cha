/* Stub
   Copyright (c) 2014 Cheryl Natsu 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   */

#ifndef _STUB_H_
#define _STUB_H_

#include <stdio.h>

#include "clsfile.h"
#include "vm.h"
#include "err.h"

typedef struct
{
    /* Startup Pathname */
    char *pathname;
    size_t pathname_len;

    cha_class_file_t *class_file;
} cha_stub_t;

/* Create and destroy stub */
cha_stub_t *cha_stub_new(void);
int cha_stub_destroy(cha_stub_t *stub);

/* Load class file */
int cha_stub_load_class_from_filename( \
        cha_err_t *err, cha_stub_t *stub, \
        const char *filename);

/* Display verbose of class file */
int cha_stub_class_verbose( \
        cha_stub_t *stub);

#endif

