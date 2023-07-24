#pragma once

#include "specific.h"
#include "general.h"

// typedef struct lisp lisp;

// typedef int atomtype;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#define MAX_NUMS 10000
#define MAXSIZE 5000
#define SIZE 5000
#define ALPHA 27

typedef struct nuclei
{
    char file[SIZE];
    int start;
    int count;
}nuclei;

typedef struct Var
{
    bool ifset;
    lisp* list;
}Var;

//读文件
// bool read_file(char* argv);

bool read_file(char* argv);
bool judge_parser(nuclei* instruct);
bool prog(nuclei* instruct);
void token_get(char* tmp,char* token,char* rest);
bool split(nuclei* instruct);
bool bracket_judge(nuclei* instruct);
void substring(char* copy);
void subprog(char* copy);
bool get_instruction(char* copy);
bool instract_judge(char* tmp);
void subbraces(char* copy);
bool fun_cmp(char* token,char* rest);
bool set_func(char* tmp);
bool literal_judge(char* tmp);
bool list_judge(char* tmp);
bool print_func(char* tmp);
bool ret_function(char* token,char* rest);
bool list_func(char* tmp);
bool list_func_cons(char* copy);
bool int_func_plus(char* copy);
bool int_func_length(char* copy);
bool bool_func(char* copy);
bool while_func(char* copy);
bool if_func(char* copy);
void split_list(char* copy,char* tmp, char* rest);
bool if_rest_test(char* copy, char* rest1, char* rest2);


//interp
bool judge_interp(nuclei* instruct, Var* arr);
bool prog_interp(nuclei* instruct, Var* arr);
bool split_interp(nuclei* instruct, Var* arr);
bool get_instruction_in(char* copy, Var* arr);
bool instract_judge_in(char* tmp, Var* arr);
bool fun_cmp_in(char* token,char* rest,Var* arr);
bool set_func_in(char* tmp, Var* arr);
bool list_judge_in(char* tmp,const char* index, Var* arr);
void submark(char* token, char* new);
bool ret_function_in(char* token,char* rest,const char* index, Var* arr);
bool list_func_cons_in(char* copy,const char* index, Var* arr);
// lisp* cons_list_atom(char* tmp, Var* l);
bool int_func_length_in(char* copy, const char* index, Var* arr);
bool int_func_plus_in(char* copy, const char* index, Var* arr);
bool bool_func_in(const char* token, const char* copy, const char* index, Var* arr);
bool print_func_in(char* tmp, Var* arr);
void judge_negative(char* cur);
bool bracket_plus(char* copy, char* index, Var* arr);
lisp* get_cons_list(char* tmp,const char* index, lisp* l, Var* arr);
void split_print_cons(char* tmp, char* token, char* rest);
lisp* print_cons(char* tmp,char* index, lisp* l, Var* arr);
// lisp* print_cons_again();
void split_plus(char* copy, char* rest1, char* rest2);
int getplus(char* rest, const char* index, Var* arr);
bool judge_cond(char* condition, Var* arr);
int judge_bracket(const char* copy);
int getlength(char* rest11, Var* arr);
void addbraces(char* copy);
void sub(char* copy);
void free_var(Var* arr);
void testinterp();


//extension
bool int_func_minus_in(char* copy, const char* index, Var* arr);
bool int_func_append_in(char* copy, const char* index, Var* arr);

//test
void testerror();

