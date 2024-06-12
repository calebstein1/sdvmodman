#define malloc_or_fail(v, n, s) if (!(v = malloc(n * s))) { perror("malloc"); exit(-1); }

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>

#include "config.h"

struct mod_t {
    char e_name[1024];
    char d_name[1024];
    int id;
    bool enabled;
};

void print_mods(struct mod_t *mods, int mods_count) {
    int i = 0;
    printf("\nWelcome to SDV Mod Manager\n\nMods Enabled:\n");
    for (; i < mods_count; i++) {
        if (mods[i].enabled) printf("    %d. %s\n", mods[i].id, mods[i].e_name);
    }

    printf("\nMods Disabled:\n");
    for (i = 0; i < mods_count; i++) {
        if (!mods[i].enabled) printf("    %d. %s\n", mods[i].id, mods[i].d_name + 1);
    }
    printf("\n");
}

int get_input(int *selected_mod) {
    char buff[5] = {};
    fgets(buff, 5, stdin);
    if (*buff == 'q') return 0;

    *selected_mod = atoi(buff);
    return 1;
}

int main() {
    DIR *mod_dir = NULL;
    struct dirent *m = NULL;
    struct mod_t *mods = NULL;
    int i, mods_count, selected_mod;
    i = mods_count = selected_mod = 0;

    if (!(mod_dir = opendir(MOD_PATH))) {
        perror("opendir");
        return -1;
    }

    while ((m = readdir(mod_dir))) {
        mods_count++;
    }
    malloc_or_fail(mods, mods_count, sizeof(struct mod_t));

    rewinddir(mod_dir);

    while ((m = readdir(mod_dir))) {
        if (strcmp(m->d_name, ".") == 0 || strcmp(m->d_name, "..") == 0) {
            mods_count--;
            continue;
        }
        mods[i].id = i + 1;
        mods[i].enabled = *(m->d_name) == '.' ? false : true;
        if (mods[i].enabled) {
            strcpy(mods[i].e_name, m->d_name);
            snprintf(mods[i].d_name, 1024, ".%s", m->d_name);
        } else {
            strcpy(mods[i].d_name, m->d_name);
            strcpy(mods[i].e_name, m->d_name + 1);
        }
        i++;
    }
    closedir(mod_dir);

    print_mods(mods, mods_count);

    for (;;) {
        printf("Enter the number for the mod you'd like to toggle (q to quit): ");
        if (!get_input(&selected_mod)) break;

        if (selected_mod < 1 || mods_count < selected_mod) {
            printf("Please enter a number between 1 and %d\n", mods_count);
            continue;
        }
        selected_mod--;

        char old[2048] = {};
        char new[2048] = {};

        if (mods[selected_mod].enabled) {
            snprintf(old, 2048, "%s%s", MOD_PATH, mods[selected_mod].e_name);
            snprintf(new, 2048, "%s%s", MOD_PATH, mods[selected_mod].d_name);
        } else {
            snprintf(old, 2048, "%s%s", MOD_PATH, mods[selected_mod].d_name);
            snprintf(new, 2048, "%s%s", MOD_PATH, mods[selected_mod].e_name);
        }
        rename(old, new);
        mods[selected_mod].enabled = mods[selected_mod].enabled ? false : true;

        print_mods(mods, mods_count);
    }

    free(mods);
    return 0;
}
