#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 16

void request_processing();
void queue_fill();
void queue_processing();
void read();
void write();
void ifetch();
void delete_request_from_queue();
struct memory_request_contents {int row_add; int bank_add; int column_add; int request_time;char operation_type[10];}queue[size];
char *array[3];
unsigned int row,column,bank;
char *ptr;long ret;int CPU_request_time; char Memory_operation_type[10];
int x;int flag=1;int pending_requests_count=0;unsigned long long int time_elapsed;
static const char filename[] = "testinput.txt";
static const char output_file[] = "output.txt";
FILE *file,*out_file ; int t_RCD=14*4,t_RC=50*4,t_RAS=36*4,t_RRD=6*4,t_RP=14*4,t_RFC=172,t_CAS=14*4,t_WR=16*4,t_RTP=8*4,t_CCD=4*4,t_BURST=4*4,t_WTR=8*4,t_CWL=10*4;
int previous_bank;

int time_since_last_operation[8][6]=
{
	{0,-1,0,0,0,0},
	{1,-1,0,0,0,0},
	{2,-1,0,0,0,0},
	{3,-1,0,0,0,0},
	{4,-1,0,0,0,0},
	{5,-1,0,0,0,0},
	{6,-1,0,0,0,0},
	{7,-1,0,0,0,0}
};					    //{BANK_No,ACTIVE_ROW,ACT,RD,WR,PRE}


char previous_operation[8][10]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};


int main()
{
    int p;
    file = fopen ( filename, "r" );
    out_file = fopen (output_file,"w");
    for (p=0;p<size;p++)
    {
        queue[p].row_add=0;
        queue[p].column_add=0;
        queue[p].bank_add=0;
        queue[p].request_time=0;
    }
    while(1)
    {

        time_elapsed++;

        if (file == NULL)
        {
            printf("unable to open file");
            break;
        }

        if ( !feof(file) )                                                  /* read a line */
        {
            request_processing();
        }

        else
        {
            printf("End of file");
            break;
        }
    }

    fclose(file);
    fclose(out_file);
    return 0;
}


void request_processing()
{
    int k;
    char line[128];int a;
        if (flag)
        {  fgets ( line, sizeof line, file );
            printf("%s\n",line);
            k=0;
            array[k] = strtok(line," ");
            while(array[k]!=NULL)
                {
                    array[++k] = strtok(NULL," ");
                }
                ret = strtol(array[0]+2, &ptr, 16);
                CPU_request_time = strtol(array[2],&ptr,10);
                for (a=0;a<10;a++)
                    {
                        Memory_operation_type[a]=*(array[1]+a);
                    }
                    column = ((ret & 0x3FF8)>>3);                       //Breaking the Address to obtain the column address
                    bank = ((ret & 0x1C000)>>14);                       //Breaking the Address to obtain the bank address
                    row = ((ret & 0xFFFE0000)>>17);                     //Breaking the Address to obtain the row address
                    flag=0;
                    }
                    if (time_elapsed >= CPU_request_time && !flag)
                     {
                         queue_fill();
                         flag=1;
                     }
}


void queue_fill()
{
    int a;
    if (pending_requests_count < size)
    {
        queue[pending_requests_count].column_add = column;
        queue[pending_requests_count].bank_add = bank;
        queue[pending_requests_count].row_add = row;
        queue[pending_requests_count].request_time = CPU_request_time;
        for (a=0;a<10;a++)
        {
            queue[pending_requests_count].operation_type[a] = Memory_operation_type[a];
        }

        printf("The row(in Hex) is 0x%08X\n", queue[pending_requests_count].row_add);
        printf("The column(in Hex) is 0x%08X\n", queue[pending_requests_count].column_add);
        printf("The bank(in Hex) is 0x%08X\n", queue[pending_requests_count].bank_add);
        printf("time elapsed is %lld\n",time_elapsed);

       printf("%s\n",queue[pending_requests_count].operation_type);
        printf("\n");
        pending_requests_count ++;

        if (pending_requests_count!=0)
        {
           queue_processing ();
        }
    }
}


void queue_processing()
{
    if (strcmp(queue[0].operation_type,"READ")==0)
    {
        read();

    }
    else if (strcmp(queue[0].operation_type,"WRITE")==0)
    {
        write();

    }
    else if (strcmp(queue[0].operation_type,"IFETCH")==0)
    {
        ifetch();

    }
    else
    {
        printf("No match");
    }
}


void read()
    {
	row_comparison();
    delete_request_from_queue();
    }


void ifetch()
    {
	row_comparison();
	delete_request_from_queue();
    }


void write()
    {
	write_row_comparison();
	delete_request_from_queue();
    }


void row_comparison()
{
    for(x=0;x<8;x++)
    {
        if((queue[0].bank_add==x) && (queue[0].row_add==time_since_last_operation[x][1]))           //comparison of row status(OPEN or CLOSED)
            {
                open_row();
                break;
            }
        else if((queue[0].bank_add==x) && (queue[0].row_add!=time_since_last_operation[x][1]))
           {
               closed_row();
               break;
           }
    }

}


