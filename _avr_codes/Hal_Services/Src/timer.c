/*
 * timer.c
 *
 * Created: 2025. 10. 09. 23:33:02
 *  Author: bagob
 */ 

 #include "timer.h"
 #include "interrupt.h"
 #include "port_config.h"


  static volatile uint32_t Millis = 0;
  static void timer_0_callback(void);



 //// private 

 static void timer_init(uint8_t timer, uint8_t mode, uint8_t prescaler){
		 
	 switch (timer)
	 {
		 case 0:
					TCCR0A = 0;				 // 1. Alaphelyzet
					TCCR0B = 0;

					TCCR0A = mode;			// 2. Timer mód beállítása
					TCCR0B = prescaler;		// 3. Prescaler beállítása
					break;
		 case 1: break;

		 case 2: break;
		 default: break;
	 }
	 
 }

 static void timer_set_value(uint8_t timer, uint8_t value)
 {
	 switch (timer)
	 {
		 case 0: OCR0A = value; break;
		 default: break;
	 }
 }


 static void timer_0_callback(void)
 {
	 Millis++;
 }


  ///////////////  public 

 void timers_init()
  {

	//sys tick 	
	timer_init(TIMER0, TIMER0_CTC, TIMER0_PRESCALE_64);
	timer_int_init(TIMER0, TIMER0_INT_COMP_A);
	timer_set_value(TIMER0, 249);							// 1ms idõzítés (sys tick)
	set_timer_int_Callback(TIMER0,timer_0_callback);
  }

 uint32_t millis(void)
 {
	 uint32_t m;
	 cli();				// megszakítás tiltás olvasás idejére --> több órajel kiolvasni 32 bitet
	 m = Millis;
	 sei();

	 return m;
 }

 void timer_1_init(void)
 {
	TCCR1A = 0;					// biztonsági 0 - ázás
	TCCR1B = 0;

	//TCCR1A |= (1 << COM1A1) | (0 << COM1A0) | (1 << WGM11);							// pwm --> pin bekapcsolás, fast pwm mode
	TCCR1A |= (1 << WGM11);																// pwm --> fast pwm mode
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);	// prescaler, fast pwm mode: 0 -> icr1
	TIMSK1 |= (1 << TOIE1);		// ctc- ovf interrupt

	OCR1A = TIM_COMPARE;		// kitöltés	beállítása
	ICR1 = TIM_START_VALUE;		// alap frekvencia beállítása

	sei();						// globális int engedélyezés

 }

  void set_timer_1_ovf_value(uint64_t ovf_value)
  {
	  ICR1 = ovf_value; 

  }

  uint16_t get_timer_1_value(void)
  {
	return ((uint16_t)(TCNT1H << 8)) | ((uint16_t) TCNT1L);		// kiolvasni a timer értékét
  }

  void enable_timer_1_out(void)
  {
	 TCCR1A |= (1 << COM1A1) | (0 << COM1A0);					// PWM kimenet engedélyezése
  }

  void disable_timer_1_out(void)
  {
	  TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));				// PWM leválasztása
  }

