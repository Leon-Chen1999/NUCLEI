#include "nuclei.h"
#include "test.c"
#include <assert.h>

#define MAX_NUMS 10000
#define MAXSIZE 5000
#define SIZE 5000
#define SMALLSIZE 100
#define ALPHA 27
#define CHARA 'A'
#define CHARZERO '0'
#define LBRACKET '('
#define RBRACKET ')'
#define NEGITAVEVAL -1
#define MARK '\''
#define MARKDOUBLE '\"'
#define SPACESIN ' '
#define SIX 6
#define FIVE 5
#define ELEVEN 11
#define TEN 10
#define TWO 2
#define THREE 3
#define FOUR 4
#define SPACE " "
#define HASH '#'
#define ARGCTWO 2
#define LBBRACKET "["
#define CHARL 'L'
#define CHARC 'C'
#define ENDFILE '\0'
#define MINUSMARK '-'
#define ENDNULL ""


int main(int argc, char* argv[]){
    if(argc != ARGCTWO){
        fprintf(stderr,"Missing argv!");
        exit(EXIT_FAILURE);
    }
    #ifdef INTERP
    bool interp = read_file(argv[1]);
    if(interp == false){
        exit(EXIT_FAILURE);
        // printf("interp fail!\n");
    }
    #else
    bool parse = read_file(argv[1]);
    if(parse == true){
        printf("Parsed OK\n");
    }else{
        printf("Parsed fail!\n");
    }   
    #endif
    testinterp(); 
    return 0;
}

bool read_file(char* argv){
    FILE* fp = fopen(argv, "r");
    //判断打开是否失败
    if(fp == NULL){
        fprintf(stderr,"Error!\n");
        exit(EXIT_FAILURE);
    }
    int line = 0;
    char Buffer[SIZE];
    nuclei copy;
    char out[SIZE];  //保存文件内容的数组
    out[0] = ENDFILE;
    while(fscanf(fp,"%[^\n]\n",Buffer) != EOF){
        if(Buffer[0]!=HASH){
            strcat(out, Buffer);
            strcat(out, SPACE);
            line++;
        }

    }
    strcpy(copy.file,out);
    #ifdef INTERP
    Var arr[ALPHA] = {{false, NULL}};
    bool judge_interp_type = judge_interp(&copy,arr);
    // free_var(arr);
    fclose(fp);
    return judge_interp_type;
    #else
    bool judge_parse = judge_parser(&copy);
    fclose(fp);
    return judge_parse;
    #endif

}

bool judge_parser(nuclei* instruct){
    bool parser_type = false;
    bool prog_type = false;
    bool bracket_type = false;
    prog_type = prog(instruct);//判断是否符合prog
    bracket_type = bracket_judge(instruct);//bracket?????
    //都符合就为true
    if(prog_type == true && bracket_type == true){
        parser_type = true;
    }
    return parser_type;
}

bool prog(nuclei* instruct){
    if(instruct->file[0] != LBRACKET){
        return false;
    }
    bool split_type = true;
    split_type = split(instruct);
    // printf("3333333333\n");
    return split_type;
}

//only get the token and the rest of the instructions
//rest is a var/ins/list
void token_get(char* tmp,char* token,char* rest){
    int end = 0;
    int index = 0;
    int len = strlen(tmp);
    int mark = 0;
    for(int i = 0;i<len;i++){
        if(tmp[index] == MARKDOUBLE){
            mark++;
        }
    }
    if(tmp[0] != MARK && mark ==0){
        while(end == 0){// while space ,get the end of the token
            if(tmp[index] == SPACESIN || tmp[index] == ENDFILE){ 
                end = index;
            }
            index++;
        }
        for(int i = 0;i<end;i++){
            token[i] = tmp[i];
        }
        token[end] = ENDFILE;
        int start = 0;
        bool space = false;
        for(int i = end;i<len;i++){
            if(tmp[i] == SPACESIN){
                space = true;
            }
            if(space == true){
                rest[start++] = tmp[i+1];
            }
        }
    }else if(tmp[0] != MARK || mark != 0){
        for(int i = 0;i<len;i++){
            token[i] = tmp[i];
        }
        token[len] = ENDFILE;
    }else if(tmp[0] == MARK){
        for(int i = 0;i<len;i++){
            token[i] = tmp[i];
        }
    }
    rest[len] = ENDFILE;
}

bool bracket_judge(nuclei* instruct){
    // printf("11111111111\n");///////////////////
    char copy[SIZE] = ENDNULL;
    strcpy(copy,instruct->file);
    int bracket = 1;
    int index = 1;
    int mark = 0;
    while(bracket != 0){
        if(copy[index] == MARK){
            mark++;
        }
        if(copy[index] == LBRACKET && mark%TWO == 0){
            bracket++;
        }
        if(copy[index] == RBRACKET && mark%TWO == 0){
            bracket--;
        }
        index++;
    }
    if(bracket != 0){
        return false;
    }
    (void) instruct;
    return true;
}

bool split(nuclei* instruct){
    char copy[SIZE] = ENDNULL;
    strcpy(copy,instruct->file);
    subprog(copy);
    bool out = true;
    out = get_instruction(copy);
    //要判断括号数是否相同，相同就结束
    return out;
}

