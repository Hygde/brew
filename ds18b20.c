#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#define DS18B20_BUF_SIZE 10
#define DIR_W1_DEVICES "/sys/bus/w1/devices"

static FILE* getFilePointer(const char* sensor_id);

static FILE* getFilePointer(const char* sensor_id) {
    FILE* pfile = NULL;
    char path[256];

    sprintf(path, "%s/%s/temperature", DIR_W1_DEVICES, sensor_id);
    pfile = fopen(path, "r");
    if (pfile == NULL) {
        fprintf(stderr, "Fail to open %s\nError code = %d\n", path, errno);
    }
    return pfile;
}

int isSensorAvailable(const char* sensor_id) {
    DIR* dir = opendir(DIR_W1_DEVICES);
    FILE* psensor = NULL;
    char path[256];
    if(dir == NULL) {
        fprintf(stderr, "Fail to open %s\nError code %d\n", DIR_W1_DEVICES, errno);
        exit(errno);
    }
    closedir(dir);
    sprintf(path, "%s/%s", DIR_W1_DEVICES, sensor_id);
    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Fail to open %s\nError code %d\n", path, errno);
        exit(errno);
    }
    closedir(dir);
    psensor = getFilePointer(sensor_id);
    if(sensor_id == NULL) {
        exit(errno);
    }
    fclose(psensor);
    return 1;
}

float getTemperature(const char* sensor_id) {
    FILE* ptempfile = getFilePointer(sensor_id);
    char buf[DS18B20_BUF_SIZE];
    float ret = 0.;

    if (ptempfile != NULL) {
        int temperature = 0;
        fgets(buf, DS18B20_BUF_SIZE, ptempfile);
        temperature = atoi(buf);
        ret = temperature / 1000.;
        fclose(ptempfile);
    }

    return ret;
}

int main(int argc, char*argv[]) {
    if(isSensorAvailable("28-3c01f0964257")) {
        printf("temperature: %f\n", getTemperature("28-3c01f0964257"));
    }
}