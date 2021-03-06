//to do: 符号表设计

#include "symboltable.h"


unsigned int hash_pjw(char *name) {
	unsigned int val=0,i;
	for(;*name;++name)
	{
		val=(val<<2)+*name;
		if(i=val&~0x3fff) val=(val^(i>>12))&0x3fff;
	}
	return val;
} 

void init_table() {
	int i;
	for (i=0; i<HASH_SLOT; i++)
		hashTable[i] = NULL;
	for (i=0; i<LAYER_DEPTH; i++)
		layerStack[i] = NULL;
	layer = 0;
}

//插入变量表，要求调用者在调用前处理好name和type
void insert(varElement* elem) {
	printf("in insert %s\n", elem->name);
	int index = hash_pjw(elem->name);
	printf("insert index %d\n", index);
	elem->slot_no = index;
	elem->layer_no = layer;
	elem->layer_next = NULL;
	elem->slot_next = NULL;
	//varElement *p = hashTable[index];
	//varElement *q = layerStack[layer];
	
	//插入槽链表头——这样应该删除操作也方便些
	if (hashTable[index] == NULL) {
		hashTable[index] = elem;
		//printf("%s\n", hashTable[index]->name);
	}
	else {
		elem->slot_next = hashTable[index];
		hashTable[index] = elem;
	}

	//插入层链表头
	if (layerStack[layer] == NULL)
		layerStack[layer] = elem;
	else {
		elem->layer_next = layerStack[layer];
		layerStack[layer] = elem;
	}
}

//查找任何最接近此层的定义
varElement *searchAll(char *name) {
	printf("in search all %s\n", name);
	int index = hash_pjw(name);
	printf("search index: %d\n", index);
	varElement *p = hashTable[index];
	while (p != NULL) {
		//因为每次都把越里层的插在前面，所以找到的第一个一定是最里层的定义
		if (strcmp(p->name, name) == 0)
			return p;
		p = p->slot_next;
	}
	return NULL;
}

//查找此层的定义
varElement *search(char *name) {
	printf("in search %s\n", name);
	int index = hash_pjw(name);
	printf("search index: %d\n", index);
	varElement *p = hashTable[index];
	//if (p == NULL) printf("null\n");
	while (p != NULL) {
		if (strcmp(p->name, name)==0 && p->layer_no == layer)
			//名称和层数都相同，表示被定义过
			return p;
		p = p->slot_next;
	}
	return NULL;
}

//进入更深的一层
void into_a_layer() {
	layer++;
}

//从较深的一层中出来，把这层中的所有变量都删除掉 
void out_of_a_layer() {
	varElement *p = layerStack[layer];
	if (p == NULL) {
		layer--;
		return;
	}
	varElement *q = NULL;	//指向同一层链表中p的下一个
	while (p != NULL) {
		q = p->layer_next;
		int slotno = p->slot_no;
		hashTable[slotno] = p->slot_next;
		free(p);
		p = q;
	}
	layer--;
}
