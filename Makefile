CFLAGS = -Wno-implicit-function-declaration -mmcu=atmega168

all: final

final: asuro.o adc.o adc_low.o  encoder_low.o encoder.o globals.o i2c.o lcd_bpi216.o leds.o lcd.o motor_low.o motor.o print.o printf.o rc5.o sound.o switches.o time.o uart.o ultrasonic.o version.o main.o
	@echo "Linking and producing the final application"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) asuro.o adc.o adc_low.o encoder_low.o encoder.o globals.o i2c.o lcd_bpi216.o leds.o lcd.o motor_low.o motor.o print.o printf.o rc5.o sound.o switches.o time.o uart.o ultrasonic.o version.o  main.o -o final
	chmod +x final

adc_low.o: adc_low.c
	@echo "Compiling the adc_low.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c adc_low.c

adc.o: adc.c
	@echo "Compiling the adc.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c adc.c

asuro.o: asuro.c
	@echo "Compiling the asuro.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c asuro.c

encoder_low.o: encoder_low.c
	@echo "Compiling the encoder_low.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c encoder_low.c

encoder.o: encoder.c
	@echo "Compiling the encoder.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c encoder.c

globals.o: globals.c
	@echo "Compiling the globals.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c globals.c

i2c.o: i2c.c
	@echo "Compiling the i2c.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c i2c.c

lcd_bpi216.o: lcd_bpi216.c
	@echo "Compiling the lcd_bpi216.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c lcd_bpi216.c

lcd.o: lcd.c
	@echo "Compiling the lcd.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c lcd.c
leds.o: leds.c
	@echo "Compiling the leds.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c leds.c

main.o: main.c
	@echo "Compiling the main.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c main.c

motor_low.o: motor_low.c
	@echo "Compiling the motor_low.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c motor_low.c

motor.o: motor.c
	@echo "Compiling the motor.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c motor.c

my_motor_low.o: my_motor_low.c
	@echo "Compiling the my_motor_low.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c my_motor_low.c

print.o: print.c
	@echo "Compiling the print.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c print.c

printf.o: printf.c
	@echo "Compiling the printf.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c printf.c

rc5.o: rc5.c
	@echo "Compiling the rc5.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c rc5.c

sound.o: sound.c
	@echo "Compiling the sound.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c sound.c

switches.o: switches.c
	@echo "Compiling the switches.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c switches.c

time.o: time.c
	@echo "Compiling the time.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c time.c

uart.o: uart.c
	@echo "Compiling the uart.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c uart.c

ultrasonic.o: ultrasonic.c
	@echo "Compiling the ultrasonic.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c ultrasonic.c

version.o: version.c
	@echo "Compiling the version.c source-file"
	/opt/avr-gcc/bin/avr-gcc $(CFLAGS) -c version.c


clean:
	@echo "removing everything but the source files"
	rm *.o final

