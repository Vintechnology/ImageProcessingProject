#pragma once
struct StrNode {
	const char* value;
	StrNode* next;
	StrNode* prev;
};

struct LinkedStrList 
{
	StrNode* head;
	StrNode* tail;
	int size;
};

namespace LinkedList 
{
	void Init(LinkedStrList* list) 
	{
		list->head = nullptr;
		list->tail = nullptr;
		list->size = 0;
	}

	void AddTail(LinkedStrList* list, const char* value) 
	{
		StrNode* p = new StrNode;
		p->value = value;

		if (list->size == 0) 
		{
			list->head = p;
			list->tail = p;
			p->prev = nullptr;
		}
		else 
		{
			p->prev = list->tail;
			list->tail->next = p;
			list->tail = p;
		}

		p->next = nullptr;
		list->size++;

	}

	void AddHead(LinkedStrList* list, const char* value) 
	{
		StrNode* p = new StrNode;
		p->value = value;

		if (list->size == 0) 
		{
			list->head = p;
			list->tail = p;
			p->next = nullptr;
		}
		else 
		{
			list->head->prev = p;
			p->next = list->head;
			list->head = p;
		}

		p->prev = nullptr;
		list->size++;
	}

	const char* Get(LinkedStrList* list ,StrNode* node)
	{
		if (node == list->head) 
		{
			list->head = node->next;
			if (list->head != nullptr)
				list->head->prev = nullptr;
		}
		else if (node == list->tail) 
		{
			list->tail = node->prev;
			if (list->tail != nullptr)
				list->tail->next = nullptr;
		}
		else 
		{
			StrNode* prevNode = node->prev;
			StrNode* nextNode = node->next;
			prevNode->next = nextNode;
			nextNode->prev = prevNode;
		}
		list->size--;

		const char* value = node->value;
		delete node;
		return value;
	}

	void Delete(LinkedStrList* list) 
	{
		while (list->head != nullptr) {
			StrNode* node = list->head;
			list->head = node->next;
			if(list->head != nullptr)
				list->head->prev = nullptr;

			delete node;
		}
		list->head = nullptr;
		list->tail = nullptr;
		delete list;
		list = nullptr;
	}
}