void open_row()
{
    int i;
    if(strcmp(previous_operation[x],queue[0].operation_type)==0)
        {
            for(i=time_elapsed-time_since_last_operation[x][3];i<t_CCD;i++,time_elapsed++)
                {

                }
        }
    else  if(strcmp(previous_operation[x],"WRITE")==0)
        {
            for(i=time_elapsed-time_since_last_operation[x][4];i<(t_CWL+t_BURST+t_WTR);i++,time_elapsed++)
                {

                }
        }
    else
        {
            for(i=time_elapsed-time_since_last_operation[x][3];i<t_CCD;i++,time_elapsed++)
                {

                }
        }
        printf("%lld RD 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
        fprintf(out_file,"%lld RD 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
        time_since_last_operation[x][3]=time_elapsed;                                                       //Recording the READ time in the 2d array
        time_since_last_operation[x][1]=queue[0].row_add;                                                   //Recording the ROW address to check open row in that bank
        strcpy(previous_operation[x],queue[0].operation_type);                                              //REcording the command presented to DDR3 RAM
        previous_bank=queue[0].bank_add;
}


void closed_row()
{
    int i;
    if(previous_bank==queue[0].bank_add)
    {
        if(strcmp(previous_operation[queue[0].bank_add],"WRITE")==0)
        {
            for(i=time_elapsed-time_since_last_operation[previous_bank][4];i<t_CWL+t_BURST+t_WR;i++,time_elapsed++)     //WRITE to PRECHARGE delay
                {

                }
            for(i=time_elapsed-time_since_last_operation[previous_bank][0];i<t_RC;i++,time_elapsed++)               //For same bank ACT to ACT delay t_RC
            {

            }
        }
        else
        {
            for(i=time_elapsed-time_since_last_operation[previous_bank][3];i<t_RTP;i++,time_elapsed++)          //READ to PRECHARGE delay
                {

                }
        }
    }

    printf("%lld PRE 0x%08X\n",time_elapsed,queue[0].bank_add);
    fprintf(out_file,"%lld PRE 0x%08X\n",time_elapsed,queue[0].bank_add);
    time_since_last_operation[x][6]=time_elapsed;
    for(i=0;i<t_RP;i++,time_elapsed++)
            {

            }
    for(i=time_elapsed-time_since_last_operation[previous_bank][6];i<t_RRD;i++,time_elapsed++)              //ACT to ACT Delay for different bank
    {

    }
        printf("%lld ACT 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].row_add);
        fprintf(out_file,"%lld ACT 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].row_add);
        time_since_last_operation[x][2]=time_elapsed;
        for(i=0;i<t_RCD;i++,time_elapsed++)
            {

            }
        printf("%lld RD 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
        fprintf(out_file,"%lld RD 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
        time_since_last_operation[x][3]=time_elapsed;
        time_since_last_operation[x][1]=queue[0].row_add;
        previous_bank=queue[0].bank_add;
        strcpy(previous_operation[x],queue[0].operation_type);
}

void write_row_comparison()
{
    for(x=0;x<8;x++)
    {
        if((queue[0].bank_add==x) && (queue[0].row_add==time_since_last_operation[x][1]))
        {
                write_open_row();
                break;

        }
    else if((queue[0].bank_add==x) && (queue[0].row_add!=time_since_last_operation[x][1]))
        {
                write_closed_row();
                break;

        }
    }

}


void write_open_row()
{
    int i;
        if(strcmp(previous_operation[x],queue[pending_requests_count].operation_type)==0)
            {
                for(i=time_elapsed-time_since_last_operation[x][4];i<t_CCD;i++,time_elapsed++)
                    {

                    }
            }
        else
	{
	    for(i=time_elapsed-time_since_last_operation[x][3];i<t_CAS+t_CCD-t_CWL+8;i++,time_elapsed++)
	        {

	        }
	}
    printf("%lld WR 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
    fprintf(out_file,"%lld WR 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
    time_since_last_operation[x][4]=time_elapsed;
    time_since_last_operation[x][1]=queue[0].row_add;
    previous_bank=queue[0].bank_add;
    strcpy(previous_operation[x],queue[0].operation_type);
}


void write_closed_row()
{
    int i;
    if(previous_bank==queue[0].bank_add)
    {
        if(strcmp(previous_operation[queue[0].bank_add],"WRITE")==0)
        {
            for(i=time_elapsed-time_since_last_operation[queue[0].bank_add][4];i<t_CWL+t_BURST+t_WR;i++,time_elapsed++)
                {

                }
            for(i=time_elapsed-time_since_last_operation[previous_bank][0];i<t_RC;i++,time_elapsed++)
                {

                }
        }
        else
        {
            for(i=time_elapsed-time_since_last_operation[queue[0].bank_add][3];i<t_RTP;i++,time_elapsed++)
                {

                }
        }
    }

    printf("%lld PRE 0x%08X\n",time_elapsed,queue[0].bank_add);
    fprintf(out_file,"%lld PRE 0x%08X\n",time_elapsed,queue[0].bank_add);
    time_since_last_operation[x][6]=time_elapsed;
    for(i=0;i<t_RP;i++,time_elapsed++)
        {

        }
    for(i=time_elapsed-time_since_last_operation[previous_bank][6];i<t_RRD;i++,time_elapsed++)
    {

    }
    printf("%lld ACT 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].row_add);
    fprintf(out_file,"%lld ACT 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].row_add);
    time_since_last_operation[x][2]=time_elapsed;
    for(i=0;i<t_RCD;i++,time_elapsed++)
        {

        }
    printf("%lld WR 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
    fprintf(out_file,"%lld WR 0x%08X 0x%08X\n",time_elapsed,queue[0].bank_add,queue[0].column_add);
    time_since_last_operation[x][4]=time_elapsed;
    time_since_last_operation[x][1]=queue[0].row_add;
    previous_bank=queue[0].bank_add;
    strcpy(previous_operation[x],queue[0].operation_type);
}


void delete_request_from_queue()
{
    int i;
    for (i=0;i<size-1;i++)
    {
        queue[i]=queue[i+1];

    }
    queue[size-1].bank_add=0;
    queue[size-1].column_add=0;
    queue[size-1].row_add=0;
    queue[size-1].request_time=0;
    pending_requests_count--;
}
