#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
// #include <bits/sockaddr.h>

int main(int argc, char const *argv[])
{
    // 创建套接字
    // 1.域
    // 2.类型
    // 3.协议
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 连接服务器
    // 1.套接字描述符
    // 2.目标地址
    // 3.目标地址长度
    struct sockaddr_un addr;
    // struct sockaddr_in addr1;
    // struct sockaddr_in6 addr2;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "my_server");
    int result = connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (result == -1)
    {
        printf("连接失败...\n");
        return EXIT_FAILURE;
    }

    char filename[50];
    char string[1024];
    memset(string, 0, sizeof(string));

    size_t size;

    printf("要发送的文件: ");
    scanf("%s",filename);
    FILE *fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END);  // 将文件指针移动到文件的末尾
    long str_len = ftell(fp);  // 获取当前文件指针距离文件开头的位置
    printf("文件大小:%ld\n",str_len);

    fseek(fp, 0, SEEK_SET);
    while ((size = fread(string, 1, 1024, fp)) > 0)
    {
        write(socket_fd, string, size);
        printf("发送内容长度:%ld\n",strlen(string));
        memset(string, 0, sizeof(string));
    }

    printf("当前文件传输完毕...\n");

    fclose(fp);
    // 关闭套接字
    close(socket_fd);

    return 0;
}
