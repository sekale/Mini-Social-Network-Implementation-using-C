/******************************************************************
* Author: Siddhant Ekale    Username: sekale  PUID: 0026277745
* Date:   April 25, 2015
* Project: 3
*******************************************************************/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

void create_attri_array(FILE *fptr,int *age,int *gender,int *m_status,int *race,int *b_place,int *lang,int *occu,int *income);
void print_retri_array(int *age,int *gender,int *m_status,int *race,int *b_place,int *lang,int *occu,int *income,int size);
float magvect(int val1, int val2);
float **make_ulab(int *age,int *gender,int *m_status,int *race,int *b_place,int *lang,int *occu,int *income,int size,float *max);
float **make_lab(float **arr_ulab,int max,int size);
float **make_graph(float **arr_lab,float delta,int size);
int query3(float **arr_lab,int size,int user_id);
int query4(float **arr_lab,int size, int user_id);
void query5(float **arr_lab, int size);
void query1(float **arr_lab, int size, int user_id);
int query45(float **arr_lab,int size, int user_id);
int query35(float **arr_lab,int size,int user_id);
int dist_query2_helper(float *distance, int *visited, int num_users);
void query2(int num_users, int source_node, float alpha ,float **arr_lab);



int main(int argc,char **argv)
{
	//Declarations
	FILE *fptr;
	int size = 0;
	fptr = fopen(argv[1],"r");
   	if(fptr == NULL)
	{
		return EXIT_FAILURE;
	}        
	

	int *age = NULL;
	int *gender = NULL;
	int *m_status = NULL;
	int *race = NULL;
	int *b_place = NULL;
	int *lang = NULL;
	int *occu = NULL;
	int *income = NULL;	
	//int n;	
	//Scanning to get the size of the file
	
	fscanf(fptr,"%d",&size);
	
	age = malloc(sizeof(int)*size);
	gender = malloc(sizeof(int)*size);
	m_status = malloc(sizeof(int)*size);
	race = malloc(sizeof(int)*size);
	b_place = malloc(sizeof(int)*size);
	lang = malloc(sizeof(int)*size);
	occu = malloc(sizeof(int)*size);
	income = malloc(sizeof(int)*size);


	//Creating all the attributes and returning since they are pointers
	create_attri_array(fptr,age,gender,m_status,race,b_place,lang,occu,income);
	free(age);
	free(gender);
	free(m_status);
	free(race);
	free(b_place);
	free(lang);
	free(occu);
	free(income);
	fclose(fptr);
	return EXIT_SUCCESS;
}

void create_attri_array(FILE *fptr,int *age,int *gender,int *m_status,int *race,int *b_place,int *lang,int *occu,int *income)
{
	int i = 0;
	int user_id = 0;
	float **arr_ulab;
	float *max = malloc(sizeof(float));
	float **lab;	
	
	int n = 0;
	int size = 0;
	float delta1 = 0;
	float delta2 = 0;
	int node_id = 0;
	float alpha = 0;

	fseek(fptr,0,SEEK_SET);

	n = fscanf(fptr,"%d,%f,%f,%d,%f",&size,&delta1,&delta2,&node_id,&alpha);
	delta1 *=100;
	delta2 *=100;
	alpha *= 100;
	
	for(i = 0; i < size; i++)
	{
		fscanf(fptr,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&user_id,&age[i],&gender[i],&m_status[i],&race[i],&b_place[i],&lang[i],&occu[i],&income[i]);
	}

	int temp = 0;
	//print_retri_array(age,gender,m_status,race,b_place,lang,occu,income,size);
	arr_ulab = make_ulab(age,gender,m_status,race,b_place,lang,occu,income,size,max);
	lab = make_lab(arr_ulab,*max,size);
	
	lab = make_graph(lab,delta1,size);
	query1(lab,size,node_id);
	query2(size,node_id,alpha,lab);
	temp = query3(lab,size,node_id);
//query5(lab,size);
	temp = query4(lab,size,node_id);
	
	query5(lab,size);

	printf("\n\n");

	//arr_ulab = make_ulab(age,gender,m_status,race,b_place,lang,occu,income,size,max); //malloced value for arr_ulab returned to this variable
	lab = make_lab(arr_ulab,*max,size); 
	
	lab = make_graph(lab,delta2,size);
	query1(lab,size,node_id);
	query2(size,node_id,alpha,lab);
	temp = query3(lab,size,node_id);
//query5(lab,size);
	temp = query4(lab,size,node_id);	
	query5(lab,size);
	printf("\n");
	
		
	//lab = make_graph(lab,delta2,size);
	

	

	
	//fclose(fptr);	

}

/*void print_retri_array(int *age,int *gender,int *m_status,int *race,int *b_place,int *lang,int *occu,int *income,int size)
{
	int count = 0;
	for(;count < size; count++)
	{
		printf("\n");
		printf("%d %d %d %d %d %d %d %d %d",count,age[count],gender[count],m_status[count],race[count],b_place[count],lang[count],occu[count],income[count]);
	}
}*/

