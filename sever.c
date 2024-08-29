#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
// #include <bits/sockaddr.h>

int main(int argc, char const *argv[])
{
    int result;

    // 创建套接字
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "my_server");

    // int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // struct sockaddr_in addr2;
    // addr2.sin_port = 9000;
    // addr2.sin_addr = "localhost";
    // addr2.sin_family = AF_INET;

    // 绑定地址
    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    // 监听
    listen(server_fd, 9);
    printf("服务器启动...\n");

    struct sockaddr_un addr_client;
    addr_client.sun_family = AF_UNIX;
    int addr_len;
    while (1)
    {
        // 接收请求，建立了 TCP 连接，获得了一个新的客户端套接字
        printf("等待客户端请求...\n");
        int client_fd = accept(
            server_fd,
            (struct sockaddr *)&addr_client,
            &addr_len
            // sizeof(addr_client)
        );

        char filename[99];
        sprintf(filename, "%ld.f", time(NULL));

        FILE *fp = fopen(filename, "a");

        // 读写
        char buf[1024];
        memset(buf, 0, sizeof(buf));

        size_t size;
        while ( 0 != (size = read(client_fd, buf, 1024)))
        {
            printf("接收到长度:%ld\n",size);
            fwrite(buf, 1, size, fp);
            memset(buf, 0, sizeof(buf));
        }
        printf("接收文件复制到文件---%s\n", filename);

        fclose(fp);
    }

    return 0;
}
