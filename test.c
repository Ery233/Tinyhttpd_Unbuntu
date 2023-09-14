#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 创建管道用于与 CGI 脚本通信
    int cgi_output[2];
    int cgi_input[2];
    pid_t pid;
    char buf[1024];
    int status;
    // 创建管道
    if (pipe(cgi_output) < 0 || pipe(cgi_input) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // 子进程，执行 CGI 脚本
        // 关闭管道不用的一端
        close(cgi_output[1]);
        close(cgi_input[0]);

        // 重定向标准输入和输出
        dup2(cgi_output[0], 0);
        dup2(cgi_input[1], 1);

        // 设置环境变量，模拟 CGI 脚本执行时的环境
        putenv("REQUEST_METHOD=GET");
        putenv("QUERY_STRING=param=value");

        // 执行 CGI 脚本
        execl("htdocs/check.cgi", "check.cgi", NULL);
        exit(EXIT_FAILURE);
    }
    else {  // 父进程，与 CGI 脚本通信
        // 关闭管道不用的一端
        close(cgi_output[0]);
        close(cgi_input[1]);

        // 向 CGI 脚本发送数据
        write(cgi_output[1], "input_data", strlen("input_data"));
        close(cgi_output[1]);

        // 从 CGI 脚本读取输出
        while (read(cgi_input[0], buf, sizeof(buf)) > 0) {
            printf("%s", buf);  // 打印 CGI 脚本的输出
        }
        close(cgi_input[0]);

        // 等待子进程退出
        while (true)
        {

        }
    }

    return 0;
}