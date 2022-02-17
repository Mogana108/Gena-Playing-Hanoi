#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

typedef struct _QueueElement {
    int move;
    int state;
} QueueElement;


int N;


#define QUEUE_SIZE (1024*1024*16)
QueueElement queue[QUEUE_SIZE];
char enqueued[QUEUE_SIZE];
int queueCount = 0;
int queueStart = 0;

int emptyQueue()
{
    return (queueCount == 0);
}


void pushQueue(int move, int state)
{
    int index = queueStart + queueCount;
    if (index >= QUEUE_SIZE) {
        index -= QUEUE_SIZE;
    }
    if (enqueued[state] != move) {
        queue[index].move  = move;
        queue[index].state = state;
        enqueued[state] = move;
        queueCount += 1;
    }
}


int popQueue(int *move)
{
    int res = queue[queueStart].state;
    *move = queue[queueStart].move;
    queueStart += 1;
    if (queueStart >= QUEUE_SIZE) {
        queueStart -= QUEUE_SIZE;
    }
    queueCount -= 1;
    return res;
}


void genMoves(int state, int *moves, int *movesCount)
{
    int rod;
    int i = 0;
    int r[4] = { -1, -1, -1, -1 };
    int tmp = state;
    while (tmp) {
        rod = tmp & 0x3;
        if (r[rod] < 0) {
            r[rod] = i;
        }
        tmp >>= 2;
        i += 1;
    }
    *movesCount = 0;
    if (r[0] >= 0) {
        if (r[0] < r[1] || r[1] < 0) {
            moves[*movesCount] = state | (1 << (r[0] * 2));
            *movesCount += 1;
        }
        if (r[0] < r[2] || r[2] < 0) {
            moves[*movesCount] = state | (2 << (r[0] * 2));
            *movesCount += 1;
        }
        if (r[0] < r[3] || r[3] < 0) {
            moves[*movesCount] = state | (3 << (r[0] * 2));
            *movesCount += 1;
        }
    }
    if (r[1] >= 0) {
        if (r[1] < r[0] || r[0] < 0) {
            moves[*movesCount] = state & (~(1 << (r[1] * 2)));
            *movesCount += 1;
        }
        if (r[1] < r[2] || r[2] < 0) {
            moves[*movesCount] = state & (~(1 << (r[1] * 2)));
            moves[*movesCount] |= (2 << (r[1] * 2));
            *movesCount += 1;
        }
        if (r[1] < r[3] || r[3] < 0) {
            moves[*movesCount] = state | (3 << (r[1] * 2));
            *movesCount += 1;
        }
    }
    if (r[2] >= 0) {
        if (r[2] < r[0] || r[0] < 0) {
            moves[*movesCount] = state & (~(2 << (r[2] * 2)));
            *movesCount += 1;
        }
        if (r[2] < r[1] || r[1] < 0) {
            moves[*movesCount] = state & (~(2 << (r[2] * 2)));
            moves[*movesCount] |= (1 << (r[2] * 2));
            *movesCount += 1;
        }
        if (r[2] < r[3] || r[3] < 0) {
            moves[*movesCount] = state | (3 << (r[2] * 2));
            *movesCount += 1;
        }
    }
    if (r[3] >= 0) {
        if (r[3] < r[0] || r[0] < 0) {
            moves[*movesCount] = state & (~(3 << (r[3] * 2)));
            *movesCount += 1;
        }
        if (r[3] < r[1] || r[1] < 0) {
            moves[*movesCount] = state & (~(3 << (r[3] * 2)));
            moves[*movesCount] |= (1 << (r[3] * 2));
            *movesCount += 1;
        }
        if (r[3] < r[2] || r[2] < 0) {
            moves[*movesCount] = state & (~(3 << (r[3] * 2)));
            moves[*movesCount] |= (2 << (r[3] * 2));
            *movesCount += 1;
        }
    }
}


int main()
{
    int i;
    for (i = 0; i < QUEUE_SIZE; ++i) {
        enqueued[i] = -1;
    }
    scanf("%d", &N);
    {
        int state = 0;
        int tmp;
        for (i = 0; i < N; ++i) {
            scanf("%d", &tmp);
            state |= (tmp - 1) << (i * 2);
        }
        pushQueue(0, state);
    }

    {
        int state;
        int move;
        int moves[6];
        int movesCount;

        while (! emptyQueue()) {
            state = popQueue(&move);
            if (! state) {
                break;
            }

            genMoves(state, moves, &movesCount);
            for (i = 0; i < movesCount; ++i) {
                pushQueue(move + 1, moves[i]);
            }
        }

        printf("%d\n", move);
    }

    return 0;
}
