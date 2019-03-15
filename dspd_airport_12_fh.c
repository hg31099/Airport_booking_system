/*  AIRPORT BOOKING SYSTEM READ ME BLOCK:
-This project adds service windows and cater flights to it.
-service window is implemented using hash table to get O(1) .
-service windows should have different name for different company
-flight id is given to be in format as first letter of service window name 
then next 2 digits are like s.no. of flight.
-ticket id is assigned in format as seat number appended with ticket id
PREFERANCES:
	1)seat type is given more preferance as this is for comfortable journey of our passengers
	2)moreover passengers with same ticket id are given nearby seats as much as possible.
	3)if prefered seats are not available then other seats which are nearby to passengers with same ticket id are alloted.
*/
//preproccesor directives
#include<stdio.h>
#include<string.h> 
#include<stdlib.h>
#include<math.h>
#define ts 60
#define sw_size 26
//------------------------------------------------------------------
typedef enum{FALSE,TRUE} boolean;
//passenger structure

typedef struct passenger_tag
{
    char name[50];
    int age;
    int luggage_num;
    int luggage_wt;
    int fine;
    int sno;
    struct passenger_tag *next;
    struct passenger_tag *prev;
}passenger;
//ticket structure

typedef struct ticket_tag
{
    char ticket_id[6];
    passenger *p;
    int pass_num;
    struct ticket_tag *next;
    struct ticket_tag *prev;
}ticket;

//flight structure

typedef struct flight_tag
{
    char fid[4];
    ticket *thptr;
    ticket *tlptr;
    int s[ts];
    struct flight_tag *next;
    struct flight_tag *prev;
}fnode;
//service window structure

typedef struct service_window
{
    char sw_name[10];
    fnode *flist;
    struct service_window *next;
    struct service_window *prev;
}swnode;
//hash table for O(1) complexity
swnode *swtable[sw_size];
//-------------------------------------------------------------------
//hashing function returns index


int hash_swtable(char c)
{
    int index;
    index=(c-'a')%sw_size;
    return index;
}
//intialisation functions
void intialise_swtable(swnode *swtable[])
{
    int i=0;
    for(i=0;i<sw_size;i++)
    {
        swtable[i]=NULL;
    }
}

swnode * intialise_swnode(swnode *swptr)
{
    swptr->sw_name[0]='\0';
    swptr->flist=NULL;
    swptr->next=NULL;
    swptr->prev=NULL;
    return swptr;
}

fnode * intialise_fnode(fnode *fptr)
{
    int i=0;
    fptr->fid[0]='\0';
    for(i=0;i<ts;i++)
    {
        fptr->s[i]=0;
    }
    fptr->thptr=NULL;
    fptr->tlptr=NULL;
    fptr->next=NULL;
    fptr->prev=NULL;
}

passenger * intialise_passenger(passenger *p)
{
    p->name[0]='\0';
    p->age=0;
    p->luggage_num=0;
    p->luggage_wt=0;
    p->fine=0;
    p->sno=0;
    p->next=NULL;
    p->prev=NULL;
    return p;
    
}

ticket * intialise_ticket(ticket *t)
{
    t->ticket_id[0]='\0';
    t->p=NULL;
    t->pass_num=0;
    t->next=NULL;
    t->prev=NULL;
    return t;
}
//----------------------------------------------------------------
//takes password input from user and stores it in str

void password_input(char str[])
{
	char c=' ';
	int i=0;
	printf("\n Enter the password [max length 10] : ");
	while (i<=9){
	    str[i]=getch();
	    c=str[i];
	    if(c==13) break;   //carriage return ... 
	    else printf("*");
	    i++;
	}
	str[i]='\0';
	//printf("\n Your password is : %s",str);
	
}
//creates and intialises service window------------------------------------

swnode * create_swnode()
{
    swnode *nptr;
    nptr=(swnode *)malloc(sizeof(swnode));
    nptr=intialise_swnode(nptr);
    return nptr;
}
//adds flight to that service window---------------------------------------------

