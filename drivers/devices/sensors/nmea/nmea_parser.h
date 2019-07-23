/*
 * nmea_parser.h
 *
 * Created: 11.06.2019 22:43:33
 *  Author: bear1ake
 */ 

#include "../../../../kernel/types.h"
char rmc_str[128];
unsigned char rmc_index;
unsigned char field_index;
unsigned char packet_type; 
 // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
volatile struct GPS_t GPS;

//EXTREMELY dirty and inefficient latitude format conversion
float convertToDecimal(float lat){
	float temp1, temp2;
	int temp3;
	temp1 = lat/100;
	temp3 = temp1;
	temp1 -= (float)temp3;
	temp1 *= 100;
	temp1 /= 60;
	temp2 = temp1 + (float)temp3;
	return temp2;
}

unsigned char number_code(char chr)
{
	if ((chr>=0x30)&&(chr<=0x39))
	{
		return chr-0x30;
	} 
	return 0xFF;
}

unsigned char compare(char * a, char * b,unsigned char  len)
{
 unsigned char i;
 for (i=0;i<len;i++)
 {
  if (a[i]!=b[i]) return 0;
 }
 return 1;
}

//$GPGSV,t,u,vv,ww,ww,www,ww,xx,xx,xxx,xx,yy,yy, yyy,yy,zz,zz,zzz,zz*hh<CR><LF>
//$GPRMC,hhmmss.ss,A,GGMM.MM,P,gggmm.mm,J,v.v,b.b,ddmmyy,x.x,n,m*hh<CR><LF>
//0      1         2 3       4 5        6 7   8   9      10  11 12
unsigned char nmea_load(char data)
{
unsigned char ret;
ret=1;
    if (data==',')
    { 
        switch(field_index)
        { 
         case 0:  //NMEA Header
             if (compare(rmc_str,"GPRMC",5)==1)
             {
             packet_type=1;
             }else if (compare(rmc_str,"GPGSV",5)==1)
             {
             packet_type=2;             
             }else{
             packet_type=0;             
             }
              
         break;
         case 1:  //time 
            //hhmmss.ss       
            if ((rmc_index>6)&&(packet_type==1))
            {
             GPS.hour=number_code(rmc_str[0])*10+number_code(rmc_str[1]);
             GPS.minute=number_code(rmc_str[2])*10+number_code(rmc_str[3]);
             GPS.second=number_code(rmc_str[4])*10+number_code(rmc_str[5]);
             
            }          
         break;
         case 2: //data valid
            if ((rmc_index>0)&&(packet_type==1)){
             if (rmc_str[0]=='A')
                GPS.valid=1;
             else
                GPS.valid=0;      
			}
         break;
         case 3: //Sats & latitude 
            if ((rmc_index>1)&&(packet_type==2))
            {   
             GPS.Sats=number_code(rmc_str[0])*10+number_code(rmc_str[1]);
            } 
            if ((rmc_index>5)&&(packet_type==1))
            {   
             GPS.latitude =atof(rmc_str);
            }         
         break;            
         case 4: //P
           if ((rmc_index>0)&&(packet_type==1))    
           {
               GPS.P=rmc_str[0];
           }
         break;
         case 5: //Longitude
           if ((rmc_index>5)&&(packet_type==1))    
           {
               GPS.longitude=atof(rmc_str);
           } 
             
         break;
         case 6: //J
           if ((rmc_index>0)&&(packet_type==1))    
           {
               GPS.J=rmc_str[0];
           }         
         break;
         
         case 7: //Speed
            if ((rmc_index>0)&&(packet_type==1))    
           {
                GPS.speed=atof(rmc_str);
           }
         
         break;    
         case 8: //course
             if ((rmc_index>0)&&(packet_type==1))    
           {
                GPS.cource=(int)atof(rmc_str);
           }  
         
         break;
           
         case 9: //date 
           if ((rmc_index>5)&&(packet_type==1))
            {
             GPS.day =number_code(rmc_str[0])*10+number_code(rmc_str[1]);
             GPS.month=number_code(rmc_str[2])*10+number_code(rmc_str[3]);
             GPS.year=number_code(rmc_str[4])*10+number_code(rmc_str[5]);
             
            }          
         
         break;
        }
        
    
    rmc_index=0;       
    rmc_str[rmc_index]=0;
    field_index++;
    }
    
    else{
     rmc_str[rmc_index]=data;
     rmc_str[rmc_index+1]=0;
     rmc_index++;               
     if (rmc_index>127)
     {        
        ret=2;
        rmc_index=0;
        field_index=0;
     }
      if (data=='$') 
        {
        ret=2;
        rmc_index=0;
        field_index=0;
        rmc_str[rmc_index]=0;
        }

         
         
    }
    


return ret;
 
}