//去掉最外面的大括号
void subprog(char* copy){
    char tmp[SIZE] = ENDNULL;
    int len = strlen(copy);
    int index = 0, left_bracket = 0, right_bracket = 0, j = 0;
    int bracket = 0, start = 0, mark = 0, end = 0;//总共有多少括号
    while(left_bracket != TWO){
        if(copy[index] == LBRACKET){
            left_bracket++;
        }
        if(left_bracket == TWO){
            start = index;
        }
        index++;
    }
    for(int i = 0;i<len;i++){
        if(copy[i] == MARK){
            mark++;
        }
        if(copy[i] == LBRACKET && mark%TWO == 0){
            bracket++;
        }
    }
    for(int i = len;i>0;i--){
        if(copy[i] == RBRACKET){
            right_bracket++;
        }
        if(right_bracket == 1){
            end = i;
        }
    }
    for(int i=start;i<end;i++){
        tmp[j] = copy[i];
        j++;
    }
    strcpy(copy," ");
    int len1 = strlen(tmp);
    for(int i=0;i<len1;i++){
        copy[i] = tmp[i];
    }
    copy[len1] = ENDFILE;
}

//把去掉最外面括号的指令再拆开成tmp和rest
bool get_instruction(char* copy){
    char rest[SIZE] = ENDNULL, tmp[SIZE] = ENDNULL;
    int len = strlen(copy);
    int index = 1, end = 0, bracket = 1,start = 0;
    while(bracket != 0){
        if(copy[index] == LBRACKET){
            bracket++;
        }else if(copy[index] == RBRACKET){
            bracket--;
        }
        index++;
    }
    end = index;
    for(int i = 0;i<end;i++){
        tmp[i] = copy[i];
    }
    for(int i = end+1;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len] = ENDFILE;
    int lenrest = strlen(rest);
    bool instract_type = false;
    instract_type = instract_judge(tmp);
    if(lenrest != 0){
        get_instruction(rest);
    }
    return instract_type;
}

//instract的判别
bool instract_judge(char* tmp){
    bool judge_type = false;
    char token[MAX_NUMS] = ENDNULL;
    char rest[MAX_NUMS] = ENDNULL;
    subbraces(tmp);
    token_get(tmp,token,rest);
    judge_type = fun_cmp(token,rest);//change to bool!!!!!!
    return judge_type;
}

//去掉每个instruction的括号
void subbraces(char* copy){
    char tmp[SIZE] = ENDNULL;
    int len = strlen(copy);
    int index = 1, mark = 0, mark2 = 0, bracket = 1, j = 0;
    while(bracket != 0){
        if(copy[index] == MARK){
            mark++;
        }        
        if(copy[index] == MARKDOUBLE){
            mark2++;
        }
        if(copy[index] == LBRACKET && mark%TWO == 0 && mark2%TWO == 0){
            bracket++;
        }else if(copy[index] == RBRACKET && mark%TWO == 0&& mark2%TWO == 0){
            bracket--;
        }
        index++;
    }
    for(int i=1;i<len-1;i++){
        tmp[j] = copy[i];
        j++;
    }
    strcpy(copy,ENDNULL);
    int len1 = strlen(tmp);
    for(int i=0;i<len1;i++){
        copy[i] = tmp[i];
    }
    copy[len1] = ENDFILE;
}

bool fun_cmp(char* token,char* rest){// change to bool!!!!!!!!!!!
    bool fun_cmp_type = false;
    if(strcmp(token, "SET") == 0){
        fun_cmp_type = set_func(rest);//judge setfunction whether true
    }
    // if token is PRINT then get in printfunc
    if(strcmp(token,"PRINT") == 0){
        fun_cmp_type = print_func(rest);
    }
    if(strcmp(token,"WHILE") == 0){
        fun_cmp_type = while_func(rest);
    }
    if(strcmp(token,"IF") == 0){
        fun_cmp_type = if_func(rest);
    }
    return fun_cmp_type;
}

bool set_func(char* tmp){
    char token[MAX_NUMS] = ENDNULL;
    char rest[MAX_NUMS] = ENDNULL;
    bool setfunc = false;
    token_get(tmp,token,rest);
    //判断完set_func里面的VAR之后判断LIST  
    if(tmp[0] >= 'A' && tmp[0] <= 'Z'){
        setfunc =  list_judge(rest);
    }
    return setfunc;
}

bool print_func(char* tmp){
    int len = strlen(tmp);
    int string_amount = 0;
    bool string_type = false;
    for(int i = 0;i<len;i++){
        if(tmp[i] == MARKDOUBLE){
            string_amount++;
        }
    }
    if(string_amount == TWO ||string_amount == 0){
        string_type = true;
    }
    return string_type;
}

//对list进行判断
bool list_judge(char* tmp){
    bool outcome = false, ret = false;
    bool val = false, literal = false, nil = false;
    char token[MAX_NUMS] = ENDNULL, rest[MAX_NUMS] = ENDNULL;
    if(tmp[0] != LBRACKET){
        token_get(tmp,token,rest);
        if(token[0] == MARK){
            literal = literal_judge(token);
        }else if(token[0] >= 'A' && token[0] <= 'Z'){
            val = true;
        }else if(token[0] == MARKDOUBLE){
            if(strcmp(token,"NIL") == 0){
                nil = true;
            }
        }
    }else if(tmp[0] == LBRACKET){
        subbraces(tmp);
        token_get(tmp,token,rest);
        ret_function(token,rest);
        ret = true;
    }
    if(val == true || literal == true || nil == true || ret == true){
        outcome = true;
    }
    return outcome;
}
//对literal进行判断，是否是(1)，1，(1 (2 3))
bool literal_judge(char* tmp){
    int mark = 0, index = 0, end = 0;//单引号的个数，两个就结束
    bool state = false;
    //匹配单引号
    while(state == false){
        if(tmp[index] == MARK ){
            mark++;
        }else if(mark == TWO){
            state = true;
            end = index;
        }
        index++;
    }//tmp[0] == ' ,tmp[1] == 1
    for(int i = 1;i<end;i++){
    }
    return state;
}