float magvect(int val1, int val2)
{
	float retval = 0;
	retval = (val1 - val2) * (val1 - val2); 
	return retval;
}

float **make_ulab(int *age,int *gender,int *m_status,int *race,int *b_place,int *lang,int *occu,int *income,int size,float *max)
{
	int count = 0;
	int row = 0;
	int col = 0;
	//float max = 0;
	float **ulab = NULL;
	ulab = malloc(size*sizeof(float*));
	for(;count < size ; count++)
	{
		*(ulab+count) =	malloc(size*sizeof(float));
	}
	*max = 0;
	for(col = 0; col < size; col++)
	{
		for(row = 0; row < size; row++)
		{
			ulab[row][col] = sqrt(pow(age[row]-age[col],2) + 
					      pow(gender[row]-gender[col],2) + 
					      pow(m_status[row]-m_status[col],2) + 
					      pow(race[row]-race[col],2) + 
					      pow(b_place[row]-b_place[col],2) + 
					      pow(lang[row]-lang[col],2) + 
					      pow(occu[row]-occu[col],2) + 
					      pow(income[row]-income[col],2));	
	
		if(ulab[row][col] > *max)
		{
			*max = ulab[row][col];
		}		
		}
	}
	
	/*for(row = 0; row < size; row++)
	{
		printf("\n");
		for(col = 0; col < size; col++)
		{
			printf("%5.2f \t",ulab[row][col]);	
		}
	}
	printf("Max = %5.2f",*max);*/
	return ulab;

}

float **make_lab(float **arr_ulab,int max,int size)
{
	int row = 0;
	int col = 0;
	int count = 0;
	float **lab = NULL;
	lab = malloc(size*sizeof(float*));
	for(;count < size; count++)
	{
		*(lab+count) =	malloc(size*sizeof(float));
	}
	
	for(row = 0; row < size ; row++)
	{
		arr_ulab[row][col] = (int)(arr_ulab[row][col] * 100);
		arr_ulab[row][col] /= 100;
		for(col = 0; col < size; col++)
		{
			lab[row][col] = 1 - (arr_ulab[row][col]/max);
		}
	}	
	/*printf("\n");
	for(row = 0; row < size; row++)
	{
		printf("\n");
		for(col = 0; col < size; col++)
		{
			printf("%.4f \t",lab[row][col]);	
		}
	}*/
	return lab;	
}

float **make_graph(float **arr_lab,float delta,int size)
{
	//printf("\n");
	int row;
	int col;
	for(row = 0; row < size; row++)
	{
		for(col = 0; col < size; col++)
		{
				arr_lab[row][col] = (int)(arr_lab[row][col] * 100);
				//arr_lab[row][col] /= 100;
			if(row == col)	
			{
				arr_lab[row][col] = 0;			
			}

			else
			{
			if(arr_lab[row][col] <= delta) //changed from < to <=
			{
				arr_lab[row][col] = 0;
			}

			}
		}
	}
	return arr_lab;
}


int query3(float **arr_lab,int size,int user_id)
{
	int row = user_id - 1;
	int col = 0;
	int count = 0;
	//int id;
	//int *sortedids = malloc(sizeof(int)*(size - 1));
	for(col = 0; col < size; col++)
	{
		if(arr_lab[row][col] != 0)
		{
			count++;
		}		
	}
	printf("\n%d",count);

	//printf("\n Immediate Neigbours : ");

	for(col = 0; col < size ; col++)
	{
		if(arr_lab[row][col] != 0)
		{
		printf(",%d",col+1);
		}	
	}
	return count;
}

int query4(float **arr_lab,int size, int user_id)
{
	
	int row = user_id - 1;
	int col = 0;
	int count = 0;
	int *indexarr = malloc(sizeof(int) * size);
	int r1 = 0;
	int c1 = 0;

	for(col =0 ; col<size; col++)
	{
		indexarr[col] = 0;
	}	

	for(col = 0; col < size; col++)
	{
		if(arr_lab[row][col] != 0)
		{
			r1 = col;
			for(c1 = 0; c1 < size; c1++)
			{
				if(arr_lab[r1][c1] !=  0)
				{
					indexarr[c1] = 1;
				}
			}
		}
	}

	indexarr[row] = 0;

	for(col = 0; col < size; col++)
	{
		if(indexarr[col] == 1)
		{
			count++;		
		}
	}

	printf("\n%d",count);
	for(col = 0; col < size ; col++)
	{
		if(indexarr[col] == 1)
		{
		printf(",%d",col+1);
		}
	}
	free(indexarr);
	return count;
}

