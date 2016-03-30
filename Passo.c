#include <16F877A.h>
#device adc=10
#FUSES NOWDT,XT,PUT,NOPROTECT,NODEBUG,BROWNOUT,NOLVP,NOCPD 
#use delay(clock=4000000)
#include <flex_lcd.c>
#define frame_time      0                  //Velocidade do motor

int8 i = 3, i_1 = 0,j;
int16 ad_antigo, ad_atual, total_passos, posicoes_na_escala;
signed int16 pos_atual =0;
int16 passos_dados =0;
int16 ultimaRPM = 0;
float passos_por_posicao;
int8 y;
signed int16 distancia;

char motor[4]=
  {
     0b1100, //PASSO1
     0b0110, //PASSO2
     0b0011, //PASSO3
     0b1001, //PASSO4      
  };  
 
int16 rpms[4]={9,35,9,20};
   
  
  
void avanca(int16 qtdPassos){
   int16 passos=qtdPassos-passos_dados;
   passos_dados=qtdPassos;
   for (i=0;i<passos;i++){
      pos_atual++;      
      if(pos_atual>3){
         pos_atual = 0;
      } 
      output_b(motor[pos_atual]);            
      delay_ms (10);
   }   
}

void retrocede(int16 qtdPassos){
   int16 passos=passos_dados-qtdPassos;
   passos_dados=qtdPassos;   
   for (i=0;i<passos;i++){      
      pos_atual--;  
      if(pos_atual<0){
         pos_atual = 3;
      } 
      output_b(motor[pos_atual]);              
      delay_ms (10);
   }        
}

void setaRPM(int16 rpm){        
   if(rpm>ultimaRPM){   
      avanca(rpm*passos_por_posicao);
      ultimaRPM=rpm;
   }else if(rpm<ultimaRPM){
      retrocede(rpm*passos_por_posicao);
      ultimaRPM=rpm;
   }
   printf(lcd_putc,"\f%lu RPM",ultimaRPM*100); 
   
}

void main ()
{ 
   posicoes_na_escala = 60; //Simulando um contagiros até 6000 RPM
   total_passos = 167; //Total de posições do motor entre o marcador 0 até 6 (passo a 1,8º)
   passos_por_posicao = (float)total_passos/posicoes_na_escala; //Total de passos para cada posição no conta giros (a agulha anda de 50 em 50 RPM)
   lcd_init();
   delay_ms (100);
  
   SETUP_ADC_PORTS(AN0);
   SETUP_ADC(ADC_CLOCK_DIV_16);
  
   set_adc_channel(0);
   delay_us (20);
   
   printf(lcd_putc,"\fPosicionando...");   
   avanca(117); // 0 RPM   
   passos_dados=0;   
   printf(lcd_putc,"\f0 RPM");
   
        
   for (i_1=0;i_1<4;i_1++){     
      delay_us (300);
      setaRPM(rpms[i_1]);
   }

  /*
  while(true){

     ad_atual = read_adc();

     distancia = ad_atual - ad_antigo;   
     printf(lcd_putc,"\fnow:%lu d:%ld\nold:%lu pp:%f",ad_atual,distancia, ad_antigo,passos_por_posicao );
     
     if (distancia != 0)
     {
        if (distancia > 0)
        {
           y = 0.195 * distancia;            
           for (j=0;j<y;j++)
           { 
              output_b(motor[i_1]);
              i_1++;
              if (i_1 > 3)
              {
                 i_1 = 0;
              }
              delay_ms(frame_time);
           }
        }            
        else if (distancia < 0)
        {
           y = 0.195 * distancia * -1;            
           for (j=0;j<y;j++)
           {                                
              output_b(motor[i]);
              i--;
              if (i == 255)
              {
                 i = 3;
              }
              delay_ms(frame_time);
           }
        }
     }
     
     ad_antigo = ad_atual;
     delay_ms (20);
     
  }
  */
}