bool ret_function(char* token,char* rest){
    bool l1 = false, l2 = false, l3 = false;
    bool l4 = false, l5 = false, l6 = false;
    if(strcmp(token,"CAR") == 0 ||strcmp(token,"CDR") == 0){
        l1 = list_func(rest);
    }    
    if(strcmp(token,"CONS") == 0){
        l2 = list_func_cons(rest);
    }
    if(strcmp(token,"LENGTH") == 0){
        l3 = int_func_length(rest);
    }
    if(strcmp(token,"PLUS") == 0){
        l4 = int_func_plus(rest);
    }
    if(strcmp(token,"LESS") == 0 ||strcmp(token,"GREATER") == 0){
        l5 = bool_func(rest);
    }
    if(strcmp(token,"EQUAL") == 0){
        l6 = bool_func(rest);
    }
    return l1 && l2 && l3 && l4 && l5 && l6;
}

bool list_func(char* tmp){
    bool list_func_type = false;
    list_func_type = list_judge(tmp);
    return list_func_type;
}

bool list_func_cons(char* copy){
    char tmp[SIZE] = ENDNULL;
    char rest[SIZE] = ENDNULL;
    int space = 0, end = 0, start = 0, i = 0;
    int len = strlen(copy);
    bool l1 = false, l2 = false;
    while(space == 0){
        if(copy[i] == SPACESIN){
            space++;
            end = i;
        }
        i++;
    }
    for(int i = 0;i<end;i++){
        tmp[i] = copy[i];
    }
    tmp[end] = ENDFILE;
    for(int i = end+1;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len-end] = ENDFILE;
    l1 =  list_judge(tmp);
    l2 = list_judge(rest);
    return l1 && l2;
}

bool int_func_plus(char* copy){
    char tmp[SIZE] = ENDNULL;
    char rest[SIZE] = ENDNULL;
    int end = 0;
    int len = strlen(copy);
    bool l1 = false;
    bool l2 = false;
    split_list(copy,tmp,rest);
    rest[len-end] = ENDFILE;
    l1 = list_judge(tmp);
    l2 = list_judge(rest);
    return l1 && l2;
}

bool int_func_length(char* copy){
    bool int_func_type = false;
    int_func_type = list_judge(copy);
    return int_func_type;
}

bool bool_func(char* copy){
    char tmp[SIZE] = ENDNULL, rest[SIZE] = ENDNULL;
    char rest1[SIZE] = ENDNULL, rest2[SIZE] = ENDNULL;
    int space = 0, end = 0, start = 0, i = 0;
    int len = strlen(copy);
    bool l1 = false, l2 = false, l3 = false;
    while(space == 0){
        if(copy[i] == SPACESIN){
            space++;
            end = i;
        }
        i++;
    }
    for(int i = 0;i<end;i++){
        tmp[i] = copy[i];
    }
    tmp[end] = ENDFILE;
    for(int i = end+1;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len-end] = ENDFILE;
    l1 = list_judge(tmp);
    split_list(rest,rest1,rest2);
    l2 = list_judge(rest1);
    l3 = list_judge(rest2);
    return l1 && l2 && l3;
}

bool while_func(char* copy){
    char condition[SIZE] = ENDNULL, rest[SIZE] = ENDNULL;
    int len = strlen(copy);
    int index = 1, end = 0, bracket = 1, start = 0;
    bool l1 = false, l2 = false;
    while(bracket != 0){
        if(copy[index] == LBRACKET){
            bracket++;
        }else if(copy[index] == RBRACKET){
            bracket--;
        }
        index++;
    }
    end = index;
    for(int i = 0;i<end;i++){
        condition[i] = copy[i];
    }
    for(int i = end+1;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len] = ENDFILE;
    subbraces(condition);
    l1 = bool_func(condition);
    subprog(rest);
    l2 = get_instruction(rest);
    return l1 && l2;
}

bool if_func(char* copy){
    char condition[SIZE] = ENDNULL, rest[SIZE] = ENDNULL;
    char rest1[SIZE] = ENDNULL, rest2[SIZE] = ENDNULL;
    int len = strlen(copy), start = 0;
    int index = 1, end = 0, bracket = 1, start_rest = 0;
    bool l1 = false, l2 = false, l3 = false;
    while(bracket != 0){
        if(copy[index] == LBRACKET){
            bracket++;
        }else if(copy[index] == RBRACKET){
            bracket--;
        }
        index++;
    }
    end = index;
    for(int i = 0;i<end;i++){
        condition[i] = copy[i];
    }
    int i = end;
    while(start_rest == 0){
        if(copy[i] == LBRACKET){
            start_rest = i;
        }
        i++;
    }
    for(int i = start_rest-1;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len] = ENDFILE;
    subbraces(condition);
    l1 = bool_func(condition);
    if_rest_test(rest,rest1,rest2);
    subprog(rest1);
    subprog(rest2);
    l2 = get_instruction(rest1);
    l3 = get_instruction(rest2);
    return l1 && l2 && l3;
}