fnode * add_flight(fnode *fptr,char idi)
{
    fnode *nptr;
    nptr=(fnode *)malloc(sizeof(fnode));
    nptr=intialise_fnode(nptr);
    fnode *ptr;
    if(fptr==NULL)
    {
        fptr=nptr;
    }
    else
    {
        ptr=fptr;
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=nptr;
        nptr->prev=ptr;
    }
    
    printf("Enter flight id:(in specified format)\n");  //in format first letter of service window and then flight number
    scanf("%s",nptr->fid);
    
    return fptr;
    
}

//-------------------------------------------------------------------------

void addFlightServiceWindow(swnode *swtable[])
{
    swnode *ptr,*nptr;
    nptr=create_swnode();
    int num=0,i=0;
    printf("Enter service window name:\n");
    scanf("%s",nptr->sw_name);
    printf("Enter number of flights to this window:\n");
    scanf("%d",&num);
    int index=0;
    index=hash_swtable(nptr->sw_name[0]);
    if(swtable[index]==NULL)
    {
        swtable[index]=nptr;
    }
    else
    {
        ptr=swtable[index];

        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=nptr;
        nptr->prev=ptr;
    }
   
    for(i=0;i<num;i++)
    {
        nptr->flist=add_flight(nptr->flist,nptr->sw_name[0]);
    }
    printf("Successfully added service window for %s \n",nptr->sw_name);
  
    
}
//this function prints all available seats of flight---------------------------------------

void print_av_seats(fnode *f_ptr)
{
    printf("\n");
 //   printf("W M A   A M W");
    int i=0,j=0,count=0;
    for(i=0;i<(ts);i++)
    {
       if(f_ptr->s[i]==0)
       {
           printf("%d ",(i+1));
           if((i+1)%3==1) printf(" W \n");
           if((i+1)%3==2) printf(" M \n");
           if((i+1)%3==0) printf(" A \n");
           count++;
       }
    }
    printf("Total available seats are %d \n",count);
}

void print_seat_chart(fnode *f_ptr)
{
	printf("\n");
	int i=0,j=0;
	printf("W  M  A \tA  M  W \n");
	for(i=0;i<(ts)/6;i++)
	{
		for(j=0;j<3;j++)
		{
			if(f_ptr->s[3*i+j]==0)
			{
				if((3*i+j)<9) printf("0%d ",(3*i+j+1));
				else  printf("%d ",(3*i+j+1));
			}
			else printf("   ");
		}
		printf("\t");
		for(j=0;j<3;j++)
		{
			if(f_ptr->s[3*(i+11)-j-1]==0)
			{
				printf("%d ",(3*(i+11)-j));
			}
			else printf("   ");
		}
		printf("\n");
	}
}

//it returns total count of available seats--------------------------------------
int num_av_seats(fnode *f_ptr)
{
     int i=0,j=0,count=0;
    for(i=0;i<(ts);i++)
    {
       if(f_ptr->s[i]==0)
       {
           count++;
       }
    }
    return count;
}
//-----------------------------------------------------------------------------------

