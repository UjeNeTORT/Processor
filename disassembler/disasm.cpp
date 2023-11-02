#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../enums.h"
#include "../processor/spu.h"

#define FPRINTF_POP(stream, id, cmd, imm_arg, reg_id, name, symbs)  \
    fprintf(stream, "(%lu) %d %n", id, cmd, &symbs);                \
    if (cmd & ARG_IMMED_VAL)                                        \
    {                                                               \
        fprintf(stream, "%d ", imm_arg);                            \
    }                                                               \
    if (cmd & ARG_REGTR_VAL)                                        \
    {                                                               \
        fprintf(stream, "%d ", reg_id);                             \
    }                                                               \
    symbs = LISTING_CODE_TEXT_DISTANCE - symbs;                     \
    for (int i = 0; i < symbs; i++)                                 \
        fprintf(stream, " ");                                       \
                                                                    \
    fprintf(stream, "%s ", name);                                   \
    if (cmd & ARG_MEMRY_VAL)                                        \
    {                                                               \
        fprintf(stream, "[");                                       \
    }                                                               \
    if (cmd & ARG_REGTR_VAL)                                        \
    {                                                               \
        fprintf(stream, "r%cx", 'a' + reg_id);                      \
        if (cmd & ARG_IMMED_VAL) {                                  \
            fprintf(stream, " + ");                                 \
        }                                                           \
    }                                                               \
    if (cmd & ARG_IMMED_VAL)                                        \
    {                                                               \
        fprintf(stream, "%d", imm_arg);                             \
    }                                                               \
    if (cmd & ARG_MEMRY_VAL)                                        \
    {                                                               \
        fprintf(stream, "]");                                       \
    }                                                               \
    fprintf(stream, "\n");

const int    LISTING_CODE_TEXT_DISTANCE = 20;
const char * DISASM_FILENAME            = "disasmed.txt";

static size_t ReadByteCode (const char * in_fname, cmd_code_t ** prog_code);
static int    DisAssemble  (const cmd_code_t * prog_code, size_t n_bytes, const char * out_fname);

int fprintf_listing_jmp     (FILE * stream, const cmd_code_t * prog_code, int ip, const char * name);
int fprintf_listing_no_arg  (FILE * stream, const cmd_code_t * prog_code, int ip, const char * name);
int fprintf_push            (FILE * stream, const cmd_code_t * prog_code, int ip, const char * name);
int fprintf_pop             (FILE * stream, const cmd_code_t * prog_code, size_t ip, const char * name);

int           CalcIpOffset (cmd_code_t cmd);

int main()
{
    fprintf(stdout, "\n"
                    "# Disassembler by NeTort\n"
                    "# (c) TIKHONOV YAROSLAV 2023\n\n");

    cmd_code_t * prog_code = NULL;
    size_t n_bytes = ReadByteCode(BIN_FILENAME, &prog_code);

    DisAssemble(prog_code, n_bytes, DISASM_FILENAME);

    free(prog_code);

    return 0;
}

size_t ReadByteCode (const char * in_fname, cmd_code_t ** prog_code)
{
    assert(in_fname);
    assert(prog_code);

    FILE * in_file = fopen(in_fname, "rb");

    // read size of the long long byte code array
    size_t n_bytes = 0;
    fread(&n_bytes, sizeof(size_t), 1, in_file);

    // read byte code array: form and fill prog_code array
    *prog_code = (cmd_code_t *) calloc(n_bytes, sizeof(cmd_code_t));
    assert(*prog_code);

    size_t readen = 0;
    readen = fread(*prog_code, sizeof(cmd_code_t), n_bytes, in_file);
    assert(readen == n_bytes);

    fclose(in_file);

    return n_bytes;
}

int DisAssemble (const cmd_code_t * prog_code, size_t n_bytes, const char * out_fname)
{
    assert (prog_code);

    FILE * fout = fopen(out_fname, "wb");

    int val    = 0;
    int reg_id = 0;
    size_t ip  = 0;
    int symbs  = 0;

    cmd_code_t cmd = 0;

    while (ip < n_bytes)
    {
        cmd = prog_code[ip];

        switch (cmd & OPCODE_MSK)
        {
            case CMD_HLT:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "hlt");

                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_PUSH:
            {

                fprintf_push(fout, prog_code, ip, "push");

                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_POP:
            {
                fprintf_pop(fout, prog_code, ip, "pop");

                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_IN:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "in");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_OUT:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "out");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_FRAME:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "frame");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_ADD:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "add");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_SUB:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "sub");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_MUL:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "mul");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_DIV:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "div");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_SQRT:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "sqrt");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_SQR:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "sqr");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_MOD:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "mod");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_IDIV:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "idiv");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_CALL:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "call");

                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_RET:
            {
                fprintf_listing_no_arg(fout, prog_code, ip, "ret");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_JMP:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "jmp");
                ip += CalcIpOffset(cmd);

                break;
            }
            // IMM
            case CMD_JA:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "ja");
                ip += CalcIpOffset(cmd);

                break;
            }
            case CMD_JAE:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "jae");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_JB:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "jb");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_JBE:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "jbe");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_JE:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "je");
                ip += CalcIpOffset(cmd);

                break;
            }

            case CMD_JNE:
            {
                fprintf_listing_jmp(fout, prog_code, ip, "jne");
                ip += CalcIpOffset(cmd);

                break;
            }

            default:
            {
                fprintf(stderr, "# Syntax Error! No command \"%d\" (%lu) found! Bye bye looser!\n", cmd, ip);

                return 1;
            }
        }
        val  = 0;
    }

    fclose(fout);

    return 0;
}

