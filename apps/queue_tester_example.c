#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Test #1: Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Test #2: Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t que;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	que = queue_create();
	queue_enqueue(que, &data);
	queue_dequeue(que, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* Test #3: Enqueue/Dequeue with different C datatypes */
void test_enqueue_different_datatype(void)
{
	queue_t que;
	int num = 4;
	char str = 'h';
	float decimal = 2.5;
    //size_t i;
    int *ptr;

	fprintf(stderr, "*** TEST enqueue_different_datatype ***\n");

	que = queue_create();
	queue_enqueue(que, &num);
	queue_enqueue(que, &str);
	queue_enqueue(que, &decimal);

	queue_dequeue(que, (void**)&ptr);
	TEST_ASSERT(ptr == &num);

	queue_dequeue(que, (void**)&ptr);
	TEST_ASSERT((char*)ptr == &str);

	queue_dequeue(que, (void**)&ptr);
	TEST_ASSERT((float*)ptr == &decimal);
}

/* Test #4: Test queue_destroy */
void test_destroy(void)
{
	queue_t que;
	int data[] = {1, 2};
	size_t i;
	int *ptr;

	fprintf(stderr, "*** TEST destroy***\n");

	/* Create queue and assign data[] values to the queue */
 	que = queue_create();
 	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
    	queue_enqueue(que, &data[i]);
	}

	/* Test for Fail case */
	TEST_ASSERT(queue_destroy(que) == -1);

	/* Test for Success case */
	queue_dequeue(que, (void**)&ptr);
	queue_dequeue(que, (void**)&ptr);
	TEST_ASSERT(queue_destroy(que) == 0);
	que = NULL;

	/* If destroyed, can't enqueue/dequeue */
	TEST_ASSERT(queue_enqueue(que, &data[0]) == -1);
}

/* Test #5: Test queue_length */
void test_length(void)
{
	queue_t que;
    int data[] = {34, 22, 13};
    size_t i;
    int *ptr;

	fprintf(stderr, "*** TEST length ***\n");

	// /* Test un-initialized queue length test */
	// TEST_ASSERT(queue_length(que) == -1);

	/* Test empty queue length condition */
    que = queue_create();
	TEST_ASSERT(queue_length(que) == 0);


	/* Test filled queue length condition */
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        queue_enqueue(que, &data[i]);
	}
	TEST_ASSERT((size_t)queue_length(que) == i);

	/* Test destroyed queue length */
	queue_dequeue(que, (void**)&ptr);
	queue_dequeue(que, (void**)&ptr);
	queue_dequeue(que, (void**)&ptr);
	queue_destroy(que);
	que = NULL;
	TEST_ASSERT(queue_length(que) == -1);
}

/* Test #6: Test queue_delete */
void test_delete(void)
{
	queue_t que;
	int a = 2;
	int b = 4; /* We will try to delete 'b' from the queue */
	int c = 3;
	int d = 5;

	fprintf(stderr, "*** TEST delete ***\n");

	/* Test data is NULL condition */
	// TEST_ASSERT(queue_delete(que, NULL) == -1);

	/* Test queue not initialized condition */
	// TEST_ASSERT(queue_delete(que, &b) == -1);

	/* Initialize queue as: a->b->c->b-> */
	que = queue_create();
	queue_enqueue(que, &a);
	queue_enqueue(que, &b);
	queue_enqueue(que, &c);
	queue_enqueue(que, &b);

	/* Test delete first 'b' from queue to form queue: a->c->b-> */
	TEST_ASSERT(queue_delete(que, &b) == 0);

	/* Test delete second 'b' from queue to form queue: a->c-> */
	TEST_ASSERT(queue_delete(que, &b) == 0);

	/* Test delete when data is not in queue */
	TEST_ASSERT(queue_delete(que, &b) == -1);
	TEST_ASSERT(queue_delete(que, &d) == -1);

}

/* Callback function that increments integer items by a certain value (or delete
 * item if item is value 42) */
static int inc_item(queue_t q, void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(queue_t q, void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;
    (void)q; //unused

    if (*a == match)
        return 1;

    return 0;
}

/* Test #7: Test iterator based on prof's example */
void test_iterator_prof(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;
    int *ptr;

	fprintf(stderr, "*** TEST interator_prof ***\n");

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        queue_enqueue(q, &data[i]);
	}

    /* Add value '1' to every item of the queue, delete item '42' */
    queue_iterate(q, inc_item, (void*)1, NULL);
    TEST_ASSERT(data[0] == 2);
    TEST_ASSERT(queue_length(q) == 9);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;     // result pointer *must* be reset first
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    TEST_ASSERT(ptr != NULL);
    TEST_ASSERT(*ptr == 5);
    TEST_ASSERT(ptr == &data[3]);
}

/* Callback function deletes all elements lower than the argument */
static int remove_under(queue_t que, void *data, void *arg)
{
    int *a = (int*)data;
    int number = (int)(long)arg;

	if(*a < number){
		queue_delete(que, data);
	}

	return 0;
}


/* Test #8: Test iteratre's resistance to items being deleted from queue*/
void test_interate_resistance(void)
{
	queue_t q;
	int data[] = {8, 11, 5, 4, 5, 11, 6, 17, 8, 9};
	size_t i;

	fprintf(stderr, "*** TEST interator_resistance ***\n");
	
	/* Initialize the queue and enqueue items */
	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
	    queue_enqueue(q, &data[i]);
	}

	/* Remove all values under '10', should be left with {11, 11, 17} = 3*/
	queue_iterate(q, remove_under, (void*)10, NULL);
	TEST_ASSERT(queue_length(q) == 3);

}

int main(void)
{
	test_create();						/* Test #1*/
	test_queue_simple();				/* Test #2*/
	test_enqueue_different_datatype();	/* Test #3*/
	test_destroy();						/* Test #4*/
	test_length();						/* Test #5*/
	test_delete();						/* Test #6*/
	test_iterator_prof();				/* Test #7*/
	test_interate_resistance();			/* Test #8*/
	return 0;
}
