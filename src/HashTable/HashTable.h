#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "hastable_iface.h"
#include "Array_AVLTrees.h"

/*
	Структура хеш-таблицы:
	- forest		- Структура "массив AVL деревьев"
	- forest.line	- Массив AVL деревьев
	- forest.len	- Число элементов в таблице (т.е в массиве)
	- forest.size	- Максимальная вместимость таблицы (т.е массива)
*/
struct ht {
	Array(AVLTree) forest;
};

#endif // __HASH_TABLE_H__
