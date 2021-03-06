#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define HASH_SLOT 0x4000	//hash表槽数（其实还未理解hash_pjw要的槽数是这么多...）
#define LAYER_DEPTH 10	//层最大深度

typedef struct Type_ *Type;
typedef struct Fun_ *Fun;
typedef struct FieldList_ *FieldList;
typedef struct varElement varElement;

//目前考虑把函数和变量放在两张表里

static varElement* hashTable[HASH_SLOT];	//hash表项
static varElement* layerStack[LAYER_DEPTH];	//控制每层的栈
static int layer = 0;	//目前在第几层

/* 类型 */
struct Type_ {
	enum{BASIC, ARRAY, STRUCTURE, FUNCTION} kind;
	union {
		int basic;	//基本类型:0-int, 1-float
		struct {	//数组类型
			int size;
			Type elem;	//一个指针，串起数组元素
		}array;
		FieldList structure;	//结构体类型信息是一个链表
	}u;
};

/* 域 */
struct FieldList_ {
	char *name;	//域名
	Type type;	//域类型
	FieldList tail;	//下一个域
};

/* 变量表项 */
struct varElement {
	char *name;	//变量名
	Type type;	//变量类型
	int layer_no;	//在哪一层
	int slot_no;	//在哪个槽
	struct varElement *layer_next;	//同一层的下一个
	struct varElement *slot_next;	//同一槽的下一个
};

void init_table();	//初始化符号表
unsigned int hashpjw(char *);
void insert(varElement *);	//插入一个变量
varElement* searchAll(char *); 	//搜索一个变量最里层的定义
varElement* search(char *);	//搜索一个变量在此层的定义
void into_a_layer();	//进入一层
void out_of_a_layer();	//从一层中出来

#endif
