#include <iostream>
#include <ctime>
// 方便调试
#include <signal.h>
#include <unistd.h>

#include "include/includeAll.h"

using namespace std;

static bool showTime = false;
int main()
{

    // kill(getpid(), SIGSTOP);
    clock_t start, end;

    Context ctx;
    ctx.init();
    ctx.step(true); // 预热

    while (ctx.getFrameId() < TOTAL_FRAME)
    {
        start = clock();
        ctx.update();
        ctx.step(false);
        end = clock();

        // 显示时间
        if (showTime)
        {
            double cost = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            fprintf(stderr, "%.3fms at frame %d\n", cost, ctx.getFrameId());
            fflush(stderr);
        }
    }
    return 0;
}