void split_list(char* copy,char* tmp, char* rest){
    int index = 0, end = 0, mark = 0, start = 0, start_r = 0;
    int len = strlen(copy);
    if(copy[0] >= 'A' && copy[0] <= 'Z'){
        end = 0;
    }else if(copy[0] == MARK){
        while(mark != TWO){
            if(copy[index] == MARK){
                mark++;
                end = index;
            }
            index++;
        }
    }else if(copy[0] == MARKDOUBLE){
        while(mark == 0){
            if(copy[index] == MARKDOUBLE){
                mark++;
                end = index;
            }
            index++;
        }
    }else if(copy[0] == LBRACKET){
        while(end == 0){
            if(copy[index] == LBRACKET){
                mark++;
            }else if(copy[index] == RBRACKET){
                mark--;
            }
            if(mark == 0 && index != 0){
                end = index;
            }
            index++;
        }

    }
    for(int i = 0;i<end+1;i++){
        tmp[start++] = copy[i];
    }
    tmp[end+1] = ENDFILE;
    while(copy[end+1] == SPACESIN){
        end++;
    }
    for(int i = end+1;i<len;i++){
        rest[start_r++] = copy[i];
    }
    rest[len] = ENDFILE;
}

bool if_rest_test(char* copy, char* rest1, char* rest2){
    int bracket2 = 0, end = 0, start1 = 0,k = 0, j = 0;
    int len = strlen(copy);
    int index = 0, bracket = 0, mark = 0, first_start = 0;
    while(bracket != FOUR){
        if(copy[index] == MARKDOUBLE){
            mark++;
        }
        if(copy[index] == LBRACKET || copy[index] == RBRACKET){
            if(mark%TWO ==0){
                bracket++;
            }
            if(bracket == 1){
                first_start = index;
            }
        }
        index++;
    }
    end = index;
    for(int i = first_start;i<end;i++,k++){
        rest1[k] = copy[i];
    }
    while(bracket2 == 0){
        if(copy[index] == LBRACKET){
            bracket2++;
        }
        index++;
    }
    start1 = index;
    for(int i = start1-1;i<len;i++){
        rest2[j++] = copy[i];
    }
    return true;
}


/*interpret*/


bool judge_interp(nuclei* instruct, Var* arr){

    bool prog_type = prog_interp(instruct,arr);//判断是否符合prog
    return prog_type;
}

bool prog_interp(nuclei* instruct, Var* arr){
    bool split_type = true;
    split_type = split_interp(instruct,arr);
    return split_type;
}

bool split_interp(nuclei* instruct, Var* arr){
    char copy[SIZE] = ENDNULL;
    strcpy(copy,instruct->file);
    subprog(copy);
    bool out = true;
    out = get_instruction_in(copy,arr);
    return out;
}


//把去掉最外面括号的指令再拆开成tmp和rest
bool get_instruction_in(char* copy, Var* arr){
    char rest[SIZE] = ENDNULL, tmp[SIZE] = ENDNULL;
    int len = strlen(copy);
    int index = 1, end = 0, bracket = 1;
    while(bracket != 0){
        if(copy[index] == LBRACKET){
            bracket++;
        }else if(copy[index] == RBRACKET){
            bracket--;
        }
        index++;
    }
    end = index;
    for(int i = 0;i<end;i++){
        tmp[i] = copy[i];
    }
    int start = 0;
    for(int i = end+1;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len] = ENDFILE;
    int lenrest = strlen(rest);
    bool instract_type = false;
    instract_type = instract_judge_in(tmp,arr);
    if(lenrest != 0){
        get_instruction_in(rest,arr);
    }
    return instract_type;
}

//instract的判别
bool instract_judge_in(char* tmp, Var* arr){
    bool judge_type = false;
    char token[MAX_NUMS] = ENDNULL;
    char rest[MAX_NUMS] = ENDNULL;
    subbraces(tmp);
    token_get(tmp,token,rest);
    judge_type = fun_cmp_in(token,rest,arr);//change to bool!!!!!!
    return judge_type;
}

bool fun_cmp_in(char* token,char* rest,Var* arr){// change to bool!!!!!!!!!!!
    bool fun_cmp_type = false;
    if(strcmp(token, "SET") == 0){
        fun_cmp_type = set_func_in(rest,arr);//judge setfunction whether true
    }
    if(strcmp(token,"PRINT") == 0){
        fun_cmp_type = print_func_in(rest,arr);
        if(fun_cmp_type == true){
            return true;
        }
    }
    if(strcmp(token,"WHILE") == 0){
        char condition[SIZE] = ENDNULL;
        split_plus(rest,condition,rest);
        sub(rest);
        while(judge_cond(condition,arr)){
            addbraces(condition);
            get_instruction_in(rest,arr);
        }
        if(fun_cmp_type == true){
            return true;
        }
    }
    if(strcmp(token,"IF") == 0){
        char condition[SIZE] = ENDNULL;
        char rest1[SIZE] = ENDNULL;
        char rest2[SIZE] = ENDNULL;
        split_plus(rest,condition,rest);
        if_rest_test(rest,rest1,rest2);
        if(judge_cond(condition,arr)){
            subbraces(rest1);
            fun_cmp_type = instract_judge_in(rest1,arr);
        }else{
            subbraces(rest2);
            fun_cmp_type = instract_judge_in(rest2,arr);
        }
        if(fun_cmp_type == true){
            return true;
        }
    }
    return fun_cmp_type;
}

