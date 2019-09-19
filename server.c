#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define PORT 5000

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t success_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t gotowork_cond = PTHREAD_COND_INITIALIZER;

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */
    i = 0;
    do
    {                          /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0);   /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void *print_threads(void *threadid)
{

    long tid;
    tid = (long)threadid;
    printf("Thread ID : %ld\n", tid);
    return;
}

void thread_function(void *new_socket_1)
{

    int sizee = 0;
    char buffer[10240] = {0};
    int valread, new_socket;

    new_socket = (int)new_socket_1;

    printf("Reading data from client. \n");

    pthread_mutex_lock(&mutex1);

    // pthread_cond_wait(&gotowork_cond,&mutex1);

    valread = read(new_socket, buffer, 1024);

    printf("buffer : %s\n", buffer);

    char temp[1024] = {0};

    sizee = strlen(buffer);

    printf("size : %d\n", sizee);

    itoa(sizee, temp);

    char *message = temp;

    send(new_socket, message, strlen(message), 0);

    // pthread_cond_signal(&success_cond);

    pthread_mutex_unlock(&mutex1);

    // printf("Message Sent\n");

    // pthread_join(threads,NULL);

    // pthread_exit(NULL);
}

int main()
{

    int NTHREAD;
    printf("Input number of threads : ");
    scanf("%d", &NTHREAD);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char *message = "I accept your message.";
    int valread;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[NTHREAD];

    int rc[NTHREAD];

    int i = 0;

    clock_t start = clock();

    for(int ll=0;ll<1000;ll++)
    {

        if (listen(server_fd, 1000) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        rc[i] = pthread_create(&threads[i], NULL, thread_function, (void *)new_socket);

        i += 1;

        if (i > NTHREAD)
        {
            i = 0;
        }

        // if (rc){
        //     printf("Error to create thread\n");
        //     exit(-1);
        // }
    }

    for (int i = 0; i < NTHREAD; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    float time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("%d , %f\n",NTHREAD,time_spent);

    exit(EXIT_SUCCESS);
}
