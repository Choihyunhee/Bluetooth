#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define HIGH 	1
#define LOW 	0
#define INPUT 	1
#define OUTPUT 	0

bool digitalPinMode(int pin, int dir){
	FILE * fd;
	char fName[128];
  
	if(( fd = fopen("/sys/class/gpio/export", "w")) == NULL) {
		printf("Error: unable to export pin\n");
		return false;
	}
	fprintf(fd, "%d\n", pin);
	fclose(fd);

	sprintf(fName, "/sys/class/gpio/gpio%d/direction", pin);
	if((fd = fopen(fName, "w")) == NULL) {
		printf("Error: can't open pin direction\n");
		return false;
	}
	if(dir == OUTPUT) {
		fprintf(fd, "out\n");
	}
	else {
		fprintf(fd, "in\n");
	}
	fclose(fd);

	return true;
}

int digitalRead(int pin) {
	FILE * fd;
	char fName[128];
	char val[2];

	sprintf(fName, "/sys/class/gpio/gpio%d/value", pin);
	if((fd = fopen(fName, "r")) == NULL) {
		printf("Error: can't open pin value\n");
		return false;
	}
	fgets(val, 2, fd);
	fclose(fd);

	return atoi(val);
}

bool digitalWrite(int pin, int val) {
	FILE * fd;
	char fName[128];

	sprintf(fName, "/sys/class/gpio/gpio%d/value", pin);
	if((fd = fopen(fName, "w")) == NULL) {
		printf("Error: can't open pin value\n");
		return false;
	}
	if(val == HIGH) {
		fprintf(fd, "1\n");
	}
	else {
		fprintf(fd, "0\n");
	}
	fclose(fd);

	return true;
}
int analogRead(int pin) {
	FILE * fd;
	char fName [128];
	char val[8];

	sprintf(fName, "/sys/devices/platform/c0000000.soc/c0053000.adc/iio:device0/in_voltage%d_raw",pin);
	if((fd=fopen(fName,"r"))==NULL){
		printf("Error: can't open analog voltage value\n");
		return 0;
	}
	fgets(val,8,fd);
	return atoi(val);
}

// period, duty_cycle -> nsec 기준
bool pwmPin(int pin, int period, int duty_cycle, int enable){
    FILE * fd;
    char fName[128];

    if(( fd = fopen("/sys/class/pwm/pwmchip0/export", "w")) == NULL) {
        printf("Error: unable to export pin\n");
        return false;
    }
    fprintf(fd, "%d\n", pin);
    fclose(fd);

    sprintf(fName, "/sys/class/pwm/pwmchip0/pwm%d/period", pin);
    if((fd = fopen(fName, "w")) == NULL) {
        printf("Error: can't open pin period\n");
        return false;
    }
    fprintf(fd, "%d\n", period);
    fclose(fd);

    sprintf(fName, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", pin);
    if((fd = fopen(fName, "w")) == NULL) {
        printf("Error: can't open pin duty_cycle\n");
        return false;
    }
    fprintf(fd, "%d\n", duty_cycle);
    fclose(fd);
	
    sprintf(fName, "/sys/class/pwm/pwmchip0/pwm%d/enable", pin);
    if((fd = fopen(fName, "w")) == NULL) {
        printf("Error: can't open pin enable\n");
        return false;
    }
    if(enable == HIGH) {
        fprintf(fd, "1\n");
    }
    else {
        fprintf(fd, "0\n");
    }
    fclose(fd);

    return true;
}

