#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

unsigned long simple_hash(const char *s) {
    unsigned long h = 5381;
    int c;

    while ((c = *s++))
        h = ((h << 5) + h) ^ c;

    return h;
}

int main(void) {
    const char *user = getlogin();
    if (!user) {
        fprintf(stderr, "Nelze zjistit uživatele.\n");
        return 1;
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("gethostname");
        return 1;
    }

    uid_t uid = getuid();

    /* spojení údajů */
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%s:%s:%d", user, hostname, uid);

    unsigned long hash = simple_hash(buffer);

    /* název souboru */
    char path[512];
    snprintf(path, sizeof(path),
             "%s/.task_%lx.dat",
             getenv("HOME"),
             hash);

    FILE *f = fopen(path, "w");
    if (!f) {
        perror("fopen");
        return 1;
    }

    fprintf(f,
        "Student: %s\n"
        "Host: %s\n"
        "UID: %d\n"
        "Hash: %lx\n",
        user, hostname, uid, hash);

    fclose(f);

    printf("FLAG{}%lx\n", hash);
    return 0;
}
