  //array ki se napolni usake x sekund (default = 0.5s) 
  bool cache[10];
  
  //int ki ga uporabimo za cache[10] da vnesemo enke pa ničle
  int increment=0;
  
  // je switch za vžig programa
  bool sw = false;
  
  /* s tem int pozneje določimo koliko
  undefined cifer (to pomeni da ne zazna niti ene 1)
  lahko dobimo (default je dvakrat)*/
  int alert=0;
  
  //kill switch - ko je to true terminata program, ga uporabimo ko dobimo *UC dvakrat
  bool kill=false;
  
  //ta long bo končni rezultat tega programa
  long output=1;
  
  // ko držimo power on button se bo ta int povečal in ko bo deset vžge {bool sw} 
  int poweron=0;

  // pin declare
  int powerOnButton = 2;
  int dotDashButton = 5;
  int powerOnLED = 7;
  int intervalLED = 8;

  //input dodaja končni spremenljivki 1 ali 0
  void input(int i){
  output = output*10+i;
  }
  
  //type vzame cache[10] in ugotovi če gre za dot, dash ali undfined  
  int type(bool cache[]){
    int i=0;
  for(int j=0;j<10;j++){
    if(cache[j]==1)
    i++;
  }
  //če je undefined (nič enk)
  if(i==0)
    return 0;
  //če je dot(če držiš pod pol sekunde)
  else if(i<5)
    return 1;
  //če je dash (če držiš med pol sekunde in polno sekundo)
  else 
    return 2;
  }

  void setup() {
    Serial.begin(9600);
    pinMode(powerOnButton,INPUT);
    pinMode(dotDashButton,INPUT);
    pinMode(powerOnLED,OUTPUT);
    pinMode(intervalLED,OUTPUT);
  }


  void loop() {
    
    /*če je dvakrat undefined bo izpisalo končno črko (v morse) in
    vžgalo kill switch pozneje bom dodal namesto kill switch reset switch, 
    ki bo pripravil arduino na naslednje branje
    alert++ začasno da ne gre nazaj v if stavek*/
    if(alert==2){
    Serial.println(output);
    kill = true;
    alert++;
    }
    if(!kill){
      
      //če držiš powerOnButton poveča poweron ki pozneje zažene program
      if(digitalRead(2)){
        poweron++;
        delay(50);
      }

      //ko držiš powerOnButton vžge {bool sw}
      if(poweron>10){
        digitalWrite(7,HIGH);
        sw = true;
        delay(10000);
        poweron=0;
      }
      // če je sw vžgan začne program
      if(sw){
        //začetek led intervala (vžig)
        digitalWrite(8,HIGH);
        //vnos 1 ali 0
        cache[increment] = digitalRead(5);
        //delay za stabilnost (0.1s)
        delay(100);
        increment++;
           //ta del deluje ko je array napolnjen (kadar je increment 9)
           if(increment == 9){
              //konec LED intervala (izklop - minila je 1s)
              digitalWrite(8,LOW);
              //poklic funkcije ki pove če je dot, dash ali undefined
              int res = type(cache);
                 switch(res){
                    /*če je undefined shranimo prvi strike če je
                    še enkrat undefined bo končalo program*/
                    case 0:alert++;break;
                    case 1:input(0);break;
                    case 2:input(1);break;
                  }
              //reset increment
              increment=0;
              /*izenači interval (1s LED vžgana (program bere)
              1s ugasnjena (program ne bere))*/
              delay(1000);
            } 
        }
}
}
