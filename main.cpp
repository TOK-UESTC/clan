#include <iostream>
#include <ctime>
// 方便调试
#include <signal.h>
#include <unistd.h>
#include <Windows.h>

#include "include/includeAll.h"

using namespace std;

static bool showTime = false;
int main()
{
    // 打开调试器附加对话框
    LPCWSTR waitMessage = L"等待附加调试器";
    LPCWSTR title = L"调试器";

    int len = WideCharToMultiByte(CP_ACP, 0, waitMessage, -1, NULL, 0, NULL, NULL);
    char *message = new char[len];
    WideCharToMultiByte(CP_ACP, 0, waitMessage, -1, message, len, NULL, NULL);

    len = WideCharToMultiByte(CP_ACP, 0, title, -1, NULL, 0, NULL, NULL);
    char *caption = new char[len];
    WideCharToMultiByte(CP_ACP, 0, title, -1, caption, len, NULL, NULL);

    MessageBox(nullptr, message, caption, MB_OK);

    delete[] message;
    delete[] caption;
    clock_t start, end;

    Context ctx;
    ctx.init();
    ctx.endStep(); // 预热
    while (ctx.getFrameId() < TOTAL_FRAME)
    {
        // start = clock();
        ctx.update();
        ctx.step(false);
        // end = clock();

        // 显示时间
        // if (showTime)
        // {
        //     double cost = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
        //     fprintf(stderr, "%.3fms at frame %d\n", cost, ctx.getFrameId());
        //     fflush(stderr);
        // }
    }
    return 0;
}
