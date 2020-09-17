#include "headers.h"
#include "macros.h"

static char* global_dir = ".";


struct Options
{
    bool using_a;
    bool using_d;
    bool using_h;
    bool using_i;
    bool using_l;
    bool using_p;
    bool using_Q;
    bool using_R;
    bool using_S;
    bool using_t;
    bool using_U;
};

void set_dir(char *path) {
	char cwd[SIZE];
	chdir(path);
	chdir("..");
	getcwd(global_dir, SIZE);
	// printf("%s\n", global_dir);
}

static void init_opts(struct Options* opts)
{
    opts->using_a = false;
    opts->using_d = false;
    opts->using_h = false;
    opts->using_i = false;
    opts->using_l = false;
    opts->using_p = false;
    opts->using_Q = false;
    opts->using_R = false;
    opts->using_S = false;
    opts->using_t = false;
    opts->using_U = false;
}

struct Options get_opts(int argc, char* argv[])
{
    struct Options opts;
    init_opts(&opts);
    int opt;
    while ((opt = getopt(argc, argv, "al")) != -1)
    {
        // printf("%c/", opt);
        switch (opt)
        {
            case 'a': opts.using_a = true; break;
            case 'l': opts.using_l = true; break;
            case '?': ;
            //exit(EX_USAGE);
        }
        // for (int i = 0; i < argc; ++i)
        //     printf("%s/", argv[i]);
        // printf("\n");
    }
    return opts;
}

static void print_permissions(mode_t mode)
{
    putchar((mode & S_IRUSR) ? 'r' : '-');
    putchar((mode & S_IWUSR) ? 'w' : '-');
    putchar((mode & S_IXUSR) ? 'x' : '-');
    putchar((mode & S_IRGRP) ? 'r' : '-');
    putchar((mode & S_IWGRP) ? 'w' : '-');
    putchar((mode & S_IXGRP) ? 'x' : '-');
    putchar((mode & S_IROTH) ? 'r' : '-');
    putchar((mode & S_IWOTH) ? 'w' : '-');
    putchar((mode & S_IXOTH) ? 'x' : '-');
}

static void print_filetype(mode_t mode)
{
    switch (mode & S_IFMT)
    {
        case S_IFREG: putchar('-'); break;
        case S_IFDIR: putchar('d'); break;
        case S_IFLNK: putchar('l'); break;
        case S_IFCHR: putchar('c'); break;
        case S_IFBLK: putchar('b'); break;
        case S_IFSOCK: putchar('s'); break;
        case S_IFIFO: putchar('f'); break;
    }
}

void readable_fs(double size, char* buf)
{
    const char* units[] = { "", "K", "M", "G", "T" };
    int i = 0;

    while (size > 1024)
    {
        size /= 1024;
        ++i;
    }

    sprintf(buf, "%.*f%s", i, size, units[i]);
}

void print_time(time_t mod_time)
{
    // get current time with year
    time_t curr_time;
    time(&curr_time);
    struct tm* t = localtime(&curr_time);
    const int curr_mon = t->tm_mon;
    const int curr_yr = 1970 + t->tm_year;

    // get mod time and year
    t = localtime(&mod_time);
    const int mod_mon = t->tm_mon;
    const int mod_yr = 1970 + t->tm_year;

    // determine format based on years
    const char* format = ((mod_yr == curr_yr)
                       && (mod_mon >= (curr_mon - 6)))
                           ? "%b %e %H:%M"
                           : "%b %e  %Y";

    char time_buf[128];
    strftime(time_buf, sizeof(time_buf), format, t);
    printf("%s", time_buf);
}

struct stat get_stats(const char* filename)
{
    char path[1024];
    sprintf(path, "%s/%s", global_dir, filename);
    struct stat sb;

    if (lstat(path, &sb) < 0)
    {   
        perror(path);
       	;
        // exit(EX_IOERR);
    }

    return sb;
}

