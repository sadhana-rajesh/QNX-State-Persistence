#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Worker: Velai paakura thread
void* worker_thread(void* arg) {
    while(1) {
        pthread_mutex_lock(&lock);
        counter++;
        printf("[Worker] Progress: %d%%\n", counter);
        pthread_mutex_unlock(&lock);
        delay(1000); // 1 second gap
    }
}

// Logger: Safe-ah save pannura thread
void* logger_thread(void* arg) {
    while(1) {
        delay(2000); // 2 second-ku oru thadava save pannum

        pthread_mutex_lock(&lock);
        int val_to_save = counter;
        time_t now;
                time(&now);
                struct tm *local = localtime(&now);
        pthread_mutex_unlock(&lock);
        // ATOMIC SAVE LOGIC
        FILE *tmp = fopen("state.tmp", "wb");
        if (tmp) {
            fwrite(&val_to_save, sizeof(int), 1, tmp);
            fflush(tmp);
            fsync(fileno(tmp)); // QNX-la idhu thaan real persistence
            fclose(tmp);
            rename("state.tmp", "system_state.bin"); // Atomic swap
            printf("[%02d:%02d:%02d] >> [Logger] State %d%% safely synced to disk.\n", local->tm_hour, local->tm_min, local->tm_sec, val_to_save);
        }
    }
}

int main() {
    pthread_t t1, t2;

    // RECOVERY: Start aagum pothu check pannum
    FILE *fp = fopen("system_state.bin", "rb");
    if(fp) {
        fread(&counter, sizeof(int), 1, fp);
        printf("!!! RECOVERED: Resuming from %d%% !!!\n", counter);
        fclose(fp);
    }

    pthread_create(&t1, NULL, worker_thread, NULL);
    pthread_create(&t2, NULL, logger_thread, NULL);

    pthread_join(t1, NULL);
    return 0;
}
