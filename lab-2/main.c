/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include <ioport.h>
#include <board.h>
#include <adc_sensors/adc_sensors.h>

void PWM_Init(void)
{
	/* Set output */
	PORTC.DIR |= PIN0_bm;

	/* Set Register */
	TCC0.CTRLA = (PIN2_bm) | (PIN0_bm);
	TCC0.CTRLB = (PIN4_bm) | (PIN2_bm) | (PIN1_bm);
	
	/* Set Period */
	TCC0.PER = 1000;

	/* Set Compare Register value*/
	TCC0.CCA = 0;
}

static void puter1(void){
	// button untuk lampu manual
	
	// jika sw1 ditekan, maka arahnya akan bergantian (berlawanan jarum jam, dan kembali searah)
	// jika sw2 ditekan, maka arahnya akan searah jarum jam
	if (gpio_pin_is_low(GPIO_PUSH_BUTTON_1) && gpio_pin_is_high(GPIO_PUSH_BUTTON_2)) {
		TCC0.CCA = 250;
		gpio_set_pin_low(LED0_GPIO);
		gpio_set_pin_high(LED1_GPIO);
	} else if (gpio_pin_is_high(GPIO_PUSH_BUTTON_0) && gpio_pin_is_high(GPIO_PUSH_BUTTON_1) && gpio_pin_is_low(GPIO_PUSH_BUTTON_2)) {
		TCC0.CCA = 1;
		gpio_set_pin_high(LED0_GPIO);
		gpio_set_pin_low(LED1_GPIO);
	} else {
		TCC0.CCA = 0;
		gpio_set_pin_high(LED0_GPIO);
		gpio_set_pin_high(LED1_GPIO);
	}
}

static void puter2(uint32_t intensity){
	//jika intensitas menurun, servo bergerak berlawanan arah jarum jam, sebaliknya bergantian (berlawanan jarum jam, dan kembali searah)
	
	if (intensity < 30) {
		TCC0.CCA = 250;
	} else {
		TCC0.CCA = 1;
	}
}

static char strbuf[128];

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	PWM_Init();
	gfx_mono_init();
	adc_sensors_init();
	
	// Inisialisasi interrupt vector
	pmic_init();
	cpu_irq_enable();

	//set background lcd on
	gpio_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);

	//set display - output lcd
	gfx_mono_draw_string("Welcome!",0, 0, &sysfont);
	delay_ms(1000);
	gfx_mono_draw_string("We will use sensor",0, 8, &sysfont);
	delay_ms(1000);
	gfx_mono_draw_string("    Sensor Read    ",0, 0, &sysfont);
	gfx_mono_draw_string("                   ",0, 8, &sysfont);
	
	// Variable untuk sampling light sensor
	uint32_t intensity = 0;
	uint8_t iterations = 0;
	#define LIGHTSENSOR_NUM_SAMPLES 20
	
	uint8_t flag = 0;

	while (1)
	{
		if (flag == 0) {
			puter1(); // servo manual dengan button	
		}
		if (gpio_pin_is_low(GPIO_PUSH_BUTTON_0)) {
			flag = 1 - flag;
			gpio_toggle_pin(LED0);
			gpio_toggle_pin(LED1);
		}
		
		ntc_measure();												// Mengambil data dari pengukuran suhu oleh NTC temperature sensor
		while(!ntc_data_is_ready());								// Menunggu data sampai siap untuk ditampilkan
		volatile int8_t temperature = ntc_get_temperature();	// Mengambil hasil olah data dalam Celcius

		snprintf(strbuf, sizeof(strbuf), "Tempr : %3d",temperature);
		gfx_mono_draw_string(strbuf,0, 8, &sysfont);

		lightsensor_measure();									// Mengambil data dari pengukuran intensitas oleh light sensor
		while(!lightsensor_data_is_ready());					// Menunggu data sampai siap untuk ditampilkan
		intensity += lightsensor_get_raw_value();				// Mengambil hasil olah data dalam raw ADC value

		// Dikarenakan hasil yang diperoleh merupakan data raw diperlukan sampling agar mendapatkan hasil yang baik
		if(iterations++ >= LIGHTSENSOR_NUM_SAMPLES) {
			iterations = 0;
			intensity /= LIGHTSENSOR_NUM_SAMPLES;
			
			snprintf(strbuf, sizeof(strbuf), "Light : %3d",intensity);
			gfx_mono_draw_string(strbuf,0, 16, &sysfont);
			
			if (flag == 1) {
				puter2(intensity);
			}
			
			intensity = 0;
		}
	}
}
