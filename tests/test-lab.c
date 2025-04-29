#include "harness/unity.h"
#include "../src/lab.h"

// NOTE: Due to the multi-threaded nature of this project. Unit testing for this
// project is limited. I have provided you with a command line tester in
// the file app/main.cp. Be aware that the examples below do not test the
// multi-threaded nature of the queue. You will need to use the command line
// tester to test the multi-threaded nature of your queue. Passing these tests
// does not mean your queue is correct. It just means that it can add and remove
// elements from the queue below the blocking threshold.


void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}




void test_create_destroy(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    queue_destroy(q);
}

void test_queue_dequeue(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    enqueue(q, &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    queue_destroy(q);
}

void test_queue_dequeue_multiple(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    enqueue(q, &data);
    enqueue(q, &data2);
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);
    TEST_ASSERT_TRUE(dequeue(q) == &data3);
    queue_destroy(q);
}

void test_queue_dequeue_shutdown(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    enqueue(q, &data);
    enqueue(q, &data2);
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);
    queue_shutdown(q);
    TEST_ASSERT_TRUE(dequeue(q) == &data3);
    TEST_ASSERT_TRUE(is_shutdown(q));
    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

/* START NEW TESTS */

void test_is_empty(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    TEST_ASSERT_TRUE(is_empty(q));
    int data = 1;
    enqueue(q, &data);
    TEST_ASSERT_TRUE(!is_empty(q));
    dequeue(q);
    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

void test_is_shutdown(void)
{
  queue_t q = queue_init(10);
  TEST_ASSERT_TRUE(q != NULL);
  TEST_ASSERT_TRUE(!is_shutdown(q));
  int data = 1;
  enqueue(q, &data);
  TEST_ASSERT_TRUE(!is_shutdown(q));
  dequeue(q);
  TEST_ASSERT_TRUE(!is_shutdown(q));
  queue_shutdown(q);
  TEST_ASSERT_TRUE(is_shutdown(q));
  queue_destroy(q);
}

void test_null_safety(void)
{
  /* if these aren't null safe, they'll probably cause a segfault */
  TEST_ASSERT_NULL(queue_init(NULL));
  queue_destroy(NULL);
  enqueue(NULL, NULL);
  TEST_ASSERT_NULL(dequeue(NULL));
  queue_shutdown(NULL);
  TEST_ASSERT_TRUE(is_empty(NULL));
  TEST_ASSERT_TRUE(is_shutdown(NULL));
}

/* END NEW TESTS */

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_create_destroy);
  RUN_TEST(test_queue_dequeue);
  RUN_TEST(test_queue_dequeue_multiple);
  RUN_TEST(test_queue_dequeue_shutdown);
  printf("========== NEW TESTS ==========\n");
  RUN_TEST(test_is_empty);
  RUN_TEST(test_is_shutdown);
  RUN_TEST(test_null_safety);
  return UNITY_END();
}
