#include "lisp.h"
#include "specific.h"

#define NIL NULL
//0 means atom, 1 means not atom
#define ATOM 0
#define NATOM 1
#define NEGATIVE 1
#define TEMPSIZE 1000
#define LISTSTRLEN 1000



lisp* lisp_atom(const atomtype a)
{
    //申请lisp空间，并且将type设置为0，car，cdr设置为NULL，atom设置为a
    lisp* l = (lisp*)malloc(sizeof(lisp));  
    l->type = ATOM;
    l->atom = a;
    l->car = NIL;
    l->cdr = NIL;
    return l;
}

lisp* lisp_cons(const lisp* l1, const lisp* l2)
{
    //申请lisp空间，并且将type设置为1，car设置为l1，cdr设置为l2
    lisp* l = (lisp*)malloc(sizeof(lisp));
    l->type = NATOM;
    l->car = (lisp*)l1;
    l->cdr = (lisp*)l2;
    return l;
}

lisp* lisp_car(const lisp* l)
{
    //判断是否是原子节点 不是的话返回car
    if (l->type == ATOM){
        return NIL;
    }
    return l->car;
}

lisp* lisp_cdr(const lisp* l)
{
    //判断是否是原子节点 不是的话返回cdr
    if (l->type == ATOM){
        return NIL;
    }
    return l->cdr;
}

atomtype lisp_getval(const lisp* l)
{
    //判断是否是原子节点 是的话返回atom，否则返回0
    if (l->type == NATOM){
        return 0;
    }
    return l->atom;
}

lisp* lisp_copy(const lisp* l)
{
    //首先判断是否为空
    if(l == NIL){
        return NIL;
    }
    //是原子节点
    if (l->type == ATOM){
        return lisp_atom(l->atom);
    }
    //不是原子节点
    return lisp_cons(lisp_copy(l->car), lisp_copy(l->cdr));
}

int lisp_length(const lisp* l)
{
    //首先判断是否为空
    if (l == NIL){
        return 0;
    }
    //是原子节点
    if (l->type == ATOM){
        return 0;
    }
    //不是原子节点
    return 1 + lisp_length(l->cdr);
}

void lisp_tostring(const lisp* l, char* str)
{
    //如果是空的话，返回()
    if(l == NIL){
        sprintf(str, "()");
        return;
    }
    //如果是原子节点，返回原子节点的值
    if (l->type == ATOM){
        sprintf(str, "%d", l->atom);
        return;
    }
    lisp* head = (lisp*)l;//头节点
    sprintf(str, "(");
    while(head != NIL){
        //car不为空时，先访问car
        if(head->car != NIL) {
            char* temp = (char*)malloc(TEMPSIZE);
            lisp_tostring(head->car, temp); //将car作为单独的表达式转换成字符串
            strcat(str, temp); //将字符串拼接到str后面
            free(temp); //释放内存
        }
        //将cdr作为head继续遍历
        head = head->cdr; 
        if (head != NIL){
            strcat(str, " "); //如果cdr不为空，将空格拼接到str后面
        }  
    }
    strcat(str, ")");   //将)拼接到str后面
}

void lisp_free(lisp** l)
{
    //如果是空的话，直接返回
    if(*l == NIL){
        return;
    }
    //如果是原子节点，直接释放内存
    if ((*l)->type == 0) {
        free(*l);
        *l = NIL;
        return;
    }
    lisp_free(&(*l)->car); //先释放car
    lisp_free(&(*l)->cdr); //释放cdr
    free(*l); //释放自身
    *l = NIL; //将指针置空
}

bool lisp_isatomic(const lisp* l){
    if(l == NIL){
        return false;
    }
    if(l->type == ATOM){
        return true;
    }
    return false;
}


