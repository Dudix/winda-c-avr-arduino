#include <avr/io.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd0(6, 5, 4, 3, 2, 1); 
LiquidCrystal lcd1(12, 11, 10, 9, 8, 7); 
LiquidCrystal lcdC (18, 17, 16, 15, 14, 13);

int button0Pin = 19;
int button1Pin = 20;
int buttonCPin = 21;

int doorOpen0Pin = 22;
int doorOpen1Pin = 23;
int doorOpenCPin = 24;

int doorMove0Pin = 25;
int doorMove1Pin = 26;
int doorMoveCPin = 27;

int doorDir0Pin = 28;
int doorDir1Pin = 29;
int doorDirCPin = 30;

int lvlActive0Pin = 31;
int lvlActive1Pin = 32;

int engineMovePin = 33;
int engineDirPin = 34;


pinMode(button0Pin, INPUT);
pinMode(button1Pin, INPUT);
pinMode(buttonCPin, INPUT);

pinMode(doorOpened0Pin, INPUT);
pinMode(doorOpened1Pin, INPUT);
pinMode(doorOpenedCPin, INPUT);

pinMode(doorClosed0Pin, INPUT);
pinMode(doorClosed1Pin, INPUT);
pinMode(doorClosedCPin, INPUT);

pinMode(doorMove0Pin, OUTPUT);
pinMode(doorMove1Pin, OUTPUT);
pinMode(doorMoveCPin, OUTPUT);

pinMode(doorDir0Pin, OUTPUT);
pinMode(doorDir1Pin, OUTPUT);
pinMode(doorDirCPin, OUTPUT);

pinMode(lvlActive0Pin, INPUT);
pinMode(lvlActive1Pin, INPUT);

pinMode(engineMovePin, OUTPUT);
pinMode(engineDirPin, OUTPUT);


digitalWrite(button0Pin, LOW);
digitalWrite(button1Pin, LOW);
digitalWrite(buttonCPin, LOW);

digitalWrite(doorOpened0Pin, HIGH);
digitalWrite(doorOpened1Pin, HIGH);
digitalWrite(doorOpenedCPin, HIGH);

digitalWrite(doorClosed0Pin, HIGH);
digitalWrite(doorClosed1Pin, HIGH);
digitalWrite(doorClosedCPin, HIGH);

digitalWrite(doorMove0Pin, LOW);
digitalWrite(doorMove1Pin, LOW);
digitalWrite(doorMoveCPin, LOW);

digitalWrite(doorDir0Pin, LOW);
digitalWrite(doorDir1Pin, LOW);
digitalWrite(doorDirCPin, LOW);

digitalWrite(lvlActive0Pin, HIGH);
digitalWrite(lvlActive1Pin, HIGH);

digitalWrite(engineMovePin, LOW);
digitalWrite(engineDirPin, LOW);