bool print_func_in(char* tmp, Var* arr){
    int len = strlen(tmp);
    char token[SIZE] = ENDNULL, rest[SIZE] = ENDNULL, str[SIZE] = ENDNULL;
    bool print_type = false;
    if(tmp[0] == MARKDOUBLE){
        for(int i = 1;i<len-1;i++){
            printf("%c",tmp[i]);
        }
        printf("\n");
        return true;
    }else if(tmp[0] == LBRACKET){//(CAR A)
        print_type = true;
        subbraces(tmp);
        token_get(tmp,token,rest);
        if(strcmp(token,"CAR") == 0){
            int j = lisp_getval(lisp_car(arr[*rest - CHARA].list));
            printf("%d\n",j);
        }else{//(CONS '1' (CONS '2' NIL))
            char rest1[SIZE] = ENDNULL, rest2[SIZE] = ENDNULL;
            split_print_cons(rest,rest1,rest2);
            lisp* lisp1 = NULL;
            lisp* lisp2 = NULL;
            char index[SMALLSIZE] = LBBRACKET;
            lisp1 = print_cons(rest1,index,lisp1,arr);
            lisp2 = print_cons(rest2,index,lisp2,arr);
            char str1[SIZE] = ENDNULL;
            char str2[SIZE] = ENDNULL;
            lisp_tostring(lisp1,str1);
            lisp_tostring(lisp2,str2);
            arr[ALPHA-1].list = cons(lisp1,lisp2);
            lisp_tostring(arr[ALPHA-1].list,str);
            printf("%s\n",str);
        }
    }else{
        print_type = true;
        lisp_tostring(arr[*tmp - CHARA].list,str);
        printf("%s\n",str);
    }
    return print_type;
}

bool set_func_in(char* tmp, Var* arr){
    char index[MAX_NUMS] = ENDNULL;
    char rest[MAX_NUMS] = ENDNULL;
    bool setfunc = false;
    token_get(tmp,index,rest);
    //判断完set_func里面的VAR之后判断LIST  
    setfunc =  list_judge_in(rest,index,arr);
    return setfunc;
}

//对list进行判断
bool list_judge_in(char* tmp,const char* index, Var* arr){
    bool outcome = false;
    char token[MAX_NUMS] = ENDNULL;
    char rest[MAX_NUMS] = ENDNULL;
    int cmp = strcmp(tmp,"NIL");
    if(tmp[0] == MARK && tmp[1] != LBRACKET){//SET A '1'
        outcome = true;
        arr[*index - CHARA].list = atom(tmp[1] - CHARZERO);
        arr[*index - CHARA].ifset = true;
    }else if(tmp[0] == MARK && tmp[1] == LBRACKET){
        outcome = true;//SET A '(1 2)'
        char new[MAX_NUMS] = ENDNULL;
        submark(tmp, new);
        int len = strlen(new), bracket = 0;
        for(int i = 0;i<len;i++){
            if(new[i] == LBRACKET){
                bracket++;
            }else if(new[i] == RBRACKET){
                bracket--;
            }
        }
        if(bracket == 0){
            arr[*index - CHARA].list = fromstring(new);
            arr[*index - CHARA].ifset = true;
        }else{
            return false;
        }
    }else if(tmp[0] >= 'A' && tmp[0] <= 'Z' && cmp != 0){
        outcome = true;// SET C A
        if(arr[*index - tmp[0]].ifset == 1){
            arr[*index - CHARA] = arr[tmp[0] - CHARA];
        }else{
            printf("Can not SET!\n");
            return false;
        }
    }else{
        subbraces(tmp);
        token_get(tmp,token,rest);
        outcome = ret_function_in(token,rest,index,arr);
        arr[*index - CHARA].ifset = true;
    }
    return outcome;
}

bool ret_function_in(char* token,char* rest,const char* index, Var* arr){
    #ifdef INTERP
    bool l2 = false, l3 = false, l4 = false;
    bool l5 = false, l6 = false, l7 = false,l8 = false;
    if(strcmp(token,"CONS") == 0){
        l2 = list_func_cons_in(rest,index,arr);
    }
    if(strcmp(token,"LENGTH") == 0){
        l3 = int_func_length_in(rest,index,arr);
    }
    if(strcmp(token,"PLUS") == 0){
        l4 = int_func_plus_in(rest,index,arr);
    }
    if(strcmp(token,"LESS") == 0 ||strcmp(token,"GREATER") == 0){
        l5 = bool_func_in(token,rest,index,arr);
    }
    if(strcmp(token,"EQUAL") == 0){
        l7 = bool_func_in(token,rest,index,arr);
    }
    if(strcmp(token,"MINUS") == 0){
        l6 = int_func_minus_in(rest,index,arr);
    }
    if(strcmp(token,"APPEND") == 0){
        l8 = int_func_append_in(rest,index,arr);
    }
    return l2 || l3 || l4 || l5 || l6 || l7|| l8;
    #else
    (void) arr;
    (void) index;
    (void) token;
    (void) rest;
    return true;
    #endif
}

