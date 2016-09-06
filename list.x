struct node{
	int x;
	int y;
	node* next;
};

program LIST{
	version LIST_OP{
		int add_list(node) = 1;
	} = 1;
} = 0x2fffffff;