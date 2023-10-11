#ifndef COMPILER_H
#define COMPILER_H

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "commands.h"

const size_t MAX_LINES = 100;
const size_t LINE_SIZE = 100;
const char * const DFLT_CMDS_FILE = "user_commands.txt";

enum ASM_OUT {
    ASM_OUT_NO_ERR = 0,
    ASM_OUT_ERR    = 1
};

enum ASM_OUT AssembleMath        (const char * fin_name, const char * fout_name, const char * cmds_file);
usr_cmd     *ParseCmdNames       (const char * filename, int * n_cmds);
int          ForbiddenCmdCode    (int code);
usr_cmd      CmdCtor             ();
int          GetCmdCode          (const usr_cmd * cmd_arr, const char * cmd_name, int cmd_arr_size);
int          PreprocessProgram   (const char * text_buf, char * buf_ready, char ** text_ready, int n_lines, int buf_size);
int          TranslateProgram    (const char ** text_ready, int n_lines, long long * code_seg);
int          WriteCodeSegmentTxt (const char * fout_name, long long * code_seg, int code_seg_len);
int          WriteCodeSegmentBin (const char * fout_name, long long * code_seg, int code_seg_len);

#endif // COMPILER_H