bool list_func_cons_in(char* copy,const char* index, Var* arr){
    char tmp[SIZE] = ENDNULL, rest[SIZE] = ENDNULL;
    int space = 0, end = 0, start = 0, i = 0;
    int len = strlen(copy);
    while(space == 0){
        if(copy[i] == SPACESIN){
            space++;
            end = i;
        }
        i++;
    }
    for(int i = 0;i<end;i++){
        tmp[i] = copy[i];
    }
    tmp[end] = ENDFILE;
    for(int i = end+1;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len-end] = ENDFILE;
    lisp* lisp1 = NULL;
    lisp* lisp2 = NULL;
    lisp1 = get_cons_list(tmp,index,lisp1,arr);
    lisp2 = get_cons_list(rest,index,lisp2,arr);
    arr[*index - CHARA].list = cons(lisp1,lisp2);
    arr[*index - CHARA].list->atom = 1;
    return true;
}

bool bool_func_in(const char* token, const char* copy, const char* index, Var* arr){
    char left[SMALLSIZE] = ENDNULL;
    char right[SMALLSIZE] = ENDNULL;
    left[0] = copy[0];
    right[0] = copy[TWO];
    left[1] = ENDFILE;
    right[1] = ENDFILE;
    int i = lisp_getval(arr[*left - CHARA].list);
    int j = lisp_getval(arr[*right - CHARA].list);
    int outcome = 1;
    if(strcmp(token,"LESS") == 0){
        if(i>=j){
            outcome = 0;
        }
    }else if(strcmp(token,"GREATER") == 0){
        if(i<=j){
            outcome = 0;
        }
    }else{
        if(i!=j){
            outcome = 0;
        }
    }
    arr[*index - CHARA].list = atom(outcome);
    return outcome;
}


bool int_func_plus_in(char* copy, const char* index, Var* arr){
    char rest1[SMALLSIZE] = ENDNULL;
    char rest2[SMALLSIZE] = ENDNULL;
    split_list(copy,rest1,rest2);
    int rest1_val = 0;
    int rest2_val = 0;
    rest1_val = getplus(rest1,index,arr);
    rest2_val = getplus(rest2,index,arr);
    int i =0;
    i = rest1_val + rest2_val;
    arr[*index - CHARA].list = atom(i);
    arr[*index - CHARA].ifset = 1;
    return true;
}

bool int_func_length_in(char* copy, const char* index, Var* arr){
    int i =lisp_length(arr[*copy - CHARA].list);
    arr[*index - CHARA].list = atom(i);
    arr[*index - CHARA].ifset = 1;
    return true;
}



void submark(char* token, char* new){
    char tmp[SIZE] = ENDNULL;
    int len = strlen(token);
    int start = 0;
    for(int i = 1;i<len-1;i++){
        tmp[start++] = token[i];
    }
    start = 0;
    for(int i = 0;i<len-TWO;i++){
        new[i] = tmp[start++];
    }
    new[len-TWO] = ENDFILE;
}


void judge_negative(char* cur){
    if(cur[0] == MINUSMARK){
        cur[0] = (cur[1] - CHARZERO) * NEGITAVEVAL;
        cur[1] = ENDFILE;
    }
}



lisp* get_cons_list(char* tmp,const char* index, lisp* l, Var* arr){
    char new[SIZE] = ENDNULL;
    if(tmp[0] == MARK){
        // '1' 
        submark(tmp,new);
        if(strlen(new) == 1){
            l = atom(*new - CHARZERO);
        }else{//'(1 (2 3))'
            l = fromstring(new);
        }
    }else if(tmp[0] == LBRACKET){//(cons ...)
        subbraces(tmp);
        char token[SMALLSIZE] = ENDNULL;
        char rest[SIZE] = ENDNULL;
        token_get(tmp,token,rest);
        if(strcmp(token,"CONS") == 0){
            list_func_cons_in(rest,index,arr);
        }
    }else if(strcmp(tmp,"NIL") == 0){//NIL
        return NIL;
    }
    else{//A
        l = arr[*tmp - CHARA].list;
    }
    return l;
}

void split_print_cons(char* tmp, char* token, char* rest){
    int len = strlen(tmp);
    for(int i = 0;i<THREE;i++){
        token[i] = tmp[i];
    }
    int start = 0;
    for(int i = THREE+1;i<len;i++){
        rest[start++] = tmp[i];
    }
}


lisp* print_cons(char* tmp,char* index, lisp* l, Var* arr){
    (void) index;
    char new[SIZE] = ENDNULL;
    if(tmp[0] == MARK){//'1'
        submark(tmp,new);
        l = atom(*new - CHARZERO);
    }else if(tmp[0] == LBRACKET){//(cons ...)
        subbraces(tmp);
        char token[SMALLSIZE] = ENDNULL;
        char rest[SIZE] = ENDNULL;
        char rest1[SIZE] = ENDNULL;
        char rest2[SIZE] = ENDNULL;
        token_get(tmp,token,rest);
        split_print_cons(rest,rest1,rest2);
        if(strcmp(token,"CONS") == 0){
            lisp* l1 = print_cons(rest1,index,l,arr);
            lisp* l2 = print_cons(rest2,index,l,arr);
            l = cons(l1,l2);
        }
    }else if(strcmp(tmp,"NIL") == 0){//NIL
        return NIL;
    }
    return l;
}