int CalcIpOffset (cmd_code_t cmd)
{
    int offset = sizeof(cmd_code_t);

    if (cmd & ARG_IMMED_VAL)
        offset += sizeof(Elem_t);

    if (cmd & ARG_REGTR_VAL)
        offset += sizeof(cmd_code_t);

    return offset;
}

int fprintf_listing_jmp (FILE * stream, const cmd_code_t * prog_code, int ip, const char * name)
{
    int symbs = 0;
    cmd_code_t cmd = *(cmd_code_t *)(prog_code + ip);
    int target = *(int *)(prog_code + ip + 1);

    fprintf(stream, "(%lu) %d %d %n", ip, cmd, target, &symbs);
    symbs = LISTING_CODE_TEXT_DISTANCE - symbs;

    for (int i = 0; i < symbs; i++)
        fprintf(stream, " ");

    fprintf(stream, "%s %d\n", name, target);

    return 0;
}

int fprintf_listing_no_arg (FILE * stream, const cmd_code_t * prog_code, int ip, const char * name)
{
    int symbs = 0;

    cmd_code_t cmd = *(cmd_code_t *)(prog_code + ip);

    fprintf(stream, "(%lu) %d %n", ip, cmd, &symbs);
    symbs = LISTING_CODE_TEXT_DISTANCE - symbs;

    for (int i = 0; i < symbs; i++)
        fprintf(stream, " ");

    fprintf(stream, "%s\n", name);

    return 0;
}

int fprintf_push (FILE * stream, const cmd_code_t * prog_code, int ip, const char * name)
{
    cmd_code_t cmd = *(cmd_code_t *)(prog_code + ip);

    int symbs = 0;
    int val   = 0;
    char reg_id = 0;

    if (cmd & ARG_IMMED_VAL)
    {
        val = *(int *)(prog_code + ip + 1);
        reg_id = *(char *)(prog_code + ip + 1 + 4);
    }
    else
        reg_id = *(char *)(prog_code + ip + 1);

    fprintf(stream, "(%lu) %d %n", ip, cmd, &symbs);

    if (cmd & ARG_IMMED_VAL)
    {
        fprintf(stream, "%d ", val);
    }
    if (cmd & ARG_REGTR_VAL)
    {
        fprintf(stream, "%d ", reg_id);
    }

    symbs = LISTING_CODE_TEXT_DISTANCE - symbs;
    for (int i = 0; i < symbs; i++)
        fprintf(stream, " ");

    fprintf(stream, "%s ", name);

    if (cmd & ARG_MEMRY_VAL)
    {
        fprintf(stream, "[");
    }
    if (cmd & ARG_REGTR_VAL)
    {
        fprintf(stream, "r%cx", 'a' + reg_id);
        if (cmd & ARG_IMMED_VAL) {
            fprintf(stream, " + ");
        }
    }
    if (cmd & ARG_IMMED_VAL)
    {
        fprintf(stream, "%d", val);
    }
    if (cmd & ARG_MEMRY_VAL)
    {
        fprintf(stream, "]");
    }
    fprintf(stream, "\n");

    return 0;
}

int fprintf_pop (FILE * stream, const cmd_code_t * prog_code, size_t ip, const char * name)
{
    int val = 0;
    char reg_id = 0;
    int symbs = 0;

    cmd_code_t cmd = *(cmd_code_t *)(prog_code + ip);

    if (cmd & ARG_IMMED_VAL)
    {
        val = *(int *)(prog_code + ip + 1);
        reg_id =  *(char *)(prog_code + ip + 1 + 4);
    }
    else
        reg_id = *(char *)(prog_code + ip + 1);


    fprintf(stream, "(%lu) %d %n", ip, cmd, &symbs);
    if (cmd & ARG_IMMED_VAL)
    {
        fprintf(stream, "%d ", val);
    }
    if (cmd & ARG_REGTR_VAL)
    {
        fprintf(stream, "%d ", reg_id);
    }

    symbs = LISTING_CODE_TEXT_DISTANCE - symbs;

    for (int i = 0; i < symbs; i++)
        fprintf(stream, " ");

    fprintf(stream, "%s ", name);
    if (cmd & ARG_MEMRY_VAL)
    {
        fprintf(stream, "[");
    }
    if (cmd & ARG_REGTR_VAL)
    {
        fprintf(stream, "r%cx", 'a' + reg_id);
        if (cmd & ARG_IMMED_VAL) {
            fprintf(stream, " + ");
        }
    }
    if (cmd & ARG_IMMED_VAL)
    {
        fprintf(stream, "%d", val);
    }
    if (cmd & ARG_MEMRY_VAL)
    {
        fprintf(stream, "]");
    }
    fprintf(stream, "\n");

    return 0;
}