bool is_dir(const char* filename)
{
    struct stat sb = get_stats(filename);

    if (lstat(filename, &sb) < 0)
    {
        perror(filename);
        return false;
    }

    return (sb.st_mode & S_IFDIR) ? true : false;
}

bool is_in_dir(const char* dir, const char* filename)
{
    DIR* dfd = opendir(dir);

    if (!dfd)
    {
        perror(dir);
        return false;
    }

    struct dirent* dp = readdir(dfd);

    while (dp)
    {
        if (strcmp(filename, dp->d_name) == 0)
        {
            closedir(dfd);
            return true;
        }      

        dp = readdir(dfd);
    }

    fprintf(stderr, "file \'%s\' not found\n", filename);

    closedir(dfd);

    return false;
}

void print_name_or_link(const char* filename, struct Options opts, mode_t mode)
{
    if (mode & S_IFLNK)
    {
        char link_buf[512];
        int count = readlink(filename, link_buf, sizeof(link_buf));

        if (count >= 0)
        {
            link_buf[count] = '\0';

            if (opts.using_Q)
            {
                printf(" \"%s\" -> \"%s\"\n", filename, link_buf);
            }
            else
            {
                printf(" %s -> %s \n", filename, link_buf);
            }

            return;
        }
    }

    if (opts.using_Q)
    {
        printf(" \"%s\"", filename);
    }
    else
    {
        printf(" %s", filename);
    }

    if (opts.using_p && is_dir(filename))
    {
        putchar('/');
    }

    putchar('\n');
}

void display_stats(char* dir, char* filename, struct Options opts)
{
    if (!is_in_dir(dir, filename))
    {
        return;
    }

    if (!opts.using_l)
    {
        printf("%s\n", filename);
        return;
    }

    global_dir = dir;

    struct stat sb = get_stats(filename);

    if (opts.using_i)
    {
        printf("%ld ", (long)sb.st_ino);
    }

    print_filetype(sb.st_mode);
    print_permissions(sb.st_mode);
    printf(" %ld ", sb.st_nlink);
    printf("%10s ", getpwuid(sb.st_uid)->pw_name);
    printf("%10s", getgrgid(sb.st_gid)->gr_name);

    if (opts.using_h)
    {
        char buf[10];
        readable_fs(sb.st_size, buf);
        printf(" %8s ", buf);
    }
    else
    {
        printf("%10ld ", (long)sb.st_size);
    }

    print_time(sb.st_mtime);
    print_name_or_link(filename, opts, sb.st_mode);
}

bool can_recurse_dir(const char* parent, char* curr)
{
    if (!strcmp(".", curr) || !strcmp("..", curr))
    {
        return false;
    }

    char path[2048];
    sprintf(path, "%s/%s", parent, curr);
    struct stat sb;

    if (lstat(path, &sb) < 0)
    {
        perror(path);
        return NULL;
        // exit(EX_IOERR);
    }

    return S_ISDIR(sb.st_mode);
}

void recurse_dirs(char* dir, struct Options opts)
{
    DIR* dfd = opendir(dir);
    struct dirent* dp = readdir(dfd);

    printf("\n%s:\n", dir);

    while ((dp = readdir(dfd)))
    {
        const bool omit_hidden = !opts.using_a && dp->d_name[0] == '.';

        if (!omit_hidden)
        {
            if (opts.using_l)
            {
                display_stats(dir, dp->d_name, opts);
            }
            else
            {
                printf("%s\n", dp->d_name);
            }
        }

        if (can_recurse_dir(dir, dp->d_name))
        {
            char next[1024];
            sprintf(next, "%s/%s", dir, dp->d_name);
            recurse_dirs(next, opts);
        }
    }

    closedir(dfd);
}

static int cmp_lex(const void* p1, const void* p2)
{
    const char* str1 = *(const void**)p1;
    const char* str2 = *(const void**)p2;

    return strcasecmp(str1, str2);
}

