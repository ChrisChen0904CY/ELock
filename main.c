#include "lock.h"
#include "lcd12864.h"
#include "pstack.h"

#define WRONG_MAX 3

unsigned char tab[]={0XC0,
					 0XF9,0XA4,0XB0,
					 0X99,0X92,0X82,
					 0XF8,0X80,0X90,
					 0X88,0X83,0XC6,
					 0XA1,0X86,0X8E,
					 0X89};

char xdata input_tab[]={'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#',
						'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
					 
// Count the Press-Stay time
int cnt;
int xdata intr = 0;
unsigned char xdata itr = 0;
unsigned char xdata flag = 0;
unsigned char xdata wrong_cnt = 0;
unsigned char xdata input_point = 0;
unsigned char xdata single = 0;
unsigned char xdata refresh = 0;
unsigned char xdata start = 0;
unsigned char xdata tinkle = 0;
unsigned char xdata broken = 0;
unsigned char xdata set_twice = 0;
unsigned char xdata temp_length = 0;
unsigned char xdata twice_length = 0;
unsigned char xdata wrong_signal = 0;
unsigned char xdata Time_Input = 0;
char xdata tmp_set[16];
char xdata twice_set[16];
int xdata wrong_break = 0;
int xdata wait = 0;
// Count for 1 second
int xdata time_cnt = 0;
						
/**

* @breif Page Status						
						
*/
unsigned xdata page_status = 1;

/**
					 
* @breif Important Paramter which refers to current status of the CLOCK
					 
* @value 0:
			Input Mode
* @value 1
			Setting Mode
* @value 2
			Unlock Mode
					 
*/
extern unsigned char xdata status = 0;
extern unsigned char xdata confirm = 0;
// Generate the lock object 
Lock xdata lock;
pstack xdata page_stack;
// The status of input
extern unsigned char inputting = 0;
// The status of Shift and defaulted by 0
extern unsigned char shift = 0;

/**

* @breif Y/M/D

*/
unsigned char xdata Time1[] = {'2', '0', '2', '3', '/',
							   '0', '5', '/',
							   '1', '2', '\0'};
unsigned char xdata Time2[] = {'2', '0', ':',
							   '0', '0', ':',
							   '0', '0', '\0'};
unsigned char xdata Time_Set[4];
/**

* @breif Y/M/D/H/M
							   
*/
unsigned char xdata Time_Input_Size[] = {4, 2, 2, 2, 2};
					 
void cls_buzz(void);
void Key_Scan16(void);
void timer_init(void);
void password_match(void);
void alarm(void);
void Delayms(int);
void Display_Locked(void);
void Display_Unlocked(void);
void Display_Page(void);
void Display_Wrong(void);
void time_fresh(void);
void Display_TimeFresh(void);
void Display_Tinkle(void);
void NO_IDEA(void);
void Password_Setting_Check(void);
unsigned char Check(void);
void Display_Config_Success(void);

key_num = 0;
Long_Press = 0;
Long_Shift = 0;
locked = 1;		
			
/*------------------------------------------------MAIN PROCESS---------------------------------------------------*/
void main(void)
{
	
	cls_buzz();
	// P2=0XC0;P0=0X04;P2=0XFF;P0=0XFF;   //数码管初始化程序，打开第一个数码管，以后讲
	// Initialize the lock
	lock_init(&lock);
	pstack_init(&page_stack);
	/*-----LCD TEST-----*/
	LCD12864_Init();
	// Stay in Welcome page for A While
	for(itr=0; itr<100; itr++){
		Delayms(500);
	}
	itr=0;
	// Locked Page
	Display_Locked();
	/*-----END TEST-----*/
	
	// Enable and initialize the timer 0
	timer_init();
	
	while(1)
	{	
		// PROGRAMMED ALL BY INTERRUPTTED
	}
	
}
/*----------------------------------------------------------------------------------------------------------------*/

/**

* @breif Close all the pheriferal

*/
void cls_buzz(void)
{
	P2 = (P2&0x1F|0xA0);
	P0 = 0x00;
	P2 &= 0x1F;
}

/**

* @breif Sacn the Matrix-Keyboard

*/
void Key_Scan16(void)
{
	unsigned char temp;
	flag = 0;
	P44=0;P42=1;P3=0X7F;//0111 1111 
	temp=P3;						//0111 1110
	temp=temp&0X0F;//0111 1110 & 0000 1111 == 0000 1111
	if(temp!=0X0F)
	{
		Delayms(5);
		temp=P3;						
		temp=temp&0X0F;
		if(temp!=0X0F)
		{
			flag = 1;
			temp=P3;
			switch(temp)
			{
				case 0X7E: key_num=1;break;
				case 0X7D: key_num=4;break;
				case 0X7B: key_num=7;break;
				case 0X77: key_num=10;break;
			}
		}
	}
	
	P44=1;P42=0;P3=0XBF;//1011 1111 
	temp=P3;						//0111 1110
	temp=temp&0X0F;//0111 1110 & 0000 1111 == 0000 1111
	if(temp!=0X0F)
	{
		Delayms(5);
		temp=P3;						
		temp=temp&0X0F;
		if(temp!=0X0F)
		{
			flag = 1;
			temp=P3;
			switch(temp)
			{
				case 0XBE: key_num=2;break;
				case 0XBD: key_num=5;break;
				case 0XBB: key_num=8;break;
				case 0XB7: key_num=11;break;
			}
		}
	}
	
	P44=1;P42=1;P3=0XDF;//1101 1111 
	temp=P3;						//0111 1110
	temp=temp&0X0F;//0111 1110 & 0000 1111 == 0000 1111
	if(temp!=0X0F)
	{
		Delayms(5);
		temp=P3;						
		temp=temp&0X0F;
		if(temp!=0X0F)
		{
			flag = 1;
			temp=P3;
			switch(temp)
			{
				case 0XDE: key_num=3;break;
				case 0XDD: key_num=6;break;
				case 0XDB: key_num=9;break;
				case 0XD7: key_num=12;break;
			}
		}
	}
	
	P44=1;P42=1;P3=0XEF;//1110 1111 
	temp=P3;						//0111 1110
	temp=temp&0X0F;//0111 1110 & 0000 1111 == 0000 1111
	if(temp!=0X0F)
	{
		Delayms(5);
		temp=P3;						
		temp=temp&0X0F;
		if(temp!=0X0F)
		{
			flag = 1;
			temp=P3;
			switch(temp)
			{
				case 0XEE: key_num=13;break;
				case 0XED: key_num=14;break;
				case 0XEB: key_num=15;break;
				case 0XE7: key_num=16;break;
			}
		}
	}
}

/**

* @breif Enable and initialize the TIMER 0

*/
void timer_init(){

	AUXR |= 0x80;	//1T模式，IAP15F2K61S2单片机特殊功能寄存器
	
	TMOD &= 0xF0;
	TL0 = 0xCD;	
	TH0 = 0xD4;	
	TF0 = 0;	
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	
}

/**

* @breif Check Whether input can be matched with password

*/
void password_match(void){
	bit wrong = 0;
	int xdata sub_id = 0;
	// Deal while input was confirmed
	
	// Fixed Length Mode
	if(lock.mode==0){
		// wrong when Input shorter than password
		if(input_point!=lock.length){
			input_point = 0;
			wrong = 1;
			return;
		}
		for(itr=0; itr<lock.length;itr++){
			// Fail to Match
			if(lock.input_stream[itr]!=lock.password[itr]){
				wrong = 1;
				// Clear the Input Steam
				input_point = 0;
				break;
			}
		}
		itr = 0;
				
		// Unlock the lock while input can match with password
		if(wrong==0){
			input_point = 0;
			start = 0;
			inputting = 1;
			locked = 0;
			Display_Unlocked();
		}
	}
	
	// Virtual Bits Mode
	else if(lock.mode==1){
		// wrong when Input shorter than password
		if(input_point<lock.length){
			input_point = 0;
			wrong = 1;
			return;
		}
		// Match from tail
		for(intr=lock.length-1; intr>=0;intr--){
			// Fail to Match
			if(lock.input_stream[intr]!=lock.password[intr]){
				wrong = 1;
				// Clear the Input Steam
				input_point = 0;
				break;
			}
		}
		intr = 0;
				
		// Unlock the lock while input can match with password
		if(wrong==0){
			input_point = 0;
			start = 0;
			inputting = 1;
			locked = 0;
			Display_Unlocked();
		}
	}
	
	// SubString Mode
	else{
		// wrong when Input shorter than password
		if(input_point<lock.length){
			input_point = 0;
			wrong = 1;
			return;
		}
		// DP MATHCING
		if(Check()!=1){
			wrong = 1;
			// Clear the Input Steam
			input_point = 0;
		}
				
		// Unlock the lock while input can match with password
		if(wrong==0){
			input_point = 0;
			start = 0;
			inputting = 1;
			locked = 0;
			Display_Unlocked();
		}
	}
	
	// Clear the confirm flag
	confirm = 0;
	start = locked;
	page_status = locked;
	wait = 0;
}

/*----------------------------------------CORE EXCUTED PART-------------------------------------------*/
// Timer Interrupt Service Function
void isr_timer_0(void)  interrupt 1  // Priority defaulted by 1
{
	// Count Variable here
	static int count = 0;
	
	if(broken==1){
		return;
	}
	
	if(wrong_cnt>=WRONG_MAX){
		alarm();
		broken = 1;
		return;
	}
	
	/*=============TIME UPDATE=============*/
	// Count for 1 second
	if(time_cnt++==1000){
		time_fresh();
		// Fresh the Screen
		if(page_status==4){
			LCD12864_Display(LINE4, &Time2);
		}
		else{
			Display_TimeFresh();
		}
		// ReCount Here
		time_cnt = 0;
	}
	/*======================================*/
	
	// See Whether There was something went wrong
	if(wrong_break!=0&&wrong_break<5000){
		wrong_break++;
		return;
	}
	else if(wrong_break==5000){
		wrong_break = 0;
		if(locked==1){
			Display_Locked();
		}
		else{
			Display_Page();
		}
	}
	
	// IF INPUT HAS STARTED
	if(start==1){
		// Wait for 10s
		if(wait++>=10000){
			wrong_cnt = WRONG_MAX;
			return;
		}
		else{
			if(wait%1000==0){
				Display_Tinkle();
				tinkle = tinkle == 0? 1 :0;
			}
		}
	}
	
	// Alarming while wrong_cnt is big enough
	if(wrong_cnt>=WRONG_MAX){
		return;
	}
	
	/** Compute the input-allow flag @referrence inputting */
	if(locked == 1){
		inputting = confirm==0 && input_point<lock.allow_length;
	}
	else if(page_status == 5){
		// PASSWORD VALUABLE LENGTH NO MORE THAN 16
		inputting = confirm==0 && input_point<16;
	}
	else if(page_status == 6){
		// PASSWORD VALUABLE LENGTH NO MORE THAN ALLOWED
		inputting = confirm==0 && input_point<Time_Input_Size[Time_Input];
	}
	
	/*================================DEAL WITH KEYBOARD==================================*/
	
	// Scan the MATRIX KEYBOARD
	Key_Scan16();
	/*=================================PRESSED=============================================*/
	/* flag is 1 means something has been pressed */
	if(flag==1){
		/* single is 1 means you can now detect whether there do exist releasing */
		single = 1;
		// Count how many time the key stay being pressed
		/* SHIFT */
		if(key_num==14){
			if(Long_Press==0){
				count++;
			}
			else{
				Long_Shift = 1;
			}
		}
		flag = 0;
	}
	/*=================================PRESSED=============================================*/
	
	/*=================================RELEASED============================================*/
	else{
		// Re-Lock when input a "*" While UNLOCKED
		// Tips: Can't Lock When Setting new Password
		if(page_status!=6&&page_status!=5&&locked==0&&key_num==10&&single==1){
			locked = 1;
			start = 0;
			input_point = 0;
			Display_Locked();
			return;
		}
		// Charcter Inputting
		if(key_num<=12&&key_num>=1&&single==1){
			// Settings
			if(page_status==2){
				// To Mode 3: PASSWORD CHANGE
				if(key_num==1){
					page_status = 3;
				}
				// To Mode 6: TIME CHECK
				else if(key_num==2){
					page_status = 6;
				}
				// Warning
				else{
					NO_IDEA();
					// Play for 5s
					wrong_break = 1;
					single = 0;
					return;
				}
				if(page_status!=page_stack.pages[page_stack.top]){
					pstack_push(&page_stack, page_status);
					Display_Page();
				}
				single = 0;
				return;
			}
			// PASSWORD SETTINGS
			else if(page_status==3){
				// To Mode 4: PASSWORD MATCH MODE
				if(key_num==1){
					page_status = 4;
				}
				// To Mode 5: PASSWORD CONTEXT MODE
				else if(key_num==2){
					page_status = 5;
				}
				// Warning
				else{
					NO_IDEA();
					// Play for 5s
					wrong_break = 1;
					single = 0;
					return;
				}
				if(page_status!=page_stack.pages[page_stack.top]){
					pstack_push(&page_stack, page_status);
					Display_Page();
				}
				single = 0;
				return;
			}
			// PASSWORD MATCH MODE SETTINGS
			else if(page_status==4){
				// To MATCH Mode k
				if(key_num<=3){
					lock.mode = key_num-1;
					pstack_pop(&page_stack);
					page_status = page_stack.pages[page_stack.top];
					Display_Page();
				}
				// Warning
				else{
					NO_IDEA();
					// Play for 5s
					wrong_break = 1;
					single = 0;
					return;
				}
				single = 0;
				return;
			}
			// PASSWORD CONTEXT CHANGE
			else if(page_status==5){
				// Input while allowed
				if(inputting==1){
					if(start==0){
						start = 1;
						input_point = 0;
						if(set_twice==0){
							temp_length = 0;
						}
						else{
							twice_length = 0;
						}
					}
					if(set_twice==0){
						tmp_set[input_point++] = input_tab[key_num-1+(shift==1?12:0)];
						temp_length++;
					}
					else{
						twice_set[input_point++] = input_tab[key_num-1+(shift==1?12:0)];
						twice_length++;
					}
					wait = 0;
				}
				// Un-Shift Automatically
				if(shift==1&&Long_Press==0){
					shift = 0;
				}
				single=0;
				Display_Tinkle();
				return;
			}
			
			// TIME CHECK MODE
			else if(page_status==6){
				// Input while allowed
				if(inputting==1){
					if(start==0){
						start = 1;
						input_point = 0;
					}
					// Adjust for '*' and '#'
					key_num = (key_num==10||key_num==12)?11:key_num;
					Time_Set[input_point++] = input_tab[key_num-1];
					wait = 0;
				}
				// Un-Shift Automatically
				if(shift==1&&Long_Press==0){
					shift = 0;
				}
				single=0;
				Display_Tinkle();
				return;
			}
			
			// Input while allowed in LOCKED MODE
			if(inputting==1){
				if(start==0){
					start = 1;
					input_point = 0;
				}
				lock.input_stream[input_point++] = input_tab[key_num-1+(shift==1?12:0)];
				wait = 0;
			}
			// Un-Shift Automatically
			if(shift==1&&Long_Press==0){
				shift = 0;
			}
			if(single==1){
				single = 0;
				Display_Tinkle();
			}
		}
		// SETTINGS
		else if(key_num==13&&single==1){
			if(locked==0&&page_status!=5&&page_status!=6){
				pstack_push(&page_stack, 2);
				page_status = 2;
				Display_Page();
			}
		}
		// SHIFT
		else if(key_num==14&&single==1){
			shift = shift==0?1:0;
			count = 0;
			if(Long_Press==1&&shift==0){
				Long_Press=Long_Shift = 0;
			}
		}
		// CANCEL
		else if(key_num==15&&single==1){
			// Can't Go Back When Change Your Password Context or Check Your Time
			if(locked==0&&page_status!=5&&page_status!=6){
				pstack_pop(&page_stack);
				page_status=page_stack.pages[page_stack.top];
				Display_Page();
				single = 0;
				return;
			}
			input_point = (input_point==0)?0:input_point-1;
			if(set_twice==0){
				temp_length = (temp_length==0)?0:temp_length-1;
			}
			else{
				twice_length = (twice_length==0)?0:twice_length-1;
			}
			wait = 0;
			Display_Tinkle();
		}
		// CONFIRM
		else if(key_num==16&&single==1){
			confirm = 1;
			if(locked==1){
				password_match();
			}
			else if(page_status==5){
				set_twice = set_twice==0?1:0;
				confirm = 0;
				start = 0;
				if(set_twice==0){
					Password_Setting_Check();
					if(wrong_signal==1){
						wrong_signal = 0;
						single = 0;
						input_point = 0;
						return;
					}
					Display_Locked();
					page_status = 1;
					locked = 1;
					// UPDATE PASSWORD
					for(itr=0; itr<input_point; itr++){
						lock.password[itr] = tmp_set[itr];
					}
					lock.length = twice_length;
					itr = input_point = 0;
					wait = 0;
					pstack_init(&page_stack);
					single = 0;
					confirm = 0;
					return;
				}
				else{
					Display_Page();
					input_point = 0;
					wait = 0;
					single = 0;
					confirm = 0;
					return;
				}
			}
			/* Confirm When Check Time */
			else if(page_status==6){
				confirm = 0;
				// Year
				if(Time_Input==0){
					// When There do exist something input
					if(input_point!=0){
						for(itr=0; itr<Time_Input_Size[Time_Input]; itr++){
							Time1[itr] = itr>=input_point?'0':Time_Set[itr];
						}
						itr = 0;
					}
					Time_Input++;
					Display_Page();
				}
				// Month
				else if(Time_Input==1){
					// When There do exist something input
					if(input_point!=0){
						for(itr=0; itr<Time_Input_Size[Time_Input]; itr++){
							Time1[itr+5] = itr>=input_point?'0':Time_Set[itr];
						}
						itr = 0;
					}
					// Check Value
					if(Time1[5]>'1'||(Time1[5]=='1'&&Time1[6]>'2')||\
					  (Time1[5]=='0'&&Time1[6]=='0')){
						Time1[5] = '0';
						Time1[6] = '1';
					}
					Time_Input++;
					Display_Page();
				}
				// Day
				else if(Time_Input==2){
					// When There do exist something input
					if(input_point!=0){
						for(itr=0; itr<Time_Input_Size[Time_Input]; itr++){
							Time1[itr+8] = itr>=input_point?'0':Time_Set[itr];
						}
						itr = 0;
					}
					// Check Value
					if(Time1[8]>'3'||(Time1[8]=='3'&&Time1[9]>'2')||\
					  (Time1[8]=='0'&&Time1[9]=='0')){
						Time1[8] = '0';
						Time1[9] = '1';
					}
					Time_Input++;
					Display_Page();
				}
				// Hour
				else if(Time_Input==3){
					// When There do exist something input
					if(input_point!=0){
						for(itr=0; itr<Time_Input_Size[Time_Input]; itr++){
							Time2[itr] = itr>=input_point?'0':Time_Set[itr];
						}
						itr = 0;
					}
					// Check Value
					if(Time2[0]>'2'||(Time2[0]=='2'&&Time2[1]>'4')){
						Time2[0] = '0';
						Time2[0] = '0';
					}
					Time_Input++;
					Display_Page();
				}
				// Minute
				else{
					// When There do exist something input
					if(input_point!=0){
						for(itr=0; itr<Time_Input_Size[Time_Input]; itr++){
							Time2[itr+3] = itr>=input_point?'0':Time_Set[itr];
						}
						itr = 0;
					}
					// Check Value
					if(Time2[3]>'6'){
						Time2[3] = '0';
						Time2[4] = '0';
					}
					Time_Input=0;
					// Back to Previous Page
					pstack_pop(&page_stack);
					page_status = page_stack.pages[page_stack.top];
					Display_Config_Success();
					wrong_break = 1;
				}
				start = 0;
				input_point = 0;
				single = 0;
				return;
			}
			
			if(locked==1){
				wrong_cnt++;
				Display_Wrong();
				wrong_break=1;
			}
		}
		
		// Clear Singe-Press After Releasing
		if(single==1){
			single = 0;
		}
	}
	/*=================================RELEASED============================================*/
	
	/*================================END DEAL KEYBOARD==================================*/
	
	// Judge Whether has been pressed for a LONG TIME
	if(count>=1500){
		count = 0;
		Long_Press = 1;
	}
	
	return;
	
}
/*-------------------------------------------------------------------------------------------------*/

/*==================================================================================================

* SOME SEGMENTATION HERE WITHOUT ANY MEANING HEY HEY HEY HEY HEY HEY HEY HEY HEY HEY HEY HEY HEY   *

===================================================================================================*/

/*---------------------------------Functoin Relization---------------------------------------------*/
void Delayms(int ms){
	unsigned char i, j;
	for(i=ms;i>0;i--){
		for(j=845;j>0;j--);
	}
}

// ALARM!!!
void alarm(void){

	LCD12864_Display(LINE1, "BROKEN");
	LCD12864_Display(LINE2, "BROKEN");
	LCD12864_Display(LINE3, "BROKEN");
	LCD12864_Display(LINE4, "BROKEN");

}

void Display_Locked(void){
	
	LCD12864_Display(LINE1, "LOCKED");
	LCD12864_Display(LINE2, "_");
	LCD12864_Display(LINE3, &Time1);
	LCD12864_Display(LINE4, &Time2);

}

void Display_Unlocked(void){

	LCD12864_Display(LINE1, "UNLOCKED");
	LCD12864_Display(LINE2, "WELCOME!!!");
	LCD12864_Display(LINE3, &Time1);
	LCD12864_Display(LINE4, &Time2);

}

void Display_Wrong(void){
	unsigned char tmp[] = {'T', 'i', 'm', 'e', 's', ':', ' ', ' ', '\0'};
	tmp[7] = '0'+wrong_cnt;
	
	LCD12864_Display(LINE1, "WRONG!!!");
	if(page_status==5){
		LCD12864_Display(LINE2, "两次密码不一致!");
	}
	else{
		LCD12864_Display(LINE2, &tmp);
	}
	LCD12864_Display(LINE3, &Time1);
	LCD12864_Display(LINE4, &Time2);

}

void time_fresh(void){

	// Carry Symbol
	bit tmp_cy = 0;
	
	// Update for SECOND
	if(Time2[7]=='9'){
		Time2[7] = '0';
		tmp_cy = 1;
	}
	else{
		Time2[7] = Time2[7]+1;
	}
	// Ten-Bit
	if(tmp_cy==1){
		if(Time2[6]=='5'){
			Time2[6] = '0';
		}
		else{
			Time2[6] = Time2[6]+1;
			tmp_cy = 0;
		}
	}
	
	// Update for MINUTE
	if(tmp_cy==1){
		if(Time2[4]=='9'){
			Time2[4] = '0';
		}
		else{
			Time2[4] = Time2[4]+1;
			tmp_cy = 0;
		}
	}
	// Ten-Bit
	if(tmp_cy==1){
		if(Time2[3]=='5'){
			Time2[3] = '0';
		}
		else{
			Time2[3] = Time2[3]+1;
			tmp_cy = 0;
		}
	}
	
	// Update for Hour
	if(tmp_cy==1){
		if(Time2[1]=='9'||(Time2[1]=='3'&&Time2[0]=='2')){
			Time2[1] = '0';
		}
		else{
			Time2[1] = Time2[1]+1;
			tmp_cy = 0;
		}
	}
	// Ten-Bit
	if(tmp_cy==1){
		if(Time2[0]=='2'){
			Time2[0] = '0';
		}
		else{
			Time2[0] = Time2[0]+1;
			tmp_cy = 0;
		}
	}
	
	// Update for MONTH
	if(tmp_cy==1){
		if(Time1[6]=='9'||(Time1[6]=='2'&&Time1[5]=='1')){
			Time1[6] = '0';
		}
		else{
			Time1[6] = Time1[6]+1;
			tmp_cy = 0;
		}
	}
	// Ten-Bit
	if(tmp_cy==1){
		if(Time1[5]=='1'){
			Time1[5] = '0';
		}
		else{
			Time1[5] = Time1[5]+1;
			tmp_cy = 0;
		}
	}
	
	// Update for YEAR
	if(tmp_cy==1){
		if(Time1[3]=='9'){
			Time1[3] = '0';
		}
		else{
			Time1[3] = Time1[3]+1;
			tmp_cy = 0;
		}
	}
	// Ten-Bit
	if(tmp_cy==1){
		if(Time1[2]=='9'){
			Time1[2] = '0';
		}
		else{
			Time1[2] = Time1[2]+1;
			tmp_cy = 0;
		}
	}
	
	// Houndred-Bit
	if(tmp_cy==1){
		if(Time1[1]=='9'){
			Time1[1] = '0';
		}
		else{
			Time1[1] = Time1[1]+1;
			tmp_cy = 0;
		}
	}
	
	// Thousand-Bit
	if(tmp_cy==1){
		if(Time1[0]=='9'){
			Time1[0] = '0';
		}
		else{
			Time1[0] = Time1[0]+1;
			tmp_cy = 0;
		}
	}

}

void Display_TimeFresh(void){
	LCD12864_Display(LINE3, &Time1);
	LCD12864_Display(LINE4, &Time2);
}

void Display_Tinkle(void){
	
	unsigned char xdata tmp[17];
	unsigned char xdata tinkle_i;
	unsigned char xdata dis_id = input_point>16 ? 16 : input_point;
	
	if(locked==1){
		
		for(tinkle_i=0; tinkle_i<dis_id; tinkle_i++){
			tmp[tinkle_i] = '*';
		}
		if(dis_id!=16){
			tmp[tinkle_i] = tinkle==1?'_':' ';
			tmp[tinkle_i+1] = '\0';
		}
		else{
			tmp[16] = '\0';
		}
		LCD12864_Display(LINE2, &tmp);
		
	}		
	// PASSWORD CONTEXT MODE
	else if(page_status==5){
		for(tinkle_i=0; tinkle_i<dis_id; tinkle_i++){
			tmp[tinkle_i] = set_twice==0?tmp_set[tinkle_i]:'*';
		}
		if(dis_id!=16){
			tmp[tinkle_i] = tinkle==1?'_':' ';
			tmp[tinkle_i+1] = '\0';
		}
		else{
			tmp[16] = '\0';
		}
		LCD12864_Display(LINE2, &tmp);
	}
	// TIME CHECK MODE
	else if(page_status==6){
		for(tinkle_i=0; tinkle_i<dis_id; tinkle_i++){
			tmp[tinkle_i] = Time_Set[tinkle_i];
		}
		if(dis_id!=16){
			tmp[tinkle_i] = tinkle==1?'_':' ';
			tmp[tinkle_i+1] = '\0';
		}
		else{
			tmp[16] = '\0';
		}
		LCD12864_Display(LINE2, &tmp);
	}

}
/*-------------------------------------------------------------------------------------------------*/

void Display_Page(void){
	// UNLOCKED
	if(page_status==0){
		Display_Unlocked();
	}
	// LOCKED
	else if(page_status==1){
		Display_Locked();
	}
	// SETTINGS
	else if(page_status==2){
		LCD12864_Display(LINE1, "更改密码");
		LCD12864_Display(LINE2, "校对时间");
	}
	// PASSWORD CHANGE
	else if(page_status==3){
		LCD12864_Display(LINE1, "更改校对密码模式");
		LCD12864_Display(LINE2, "更改密码内容");
	}
	// PASSWORD MATCH MODE
	else if(page_status==4){
		LCD12864_Display(LINE1, "普通匹配模式");
		LCD12864_Display(LINE2, "虚位密码模式");
		LCD12864_Display(LINE3, "子串实现模式");
	}
	// PASSWORD CONTEXT MODE
	else if(page_status==5){
		if(set_twice==0){
			LCD12864_Display(LINE1, "请输入新密码: ");
		}
		else{
			LCD12864_Display(LINE1, "请再次输入新密码: ");
		}
		LCD12864_Display(LINE2, "_");
	}
	// TIME CHECK
	else{
		if(Time_Input==0){
			LCD12864_Display(LINE1, "请输入年份:");
		}
		else if(Time_Input==1){
			LCD12864_Display(LINE1, "请输入月份:");
		}
		else if(Time_Input==2){
			LCD12864_Display(LINE1, "请输入日期:");
		}
		else if(Time_Input==3){
			LCD12864_Display(LINE1, "请输入小时:");
		}
		else{
			LCD12864_Display(LINE1, "请输入分钟:");
		}
		LCD12864_Display(LINE2, "_");
		LCD12864_Clear_Line(LINE3);
		LCD12864_Clear_Line(LINE4);
	}

}

void NO_IDEA(void){

		LCD12864_Display(LINE1, "我不懂你的意思");
		LCD12864_Display(LINE2, "没有那样的东西");

}

/*-----------------------------Setting Function Relization-----------------------------------------*/
void Password_Setting_Check(void){
	
	if(twice_length!=temp_length){
		wrong_signal = 1;
	}
	else{
		for(itr=0; itr<twice_length; itr++){
			if(tmp_set[itr]!=twice_set[itr]){
				wrong_signal = 1;
				break;
			}
		}
		itr = 0;
	}
	
	if(wrong_signal==1){
		temp_length=twice_length = 0;
		input_point = 0;
		start = 0;
		wait = 0;
		wrong_break = 1;
		set_twice = 0;
		Display_Wrong();
	}
	
}
/*-------------------------------------------------------------------------------------------------*/

unsigned char Check(void){

	// Status Variables
	unsigned char xdata p, q, r, s, tail, tail_cnt;
	unsigned char xdata len1, len2;
	unsigned char xdata check_i, check_j;
	len1 = input_point;
	len2 = lock.length;
	p = q = r = s = 1;
	tail =  tail_cnt = 0;
	
	q = (lock.input_stream[0] == lock.password[0])?1:0;
	r = ((lock.input_stream[1] == lock.password[0]) ||\
		(len2>=1?(lock.input_stream[1] == lock.password[1]):1))?1:0;
	s = ((lock.input_stream[2] == lock.password[0]) ||\
		(len2>=1?(lock.input_stream[1] == lock.password[1]):1) ||\
		(len2>=2?(lock.input_stream[2] == lock.password[2]):1))?1:0;
	
	tail++;
	for(check_i = 3; check_i < len1-1; check_i++){
		if(tail>=len2){
			break;
		}
		p = q;
		q = r;
		r = s;
		s = ((lock.input_stream[check_i] == lock.password[tail]))?1:0;
		for(check_j = 1; check_j <= tail_cnt; check_j++){
			s = ((s==1)||(tail+check_j>=len2?1:(lock.input_stream[check_i] == lock.password[tail+check_j])))?1:0;
		}
		if(tail_cnt++==3){
			tail++;
			tail_cnt = 0;
		}
		s = ((s==1)&&(p==1||q==1||r==1))?1:0;
		
	}
	
	s = ((q==1||r==1||s==1)&&(lock.input_stream[len1-1]==lock.password[len2-1]))?1:0;
	return s;
	
}

void Display_Config_Success(void){

	LCD12864_Display(LINE1, "恭喜你！！！");
	LCD12864_Display(LINE2, "时间校准成功！！");

}

