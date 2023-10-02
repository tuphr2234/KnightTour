#include <stdio.h> 	/* printf */
#include <assert.h> /* assert */
#include <time.h>	/* clock  */

#define SIZE 64
#define SQUARE_SIZE 8

#define INVALID_LOC 66
#define SUCCESS 1
#define FAILURE 0

typedef struct lut
{
	size_t point[8];
	
}lut_t;


size_t KnightTour(size_t bit_arr,size_t loc, size_t *results );
static void PrintCoordinates(size_t loc);

int main()
{
	size_t bit_arr = 0;
	size_t results[SIZE] = {0};
	size_t status = 0;
	size_t i = 0;
	size_t j = 0;
	clock_t start, end;
	start = clock();

	for(i = 0 ; i < SIZE-1 ; i++)
	{
		status = KnightTour( bit_arr,i, results );
		
		printf("status : %ld\n" , status);
		printf("SOLUTION %ld:\n",i);
			
		for(j = 0 ; j < SIZE ;j++)
		{
			PrintCoordinates(results[j]);
		}

		bit_arr = 0;
	}
		end = clock();
		printf("time is : %f \n" , (float)((float)end-(float)start)/CLOCKS_PER_SEC);
	
	return 0;
}

static size_t Occupy(size_t bit_arr, size_t loc)
{
	size_t one = 1;
	bit_arr = bit_arr | (one<<(SIZE-1-loc));
	return bit_arr;
}

static size_t IsOccupied(size_t bit_arr, size_t loc)
{
	size_t one = 1;
	return bit_arr & (one<<(SIZE-1-loc));
}

static void PrintCoordinates(size_t loc)
{
	printf("X : %ld ,  Y : %ld\n",loc%SQUARE_SIZE,loc/SQUARE_SIZE);
}


static size_t GetNumOfOptions( lut_t *lut,size_t lut_index , size_t bit_arr)
{
	size_t i = 0;
	size_t counter = 0;

	
	if(lut_index == INVALID_LOC)
	{
		return SQUARE_SIZE + 1;
	}
	
	for(i = 0 ; i < SQUARE_SIZE ; i++)
	{
		counter+= (lut[lut_index].point[i] != INVALID_LOC) && (!IsOccupied(bit_arr,lut[lut_index].point[i]));
	}
	
	return counter;
}

static int cmpLuts (size_t a, size_t b, lut_t *lut,size_t bit_arr) 
{
	return GetNumOfOptions(lut,a,bit_arr) < GetNumOfOptions(lut,b,bit_arr) ;
}

static void Swap(size_t *a , size_t *b)
{
	size_t temp = *a;
	*a = *b;
	*b = temp;
}


static void SortByLuts(size_t *arr,lut_t *lut,size_t bit_arr)
{
  	size_t i = 0 , j = 0;
	size_t min = arr[0];
	size_t min_ind = 0;

	
	for(i = 0 ; i < SQUARE_SIZE ; i++)
	{
		for(j = i ; j < SQUARE_SIZE ; j++)
		{
			if( cmpLuts(arr[j],min,lut,bit_arr) )
			{
				min = arr[j];
				min_ind = j;
			}
		}
		min = INVALID_LOC;
	
		Swap(arr+i,arr+min_ind);	
	}
}

static size_t Warnsdroff(size_t bit_arr,size_t loc ,lut_t *lut , size_t *results , size_t result_index)
{
	size_t i = 0;
	size_t success = 0;
	size_t next_loc = INVALID_LOC;
	size_t zero = 0;

	results[result_index] = loc;
	bit_arr = Occupy(bit_arr,loc);
	
	if(bit_arr == ~zero)
	{
		return SUCCESS;
	}

	SortByLuts(lut[loc].point,lut, bit_arr); /* for optimized version */
	for(i = 0 ; i < SQUARE_SIZE ; i++)
	{
		next_loc = lut[loc].point[i];
		
		if(INVALID_LOC != next_loc)
		{
			if(!IsOccupied(bit_arr,next_loc))
			{
				success = Warnsdroff(bit_arr , next_loc, lut ,  results , result_index + 1);
				if(success)
				{
					return SUCCESS;
				}
			}
		}
	}
	return FAILURE;
}


static size_t GetPosition(size_t y, size_t x)
{
	if(x > SQUARE_SIZE-1 ||  y > SQUARE_SIZE-1 )
	{
		return INVALID_LOC;
	}
	
	return y*SQUARE_SIZE+x;
}


size_t KnightTour(size_t bit_arr,size_t loc, size_t *results )
{
	size_t i = 0;
	size_t j = 0;
	lut_t lut[64] = {0};
	assert(results);
	
	for(i = 0 ; i < SQUARE_SIZE ; ++i)
	{
		for(j = 0 ; j < SQUARE_SIZE ; ++j)
		{
			lut[i*SQUARE_SIZE+j].point[0] = GetPosition(i-1,j-2);
			lut[i*SQUARE_SIZE+j].point[1] = GetPosition(i+1,j-2);
			lut[i*SQUARE_SIZE+j].point[2] = GetPosition(i-1,j+2);
			lut[i*SQUARE_SIZE+j].point[3] = GetPosition(i+1,j+2);
			lut[i*SQUARE_SIZE+j].point[4] = GetPosition(i-2,j-1);
			lut[i*SQUARE_SIZE+j].point[5] = GetPosition(i+2,j-1);
			lut[i*SQUARE_SIZE+j].point[6] = GetPosition(i-2,j+1);
			lut[i*SQUARE_SIZE+j].point[7] = GetPosition(i+2,j+1);
		}
	}

	i = 0;
	
	/* KnightTourRec for non optimized version */
	return Warnsdroff(bit_arr,loc,lut,results,i); 
	
}



