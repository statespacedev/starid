#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include "dw_def.h"
#include "dw_shm.h"
#include "mathfkt.h"     /* math. Funktionen (incl. <math.h> */

extern int seed();
extern int incident();
extern float ran();

int main()    
{

  int i,w;
  int i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,isum,i05,i10;
  float rnd;
  i0=i1=i2=i3=i4=i5=i6=i7=i8=i9=i05=i10=0;

  for (i=0;i<1000000;i++)
    {
      rnd=ran();
      w=incident(75);
      if (w==0) i05++;
      if (w==1) i10++;
      //      printf("i=%d rnd=%f\n",i,rnd);
      if (rnd < 0.1 && rnd >= 0.0) i0++;
      if (rnd < 0.2 && rnd >= 0.1) i1++;
      if (rnd < 0.3 && rnd >= 0.2) i2++;
      if (rnd < 0.4 && rnd >= 0.3) i3++;
      if (rnd < 0.5 && rnd >= 0.4) i4++;
      if (rnd < 0.6 && rnd >= 0.5) i5++;
      if (rnd < 0.7 && rnd >= 0.6) i6++;
      if (rnd < 0.8 && rnd >= 0.7) i7++;
      if (rnd < 0.9 && rnd >= 0.8) i8++;
      if (rnd <= 1.0 && rnd >= 0.9) i9++;
    }
  isum=i0+i1+i2+i3+i4+i5+i6+i7+i8+i9;
  printf("i0=%d i1=%d i2=%d i2=%d i4=%d i5=%d i6=%d i7=%d i8=%d i9=%d isum=%d\n",i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,isum);
  printf("ja zu nein: %d zu %d\n",i05,i10);
exit(0);

}

float ran()
/*****************************************************************************/
/*** ran() liefert eine Float-Zufallszahl 0.0 bis 1.0                      ***/
/*****************************************************************************/
{
int id,m1,ia1,m2,ia2,ic2,ic1,m3,ia3,ic3,iff,ix1,ix2,ix3,j,dummy,idum,err_cnt;
float r[97],rm1,rm2;

id=seed();	
m1=259200;
ia1=7141;
ic1=54773;
rm1=1.0/(float) m1;
m2=134456;
ia2=8121;
ic2=28411;
rm2=1.0/(float) m2;
m3=243000;
ia3=4561;
ic3=51349;
iff=0;
err_cnt = 0;
ix1=seed();
ix2=seed();
ix3=seed();
idum=seed();

init:
dummy=seed();
idum=idum+dummy/10000;
idum=(idum*10000)-dummy*id;
idum=idum*id; 

/* as above, initailize on first call even if idum is not negative */

if ( (idum<0) || (iff==0) )
   {
   iff=1;
   ix1=((ic1-idum)%m1);   /* seed the first routine */
   ix1=((ia1*ix1+ic1)%m1);
   ix2=ix1%m2;  /* and use it to seed the second */
   ix1=((ia1*ix1+ic1)%m1);
   ix3=ix1%m3;  /* and the third routine */
   /* fill the table with sequ. uniform deviates gen. by the routines 1,2 */
   for (j=0;j<97;j++)
       {
       ix1=((ia1*ix1+ic1)%m1);
       ix2=((ia2*ix2+ic2)%m2);
       r[j]=((rm2*ix2)+ix1)*rm1; /* low and high-order pieces combined here */
       }
   idum=1;
   }
/*  except when initializing,this is where we start generate the next number */
/*  for each sequence */
ix1=(ia1*ix1+ic1)%m1;
ix2=(ia2*ix2+ic2)%m2;
ix3=(ia3*ix3+ic3)%m3;
/* use the third sequ. to get an int between 1 and 97 */;
j=1 + ((float)(97*ix3) / (float)m3);
if (err_cnt>100)
   {
   j = 0;
   err_cnt = 0;
   }
if (j>96 || j<0)
   {
   err_cnt++;
   goto init;
   }
/* return that table entry and refill it */
r[j]= rm1 * (ix1+(ix2 * rm2));
dummy=r[j]*1000;
r[j]=r[j]*1000.0-(float)dummy;
r[j]=sqrt( (r[j]*r[j]) ); 
return(r[j]);
}    /**** Ende ran.c ****/


int seed(void)
/* benoetigt -lrt beim Linken */
{
  int i;
  struct timespec ntime;
  int iretval;
  iretval = clock_gettime(CLOCK_REALTIME, &ntime);
  i = (int) ntime.tv_nsec%10000;
  return i;
}

int incident(w)
    int w;
/************************************************************************/
/*** berechnet, ob das Ereignis mit der Wahrscheinlchkeit w eintrifft  **/
/*** w = 1...100 , incident=0 =nicht eing.                            ***/
/************************************************************************/
{
int diff;
float rnd;

rnd=ran();

rnd=rnd*100.0;
diff=(NINT(rnd));

if (diff<w) 
   {
   return(1);
   }
else
   {
   return(0);
   }

}
