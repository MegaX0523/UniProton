#include "filter.h"

static void static_deque_init(StaticDeque *deque);
static DequeNode *static_node_alloc(double data);
static int deque_push_front(StaticDeque *deque, double data);

static DequeNode node_pool[MAX_DEQUE_SIZE];
static int node_index = 0;
static double W[LMS_M] = {0};      // 滤波器系数
static double W_sec[LMS_M] = {0};  // 次级通道参数
static double MU_current = MU_MAX; // 步长参数

static StaticDeque LMS_Deque = {0};

void FilterInit(void)
{
    node_index = 0;
    for (int i = 0; i < MAX_DEQUE_SIZE; i++)
    {
        node_pool[i].prev = NULL;
        node_pool[i].next = NULL;
        node_pool[i].data = 0;
    }
    for (int i = 0; i < LMS_M; i++)
    {
        W[i] = 0;
    }
    for (int i = 0; i < LMS_M; i++)
    {
        W_sec[i] = 0;
    }
}

/* 静态队列操作函数 */
static void static_deque_init(StaticDeque *deque)
{
    deque->front = NULL;
    deque->rear = NULL;
    deque->size = 0;
}

static DequeNode *static_node_alloc(double data)
{
    if (node_index >= MAX_DEQUE_SIZE)
        node_index = 0;
    DequeNode *node = &node_pool[node_index++];
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

static int deque_push_front(StaticDeque *deque, double data)
{
    if (deque->size >= LMS_M)
    {
        DequeNode *old = deque->rear;
        if (deque->size > 1)
        {
            deque->rear = old->prev;
            deque->rear->next = NULL;
        }
        else
        {
            deque->front = deque->rear = NULL;
        }
        deque->size--;
    }
    DequeNode *node = static_node_alloc(data);
    if (!node)
    {
        return -1;
    }
    if (deque->size == 0)
    {
        deque->front = deque->rear = node;
    }
    else
    {
        node->next = deque->front;
        deque->front->prev = node;
        deque->front = node;
    }
    deque->size++;
    return 0;
}

void W_update(double err_signal)
{
    DequeNode *current = LMS_Deque.front;
    for (int i = 0; i < LMS_M && current; i++)
    {
        W[i] += MU_current * err_signal * current->data;
        current = current->next;
    }
}

double output_get(double ref_signal)
{
    static bool initialized = 0;
    if (!initialized)
    {
        static_deque_init(&LMS_Deque);
        initialized = 1;
    }
    double sum = 0;
    int i = 0;
    if (deque_push_front(&LMS_Deque, ref_signal) != 0)
    {
        return 0; // 错误处理
    }
    DequeNode *current = LMS_Deque.front;
    for (i = 0; i < LMS_M && current; i++)
    {
        sum += current->data * W[i];
        current = current->next;
    }
    return sum;
}