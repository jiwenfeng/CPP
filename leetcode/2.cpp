#include <iostream>
#include <list>

using namespace std;

/*
 *Link : https://leetcode.com/problems/add-two-numbers/
 * */

struct ListNode
{
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL){}
};

class Solution
{
public:
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
	{
		ListNode *head = NULL, *i1 = NULL, *i2 = NULL;
		ListNode **i = &head;
		int flag = 0;
		for(i1 = l1, i2 = l2; i1 != NULL && i2 != NULL; i1 = i1->next, i2 = i2->next)
		{
			int n = flag + i1->val + i2->val;
			flag = n / 10;
			ListNode *node = new ListNode(n % 10);
			*i = node;
			i = &node->next;
		}
		for(; i1 != NULL; i1 = i1->next)
		{
			int n = flag + i1->val;
			flag = n / 10;
			ListNode *node = new ListNode(n % 10);
			*i = node;
			i = &node->next;
		}
		for(; i2 != NULL; i2 = i2->next)
		{
			int n = flag + i2->val;
			flag = n / 10;
			ListNode *node = new ListNode(n % 10);
			*i = node;
			i = &node->next;
		}
		if(flag != 0)
		{
			ListNode *node = new ListNode(flag);
			*i = node;
			i = &node->next;

		}
		return head;
	}
};
