#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <hiredis.h>


int hostPort;
char * hostName;
char * queueName;
char * maxWait;

static int verbose_flag;

int deque(redisContext * ctx, const char * queue, const char * wait,
          char * into, size_t max);

int enque(redisContext * ctx, const char * queue, const int argc,
          const char ** argv);

void help(const int optlen, const struct option opts[]);

int
main(int argc, char ** argv)
{
    char * port;

    if ((port = getenv("REDIS_PORT")) == NULL) {
        hostPort = 6379;

    } else {
        hostPort = atoi(port);
    }

    if ((hostName = getenv("REDIS_HOST")) == NULL) {
        hostName = "localhost";
    }

    if ((queueName = getenv("QTT_QUEUE")) == NULL) {
        queueName = "qtt:job:queue";
    }

    if ((maxWait = getenv("QTT_BLOCK_WAIT")) == NULL) {
        maxWait = "3";
    }

    int optcnt = 0;
    static int help_flag = 0;
    static struct option long_options[] = {
        {"help", no_argument, &help_flag, 1},
        {"verbose", no_argument, &verbose_flag, 1},
        {"port", required_argument, 0, 'p'},
        {"host", required_argument, 0, 'h'},
        {"queue", required_argument, 0, 'q'},
        {"wait", required_argument, 0, 'w'},
    };

    while(1) {
        int option_index = 0;
        int c = getopt_long_only(argc, argv, "p:h:q:w:", long_options, &option_index);

        if(c == -1) {
            break;
        }

        optcnt++;

        switch(c) {
        case 0:
            if(long_options[option_index].flag != 0) {
                break;
            }

            fprintf(stderr, "option %s", long_options[option_index].name);

            if(optarg) {
                fprintf(stderr, " with arg %s", optarg);
            }

            fprintf(stderr, "\n");
            break;

        case 'h':
            hostName = optarg;
            optcnt++;
            break;

        case 'p':
            hostPort = atoi(optarg);
            optcnt++;
            break;

        case 'w':
            maxWait = optarg;
            optcnt++;
            break;

        case 'q':
            queueName = optarg;
            optcnt++;
            break;

        case '?':
            help_flag = 1;
            break;

        default:
            fprintf(stderr, "suprise! c is %c\n", c);
            abort();
        }
    }

    if (help_flag) {
        help(sizeof(long_options) / sizeof(long_options[0]), long_options);
        return -1;
    }

    int remaining = argc - 1 - optcnt;
    char * vals[remaining];

    if(verbose_flag) {
        fprintf(stderr, "Remaining args %d\n", remaining);
    }

    if (remaining > 0) {
        int ix, iv = 0;

        for(ix = optind; ix < argc; ix++) {
            if(argv[ix] && argv[ix][0] == '-') {
                if(verbose_flag) {
                    fprintf(stderr, "unexpected short option '%s'\n", argv[ix]);
                }

                remaining--;
                continue;
            }

            vals[iv++] = argv[ix];
        }
    }

    if(verbose_flag) {
        fprintf(stderr, "Connecting to %s:%d - q:%s - w:%s\n", hostName, hostPort,
                queueName, maxWait);
    }

    redisContext  * ctx = redisConnect(hostName, hostPort);

    if (ctx == NULL || ctx->err) {
        if (ctx) {
            fprintf(stderr, "connection error: %s\n", ctx->errstr);
            redisFree(ctx);

        } else {
            fprintf(stderr, "connection error: unable to allocate context\n");
        }

        return 1;
    }

    int success = -1;

    if (remaining == 0) {
        const int max = 255;
        char buf[max];

        if(deque(ctx, queueName, maxWait, buf, max) == 0) {
            fprintf(stdout, "%s\n", buf);
            success = 0;
        }

    } else {
        success = enque(ctx, queueName, remaining, (const char **)&vals);
    }

    redisFree(ctx);
    return success;
}

int
deque(
    redisContext * ctx, const char * queue, const char * wait,
    char * into, size_t max)
{
    int errorCode = -1;
    const char * action[] = {"BLPOP", queue, wait};
    redisReply * reply = redisCommandArgv(ctx, 3, action, NULL);

    if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 2) {
        if( snprintf(into, max, "%s", reply->element[1]->str) > 0) {
            errorCode = 0;
        }
    }

    freeReplyObject(reply);
    return errorCode;
}

int
enque(redisContext * ctx, const char * queue,
      const int argc,  const char ** argv)
{
    int errorCode = -1;
    const char * action[] = {"RPUSH", queue, ""};
    redisReply * reply;

    for(int i = 0; i < argc; i++) {
        action[2] = argv[i];
        redisAppendCommandArgv(ctx, 3, action, NULL);
    }

    redisGetReply(ctx, (void **)&reply);

    switch (reply->type) {
    case REDIS_REPLY_INTEGER:
        errorCode = 0;
        break;

    default:
        errorCode = -1;
        break;
    }

    freeReplyObject(reply);
    return errorCode;
}

void
help(const int optlen, const struct option opts[])
{
    fprintf(stderr, "Usage:\n\n");

    for(int ix = 0; ix < optlen; ix++) {
        fprintf(stderr, "  --%s\n", opts[ix].name);
    }
}