int getplus(char* rest, const char* index, Var* arr){
    char new[SIZE] = ENDNULL;
    int val = 0;
    if(rest[0] == MARK){//'1'
        submark(rest,new);
        if(new[0] == MINUSMARK){
            new[0] = new[1];
            new[1] = ENDFILE;
            val = (*new - CHARZERO) * NEGITAVEVAL;
        }else{
            val = *new - CHARZERO;
        }
    }else if(rest[0] == LBRACKET){
        if(rest[FIVE] == LBRACKET){//(CAR (CDR L))
            cdr(arr[rest[TEN] - CHARA].list)->type = 1;
            lisp* l =NULL;
            arr[rest[TEN] - CHARA].list->type = 1;
            l = arr[rest[TEN] - CHARA].list;
            l = cdr(l);
            l->type = 1;
            val = lisp_getval(car(l));
        }else{//(CAR L)
            arr[rest[FIVE] - CHARA].list->type = 1;
            val = lisp_getval(car(arr[rest[FIVE] - CHARA].list));
        }  
    }else{//A
        val = lisp_getval(arr[*rest - CHARA].list);
    }
    (void) index;
    return val;
}

void split_plus(char* copy, char* rest1, char* rest2){
    int bracket = 0, mark = 0, end = 0;
    int len = strlen(copy);
    int i = 0, start = 0, begin = 0;
    while(end == 0){
        if(copy[i] == LBRACKET){
            bracket++;
        }else if(copy[i] == RBRACKET){
            bracket--;
        }else if(copy[i] == MARK){
            mark++;
        }
        if(copy[i] == RBRACKET && bracket==0 && mark%TWO==0){
            end = i;
        }
        i++;
    }
    for(int i = 0;i<end+1;i++){
        rest1[i] = copy[i];
    }
    rest1[end+1] = ENDFILE;
    while(begin == 0){
        if(copy[end++] == LBRACKET){
            begin = end;
        }
    }
    for(int i = begin-1;i<len;i++){
        rest2[start++] = copy[i];
    }
    rest2[start] = ENDFILE;
}

bool judge_cond(char* condition, Var* arr){
    subbraces(condition);
    char token[SMALLSIZE] = ENDNULL, rest[SMALLSIZE] = ENDNULL;
    char rest1[SMALLSIZE] = ENDNULL, rest2[SMALLSIZE] = ENDNULL;
    char new1[SMALLSIZE] = ENDNULL, new2[SMALLSIZE] = ENDNULL;
    token_get(condition,token,rest);
    split_list(rest,rest1,rest2);
    submark(rest1,new1);
    submark(rest2,new2);
    if(strcmp(token,"EQUAL") == 0){
        if(rest1[1] == CHARL){
            char token11[SIZE] = ENDNULL, rest11[SIZE] = ENDNULL;
            token_get(rest1,token11,rest11);
            int i = getlength(rest11,arr);
            if(i - *new2 == 0){
                return true;
            }
        }else if(rest1[1] == CHARC){//(car L)
            int i = 0;
            i = lisp_getval(car(arr[rest1[FIVE] - CHARA].list));
            if(i - *new2 == 0){
                return true;
            }
        }
        if(*new1 - *new2 == 0){
            return true;
        }
    }else if(strcmp(token,"GREATER") == 0){
        if(*new1 - *new2 > 0){
            return true;
        }
    }else if(strcmp(token,"LESS") == 0){
        //(LESS '1' '2')(LESS '0' C)(LESS '1' '2')(LESS C '20')
        int i =0, j =0;
        char new123[SIZE] = ENDNULL;
        if(*rest1 >= CHARA && *rest1 <= 'Z'){//A
            i = lisp_getval(arr[*rest1 -CHARA].list);
        }else{//'1'
            submark(rest1,new123);
            i = *new123 - CHARZERO;
        }
        if(*rest2 >= CHARA && *rest2 <= 'Z'){//A
            j = lisp_getval(arr[*rest2 -CHARA].list);
        }else{//'1'
            submark(rest2,new123);
            if(strlen(new123)!=1){
                j = (new123[0] - CHARZERO) * TEN + (new123[1] - CHARZERO);
            }else{
                j = *new123 - CHARZERO;
            }
        }
        if(i < j ){
            return true;
        }
    }
    return false;
}

int judge_bracket(const char* copy){
    int bracket = 1;
    int index = 1;
    while(bracket != 0){
        if(copy[index] == LBRACKET){
            bracket++;
        }else if(copy[index] == RBRACKET){
            bracket--;
        }
        index++;
    }
    return index;
}

int getlength(char* rest11, Var* arr){
    if(strcmp(rest11,"NIL") == 0){
        return 0;
    }else if(rest11[0] == LBRACKET){
        return rest11[1];
    }else{
        return lisp_length(arr[*rest11 - CHARA].list);
    }
}


void addbraces(char* copy){
    char tmp[SIZE] = ENDNULL;
    int len = strlen(copy);
    tmp[0] = LBRACKET;
    for(int i = 0;i<len;i++){
        tmp[i+1] = copy[i];
    }
    tmp[len+1] = RBRACKET;
    tmp[len+TWO] = ENDFILE;
    int len1 = strlen(tmp);
    for(int i = 0;i<len1;i++){
        copy[i] = tmp[i];
    }
    copy[len1+1] = ENDFILE;
}