void DisplayAvailableSeats(char f_id[])
{
  int index,found=0,i,count=0;
  swnode *ptr;
  fnode *fptr;
  index=hash_swtable(f_id[0]);
  if(swtable[index]==NULL)   //search for that index in hash table
  {
      printf("No such flight exists \n");
  }
  else   //if found then search for that flight id
  {
      ptr=swtable[index];
      fptr=ptr->flist;
      while(found==0 && ptr!=NULL)
      {
          while(found==0 && fptr!=NULL)
          {
              if(strcmp((fptr)->fid,f_id)==0)
              {
                 print_av_seats(fptr);
                 print_seat_chart(fptr);
                  found=1;
              }
              else
              {
                  fptr=fptr->next;
                  
              }
          }
          ptr=ptr->next;
          if(ptr!=NULL)
          {
            fptr=ptr->flist;
          }
      }  
      
      if(found==0)
      {
          printf("No such flight exists \n");
      }
      
  }

    
}
//--------------------------------------------------------------------------------
void allocateBoardingPass(char f_id[])
{
    int pnum=0;
    printf("Enter number of passengers \n");
    scanf("%d",&pnum);
    int index,found=0,i,count=0;
      swnode *ptr;
      fnode *fptr;
      index=hash_swtable(f_id[0]);    //using flight id ,searching for flight
      if(swtable[index]==NULL)
      {
          printf("No such flight exists \n");
      }
      else
      {
          ptr=swtable[index];
          fptr=ptr->flist;
          while(found==0 && ptr!=NULL)
          {
              while(found==0 && fptr!=NULL)
              {
                  if(strcmp((fptr)->fid,f_id)==0)
                  {
                      count=num_av_seats(fptr);  //available seats should be>= passenger _num
                      found=1;
                  }
                  else
                  {
                      fptr=fptr->next; 
                  }
              }
              if(found==0)
              {
                ptr=ptr->next;
                if(ptr!=NULL)
                {
                    fptr=ptr->flist;
                }
              }    
          }  
          
          if(found==0)
          {
              printf("No such flight exists\n");
              
          }
        
      }
  i=0;  //tail pointer used to get O(1) complexity in inserting at end
  passenger *pptr,*tptr,*nptr;
  passenger *ahptr,*whptr,*mhptr,*atptr,*wtptr,*mtptr,*wtravptr,*mtravptr,*atravptr;
  wtravptr=mtravptr=atravptr=ahptr=whptr=mhptr=atptr=wtptr=mtptr=NULL;
  
  char ch;
  
  ticket  *tickptr;
  tickptr=NULL;
  int win_seat=0;
  int ais_seat=0,fine=0,mid_seat=0;
  //if flight is found then take the input from the passengers and store it in linked lists
  //3 different linked lists are created for each seat preferances
  int choice=1;
  printf("1)to give input \n 2)to take default input");
  scanf("%d",&choice);
  if(found==1 && count>=pnum)
  {
      tickptr=(ticket *)malloc(sizeof(ticket));
      tickptr=intialise_ticket(tickptr);
      tickptr->pass_num=pnum;
      if(choice==1)
    {
      for(i=0;i<pnum;i++)
      {
      	
		  
          pptr=(passenger *)malloc(sizeof(passenger));
          pptr=intialise_passenger(pptr);
          printf("Enter passengers details:\n");
          printf("name: \n");
          scanf("%s",pptr->name);
          printf("age: \n");
          scanf("%d",&pptr->age);
          printf("luggage nos.: \n");
          scanf("%d",&pptr->luggage_num);
          printf("luggage weight: \n");
          scanf("%d",&pptr->luggage_wt);
          fine=((pptr->luggage_wt)-15)*100;
          if(fine<0) fine=0;
       
       
          if((pptr->age)<=15)
          {
              win_seat++;
               nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
             strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(whptr==NULL)
                {
                    whptr=nptr;
                    wtptr=nptr;
                }
                else
                {
                    wtptr->next=nptr;
                    nptr->prev=wtptr;
                    wtptr=nptr;
                }
          }
          if((pptr->age)>=60)
          {
              ais_seat++;
              nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(ahptr==NULL)
                {
                    ahptr=nptr;
                    atptr=nptr;
                }
                else
                {
                    atptr->next=nptr;
                    nptr->prev=atptr;
                    atptr=nptr;
                }
          }
        if((pptr->age)<60 &&(pptr->age)>15)
        {
            mid_seat++;
            nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(mhptr==NULL)
                {
                    mhptr=nptr;
                    mtptr=nptr;
                }
                else
                {
                    mtptr->next=nptr;
                    nptr->prev=mtptr;
                    mtptr=nptr;
                }
            
        }
        
      }
  }
  
      else
       {
       	char str[100];
  int d1=0,d2=0,d3=0;
       		FILE *f;
  			f = fopen("airport.txt","r");
  		//	ch=fgetc(f);
  		i=0;
  			while(fscanf(f,"%s %d %d %d",str,&d1,&d2,&d3)!=-1)
  			{
		///	printf("%d",d1);
		pptr=(passenger *)malloc(sizeof(passenger));
          pptr=intialise_passenger(pptr);
			  strcpy(pptr->name,str);
			  	pptr->age=d1;
	  			pptr->luggage_num=d2;		
	  			pptr->luggage_wt=d3;	
	  			fine=((pptr->luggage_wt)-15)*100;
	          	if(fine<0) fine=0;
	          	if((pptr->age)<=15)
    	      {
              win_seat++;
               nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
             strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(whptr==NULL)
                {
                    whptr=nptr;
                    wtptr=nptr;
                }
                else
                {
                    wtptr->next=nptr;
                    nptr->prev=wtptr;
                    wtptr=nptr;
                }
          }
          if((pptr->age)>=60)
          {
              ais_seat++;
              nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(ahptr==NULL)
                {
                    ahptr=nptr;
                    atptr=nptr;
                }
                else
                {
                    atptr->next=nptr;
                    nptr->prev=atptr;
                    atptr=nptr;
                }
          }
        if((pptr->age)<60 &&(pptr->age)>15)
        {
            mid_seat++;
            nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(mhptr==NULL)
                {
                    mhptr=nptr;
                    mtptr=nptr;
                }
                else
                {
                    mtptr->next=nptr;
                    nptr->prev=mtptr;
                    mtptr=nptr;
                }
            
        }
	        
	        }
	      fclose(f);  
	   
	   
}
      pnum=win_seat+ais_seat+mid_seat;
    // we have got count of each sseat preferances and corrosponding linke lists
    //seats are alloted accord to prefernances for child and aged people
    //and nearby seats are given to passengers with same ticket id
    wtravptr=whptr;
    mtravptr=mhptr;
    atravptr=ahptr;
    i=0;
    int seat_num=1;
    for(i=0;i<ts && pnum>0;i++)
    {
        seat_num=i+1;
        if(fptr->s[i]==0)
        {
            if(seat_num%3==1)
            {
              if(win_seat>0)
              {
                wtravptr->sno=seat_num;
                win_seat--;
                pnum--;
                fptr->s[i]=1;
                wtravptr=wtravptr->next;
              }
              else
              {
                 if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->s[i]=1;
                     mtravptr=mtravptr->next;
                 }
              }
            }
            if(seat_num%3==2)
            {
               if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->s[i]=1;
                     mtravptr=mtravptr->next;
                 }
                
            }
            if(seat_num%3==0)
            {
                if(ais_seat>0)
              {
                atravptr->sno=seat_num;
                ais_seat--;
                pnum--;
                fptr->s[i]=1;
                atravptr=atravptr->next;
              }
              else
              {
                 if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->s[i]=1;
                     mtravptr=mtravptr->next;
                 }
              }
                
            }
        }
        
        
    }
    //if prefered  seats are not available,then we allocate them other nearby seats
    if(pnum>0)
    {
        for(i=0;i<ts && pnum>0;i++)
        {
            seat_num=i+1;
            if(fptr->s[i]==0)
            {
                if(win_seat>0)
              {
                wtravptr->sno=seat_num;
                win_seat--;
                pnum--;
                fptr->s[i]=1;
                wtravptr=wtravptr->next;
              }
              else
              {
                  if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->s[i]=1;
                     mtravptr=mtravptr->next;
                 }
                 else if(ais_seat>0)
                 {
                    atravptr->sno=seat_num;
                    ais_seat--;
                    pnum--;
                    fptr->s[i]=1;
                    atravptr=atravptr->next;
                 }
              }
                  
            }
        }
           printf("prefered seats can't ce alloted.Inconvenience is regreted \n");     
     }
     passenger *mainptr;
    //concatenation of 3 linked lists
    //3 linked lists we created contains seat number and we need to concatenate it
    if(ahptr==NULL && mhptr==NULL) mainptr=whptr;
    if(whptr==NULL && mhptr==NULL) mainptr=ahptr;
    if(ahptr==NULL && whptr==NULL) mainptr=mhptr;
    
    if(whptr==NULL && ahptr!=NULL && mhptr!=NULL)
    {
        mainptr=ahptr;
        atptr->next=mhptr;
        mhptr->prev=atptr;
    }
    else
    {
        if(mhptr==NULL && ahptr!=NULL && whptr!=NULL)
        {
            mainptr=ahptr;
            atptr->next=whptr;
            whptr->prev=atptr;
        }
        if(ahptr==NULL && mhptr!=NULL && whptr!=NULL)
        {
            mainptr=whptr;
            wtptr->next=mhptr;
            mhptr->prev=wtptr;
        }
            
    }
    
    if(mhptr!=NULL && ahptr!=NULL && whptr!=NULL)
    {
        mainptr=whptr;
        wtptr->next=mhptr;
        mhptr->prev=wtptr;
        mtptr->next=ahptr;
        ahptr->prev=mtptr;
    }
    
        
    tickptr->p=mainptr; 
    strcpy(tickptr->ticket_id,f_id); //ticket id has first 3 char as flight-id and next 2 char as seat_num
    
        tickptr->ticket_id[3]=(mainptr->sno)/10+'0';
        tickptr->ticket_id[4]=(mainptr->sno)%10+'0';
    
    
    
    if(fptr->thptr==NULL)
    {
        fptr->thptr=tickptr;
        fptr->tlptr=tickptr;
    }
    else
    {
        (fptr->tlptr)->next=tickptr;
        tickptr->prev=fptr->tlptr;
        fptr->tlptr=tickptr;
        
    }
    
    printf("Passengers welcome aboard\n");
    printf("please note %s ticket id for future reference \n",tickptr->ticket_id); //note this ticket id
    
    }//end of found(if)
      else printf("required number of seats are not available \nInconvenience caused is deeply regreted \n");
      
      
     
      
  }//end of function
  