int main(void){

	lcd0.begin(16, 2);
	lcd1.begin(16, 2);
	lcdC.begin(16, 2);
	
	int targetLvl = 0;

    while(1){
		
		closeDoors();
		moveCabin();
		openDoors();
		waitForAction();
		
	}

	void closeDoors(){
		
		bool closed0 = false;
		bool closed1 = false;
		bool closedC = false;
		bool wasWarning = false;
		
		while(!closed0 || !closed1 || !closedC){
			
			closed0 = digitalRead(doorClosed0Pin);
			closed1 = digitalRead(doorClosed1Pin);
			closedC = digitalRead(doorClosedCPin);
			
			if( !closed0 ){
				
				if(wasWarning){
					digitalWrite(doorDir0Pin, HIGH);
					digitalWrite(doorMove0Pin, HIGH);
				}else{
					lcd0.clear();
					lcd0.print("Zamykanie drzwi");
				}
				
			}else{
				
				digitalWrite(doorMove0Pin, LOW);
				
			}
		
			if( !closed1 ){
				
				if(wasWarning){
					digitalWrite(doorDir1Pin, HIGH);
					digitalWrite(doorMove1Pin, HIGH);
				}else{
					lcd1.clear();
					lcd1.print("Zamykanie drzwi");
				}
				
			}else{
				
				digitalWrite(doorMove1Pin, LOW);
				
			}
				
			if( !closedC ){
				
				if(wasWarning){
					digitalWrite(doorDirCPin, HIGH);
					digitalWrite(doorMoveCPin, HIGH);
				}else{
					lcdC.clear();
					lcdC.print("Zamykanie drzwi");
				}
				
			}else{
				
				digitalWrite(doorMoveCPin, LOW);
				
			}
		
			if(!wasWarning){
				wasWarning = true;
				delay(2000);
			}
		}
		
		
		lcd0.clear();
		lcd1.clear();
		lcdC.clear();
		
	}
	
	void openDoors(){
		
		bool opened0 = true;
		bool opened1 = true;
		bool openedC = true;
		
		while(opened0 && opened1 && openedC){
			
			opened0 = digitalRead(doorOpened0Pin);
			opened1 = digitalRead(doorOpened1Pin);
			openedC = digitalRead(doorOpenedCPin);
		
			if( targetLvl == 0 ){
			
				if( !opened0 ){
				
					digitalWrite(doorDir0Pin, LOW);
					digitalWrite(doorMove0Pin, HIGH);
				
				}else{
					
					digitalWrite(doorMove0Pin, LOW);
				
				}
				
			}
		
			if( targetLvl == 1 ){
			
				if( !opened1 ){
				
					digitalWrite(doorDir1Pin, LOW);
					digitalWrite(doorMove1Pin, HIGH);
					
				}else{
				
					digitalWrite(doorMove1Pin, LOW);
					
				}
				
			}
		
			if( !openedC ){
				
				digitalWrite(doorDirCPin, LOW);
				digitalWrite(doorMoveCPin, HIGH);
				
			}else{
			
				digitalWrite(doorMoveCPin, LOW);
			
			}
			
		}
		
	}
	
	void moveCabin(){
		
		if(targetLvl == 0){
			digitalWrite(engineDirPin, LOW);
		}else{
			digitalWrite(engineDirPin, HIGH);
		}

		lcdC.clear();	
		lcdC.print("Zmiana poziomu");
		
		while(1){		

			if( 
				( targetLvl == 0 && digitalRead(lvl0ActivePin) == LOW ) || 
				( targetLvl == 1 && digitalRead(lvl1ActivePin) == LOW ) 
			){
				digitalWrite(engineMovePin, HIGH);
			}else{
				break;
			}
		
		}
		
		lcdC.clear();	
		digitalWrite(engineMovePin, LOW);
		
	}
	
	void waitForAction(){
		
		while(1){
			
			delay(500);
			
			if( ( digitalRead(button0Pin) == HIGH && targetLvl != 0 ) || 
				( digitalRead(button1Pin) == HIGH && targetLvl != 1 ) ||
				digitalRead(buttonCPin) == HIGH
			){
				
				if( ( digitalRead(button0Pin) == HIGH && targetLvl != 0 ) ){
					
					lcd0.clear();
					lcd0.print("Proszę czekać");
					
					digitalWrite(button0Pin, LOW);
					targetLvl = 0;
					
				}else if( ( digitalRead(button1Pin) == HIGH && targetLvl != 1 ) ){
					
					lcd1.clear();
					lcd1.print("Proszę czekać");
					
					digitalWrite(button1Pin, LOW);
					targetLvl = 1;
					
				}else if( digitalRead(buttonCPin) == HIGH ){
					
					digitalWrite(buttonCPin, LOW);
					
					if(targetLvl == 0){
						targetLvl = 1;
					}else{
						targetLvl = 0;
					}
					
				}
				
				break;
			}
			
		}
		
	}
	
}
