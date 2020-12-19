#include <iostream>
#include "stdbool.h"

 using namespace std;



void my_sort(int count ,int *ar);
void my_swap(int *x,int *y);
  
 int main()
 {
	 int count; 
     cin >> count;
     int asorted_array[count];
     for(int i = 0; i <count; i++)
     {
		 cin >> asorted_array[i];
	 }
	 cout << "Sorted List" << endl;
	 
	 
	 my_sort(count,&asorted_array[0]);
	 for(int i = 0; i <count; i++)
     {
		 cout << asorted_array[i] << endl;
	 }
	 
     return 0;
 }
 
 
 void my_sort(int count ,int *ar)
 {
	 int sorter_index = 0;
	 while(sorter_index < count)
	 {
		 for(int i = 0,j = 1; i<count; i++,j++)
		 {
			 if(j != count )
			{
				if(ar[i] <= ar[j])
				{
					sorter_index++;
				}
				else
				{
					my_swap(&ar[i],&ar[j]);
					sorter_index = 0;
				}
			}
		 }
	 }


 }
 
 void my_swap(int *x,int *y)
 {
	 int temp = *x;
	 *x = *y;
	 *y = temp;
 }