void sub(char* copy){
    char tmp[SIZE] = ENDNULL;
    int bracket = 0, bracket2 = 0, lbracket = 0;
    int begin = 0, start = 0, start1 = 0, start2 = 0;
    int len = strlen(copy);
    for(int i = 0;i<len;i++){
        if(copy[i] == LBRACKET || copy[i] == RBRACKET){
            bracket++;
        }
    }
    while(begin == 0){
        if(copy[start++] == LBRACKET){
            lbracket++;
        }
        if(lbracket == TWO){
            begin = start - 1;//找到第二个括号
        }
    }
    while(bracket - bracket2 != 1){
        if(copy[start1] == LBRACKET || copy[start1] == RBRACKET){
            bracket2++;
        }
        start1++;//找到倒数第二个括号
    }
    for(int i = begin;i<start1;i++){
        tmp[start2++] = copy[i];
    }
    int len1 = strlen(tmp);
    for(int i = 0;i<len1+1;i++){
        copy[i] = tmp[i];
    }
    copy[len1+TWO] = ENDFILE;
}

void free_var(Var* arr){
    for(int i = 0;i<ALPHA+1;i++){
        lisp_free(&arr[i].list);
    }
}


//extension


bool int_func_minus_in(char* copy, const char* index, Var* arr){
    char rest1[SMALLSIZE] = ENDNULL;
    char rest2[SMALLSIZE] = ENDNULL;
    split_list(copy,rest1,rest2);
    int rest1_val = 0;
    int rest2_val = 0;
    rest1_val = getplus(rest1,index,arr);
    rest2_val = getplus(rest2,index,arr);
    int i =0;
    i = rest1_val + rest2_val;
    arr[*index - CHARA].list = atom(i);
    arr[*index - CHARA].ifset = 1;
    return true;
}

bool int_func_append_in(char* copy, const char* index, Var* arr){
    char tmp[SIZE] = ENDNULL, rest[SIZE] = ENDNULL;
    char new1[SIZE] = ENDNULL, new2[SIZE] = ENDNULL;
    int mark = 0, end = 0, start = 0, i = 0;
    int count = 0;
    int len = strlen(copy);
    while(mark != 2){
        if(copy[i] == MARK){
            mark++;
            end = i;
        }
        i++;
    }
    for(int i = 0;i<end+1;i++){
        tmp[i] = copy[i];
    }
    tmp[end+1] = ENDFILE;
    for(int i = end+TWO;i<len;i++){
        rest[start++] = copy[i];
    }
    rest[len-end] = ENDFILE;
    submark(tmp,new1);
    submark(rest,new2);
    subbraces(new2);
    int len2 = strlen(new2);
    for(int i = 0;i<len2;i++){
        if(new2[i] != MARK && new2[i] != SPACESIN){
            count++;
        }
    }
    lisp* lisp1 = NULL;
    lisp1 = fromstring(new1);
    if(lisp1->cdr != NULL){
    }
    lisp* l2 = lisp1->cdr;
    l2->cdr = fromstring(new2);
    arr[*index - CHARA].list = l2;
    // arr[*index - CHARA].list->atom = 1;
    return true;

}



//test

void testinterp(){
    char test[SIZE] = ENDNULL;
    char rest[SIZE] = ENDNULL;
    char token[SIZE] = ENDNULL;
    char index[SMALLSIZE] = "A";
    char tmp[SIZE] = ENDNULL;
    char rest2[SIZE] = ENDNULL;
    char str[SIZE] = ENDNULL;
    
    lisp* lisp1 = NULL;
    lisp* lisp2 = NULL;

    Var arr[ALPHA] = {{false, NULL}};
    //unit test1
    strcpy(test, "(CAR '(1 2 3))')");
    subbraces(test);
    assert(fun_cmp_in("CAR", "'(1 2 3))'",arr) == false);
    assert(instract_judge_in(test,arr) == false);

    //unit test2
    strcpy(test, "(LESS '2' '1')");
    assert(instract_judge_in(test,arr) == false);

    //unit test3
    strcpy(test, "(GREATER '1' '2')");
    assert(instract_judge_in(test,arr) == false);

    //unit test4
    strcpy(test, "(EQUAL '2' '1')");
    assert(instract_judge_in(test,arr) == false);

    //unit test5
    strcpy(test, "(CONS '1' '(1 2 3)')");
    subbraces(test);
    token_get(test,token,rest);
    strcpy(tmp, "'1'");
    strcpy(rest2, "'(1 2 3)'");
    lisp1 = get_cons_list(tmp,index,lisp1,arr);
    lisp2 = get_cons_list(rest2,index,lisp2,arr);
    arr[*index - CHARA].list = cons(lisp1,lisp2);
    arr[*index - CHARA].list->atom = 1;
    lisp_tostring(arr[*index - CHARA].list,str);
    assert(strcmp(str,"(1 1 2 3)") == 0);
    lisp_free(&lisp1);
    lisp_free(&lisp2);


    //unit test6
    strcpy(test, "(CONS '(1 2 3)' '(4 5)')");
    subbraces(test);
    token_get(test,token,rest);
    strcpy(tmp,"'(1 2 3)'" );
    strcpy(rest2,"'(4 5)'");
    lisp1 = get_cons_list(tmp,index,lisp1,arr);
    lisp2 = get_cons_list(rest2,index,lisp2,arr);
    arr[*index - CHARA].list = cons(lisp1,lisp2);
    arr[*index - CHARA].list->atom = 1;
    lisp_tostring(arr[*index - CHARA].list,str);
    assert(strcmp(str,"((1 2 3) 4 5)") == 0);
    lisp_free(&lisp1);
    lisp_free(&lisp2);

    //unit test7
    strcpy(test, "(SET A '1')");
    subbraces(test);
    token_get(test,token,rest);
    set_func_in(rest,arr);
    assert(lisp_getval(arr[0].list) == 1);


}





