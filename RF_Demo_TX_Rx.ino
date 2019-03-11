#define RF_pin 9      // Make RF pin output in the setup.
#define RF_rx 10      // Make RF rx input in the setup.

unsigned long currentMicros;
const bool role = 0; // role = 0 configures arduino as rx
                     // role = 1 configures arduino as tx
void send_int(int a)  // This function transmits an integer with necesarry headers for detection
{
  byte i = 0;
  while(i <20 ) // Cycle RF transmitter on-off to "wake up" the receiver
  {
    digitalWrite(RF_pin,HIGH);  
    delayMicroseconds(500);
    digitalWrite(RF_pin,LOW);
     delayMicroseconds(500);
    i++;
  }
  i = 0;
  digitalWrite(RF_pin,HIGH); // Header that 'tells' the micro-controller that valid data is coming.
  delay(2);
  digitalWrite(RF_pin,LOW);
  delay(1);
  for(i = 0; i < 16 ; i++)  // Actual transmission of data with a sort of PWM.
  {
    digitalWrite(RF_pin, HIGH);
    delayMicroseconds(500);
    if( ((a>>i)&1) == 0)
      digitalWrite(RF_pin,LOW);
    delayMicroseconds(500);
    digitalWrite(RF_pin,LOW);
    delayMicroseconds(500);
  }
   digitalWrite(RF_pin,LOW); // Set the RF pin low 
}

int receive_int()   // This function receives an integer along with some support logic to detect the header
{
  int temp = 0;
  int j = 0;
  unsigned long mark = 0;
  while(j < 16)
  {
   while(digitalRead(0) == LOW);
   mark = micros();
   while(digitalRead(0) == HIGH);
   if(micros() - mark>700)
     temp = temp|(1<<j);
    j++;
  }
return temp;    // Return the integer
}
int data = 0;
void setup()
{
if(role==1)
  pinMode(RF_pin,OUTPUT); // Configuring output pin for Tx
else
  pinMode(RF_rx,INPUT);   // Need not be done explicitly
}
void loop()
{
  if(role==0)
  {
    while(1) //This is an infinite loop that searches for data 
    {
      if(digitalRead(0))
      {
        currentMicros = micros();
        while(digitalRead(0));
        if(micros()-currentMicros > 1200)
        {
         data = receive_int();
         // Do whatever you want to do with the data.
        }
      }
    }
  }
  else // This is an infinite loop that transmits data
  {
    while(1)
    {
      send_int(data); 
      delay(1000);
    }
  }
}
