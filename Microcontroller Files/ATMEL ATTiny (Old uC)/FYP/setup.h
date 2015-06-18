//#define CLOCK8MHZ		// define this if system frequency is set at 8MHz

#ifdef CLOCK8MHZ
#define F_CPU		8000000UL 
#else
#define F_CPU		1000000UL
#endif

#define PCAP_FREQ	200000

#define HV_LIMIT	160
#define VCC			3.3

// pin definitions
#define HV_ON							PB0
#define HV_ON_PORT						PORTB
#define HV_ON_DIRECTION_PORT			DDRB

#define MIRROR_CONTROL					PB2			//PB1
#define	MIRROR_CONTROL_PORT				PORTB
#define MIRROR_CONTROL_DIRECTION_PORT	DDRB

#define PWM_ADJUST						PA3			
#define	PWM_ADJUST_PORT					PORTA		
#define PWM_DIRECTION_PORT				DDRA		
#define PWM_COMPARE_REGISTER			OCR0B

#define PWM_OUTPUT_COMPARE				TOCC2OE		// on the new one it is TOCC7OE

#define SYNC							PA0
#define SYNC_PORT						PORTA
#define SYNC_DIRECTION_PORT				DDRA

#define PCAP_PIN						PA7			//PB2
#define PCAP_PORT						PORTA
#define PCAP_DIRECTION_PORT				DDRA
#define PCAP_ICP						ICES1
//#define PCAP_PCINT						PCINT7