//-----------------------------------------------------------------------------
//Display full data of flight i.e all passengers in that flight


void DisplayFlightData(char f_id[])
{
     int index,found=0,i;
      swnode *ptr;
      fnode *fptr;
      index=hash_swtable(f_id[0]);
      if(swtable[index]==NULL)
      {
          printf("No such flight exists \n");
      }
      else
      {
          ptr=swtable[index];
          fptr=ptr->flist;
          while(found==0 && ptr!=NULL)
          {
              while(found==0 && fptr!=NULL)
              {
                  if(strcmp((fptr)->fid,f_id)==0)
                  {
                      found=1;
                  }
                  else
                  {
                      fptr=fptr->next;
                      
                  }
              }
              if(found==0)
              {
                ptr=ptr->next;
                if(ptr!=NULL)
                {
                    fptr=ptr->flist;
                }
              }    
          }  
          
          if(found==0)
          {
              printf("No such flight exists \n");
              
          }
        
      }//else ends
      if(found==1)
      {
      ticket *tickptr;
      passenger *pptr;
      tickptr=NULL;
      pptr=NULL;
      tickptr=fptr->thptr;
      while(tickptr!=NULL)
      {
          printf("*********************************************\n");
          printf("Ticket ID: %s \n",tickptr->ticket_id);
          printf("*********************************************\n");
          i=1;
          pptr=tickptr->p;
          while(pptr!=NULL)
          {
            printf("**passenger %d \n",i);
            printf("**Name:%s \n",pptr->name);
            printf("**Age: %d \n",pptr->age);
            printf("**Seat no.:%d \n",pptr->sno);
            if((pptr->sno)%3==1)printf("**Seat Type: W \n");
            if((pptr->sno)%3==2)printf("**Seat Type: M \n");
            if((pptr->sno)%3==0)printf("**Seat Type: A \n");
			printf("**Luggage nos: %d \n",pptr->luggage_num);
            printf("**Luggage weight: %d \n",pptr->luggage_wt);
            printf("**Fine :%d \n",pptr->fine);
            printf("________________________________________________\n");
            pptr=pptr->next;
            i++;
          }
          tickptr=tickptr->next;
      }
      
      }
      
}
//-----------------------------------------------------------------------------  
  