lisp* lisp_fromstring(const char* str)
{
    //表明是一个原子节点
    if (str[0] != '(' && str[0] != ' '){
        atomtype a = 0;
        int flag = 0;
        int index = 0;
        //判断是不是负数，负数flag置为1
        if(str[0] == '-'){
            flag = NEGATIVE;
            index++;
        }
        // 获取数字
        while(str[index] != '\0' && (str[index] >= '0' && str[index] <= '9')){
            a = a * 10 + (str[index] - '0');
            index++;
        }
        if(flag == NEGATIVE){
            a = -a; //如果是负数，将a取反
        }
        return lisp_atom(a); //返回原子节点
    }
    //不是原子节点
    int i = 1;
    int length = strlen(str);
    lisp* car = NIL;
    lisp* cdr = NIL;
    while (i < length){
        //car的内容，将他单独摘出来
        if (str[i] == '('  &&  car == NIL){
            int index = i+1;
            int bracket = 1;
            //括号匹配，匹配左括号对应的右括号位置
            while(bracket != 0){
                if(str[index] == '('){
                    bracket++;
                }
                if(str[index] == ')'){
                    bracket--;
                }
                index++;
            }
            index--;
            char* temp = (char*)malloc(index-i+2);
            strncpy(temp, str+i, index-i+1); //temp为括号内的内容
            temp[index-i+1] = '\0'; //截断
            car = lisp_fromstring(temp); //将括号内的内容转换成lisp表达式，赋给car
            free(temp);
            i = index+1; //i指向右括号后面的位置
        }
        //cdr的内容
        else if (str[i] == ' ' || str[i] == '('){
            char* temp;
            if(str[i] == '(')
            {
                temp = (char*)malloc(length-i+2);
                temp[0] = ' ';
                strncpy(temp+1, str+i, length-i+1);
            }
            else
            {
                temp = (char*)malloc(length-i+1);
                strncpy(temp, str+i, length-i+1);
            }
            cdr = lisp_fromstring(temp);  // 将cdr的内容转换成lisp表达式，赋给cdr
            free(temp);
            return lisp_cons(car, cdr); //返回一个新的表达式
        }
        else if (str[i] == ')'){
            i++;
        }     
        else
        {
            car = lisp_fromstring(str+i);
            i++; 
            //将数字全部跳过
            while(str[i] >= '0' && str[i] <= '9'){
                i++;
            }
        }
         
    }
    // 如果car和cdr都为空，返回空表达式
    if(car == NIL && cdr == NIL){
        return NIL;
    }
    return lisp_cons(car, cdr); //返回一个新的表达式
}

lisp* lisp_list(const int n, ...)
{
    va_list args;
    va_start(args, n);
    lisp* head = NIL;
    lisp* p;
    //遍历每一个lisp参数，尾插法插入到链表中
    for(int i = 0; i < n;i++){
        lisp* l = va_arg(args, lisp*);
        if(i == 0){
            head = lisp_cons(l, NIL);
            p = head;
        }
        else{
            p->cdr = lisp_cons(l, NIL);
            p = p->cdr;
        }
    }
    va_end(args); //结束遍历
    return head; //返回头节点
}

void lisp_reduce(void (*func)(lisp* l, atomtype* n), lisp* l, atomtype* acc)
{
    if(l == NIL){
        return;
    }
    //表明是原子节点，直接返回x
    if(l->type == 0){
        func(l, acc);   
        return;
    }
    //表明l后续为空，只有car
    if(l->type == 1 && l->cdr == NIL){
        func(l->car, acc);
        return;
    }
    lisp_reduce(func, l->car, acc); //递归调用
    lisp_reduce(func, l->cdr, acc); //递归调用
}
void test(void){
    char str[LISTSTRLEN];
    printf("Test Start ... \n");

    lisp_tostring(NIL, str);
    assert(lisp_length(NIL)==0);
    assert(strcmp(str, "()")==0);

    assert(lisp_isatomic(NULL)==false);
    lisp* a1 = atom(2);
    assert(lisp_length(a1)==0);
    assert(lisp_isatomic(a1)==true);
    lisp_free(&a1);
    assert(a1==NULL);

    lisp* l1 = cons(atom(2), NIL);
    assert(l1);
    assert(lisp_length(l1)==1);
    lisp_tostring(l1, str);
    assert(strcmp(str, "(2)")==0);
    assert(lisp_getval(car(l1))==2);
    assert(lisp_isatomic(l1)==false);
    assert(lisp_isatomic(lisp_car(l1))==true);

    lisp* l2 = cons(atom(1), l1);
    assert(l2);
    assert(lisp_length(l2)==2);
    lisp_tostring(l1, str);
    lisp_tostring(l2, str);
    assert(strcmp(str, "(1 2)")==0);

    lisp* l3 = cons(atom(3), cons(atom(4), cons(atom(5), NIL)));
    assert(l3);
    assert(lisp_length(l3)==3);
    lisp_tostring(l3, str);
    assert(strcmp(str, "(3 4 5)")==0);

    lisp* l4 = cons(l2, l3);
    assert(l4);
    assert(lisp_length(l4)==4);
    lisp_tostring(l4, str);
    assert(strcmp(str, "((1 2) 3 4 5)")==0);

    lisp* l5 = cons(atom(0), l4);
    assert(l5);
    assert(lisp_length(l5)==5);
    lisp_tostring(l5, str);
    assert(strcmp(str, "(0 (1 2) 3 4 5)")==0);
    //printf("%s\n", str);
    lisp_free(&l5);
    assert(!l5);

    // lisp* b1 = fromstring("(1(2(3(4 5))))");
    // lisp_tostring(b1, str);
    // printf("%s\n", str);
    // lisp* b2 = fromstring("((1 2)3 4 5)");
    // lisp_tostring(b2, str);
    // printf("%s\n", str);
    // lisp_free(&b1);
}