static int cmp_time(const void* p1, const void* p2)
{
    const char* str1 = *(const char**)p1;
    const char* str2 = *(const char**)p2;

    time_t time1 = get_stats(str1).st_mtime;
    time_t time2 = get_stats(str2).st_mtime;

    return time1 < time2;
}

static int cmp_size(const void* p1, const void* p2)
{
    const char* str1 = *(const char**)p1;
    const char* str2 = *(const char**)p2;

    long int size1 = get_stats(str1).st_size;
    long int size2 = get_stats(str2).st_size;

    return size1 < size2;
}

void display_dir(char* dir, struct Options opts)
{
    DIR* dfd = opendir(dir);
    struct dirent* dp = readdir(dfd);
    long curr_alloc_amt = 30000;
    char** dir_arr = malloc(curr_alloc_amt * sizeof(char*));

    if (!dir_arr)
    {
        abort();
    }

    long int count = 0;

    while (dp)
    {
        const bool omit_hidden = !opts.using_a && dp->d_name[0] == '.';

        if (!omit_hidden)
        {
            if (count >= curr_alloc_amt)
            {
                curr_alloc_amt *= 2;
                dir_arr = realloc(dir_arr, curr_alloc_amt * sizeof(char*));

                if (!dir_arr)
                {
                    abort();
                }
            }

            dir_arr[count] = dp->d_name;
            count++;
        }

        dp = readdir(dfd);
    }

    global_dir = dir;

    if (!opts.using_U && opts.using_t)
    {
        qsort(dir_arr, count, sizeof(char*), cmp_time);
    }
    else if (!opts.using_U && opts.using_S)
    {
        qsort(dir_arr, count, sizeof(char*), cmp_size);
    }
    else if (!opts.using_U)
    {
        qsort(dir_arr, count, sizeof(char*), cmp_lex);
    }

    for (long int i = 0; i < count; ++i)
    {
        display_stats(dir, dir_arr[i], opts);
    }

    closedir(dfd);
    free(dir_arr);
}

void scan_dirs(int count, char* args[], struct Options opts)
{
	// printf("%d %d\n", opts.using_l, opts.using_a);
    if (opts.using_d)
    {
        const bool no_dirs_given = (count - optind) == 0;

        if (no_dirs_given)
        {
            display_stats(".", ".", opts);
        }

        // loop through directories
        for (int i = optind; i < count; ++i)
        {
            display_stats(".", args[i], opts);
        }

        return;
    }

    // no other arguments
    if (!opts.using_R && (optind == count))
    {
        display_dir(".", opts);
    }

    if (opts.using_R && !opts.using_d)
    {
        recurse_dirs(".", opts);
        return;
    }

    const bool multiple_dirs = (count - optind) >= 2;

    // loop through directories
    for (int i = optind; i < count; ++i)
    {
        // if (!is_dir(args[i]))
        // {
        //     display_stats(".", args[i], opts);
        //     continue;
        // }

        // display directory name
        //   for multiple directories
        if (multiple_dirs)
        {
            printf("\n%s:\n", args[i]);
        }

        // if (!is_in_dir(".", args[i]))
        // {
        //     continue;
        // }
        global_dir = ".";
        set_dir(args[i]);
        display_dir(args[i], opts);
    }
}

void __init__(int argc, char* argv[]) {
    // int index = argc;
    // while (index <= 10)
    //     argv[index++] = NULL; 
}

void ls(int argc, char argv[][SIZE]) {
	global_dir = ".";
	optind = 1;
    char* first = ".";
    char* edit[SIZE];
    // argv[2] = "-a";
    // argv[2] = "willtryagain/consistent";
    // argc = 3;
    strcpy(argv[0], "./a.out");
    for (int i = 0; i < argc; ++i)
       edit[i] = argv[i];
    scan_dirs(argc, edit, get_opts(argc, edit));
    // printf("%s\n", global_dir);
}