void DisplayPassengerDetails(char t_id[])
{
    char f_id[4];
    f_id[0]=t_id[0];
    f_id[1]=t_id[1];
    f_id[2]=t_id[2];
    int index,found=0,i;
      swnode *ptr;
      fnode *fptr;
      index=hash_swtable(f_id[0]);
      if(swtable[index]==NULL)
      {
          printf("No such flight exists \n");
      }
      else
      {
          ptr=swtable[index];
          fptr=ptr->flist;
          while(found==0 && ptr!=NULL)
          {
              while(found==0 && fptr!=NULL)
              {
                  if(strcmp((fptr)->fid,f_id)==0)
                  {
                      found=1;
                  }
                  else
                  {
                      fptr=fptr->next;
                      
                  }
              }
              if(found==0)
              {
                ptr=ptr->next;
                if(ptr!=NULL)
                {
                    fptr=ptr->flist;
                }
              }    
          }  
          
          if(found==0)
          {
              printf("No such flight exists \n");
              
          }
          
        
      }
      
      if(found==1)
      {
          ticket *tickptr;
          passenger *pptr;
          pptr=NULL;
          tickptr=NULL;
          tickptr=fptr->thptr;
          found=0;
          while(found==0 && tickptr!=NULL)
          {
              if(strcmp((tickptr->ticket_id),t_id)==0)
              {
                  found=1;
                  printf("**************************************************\n");
                  printf("-----------WE WISH YOU A HAPPY JOURNEY-------------\n");
                  printf("Flight id of your flight: %s \n",f_id);
                  printf("Ticket ID: %s \n",tickptr->ticket_id);
                  printf("**************************************************\n");
                  i=1;
                  pptr=tickptr->p;
                  while(pptr!=NULL)
                  {
                    printf("*passenger %d \n",i);
                    printf("*Name:%s \n",pptr->name);
                    printf("*Age: %d \n",pptr->age);
                    printf("*Seat no.:%d \n",pptr->sno);
                    if((pptr->sno)%3==1)printf("*Seat Type: W \n");
 	        	    if((pptr->sno)%3==2)printf("*Seat Type: M \n");
    		        if((pptr->sno)%3==0)printf("*Seat Type: A \n");
                    printf("*Luggage nos: %d \n",pptr->luggage_num);
                    printf("*Luggage weight: %d \n",pptr->luggage_wt);
                    printf("*Fine :%d \n",pptr->fine);
                    printf("________________________________________________\n");
                    pptr=pptr->next;
                    i++;
                  }
                  
               }
               tickptr=tickptr->next;
           }
           if(found==0) printf("Ticket id not found -:please enter correct ticket id\n");
      }
      
      
}
//password checker for airport manager
//default password is asd........change the password in code if you want to change password

