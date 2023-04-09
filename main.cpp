#include "include/includeAll.h"

int workbenchMapper[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 多出一个是为了和workbenchType对齐 balanceMap
int balanceMapper[7] = {0, 0, 0, 0, 1, 1, 1};             // 工作台类型4，5，6的工作台均衡生产
int leftFrame = 15000;
Pools pools;

#ifdef W_DEBUG
#include <Windows.h>

void showDialog()
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
}
#endif

using namespace std;
static bool showTime = true;

int main(int argc, char *argv[])
{
#ifdef W_DEBUG
    showDialog();
#endif
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    Context ctx;
    ctx.init();
    ctx.endStep();
    while (ctx.getFrameId() < TOTAL_FRAME)
    {
        start = chrono::steady_clock::now();
        ctx.update();
        ctx.step();
        end = chrono::steady_clock::now();

        // 显示时间
        if (showTime)
        {
            double cost = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            if (cost > 10.0)
            {
                fprintf(stderr, "%.3fms at frame %d\n", cost, ctx.getFrameId());
                fflush(stderr);
            }
        }
    }
    return 0;
}