void query5(float **arr_lab,int size)
{
	//int row = 0;
	int col = 0;
	//int count = 0;
	float sum = 0;

	for(col = 0; col < size; col++)
	{
	 sum += query35(arr_lab,size,col + 1);
	}

	sum = sum/size;
	//sum = (int)(sum * 100);
	//sum /= 100;
	printf("\n%.2f",sum);
	sum = 0;
		
	for(col = 0; col < size; col++)
	{
	 sum += query45(arr_lab,size,col + 1);	
	}
	//printf("\nsize = %d, sum = %f",size,sum);
	sum = sum/size;
	//sum = (int)(sum * 100);
	//sum /= 100;
	printf("\n%.2f",sum);

	
}

void query1(float **arr_lab, int size, int user_id)
{
	int row = user_id - 1;
	int col = 0;
	//int i = 0;
	float min = 0;
	float min2 = 0;
	//int count = 0;
	min = 10000;
	for(col = 0; col < size; col++)
	{
		if(arr_lab[row][col]  < min && arr_lab[row][col] != 0)	
		{
		min = arr_lab[row][col];
		}
	}	
	min = (int)(min * 100);
	min /= 100;	
	min2 = min/100;
	printf("%.2f",min2);
	for(col = 0; col < size; col++)
	{
		if(arr_lab[row][col] == min)
		{
		printf(",%d",col+1); 
		}
	}
}

//-----------------------------------------------------------------------------------


/*
Function Query 35:
Works towards calculating the average
*/

int query35(float **arr_lab,int size,int user_id)
{
	int row = user_id - 1;
	int col = 0;
	int count = 0;
	//int id;
	//int *sortedids = malloc(sizeof(int)*(size - 1));
	for(col = 0; col < size; col++)
	{
		if(arr_lab[row][col] != 0)
		{
			count++;
		}		
	}
	//printf("\n%d,",count);
	//printf("\n Immediate Neigbours : ");
	for(col = 0; col < size ; col++)
	{
		if(arr_lab[row][col] != 0)
		{
		//printf("%d,",col+1);
		}	
	}

	//printf("\n");
	return count;
}

int query45(float **arr_lab,int size, int user_id)
{
	int row = user_id - 1;
	int col = 0;
	int count = 0;
	int *indexarr = malloc(sizeof(int) * size);
	int r1 = 0;
	int c1 = 0;	

	for(col =0 ; col<size; col++)
	{
		indexarr[col] = 0;
	}

	for(col = 0; col < size; col++)
	{
		if(arr_lab[row][col] != 0)
		{
			r1 = col;
			for(c1 = 0; c1 < size; c1++)
			{
				if(arr_lab[r1][c1] !=  0)
				{
					indexarr[c1] = 1;
				}
			}
		}
	}

	indexarr[row] = 0;

	for(col = 0; col < size; col++)
	{
		if(indexarr[col] == 1)
		{
			count++;		
		}
	}
	//printf("\ncount = %d",count);
	free(indexarr);
	return count;
}
//------------------------------------------------------Query 2------------------------------------------------------

int compar_float (const void * a, const void * b){
   return ( *(float*)a - *(float*)b );
}

int dist_query2_helper(float *distance, int *visited, int num_users)
{
  float min = 100;  
  int min_index = 0;
  int c = 0;
  for (c = 0; c < num_users; c++)
    {
      if (visited[c] == 0 && distance[c] <= min)
        {
          min = distance[c];
          min_index = c;
        }
    }
   return min_index;
}

void query2(int usersno, int source_node, float alpha ,float **arr_lab)
{
 	float * distance_graph = malloc(sizeof(float)*usersno);     
  	int * visited = malloc(sizeof(float)*usersno); 
  
	int lowest_multiple = 0;
	int count = 0;
	int r = 0;  
	int c = 0;  
 
  	for (lowest_multiple = 0; lowest_multiple < usersno; lowest_multiple++)
    	{
      		distance_graph[lowest_multiple] = INT_MAX;  
      	visited[lowest_multiple] = 0;  
    	}
  	distance_graph[source_node-1] = 0;   
  

  	for (lowest_multiple = 0; lowest_multiple < usersno - 1; lowest_multiple++)
    	{
      
      	r = dist_query2_helper(distance_graph, visited,usersno);
     	visited[r] = 1;
      
      	for (c = 0; c < usersno; c++)
        {
          	if (!visited[c] && arr_lab[r][c] && distance_graph[r] != INT_MAX && distance_graph[r]+arr_lab[r][c] < distance_graph[c])
            	distance_graph[c] = distance_graph[r] + arr_lab[r][c];
        }
}

  	qsort(distance_graph,usersno,sizeof(float),*compar_float);
  	for(lowest_multiple= 0; lowest_multiple < usersno; lowest_multiple++)
    	{
      		if(distance_graph[lowest_multiple] < alpha)
       		count++;
    	}
  	count--;  
  	printf("\n%d",count);
  	free(distance_graph);
  	free(visited);
}