boolean check_password()
{
	printf("For security reasons we require your credentials as airport manager\n");
	printf("Please cooperate with us \n");	
	char str[20],pass_str[20]="asd";
	boolean b;
	str[0]='\0';
	password_input(str);
	if(strcmp(str,pass_str)==0)
	{
		printf("\tPassowrd is matched\n");
		b=TRUE;
	}
	else
	{
		printf("\tIncorrect password entered\n");
		b=FALSE;
	}
	return b;
}
//print function for main driver funtion---------------------------

void front_end()
{
	printf("\t\t|************************ HELLO USER************************* |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|          A hearty welcome to flight booking system          |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                              - A project by-:Hardik gandhi  |\n");
	printf("\t\t|************************************************************ |\n");
}
void print()
{
    printf("Enter your choice\n");
    printf("1)To add service window \n");
    printf("2)To book a ticket \n");
    printf("3)To Display flight data-:details of checked-in passengers \n");
    printf("4)To Display passenger details \n");
    printf("5)To Display Available Seats in flight \n");
    printf("6)To see this menu again \n");
}
//main driver function---------------------------



int main()
{
  	boolean b;
    int choice=1;
    swnode *swptr;
    swptr=NULL;
    intialise_swtable(swtable);
    char flight_id_user[4];
    char ticket_id_user[6];
    front_end();
    print();
    while(choice)
    {
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:b=check_password();
			if(b) 
			{
				addFlightServiceWindow(swtable);
		    }
            break;
            case 2:printf("Enter flight id to book :\n");
            scanf("%s",flight_id_user);
            allocateBoardingPass(flight_id_user);
            break;
            case 3:printf("Enter flight id to display details :\n");
            scanf("%s",flight_id_user);
            DisplayFlightData(flight_id_user);
            break;
            case 4:printf("Enter ticket id to get details \n");
            scanf("%s",ticket_id_user);
            DisplayPassengerDetails(ticket_id_user);
            break;
            case 5: printf("Enter flight id :\n");
            scanf("%s",flight_id_user);
            DisplayAvailableSeats(flight_id_user);
            break;
            case 6: print();
            break;
            
        }
        
    }    
	printf("\n**********Thank you for visiting************");
    return 0;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*END-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*










